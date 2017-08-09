#include <emdl/dul/Transport.h>
#include <emdl/dul/StateMachine.h>

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

#include <memory>
#include <string>

#include <iostream>

namespace emdl
{
	namespace dul
	{
		Transport::Transport(StateMachine& stateMachine, boost::asio::io_service& service)
			: m_stateMachine(stateMachine)
			, m_service(service)
		{
		}

		Transport::~Transport()
		{
			close();
		}

		boost::asio::io_service& Transport::service()
		{
			if (!m_socket)
				throw Exception("Not connected");
			return m_socket->get_io_service();
		}

		Transport::Socket::endpoint_type Transport::remoteEndpoint() const
		{
			if (!m_socket)
				throw Exception("Not connected");
			return m_socket->remote_endpoint();
		}

		void Transport::setSocket(Socket socket)
		{
			if (isOpen())
				throw Exception("Already connected");

			m_socket = std::move(socket);

			readHeader();
		}

		bool Transport::isOpen() const
		{
			return (m_socket && m_socket->is_open());
		}

		void Transport::connect(const Socket::endpoint_type& endpoint)
		{
			if (isOpen())
				throw Exception("Already connected");

			m_socket = Socket{m_service};
			boost::system::error_code error;
			m_socket->connect(endpoint, error);

			if (error)
				throw SocketClosed("Connect error: " + error.message());

			readHeader();
		}

		void Transport::close()
		{
			if (isOpen())
				m_socket.reset(); // The destructor of the socket object takes care of everything
		}

		void Transport::write(const std::string& data)
		{
			if (!isOpen())
				throw SocketClosed("Not connected");

			boost::system::error_code error;
			boost::asio::write(*m_socket, boost::asio::buffer(data), error);
			if (error)
				throw SocketClosed("Operation error: " + error.message());
		}

		void Transport::readHeader()
		{
			const auto buf = boost::asio::buffer(&m_readHeader, 6);
			//boost::asio::read(*m_socket, buf);
			//	readBody();
			boost::asio::async_read(*m_socket, buf, [this](boost::system::error_code ec, std::size_t) {
				if (!ec)
					readBody();
				else
					onError(ec);
			});
		}

		void Transport::readBody()
		{
			const size_t size = ntohl(m_readHeader.size); // Big endian to little endian
			m_readBody = std::string(size, '\0');

			const auto buf = boost::asio::buffer(&m_readBody[0], size);
			//	boost::asio::read(*m_socket, buf);
			//	m_stateMachine.onReceivedPDU(m_readHeader, std::move(m_readBody));
			boost::asio::async_read(*m_socket, buf, [this](boost::system::error_code ec, std::size_t) {
				if (!ec)
				{
					m_stateMachine.onReceivedPDU(m_readHeader, std::move(m_readBody));
					readHeader();
				}
				else
					onError(ec);
			});
		}

		void Transport::onError(boost::system::error_code ec)
		{
			std::cerr << "Socket error\n";
			m_socket.reset();
			// TODO: add logs!
		}
	}
}

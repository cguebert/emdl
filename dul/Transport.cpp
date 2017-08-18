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
		Transport::Transport(boost::asio::io_service& service)
			: m_service(service)
			, m_strand(m_service)
		{
		}

		void Transport::setStateMachine(std::shared_ptr<StateMachine> stateMachine)
		{
			m_stateMachine = stateMachine;
		}

		boost::asio::io_service& Transport::service()
		{
			return m_service;
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
			if (!m_closed)
			{
				m_closed = true;
				auto stateMachine = m_stateMachine.lock();
				if (stateMachine)
					stateMachine->onTransportClose();

				m_strand.post([this] {
					m_socket.reset(); // The destructor of the socket object takes care of everything
				});
			}
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
			auto self = shared_from_this();
			boost::asio::async_read(*m_socket, buf, m_strand.wrap([this, self](boost::system::error_code ec, std::size_t) {
				if (!ec)
					readBody();
				else
					onError(ec);
			}));
		}

		void Transport::readBody()
		{
			const size_t size = ntohl(m_readHeader.size); // Big endian to little endian
			m_readBody = std::string(size, '\0');

			const auto buf = boost::asio::buffer(&m_readBody[0], size);
			auto self = shared_from_this();
			boost::asio::async_read(*m_socket, buf, m_strand.wrap([this, self](boost::system::error_code ec, std::size_t) {
				if (!ec)
				{
					auto stateMachine = m_stateMachine.lock();
					if (stateMachine)
						stateMachine->onReceivedPDU(m_readHeader, std::move(m_readBody));
					readHeader();
				}
				else
					onError(ec);
			}));
		}

		void Transport::onError(boost::system::error_code ec)
		{
			// TODO: add logs!
			if (!m_closed)
			{
				if (ec != boost::asio::error::eof)
					std::cerr << "Socket error\n";
				m_closed = true;
				m_socket.reset();

				auto stateMachine = m_stateMachine.lock();
				if (stateMachine)
					stateMachine->onTransportClose();
			}
		}
	}
}

#include <emdl/dul/Transport.h>
#include <emdl/dul/StateMachine.h>

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>

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

		const boost::asio::io_service& Transport::service() const
		{
			return m_service;
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

			start();
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

			start();
		}

		void Transport::close()
		{
			if (isOpen())
				m_socket.reset(); // The destructor of the socket object takes care of everything
			stop();
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

		void Transport::start()
		{
			readHeader();

			m_thread = std::make_unique<std::thread>([this]() {
				while (!m_service.stopped())
				{
					try
					{
						m_service.run();
					}
					catch (const std::exception& e)
					{
						std::cerr << "Exception in Transport thread: " << e.what() << "\n";
						// TODO: add logs!
					}
				}
				std::cout << "Transport thread stopped\n";
			});
		}

		void Transport::stop()
		{
			if (m_thread)
			{
				m_service.stop();
				m_thread->join();
				m_thread.reset();
			}
		}

		void Transport::readHeader()
		{
			const auto buf = boost::asio::buffer(&m_readHeader, 6);
			boost::asio::async_read(*m_socket, buf, [this](boost::system::error_code ec, std::size_t /*length*/) {
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
			boost::asio::async_read(*m_socket, buf, [this](boost::system::error_code ec, std::size_t /*length*/) {
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
			// TODO: add logs!
		}
	}
}

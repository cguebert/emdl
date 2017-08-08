#include <emdl/dul/Transport.h>

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>

namespace emdl
{
	namespace dul
	{
		Transport::Transport(StateMachine& stateMachine)
			: m_stateMachine(stateMachine)
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
		}

		void Transport::close()
		{
			if (isOpen())
				m_socket.reset(); // The destructor of the socket object takes care of everything
		}

		std::string Transport::read(std::size_t length)
		{
			if (!isOpen())
				throw SocketClosed("Not connected");

			std::string data(length, 'a');

			boost::system::error_code error;
			boost::asio::read(*m_socket,
							  boost::asio::buffer(&data[0], data.size()),
							  error);
			if (error)
				throw SocketClosed("Operation error: " + error.message());

			return data;
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
	}
}

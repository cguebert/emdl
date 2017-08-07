#include <emdl/dul/Transport.h>

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>

namespace emdl
{
	namespace dul
	{
		Transport::Transport()
			: m_service()
			, m_socket(nullptr)
			, m_timeout(boost::posix_time::pos_infin)
			, m_deadline(m_service)
		{
		}

		Transport::~Transport()
		{
			if (is_open())
			{
				close();
			}
		}

		const boost::asio::io_service& Transport::get_service() const
		{
			return m_service;
		}

		boost::asio::io_service& Transport::get_service()
		{
			return m_service;
		}

		std::shared_ptr<const Transport::Socket> Transport::get_socket() const
		{
			return m_socket;
		}

		std::shared_ptr<Transport::Socket> Transport::get_socket()
		{
			return m_socket;
		}

		Transport::duration_type Transport::get_timeout() const
		{
			return m_timeout;
		}

		void Transport::set_timeout(duration_type timeout)
		{
			m_timeout = timeout;
		}

		bool Transport::is_open() const
		{
			return (m_socket != nullptr && m_socket->is_open());
		}

		void Transport::connect(const Socket::endpoint_type& peer_endpoint)
		{
			if (is_open())
			{
				throw Exception("Already connected");
			}

			m_socket = std::make_shared<Socket>(m_service);
			boost::system::error_code error;
			m_socket->connect(peer_endpoint, error);

			if (error)
				throw SocketClosed("Connect error: " + error.message());

			/*    auto source = Source::NONE;
	boost::system::error_code error;
	_start_deadline(source, error);

	m_socket = std::make_shared<Socket>(m_service);
	m_socket->async_connect(
		peer_endpoint,
		[&source,&error](const boost::system::error_code & e)
		{
			source = Source::OPERATION;
			error = e;
		}
	);

	_run(source, error);*/
		}

		void Transport::receive(std::shared_ptr<Socket> socket)
		{
			if (is_open())
			{
				throw Exception("Already connected");
			}

			m_socket = std::move(socket);
		}

		void Transport::close()
		{
			if (is_open())
			{
				//    m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
				//    m_socket->close();
				m_socket = nullptr; // The destructor of the socket object takes care of everything
			}
		}

		std::string Transport::read(std::size_t length)
		{
			if (!is_open())
				throw SocketClosed("Not connected");

			std::string data(length, 'a');

			auto source = Source::NONE;
			boost::system::error_code error;
			start_deadline(source, error);

			boost::asio::read(*m_socket,
							  boost::asio::buffer(&data[0], data.size()),
							  error);
			if (error)
				throw SocketClosed("Operation error: " + error.message());

			/*    boost::asio::async_read(
		*m_socket,
		boost::asio::buffer(&data[0], data.size()),
		[&source,&error](const boost::system::error_code & e, std::size_t)
		{
			source = Source::OPERATION;
			error = e;
		}
	);

	_run(source, error);*/

			return data;
		}

		void Transport::write(const std::string& data)
		{
			if (!is_open())
				throw SocketClosed("Not connected");

			auto source = Source::NONE;
			boost::system::error_code error;
			start_deadline(source, error);

			boost::asio::write(*m_socket, boost::asio::buffer(data), error);
			if (error)
				throw SocketClosed("Operation error: " + error.message());
			/*    boost::asio::async_write(
		*m_socket, boost::asio::buffer(data),
		[&source,&error](const boost::system::error_code & e, std::size_t)
		{
			source = Source::OPERATION;
			error = e;
		}
	);

	_run(source, error);*/
		}

		void Transport::start_deadline(Source& source, boost::system::error_code& error)
		{
			return;

			const auto canceled = m_deadline.expires_from_now(m_timeout);
			if (canceled != 0)
			{
				throw Exception("TCP timer started with pending operations");
			}

			m_deadline.async_wait(
				[&source, &error](const boost::system::error_code& e) {
					source = Source::TIMER;
					error = e;
				});
		}

		void Transport::stop_deadline()
		{
			m_deadline.expires_at(boost::posix_time::pos_infin);
		}

		void Transport::run(Source& source, boost::system::error_code& error)
		{
			// WARNING: it seems that run_one runs a *simple* operation, not a
			// *composed* operation, as is done by async_read/async_write
			while (source == Source::NONE)
			{
				const auto ran = m_service.run_one();
				if (ran == 0)
				{
					throw Exception("No operations ran");
				}
				m_service.reset();
			}

			if (source == Source::OPERATION)
			{
				if (error)
				{
					throw SocketClosed("Operation error: " + error.message());
				}
				/*
		source = Source::NONE;
		_stop_deadline();

		while(source == Source::NONE)
		{
			const auto polled = m_service.poll_one();
			if(polled == 0)
			{
				throw Exception("No operations polled");
			}
			m_service.reset();
		}

		if(source != Source::TIMER)
		{
			throw Exception("Unknown event");
		}
		else if(error != boost::asio::error::operation_aborted)
		{
			throw Exception("TCP timer error: "+error.message());
		}*/
			}
			else if (source == Source::TIMER)
			{
				throw Exception("TCP time out");
			}
			else
			{
				throw Exception("Unknown source");
			}
		}
	}
}

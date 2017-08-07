#pragma once

#include <memory>
#include <string>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>

#include <emdl/emdl_api.h>
#include <emdl/Exception.h>

namespace emdl
{
	namespace dul
	{
		/**
		 * @brief TCP transport for the DICOM Upper Layer.
		 *
		 * The behavior of connect, receive, read and write is governed by the timeout
		 * value: if the timeout expires before the operation is completed, an exception
		 * will be raised.
		 */
		class EMDL_API Transport
		{
		public:
			/// @brief Socket type.
			typedef boost::asio::ip::tcp::socket Socket;

			/// @brief Duration of the timeout.
			typedef boost::asio::deadline_timer::duration_type duration_type;

			/// @brief Constructor.
			Transport();

			/// @brief Destructor.
			~Transport();

			/// @brief Return the io_service.
			const boost::asio::io_service& get_service() const;

			/// @brief Return the io_service.
			boost::asio::io_service& get_service();

			/// @brief Return the socket.
			std::shared_ptr<const Socket> get_socket() const;

			/// @brief Return the socket.
			std::shared_ptr<Socket> get_socket();

			/// @brief Return the timeout, default to infinity.
			duration_type get_timeout() const;

			/// @brief Set the timeout.
			void set_timeout(duration_type timeout);

			/// @brief Test whether the transport is open.
			bool is_open() const;

			/// @brief Connect to the specified endpoint, raise an exception upon error.
			void connect(const Socket::endpoint_type& peer_endpoint);

			/**
			 * @brief Receive a connection on the specified socket, raise an
			 * exception upon error.
			 */
			void receive(std::shared_ptr<Socket> socket);

			/// @brief Close the connection.
			void close();

			/// @brief Read data, raise an exception on error.
			std::string read(std::size_t length);

			/// @brief Write data, raise an exception on error.
			void write(const std::string& data);

		private:
			boost::asio::io_service m_service;
			std::shared_ptr<Socket> m_socket;
			duration_type m_timeout;
			boost::asio::deadline_timer m_deadline;

			enum class Source
			{
				NONE,
				TIMER,
				OPERATION,
			};

			void start_deadline(Source& source, boost::system::error_code& error);
			void stop_deadline();

			void run(Source& source, boost::system::error_code& error);
		};

		/// Exception reported when the socket is closed without releasing the association.
		class SocketClosed : public Exception
		{
		public:
			SocketClosed(const std::string& message)
				: Exception(message)
			{
			}
		};
	}
}

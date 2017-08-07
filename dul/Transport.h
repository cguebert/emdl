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
		/// TCP transport for the DICOM Upper Layer.
		class EMDL_API Transport
		{
		public:
			typedef boost::asio::ip::tcp::socket Socket;
			typedef boost::asio::deadline_timer::duration_type duration_type;

			/// Destructor.
			~Transport();

			const boost::asio::io_service& service() const; /// Return the io_service.
			boost::asio::io_service& service();             /// Return the io_service.

			std::shared_ptr<const Socket> socket() const; /// Return the socket.
			std::shared_ptr<Socket> socket();             /// Return the socket.

			bool isOpen() const; /// Test whether the transport is open.

			/// Connect to the specified endpoint, raise an exception upon error.
			void connect(const Socket::endpoint_type& peer_endpoint);

			/// Receive a connection on the specified socket, raise an exception upon error.
			void receive(std::shared_ptr<Socket> socket);

			void close(); /// Close the connection.

			/// Read data, raise an exception on error.
			std::string read(std::size_t length);

			/// Write data, raise an exception on error.
			void write(const std::string& data);

		private:
			boost::asio::io_service m_service;
			std::shared_ptr<Socket> m_socket;

			enum class Source
			{
				NONE,
				TIMER,
				OPERATION,
			};
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

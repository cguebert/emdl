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
			using Socket = boost::asio::ip::tcp::socket;
			using duration_type = boost::asio::deadline_timer::duration_type;

			/// Destructor.
			~Transport();

			const boost::asio::io_service& service() const; /// Return the io_service.
			boost::asio::io_service& service();             /// Return the io_service.

			std::shared_ptr<const Socket> socket() const; /// Return the socket.
			std::shared_ptr<Socket> socket();             /// Return the socket.

			void setSocket(std::shared_ptr<Socket> socket); /// Set the socket, raise an exception if already set.

			void connect(const Socket::endpoint_type& endpoint); /// Connect to the specified endpoint, raise an exception upon error.
			bool isOpen() const;                                 /// Test whether the transport is open.
			void close();                                        /// Close the connection.

			std::string read(std::size_t length); /// Read data, raise an exception on error.
			void write(const std::string& data);  /// Write data, raise an exception on error.

		private:
			boost::asio::io_service m_service;
			std::shared_ptr<Socket> m_socket;
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

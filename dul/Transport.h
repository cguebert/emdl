#pragma once

#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/optional.hpp>

#include <emdl/emdl_api.h>
#include <emdl/Exception.h>

namespace emdl
{
	namespace dul
	{
		class StateMachine;

		struct PDUHeader
		{
			uint8_t type;
			uint8_t reserved;
			uint32_t size; // In Big-Endian
		};

		/// TCP transport for the DICOM Upper Layer.
		class EMDL_API Transport
		{
		public:
			using Socket = boost::asio::ip::tcp::socket;
			using duration_type = boost::asio::deadline_timer::duration_type;

			Transport(StateMachine& stateMachine);
			~Transport();

			const boost::asio::io_service& service() const; /// Return the io_service.
			boost::asio::io_service& service();             /// Return the io_service.

			Socket::endpoint_type remoteEndpoint() const; /// Return the remote endpoint of the socket, raise an exception if not connected.

			void setSocket(Socket socket); /// Set the socket, raise an exception if already set.

			void connect(const Socket::endpoint_type& endpoint); /// Connect to the specified endpoint, raise an exception upon error.
			bool isOpen() const;                                 /// Test whether the transport is open.
			void close();                                        /// Close the connection.

			void write(const std::string& data); /// Write data, raise an exception on error.

		private:
			void start();
			void stop();

			void readHeader();
			void readBody();
			void onError(boost::system::error_code ec);

			StateMachine& m_stateMachine;
			boost::asio::io_service m_service;
			boost::optional<Socket> m_socket;
			std::unique_ptr<std::thread> m_thread;

			PDUHeader m_readHeader;
			std::string m_readBody;
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

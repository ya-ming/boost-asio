@startuml
class boost::asio::io_context
class tcp::socket
class tcp::endpoint
class tcp::acceptor

class tcp_connection {
  +static pointer create(boost::asio::io_context& io_context)
  +tcp::socket& socket()
  +void start()
  -tcp_connection(boost::asio::io_context& io_context)
  -void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
  -tcp::socket socket_;
  -std::string message_;  
}
class tcp_server {
  +tcp_server(boost::asio::io_context& io_context)
  -start_accept()
  -void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)

  -boost::asio::io_context& io_context_;
  -tcp::acceptor acceptor_;
}

class udp::socket
class udp::endpoint

class udp_server {
  +udp_server(boost::asio::io_context& io_context)
  -void start_service()
  -void handle_receive(const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
  -void handle_send(boost::shared_ptr<std::string> /*message*/, const boost::system::error_code& /*error*/, std::size_t /*bytes_transferred*/

  -udp::socket socket_;
  -udp::endpoint remote_endpoint_;
  -boost::array<char, 1> recv_buffer_;
}

class main

main .. tcp_server
main .. boost::asio::io_context

tcp_server .. boost::asio::io_context
tcp_server .. tcp::acceptor
tcp_server .. tcp::endpoint
tcp_server .. tcp_connection

tcp_connection .. boost::asio::io_context
tcp_connection .. tcp::socket

main .. udp_server

udp_server .. boost::asio::io_context
udp_server .. udp::socket
udp_server .. udp::endpoint

@enduml

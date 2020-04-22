@startuml
class boost::asio::io_context
class tcp::socket
class tcp::endpoint
class tcp::acceptor

class session {
  +session(tcp::socket socket)
  +void start()
  -void do_read()
  -void do_write()
  -tcp::socket socket_
  -enum { max_length = 1024 }
  -char data_[max_length]
}
class server {
  +server(boost::asio::io_context& io_context, short port)
  -do_accept()

  -tcp::acceptor acceptor_
}

class main

main .. server
main .. boost::asio::io_context

server .. boost::asio::io_context
server .. tcp::acceptor
server .. tcp::endpoint
server .. session

session .. boost::asio::io_context
session .. tcp::socket

@enduml

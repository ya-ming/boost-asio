@startuml
class boost::asio::io_context
class udp::socket
class udp::endpoint

class server {
  +server(boost::asio::io_context& io_context, short port)
  +void start()
  -void do_receive()()
  -void do_send()
  -udp::socket socket_
  -udp:endpoint remote_endpoint_
  -enum { max_length = 1024 }
  -char data_[max_length]
}

class main

main .. server
main .. boost::asio::io_context

server .. boost::asio::io_context
server .. udp::endpoint
server .. udp::socket

@enduml

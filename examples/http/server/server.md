@startuml

class boost::asio::io_context
class tcp::acceptor
class tcp::resolver
class tcp::socket
class tcp::endpoint

class header {

}

class request {

}

class reply {

}

class mime_types {

}

class request_handler {
  +void handle_request(const request &req, reply &rep)
  -static bool url_decode(const std::string &in, std::string &out)
}

class request_parser {
  +parse()
}

class connection {
  +void start()
  +void stop()
  +void do_read()
  +void do_write()
  -tcp::socket socket_
  -std::array<char, 8192> buffer_
  -request request_
  -reply reply_
}

class connection_manager {
  +void start(connection_ptr c)
  +void stop(connection_ptr c)
  +void stop_all()
  -std::set<connection_ptr> connections_
}

class server {
  +void run()
  +void do_accept()
  +void do_wait_stop()
  -boost::asio::io_context io_context_
  -boost::asio::signal_set signals_
  -boost::asio::ip::tcp::acceptor acceptor_
  -connection_manager connection_manager_
  -request_handler request_handler_
}

class main {

}

request o..header
reply o.. header

request_handler .. request
request_handler .. reply
request_handler .. mime_types

connection .. tcp::socket
connection .. request_handler
connection .. request_parser

connection_manager o.. connection

server .. boost::asio::io_context
server .. tcp::acceptor
server .. tcp::endpoint
server .. tcp::socket
server .. tcp::resolver
server .. connection
server .. connection_manager
server .. request_handler

main .. server

@enduml

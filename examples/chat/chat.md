@startuml
class boost::asio::io_context
class tcp::socket
class tcp::endpoint
class tcp::acceptor

class chat_message {
  +chat_message()
  +const char* data() const
  +char* data()
  +std::size_t length() const
  +const char* body() const
  +char* body()
  +std::size_t body_length() const
  +void body_length(std::size_t new_length)
  +bool decode_header()
  +void encode_header()
  -char data_[header_length + max_body_length]
  -std::size_t body_length_
}

class chat_message_queue

package "Chat Server" {

class chat_participaint {
  +virtual ~chat_participaint() {}
  +virtual void deliver(const chat_message& msg) = 0
  -std::set<chat_participaint_ptr> participaints_
  -enum { max_recent_msgs = 100 }
  -chat_message_queue recent_msgs_
}

class chat_room {
  +void join(chat_participaint_ptr participaint)
  +void leave(chat_participaint_ptr participaint)
  +void deliver(const chat_message& msg)
}

class chat_session {
  +chat_session(tcp::socket socket, chat_room& room)
  +tcp::socket& socket()
  +void start()
  +void deliver(const chat_message& msg)
  -void do_read_header()
  -void do_read_body()
  -void do_write()
  -tcp::socket socket_
  -chat_room& room_
  -chat_message read_msg_
  -chat_message_queue write_msgs_
}

class chat_server {
  +chat_server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint)
  -do_accept()
  -tcp::acceptor acceptor_;
  -chat_room room_;
}

  chat_server .. boost::asio::io_context
  chat_server .. tcp::acceptor
  chat_server .. tcp::endpoint
  chat_server .. chat_room

  chat_session .. boost::asio::io_context
  chat_session .. tcp::socket
  chat_session --|> chat_participaint
  chat_session .. chat_message
  chat_session .. chat_message_queue
  chat_session .. chat_room

  chat_room --* participaint
  chat_room .. chat_message_queue
}

class main

main o-- chat_server
main .. boost::asio::io_context

chat_message_queue o-- chat_message

package "Chat Client" {

  class chat_client {
    +chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints
    +void write(const chat_message& msg)
    +void close()
    -void do_connect(const tcp::resolver::results_type& endpoints)
    -void do_read_header()
    -void do_read_body()
    -void do_write()
    -boost::asio::io_context& io_context_
    -tcp::socket socket_
    -chat_message read_msg_
    -chat_message_queue write_msgs_
  }

  chat_client .. boost::asio::io_context
  chat_client .. tcp::socket
  chat_client .. chat_message
  chat_client .. chat_message_queue
}
main .. chat_client
main .. tcp::resolver

@enduml

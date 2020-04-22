#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

/*** tcp server ***/
/* We will use shared_ptr and enable_shared_from_this because we want to 
   keep the tcp_connection object alive as long as there is an operation that refers to it. */
class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
  typedef boost::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context &io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket &socket()
  {
    return socket_;
  }

  /* In the function start(), we call boost::asio::async_write() to serve the data to the client.
     Note that we are using boost::asio::async_write(), rather than ip::tcp::socket::async_write_some(),
     to ensure that the entire block of data is sent. */
  void start()
  {
    /* The data to be sent is stored in the class member message_ as we need to keep the data valid until the asynchronous operation is complete. */
    message_ = make_daytime_string();

    /* When initiating the asynchronous operation, and if using boost::bind(), 
       you must specify only the arguments that match the handler's parameter list.
       In this program, both of the argument placeholders (boost::asio::placeholders::error and boost::asio::placeholders::bytes_transferred)
       could potentially have been removed, since they are not being used in handle_write(). */
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

private:
  tcp_connection(boost::asio::io_context &io_context)
      : socket_(io_context)
  {
  }

  /* Any further actions for this client connection are now the responsibility of handle_write(). */
  void handle_write(const boost::system::error_code & /*error*/,
                    size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  /* The constructor initialises an acceptor to listen on TCP port 13. */
  tcp_server(boost::asio::io_context &io_context)
      : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {

    start_accept();
  }

private:
  /* The function start_accept() creates a socket and initiates an asynchronous accept operation to wait for a new connection. */
  void start_accept()
  {
    tcp_connection::pointer new_connection = tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(
                               &tcp_server::handle_accept,
                               this,
                               new_connection,
                               boost::asio::placeholders::error));
  }

  /* The function handle_accept() is called when the asynchronous accept operation initiated by start_accept() finishes. 
     It services the client request, and then calls start_accept() to initiate the next accept operation. */
  void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code &error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_context &io_context_;
  tcp::acceptor acceptor_;
};
/*** end of tcp server ***/

/*** udp server ***/
class udp_server
{
public:
  /* The constructor initialises a socket to listen on UDP port 13. */
  udp_server(boost::asio::io_context &io_context)
      : socket_(io_context, udp::endpoint(udp::v4(), 13))
  {
    start_service();
  }

private:
  void start_service()
  {

    /* The function ip::udp::socket::async_receive_from() will cause the application to listen in the background for a new request.
       When such a request is received, the io_context object will invoke the handle_receive() function with two arguments: 
       a value of type boost::system::error_code indicating whether the operation succeeded or failed, 
       and a size_t value bytes_transferred specifying the number of bytes received. */
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
  }

  /* The function handle_receive() will service the client request. */
  void handle_receive(const boost::system::error_code &error, std::size_t /*bytes_transferred*/)
  {

    /* The error parameter contains the result of the asynchronous operation.
       Since we only provide the 1-byte recv_buffer_ to contain the client's request,
       the io_context object would return an error if the client sent anything larger.
       We can ignore such an error if it comes up. */
    if (!error)
    {
      /* Determine what we are going to send. */
      boost::shared_ptr<std::string> message(new std::string(make_daytime_string()));

      /* We now call ip::udp::socket::async_send_to() to serve the data to the client. */
      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                            /* When initiating the asynchronous operation, and if using boost::bind(), 
           you must specify only the arguments that match the handler's parameter list.
           In this program, both of the argument placeholders
           (boost::asio::placeholders::error and boost::asio::placeholders::bytes_transferred)
           could potentially have been removed. */
                            boost::bind(&udp_server::handle_send, this, message,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));

      /* Start listening for the next client request. */
      start_service();

      /* Any further actions for this client request are now the responsibility of handle_send(). */
    }
  }

  /* The function handle_send() is invoked after the service request has been completed. */
  void handle_send(boost::shared_ptr<std::string> /*message*/,
                   const boost::system::error_code & /*error*/,
                   std::size_t /*bytes_transferred*/
  )
  {
  }

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, 1> recv_buffer_;
};
/*** end of udp server ***/

int main()
{
  try
  {
    /* Create a server object to accept incoming client requests, and run the io_context object. */
    boost::asio::io_context io_context;
    tcp_server server1(io_context);
    udp_server server2(io_context);
    io_context.run();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
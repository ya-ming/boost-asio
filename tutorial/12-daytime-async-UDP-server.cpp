#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

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

int main()
{
  try
  {
    /* Create a server object to accept incoming client requests, and run the io_context object. */
    boost::asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
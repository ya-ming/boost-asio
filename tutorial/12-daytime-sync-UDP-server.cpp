#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    boost::asio::io_context io_context;

    /* Create an ip::udp::socket object to receive requests on UDP port 13. */
    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    /* Wait for a client to initiate contact with us.
       The remote_endpoint object will be populated by ip::udp::socket::receive_from(). */
    for (;;)
    {
      boost::array<char, 1> recv_buf;
      udp::endpoint remote_endpoint;
      boost::system::error_code error;

      socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);

      /* Determine what we are going to send back to the client. */
      std::string message = make_daytime_string();

      /* Send the response to the remote_endpoint. */
      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);
    }
  }
  /* Finally, handle any exceptions. */
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
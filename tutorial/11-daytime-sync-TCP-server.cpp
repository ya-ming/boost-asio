#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// We define the function make_daytime_string() to create the string to be sent back to the client.
// This function will be reused in all of our daytime server applications.
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

int main()
{
  try
  {
    boost::asio::io_context io_context;

    // A ip::tcp::acceptor object needs to be created to listen for new connections.
    // It is initialised to listen on TCP port 13, for IP version 4.
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

    // This is an iterative server, which means that it will handle one connection at a time.
    // Create a socket that will represent the connection to the client, and then wait for a connection.
    for (;;)
    {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      // A client is accessing our service.
      // Determine the current time and transfer this information to the client.
      std::string message = make_daytime_string();

      boost::system::error_code ignored_error;
      boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
  }
  // Finally, handle any exceptions.
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
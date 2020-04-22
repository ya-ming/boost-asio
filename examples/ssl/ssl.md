@startuml

==Server Init==

create io_context
server_main -> io_context

create server
server_main -> server: s(io_context, atoi(argv[1]))

server_main -> io_context: run()

==Client Init==

create io_context
client_main -> io_context

create resolver
client_main -> resolver: resolve()

create ssl_context
client_main -> ssl_context: boost::asio::ssl::context::sslv23

client_main -> ssl_context: load_verify_file("certificate.pem")

create client
client_main -> client: c(io_context, ctx, endpoints)

client_main -> io_context: run()

==Client and Server are Running==

server->server: do_accept() async_accept

client->client: connect() async_connect

server->session: start()

session->session: do_handshake() async_handshake

client->client: handshake() async_handshake

client->client: send_request() async_write

session->session: do_read() async_read_some

session->session: do_write() async_write

client->client: receive_response() async_read

server->server: do_accept() async_accept

@enduml

#include "../include/server.hpp"

void BaseServer::session(tcp::socket socket) {
  try {
    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    http::read(socket, buffer, req);
    http::response<http::string_body> res;
    auto ctx = Context(req, res);

    // Delegate the routing to the Router instance
    if (!router->route(ctx)) {
      // If no route matches, respond with Not Found
      res.result(http::status::not_found);
      res.body() = "Resource not found";
    }
    if (res.body().length() != 0)
      res.prepare_payload();

    http::write(socket, res);
  } catch (std::exception const &e) {
    std::cerr << "Session error: " << e.what() << std::endl;
  }
}

void BaseServer::setting() {
  beast::error_code ec;

  auto const address = net::ip::make_address("0.0.0.0");
  tcp::endpoint endpoint{address, port};
//  tcp::acceptor acceptor_{io_context};

  // Open the acceptor
  acceptor_.open(endpoint.protocol(), ec);
  if (ec) {
      std::cerr << "Open error: " << ec.message() << std::endl;
      return;
  }

  // Allow address reuse
  acceptor_.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
      std::cerr << "Set option error: " << ec.message() << std::endl;
      return;
  }

  // Bind to the server address
  acceptor_.bind(endpoint, ec);
  if (ec) {
      std::cerr << "Bind error: " << ec.message() << std::endl;
      return;
  }

  // Start listening for connections
  acceptor_.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
      std::cerr << "Listen error: " << ec.message() << std::endl;
      return;
  }
}

void BaseServer::run() {
  for (;;) {
    tcp::socket socket{io_context};
    acceptor_.accept(socket);
    std::thread(&Server::session, this, std::move(socket)).detach();
  }

  /*
  tcp::acceptor acceptor{
      io_context, {tcp::v4(), static_cast<boost::asio::ip::port_type>(port)}};

  for (;;) {
    tcp::socket socket{io_context};
    acceptor.accept(socket);
    std::thread(&Server::session, this, std::move(socket)).detach();
  }
  */
}

short BaseServer::getPort() { return port; }

//

void AsyncServer::run() {
  acceptor_.async_accept(net::make_strand(io_context),
    [this](beast::error_code ec, tcp::socket socket) {
        if (!ec) {
            session(std::move(socket));
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
        }
        // Start accepting the next connection
        run();
    });

}
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

BaseServer::~BaseServer() {
    beast::error_code ec;

    acceptor_.cancel(ec); // Cancel any pending operations
    if (ec) {
        std::cerr << "Cancel error: " << ec.message() << std::endl;
    }

    acceptor_.close(ec); // Close the acceptor
    if (ec) {
        std::cerr << "Close error: " << ec.message() << std::endl;
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
  std::cout << "run: " << std::endl;
  acceptor_.async_accept(net::make_strand(io_context),
    [this](beast::error_code ec, tcp::socket socket) {
        std::cout << "callback: " << std::endl;
        if (!ec) {
            BaseServer::session(std::move(socket));
        } else {
            std::cerr << "Accept error: " << ec.message() << std::endl;
        }
        std::cout << "end: " << std::endl;
        AsyncServer::run();
    });

  // Run the io_context in a separate thread if desired
  //std::thread t([&ioc]() { ioc.run(); });
  io_context.run();
}

/*
The io_context.run() function in Boost.Asio is a crucial part of the library’s asynchronous I/O operations. Here’s what it does:

Event Loop: io_context.run() starts an event loop that processes all the asynchronous operations (handlers) that have been scheduled on the io_context. This loop continues running until there are no more handlers to execute or the io_context is stopped.

Blocking Call: The run() function blocks the calling thread until all work has finished. This means it will keep the thread busy processing I/O events until there are no more events to handle or the io_context is explicitly stopped1.

Thread Pool: Multiple threads can call io_context.run() to create a pool of threads that can concurrently process handlers. This is useful for improving performance in multi-threaded applications

!!!
 If you are not using any asynchronous operations in Boost.Asio, you don’t need to call io_context.run(). The io_context.run() function is specifically designed to process asynchronous tasks and events. If your application only uses synchronous operations, you can handle those operations directly without involving io_context.run().
*/
#pragma once

#include "context.hpp"
#include "router.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
//#include <boost/json.hpp>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

#define GET (http::verb::get)
#define POST (http::verb::post)
#define PUT (http::verb::put)
#define PATCH (http::verb::patch)
#define DELETE (http::verb::delete_)

class IServer {
public:
  virtual void setting() = 0;
  virtual void session(tcp::socket socket) = 0;
  virtual void run() = 0;
  virtual short getPort() = 0;
};

class BaseServer : public IServer {
protected:
  unsigned short port;
  std::shared_ptr<Router> router;
  net::io_context io_context{1};
  tcp::acceptor acceptor_;

public:
  BaseServer(short port, std::shared_ptr<Router> router)
    : port(port), router(std::move(router)), acceptor_(net::make_strand(io_context))
  {
    setting();
  }

public:
  void setting() override;
  void session(tcp::socket socket) override;
  void run() override;
  short getPort() override;
};

class Server : public BaseServer {
public:
  Server(short port, std::shared_ptr<Router> router)
    : BaseServer(port, router) {}
};

class AsyncServer : public BaseServer {
public:
  AsyncServer(short port, std::shared_ptr<Router> router)
    : BaseServer(port, router) {}

public:
  void run() override;
};
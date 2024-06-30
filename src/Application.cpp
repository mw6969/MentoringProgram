#include "Application.h"
#include "Client.h"
#include "Cryptor.h"
#include "Logger.h"
#include "Server.h"

#include <boost/asio/io_service.hpp>

constexpr std::string host("localhost");
constexpr unsigned short port(9999);

Application::Application(int argc, char *argv[])
    : mode_(argc > 1 ? argv[1] : "") {
  for (int i = 2; i < argc; ++i) {
    fileNames_.push_back(argv[i]);
  }
}

void Application::run() {
  auto cryptor = std::make_shared<Cryptor>();
  Logger &logger = Logger::getInstance();
  boost::asio::io_service ioService;
  try {
    if (mode_ == "server") {
      Server server(ioService, port, cryptor);
      ioService.run();
    } else if (mode_ == "client") {
      if (fileNames_.empty()) {
        logger.log("You must provide at least one filename for client mode");
      }
      Client client(ioService, host, port, cryptor);
      client.sendFiles(std::move(fileNames_));
      logger.log("Files sent");
    } else {
      logger.log("Invalid mode. Use server or client");
    }
  } catch (const std::exception &e) {
    logger.log(e.what());
  }
}
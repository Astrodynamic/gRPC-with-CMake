#include "server.h"

#include <iostream>

Server::Server() {
  m_service = std::make_unique<RobotControlImpl>();
}

void Server::Run(const std::string& address) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(m_service.get());

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

  std::cout << "Server listening on " << address << std::endl;

  server->Wait();
}
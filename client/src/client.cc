#include "client.h"

#include <iostream>
#include <string>

RobotControlClient::RobotControlClient(std::shared_ptr<grpc::Channel> channel) : m_stub(robot::RobotControl::NewStub(channel)) {}

void RobotControlClient::Move(int x, int y) {
  robot::MoveRequest request;
  request.set_x(x);
  request.set_y(y);

  robot::MoveResponse response;
  grpc::ClientContext context;

  grpc::Status status = m_stub->Move(&context, request, &response);

  if (status.ok()) {
    std::cout << "Move response: " << response.message() << std::endl;
  } else {
    std::cout << "Move RPC failed." << std::endl;
  }
}

void RobotControlClient::Stop() {
  robot::StopRequest request;
  robot::StopResponse response;
  grpc::ClientContext context;

  grpc::Status status = m_stub->Stop(&context, request, &response);

  if (status.ok()) {
    std::cout << "Stop response: " << response.message() << std::endl;
  } else {
    std::cout << "Stop RPC failed." << std::endl;
  }
}
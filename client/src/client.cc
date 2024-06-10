#include "client.h"

#include <iostream>
#include <string>

RobotControlClient::RobotControlClient(std::shared_ptr<grpc::Channel> channel) : m_stub(robot::RobotControl::NewStub(channel)) {
  m_cq = std::make_unique<grpc::CompletionQueue>();
}

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

void RobotControlClient::AsyncMove(int x, int y) {
  robot::MoveRequest request;
  request.set_x(x);
  request.set_y(y);

  robot::MoveResponse response;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::MoveResponse>> rpc(m_stub->AsyncMove(&context, request, m_cq.get()));

  grpc::Status status;
  rpc->Finish(&response, &status, (void*)1);

  void* got_tag;
  bool ok = false;
  if (m_cq->Next(&got_tag, &ok) && ok && got_tag == (void*)1) {
    if (status.ok()) {
      std::cout << "Move response: " << response.message() << std::endl;
    } else {
      std::cout << "Move RPC failed." << std::endl;
    }
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

void RobotControlClient::AsyncStop() {
  robot::StopRequest request;
  robot::StopResponse response;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::StopResponse>> rpc(m_stub->AsyncStop(&context, request, m_cq.get()));

  grpc::Status status;
  rpc->Finish(&response, &status, (void*)1);

  void* got_tag;
  bool ok = false;
  if (m_cq->Next(&got_tag, &ok) && ok && got_tag == (void*)1) {
    if (status.ok()) {
      std::cout << "Stop response: " << response.message() << std::endl;
    } else {
      std::cout << "Stop RPC failed." << std::endl;
    }
  }
}

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "myproto/control.grpc.pb.h"

class RobotControlClient {
 public:
  RobotControlClient(std::shared_ptr<grpc::Channel> channel) : stub_(robot::RobotControl::NewStub(channel)) {}

  void Move(int x, int y) {
    robot::MoveRequest request;
    request.set_x(x);
    request.set_y(y);

    robot::MoveResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Move(&context, request, &response);

    if (status.ok()) {
      std::cout << "Move response: " << response.message() << std::endl;
    } else {
      std::cout << "Move RPC failed." << std::endl;
    }
  }

  void Stop() {
    robot::StopRequest request;
    robot::StopResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->Stop(&context, request, &response);

    if (status.ok()) {
      std::cout << "Stop response: " << response.message() << std::endl;
    } else {
      std::cout << "Stop RPC failed." << std::endl;
    }
  }

 private:
  std::unique_ptr<robot::RobotControl::Stub> stub_;
};

int main(int argc, char** argv) {
  RobotControlClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  std::cout << "Moving to (10, 20)" << std::endl;
  client.Move(10, 20);

  std::cout << "Stopping robot" << std::endl;
  client.Stop();

  return 0;
}

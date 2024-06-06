#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "myproto/control.grpc.pb.h"

class RobotControlServiceImpl final : public robot::RobotControl::Service {
  grpc::Status Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) override {
    response->set_message("Moved to (" + std::to_string(request->x()) + ", " + std::to_string(request->y()) + ")");
    return grpc::Status::OK;
  }

  grpc::Status Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) override {
    response->set_message("Robot stopped");
    return grpc::Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");

  RobotControlServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}

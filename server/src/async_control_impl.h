#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>

#include "robot/control.grpc.pb.h"

class RobotControlAsyncServerImpl final {
 public:
  ~RobotControlAsyncServerImpl();

  auto Run(std::string soket) -> void;
  auto Shutdown() -> void;

 private:
  auto HandleRpcs() -> void;

  std::unique_ptr<grpc::ServerCompletionQueue> m_cq;
  robot::RobotControl::AsyncService m_service;
  std::unique_ptr<grpc::Server> m_server;
};

#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>
#include <string>

#include "robot/control.grpc.pb.h"

class RobotControlCallbackImpl final : public robot::RobotControl::CallbackService {
  auto Move(grpc::CallbackServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) -> grpc::ServerUnaryReactor* override;
  auto Stop(grpc::CallbackServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) -> grpc::ServerUnaryReactor* override;
};

class RobotControlCallbackServerImpl final {
 public:
  RobotControlCallbackServerImpl();

  auto Run(const std::string& soket) -> void;

 private:
  std::unique_ptr<RobotControlCallbackImpl> m_service;
};
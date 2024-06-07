#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>

#include "robot/control.grpc.pb.h"

class RobotControlClient {
 public:
  explicit RobotControlClient(std::shared_ptr<grpc::Channel> channel);

  void Move(int x, int y);
  void AsyncMove(int x, int y);

  void Stop();
  void AsyncStop();

 private:
  std::unique_ptr<robot::RobotControl::Stub> m_stub;
  std::unique_ptr<grpc::CompletionQueue> m_cq;
};
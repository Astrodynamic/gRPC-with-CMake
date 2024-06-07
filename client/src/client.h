#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>

#include "robot/control.grpc.pb.h"

class RobotControlClient {
 public:
  RobotControlClient(std::shared_ptr<grpc::Channel> channel);

  void Move(int x, int y);
  void Stop();

 private:
  std::unique_ptr<robot::RobotControl::Stub> m_stub;
};
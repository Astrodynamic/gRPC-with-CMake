#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>

#include "robot/control.grpc.pb.h"

class RobotControlAsyncClientImpl final {
 public:
  explicit RobotControlAsyncClientImpl(std::shared_ptr<grpc::Channel> channel);
  virtual ~RobotControlAsyncClientImpl();

  void Run();
  void Shutdown();

  void Move(int x, int y);
  void AsyncMove(int x, int y);
  void AsyncMove2(int x, int y);

  void Stop();
  void AsyncStop();
  void AsyncStop2();

 private:
  void HandleRpcs();

  std::unique_ptr<robot::RobotControl::Stub> m_stub;
  std::unique_ptr<grpc::CompletionQueue> m_cq;
};

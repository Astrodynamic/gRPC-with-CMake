#pragma once

#include <grpcpp/grpcpp.h>

#include <string>
#include <memory>
#include <future>

#include "robot/control.grpc.pb.h"

class RobotControlAsyncClientImpl final {
 public:
  explicit RobotControlAsyncClientImpl(std::shared_ptr<grpc::Channel> channel);
  virtual ~RobotControlAsyncClientImpl();

  auto Run() -> void;
  auto Shutdown() -> void;

  auto Move(int x, int y) -> void;
  auto AsyncMove(int x, int y) -> void;
  auto AsyncMove2(int x, int y) -> void;
  auto AsyncMove3(int x, int y) -> std::future<robot::MoveResponse>;

  auto CallbackMove(int x, int y) -> std::string;

  auto Stop() -> void;
  auto AsyncStop() -> void;
  auto AsyncStop2() -> void;
  auto AsyncStop3() -> std::future<robot::StopResponse>;

  auto CallbackStop() -> std::string;

 private:
  auto HandleRpcs() -> void;

  std::unique_ptr<robot::RobotControl::Stub> m_stub;
  std::unique_ptr<grpc::CompletionQueue> m_cq;
};

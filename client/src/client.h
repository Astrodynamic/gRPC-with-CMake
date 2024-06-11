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
  struct CallData {
    virtual ~CallData() = default;
    virtual void Proceed(bool ok) = 0;

    grpc::ClientContext m_ctx;
    grpc::Status m_status;
  };

  struct MoveCallData : public CallData {
    void Proceed(bool ok) override;

    robot::MoveResponse m_response;
    std::unique_ptr<grpc::ClientAsyncResponseReader<robot::MoveResponse>> m_responder;
  };

  struct StopCallData : public CallData {
    void Proceed(bool ok) override;

    robot::StopResponse m_response;
    std::unique_ptr<grpc::ClientAsyncResponseReader<robot::StopResponse>> m_responder;
  };

  void HandleRpcs();

  std::unique_ptr<robot::RobotControl::Stub> m_stub;
  std::unique_ptr<grpc::CompletionQueue> m_cq;
};
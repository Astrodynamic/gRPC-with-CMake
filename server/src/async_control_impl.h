#pragma once

#include <grpcpp/grpcpp.h>

#include <memory>

#include "robot/control.grpc.pb.h"

class RobotControlAsyncImpl final {
 public:
  ~RobotControlAsyncImpl();

  void Run();

 private:
  class CallData {
   public:
    virtual ~CallData() = default;
    virtual void Proceed(bool ok) = 0;

   protected:
    CallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);

    enum CallStatus { CREATE,
                      PROCESS,
                      FINISH };

    robot::RobotControl::AsyncService* m_service;
    grpc::ServerCompletionQueue* m_cq;
    grpc::ServerContext m_ctx;

    CallStatus m_status;
  };

  class MoveCallData : public CallData {
   public:
    MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
    void Proceed(bool ok) override;

   private:
    robot::MoveRequest m_request;
    robot::MoveResponse m_response;
    grpc::ServerAsyncResponseWriter<robot::MoveResponse> m_responder;
  };

  class StopCallData : public CallData {
   public:
    StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
    void Proceed(bool ok) override;

   private:
    robot::StopRequest m_request;
    robot::StopResponse m_response;
    grpc::ServerAsyncResponseWriter<robot::StopResponse> m_responder;
  };

  void HandleRpcs();

  std::unique_ptr<grpc::ServerCompletionQueue> m_cq;
  robot::RobotControl::AsyncService m_service;
  std::unique_ptr<grpc::Server> m_server;
};

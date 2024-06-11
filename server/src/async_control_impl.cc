#include "async_control_impl.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

class CallData {
 public:
  virtual ~CallData() = default;
  virtual auto Proceed(bool ok) -> void = 0;

 protected:
  CallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);

  enum class CallStatus { CREATE,
                          PROCESS,
                          FINISH };

  robot::RobotControl::AsyncService* m_service;
  grpc::ServerCompletionQueue* m_cq;
  grpc::ServerContext m_ctx;

  CallStatus m_status;
};

CallData::CallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : m_service(service), m_cq(cq), m_status(CallStatus::CREATE) {}

class MoveCallData : public CallData {
 public:
  MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
  auto Proceed(bool ok) -> void override;

 private:
  robot::MoveRequest m_request;
  robot::MoveResponse m_response;
  grpc::ServerAsyncResponseWriter<robot::MoveResponse> m_responder;
};

MoveCallData::MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

auto MoveCallData::Proceed(bool ok) -> void {
  if (m_status == CallStatus::CREATE) {
    m_status = CallStatus::PROCESS;
    m_service->RequestMove(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
  } else if (m_status == CallStatus::PROCESS) {
    new MoveCallData(m_service, m_cq);
    m_response.set_message("Moved to (" + std::to_string(m_request.x()) + ", " + std::to_string(m_request.y()) + ")");

    m_status = CallStatus::FINISH;
    m_responder.Finish(m_response, grpc::Status::OK, this);
  } else {
    delete this;
  }
}

class StopCallData : public CallData {
 public:
  StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
  auto Proceed(bool ok) -> void override;

 private:
  robot::StopRequest m_request;
  robot::StopResponse m_response;
  grpc::ServerAsyncResponseWriter<robot::StopResponse> m_responder;
};

StopCallData::StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

auto StopCallData::Proceed(bool ok) -> void {
  if (m_status == CallStatus::CREATE) {
    m_status = CallStatus::PROCESS;
    m_service->RequestStop(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
  } else if (m_status == CallStatus::PROCESS) {
    new StopCallData(m_service, m_cq);
    m_response.set_message("Robot stopped");
    m_status = CallStatus::FINISH;
    m_responder.Finish(m_response, grpc::Status::OK, this);
  } else {
    delete this;
  }
}

RobotControlAsyncServerImpl::~RobotControlAsyncServerImpl() {
  Shutdown();
}

auto RobotControlAsyncServerImpl::Run(std::string soket) -> void {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(soket, grpc::InsecureServerCredentials());
  builder.RegisterService(&m_service);
  m_cq = builder.AddCompletionQueue();
  m_server = builder.BuildAndStart();
  std::cout << "Server listening on " << soket << std::endl;

  HandleRpcs();
}

auto RobotControlAsyncServerImpl::Shutdown() -> void {
  m_server->Shutdown();
  m_cq->Shutdown();
}

auto RobotControlAsyncServerImpl::HandleRpcs() -> void {
  new MoveCallData(&m_service, m_cq.get());
  new StopCallData(&m_service, m_cq.get());

  void* tag;
  bool ok;
  while (true) {
    grpc::CompletionQueue::NextStatus status = m_cq->AsyncNext(&tag, &ok, gpr_time_from_seconds(1, GPR_TIMESPAN));
    if (status == grpc::CompletionQueue::GOT_EVENT) {
      static_cast<CallData*>(tag)->Proceed(ok);
    } else if (status != grpc::CompletionQueue::TIMEOUT) {
      std::cout << "Unknown completion queue status: " << status << std::endl;
    } else if (status == grpc::CompletionQueue::SHUTDOWN) {
      break;
    }
  }
}

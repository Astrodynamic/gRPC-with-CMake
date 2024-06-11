#include "async_control_impl.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

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

CallData::CallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : m_service(service), m_cq(cq), m_status(CREATE) {}

class MoveCallData : public CallData {
 public:
  MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
  void Proceed(bool ok) override;

 private:
  robot::MoveRequest m_request;
  robot::MoveResponse m_response;
  grpc::ServerAsyncResponseWriter<robot::MoveResponse> m_responder;
};

MoveCallData::MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

void MoveCallData::Proceed(bool ok) {
  if (m_status == CREATE) {
    m_status = PROCESS;
    m_service->RequestMove(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
  } else if (m_status == PROCESS) {
    new MoveCallData(m_service, m_cq);
    m_response.set_message("Moved to (" + std::to_string(m_request.x()) + ", " + std::to_string(m_request.y()) + ")");

    m_status = FINISH;
    m_responder.Finish(m_response, grpc::Status::OK, this);
  } else {
    delete this;
  }
}

class StopCallData : public CallData {
 public:
  StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq);
  void Proceed(bool ok) override;

 private:
  robot::StopRequest m_request;
  robot::StopResponse m_response;
  grpc::ServerAsyncResponseWriter<robot::StopResponse> m_responder;
};

StopCallData::StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

void StopCallData::Proceed(bool ok) {
  if (m_status == CREATE) {
    m_status = PROCESS;
    m_service->RequestStop(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
  } else if (m_status == PROCESS) {
    new StopCallData(m_service, m_cq);
    m_response.set_message("Robot stopped");
    m_status = FINISH;
    m_responder.Finish(m_response, grpc::Status::OK, this);
  } else {
    delete this;
  }
}

RobotControlAsyncServerImpl::~RobotControlAsyncServerImpl() {
  Shutdown();
}

void RobotControlAsyncServerImpl::Run(std::string soket) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(soket, grpc::InsecureServerCredentials());
  builder.RegisterService(&m_service);
  m_cq = builder.AddCompletionQueue();
  m_server = builder.BuildAndStart();
  std::cout << "Server listening on " << soket << std::endl;

  HandleRpcs();
}

void RobotControlAsyncServerImpl::Shutdown() {
  m_server->Shutdown();
  m_cq->Shutdown();
}

void RobotControlAsyncServerImpl::HandleRpcs() {
  new MoveCallData(&m_service, m_cq.get());
  new StopCallData(&m_service, m_cq.get());

  void* tag;
  bool ok;
  while (m_cq->Next(&tag, &ok)) {
    static_cast<CallData*>(tag)->Proceed(ok);
  }
}

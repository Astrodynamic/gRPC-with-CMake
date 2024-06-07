#include "async_control_impl.h"

#include <iostream>
#include <string>
#include <thread>
#include <vector>

RobotControlAsyncImpl::~RobotControlAsyncImpl() {
  m_server->Shutdown();
  m_cq->Shutdown();
}

void RobotControlAsyncImpl::Run() {
  std::string server_address("0.0.0.0:50051");

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&m_service);
  m_cq = builder.AddCompletionQueue();
  m_server = builder.BuildAndStart();
  std::cout << "Server listening on " << server_address << std::endl;

  HandleRpcs();
}

RobotControlAsyncImpl::CallData::CallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : m_service(service), m_cq(cq), m_status(CREATE) {}

RobotControlAsyncImpl::MoveCallData::MoveCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

void RobotControlAsyncImpl::MoveCallData::Proceed(bool ok) {
  if (m_status == CREATE) {
    m_service->RequestMove(&m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
    m_status = PROCESS;
  } else if (m_status == PROCESS) {
    new MoveCallData(m_service, m_cq);
    m_response.set_message("Moved to (" + std::to_string(m_request.x()) + ", " + std::to_string(m_request.y()) + ")");
    m_responder.Finish(m_response, grpc::Status::OK, this);
    m_status = FINISH;
  } else {
    delete this;
  }
}

RobotControlAsyncImpl::StopCallData::StopCallData(robot::RobotControl::AsyncService* service, grpc::ServerCompletionQueue* cq) : CallData(service, cq), m_responder(&m_ctx) {
  Proceed(true);
}

void RobotControlAsyncImpl::StopCallData::Proceed(bool ok) {
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

void RobotControlAsyncImpl::HandleRpcs() {
  new MoveCallData(&m_service, m_cq.get());
  new StopCallData(&m_service, m_cq.get());

  void* tag;
  bool ok;
  while (m_cq->Next(&tag, &ok)) {
    static_cast<CallData*>(tag)->Proceed(ok);
  }
}
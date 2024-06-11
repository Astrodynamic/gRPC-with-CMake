#include "client.h"

#include <iostream>
#include <string>

struct CallData {
  virtual ~CallData() = default;
  virtual auto Proceed(bool ok) -> void = 0;

  grpc::ClientContext m_ctx;
  grpc::Status m_status;
};

struct MoveCallData : public CallData {
  auto Proceed(bool ok) -> void override;

  robot::MoveResponse m_response;
  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::MoveResponse>> m_responder;
};

auto MoveCallData::Proceed(bool ok) -> void {
  if (m_status.ok()) {
    std::cout << "Move response: " << m_response.message() << std::endl;
  } else {
    std::cout << "Move RPC failed." << std::endl;
  }
  delete this;
}

struct StopCallData : public CallData {
  auto Proceed(bool ok) -> void override;

  robot::StopResponse m_response;
  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::StopResponse>> m_responder;
};

auto StopCallData::Proceed(bool ok) -> void {
  if (m_status.ok()) {
    std::cout << "Stop response: " << m_response.message() << std::endl;
  } else {
    std::cout << "Stop RPC failed." << std::endl;
  }
  delete this;
}

RobotControlAsyncClientImpl::RobotControlAsyncClientImpl(std::shared_ptr<grpc::Channel> channel) : m_stub(robot::RobotControl::NewStub(channel)) {
  m_cq = std::make_unique<grpc::CompletionQueue>();
}

RobotControlAsyncClientImpl::~RobotControlAsyncClientImpl() {
  Shutdown();
}

auto RobotControlAsyncClientImpl::Run() -> void {
  HandleRpcs();
}

auto RobotControlAsyncClientImpl::Shutdown() -> void {
  m_cq->Shutdown();
}

auto RobotControlAsyncClientImpl::Move(int x, int y) -> void {
  robot::MoveRequest request;
  request.set_x(x);
  request.set_y(y);

  robot::MoveResponse response;
  grpc::ClientContext context;

  grpc::Status status = m_stub->Move(&context, request, &response);

  if (status.ok()) {
    std::cout << "Move response: " << response.message() << std::endl;
  } else {
    std::cout << "Move RPC failed." << std::endl;
  }
}

auto RobotControlAsyncClientImpl::AsyncMove(int x, int y) -> void {
  robot::MoveRequest request;
  request.set_x(x);
  request.set_y(y);

  robot::MoveResponse response;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::MoveResponse>> rpc(m_stub->AsyncMove(&context, request, m_cq.get()));

  grpc::Status status;
  rpc->Finish(&response, &status, (void*)1);

  void* got_tag;
  bool ok = false;
  if (m_cq->Next(&got_tag, &ok) && ok && got_tag == (void*)1) {
    if (status.ok()) {
      std::cout << "Move response: " << response.message() << std::endl;
    } else {
      std::cout << "Move RPC failed." << std::endl;
    }
  }
}

auto RobotControlAsyncClientImpl::AsyncMove2(int x, int y) -> void {
  robot::MoveRequest request;
  request.set_x(x);
  request.set_y(y);

  MoveCallData* call = new MoveCallData();
  call->m_responder = m_stub->PrepareAsyncMove(&call->m_ctx, request, m_cq.get());
  call->m_responder->StartCall();
  call->m_responder->Finish(&call->m_response, &call->m_status, (void*)call);
}

auto RobotControlAsyncClientImpl::Stop() -> void {
  robot::StopRequest request;
  robot::StopResponse response;
  grpc::ClientContext context;

  grpc::Status status = m_stub->Stop(&context, request, &response);

  if (status.ok()) {
    std::cout << "Stop response: " << response.message() << std::endl;
  } else {
    std::cout << "Stop RPC failed." << std::endl;
  }
}

auto RobotControlAsyncClientImpl::AsyncStop() -> void {
  robot::StopRequest request;
  robot::StopResponse response;
  grpc::ClientContext context;

  std::unique_ptr<grpc::ClientAsyncResponseReader<robot::StopResponse>> rpc(m_stub->AsyncStop(&context, request, m_cq.get()));

  grpc::Status status;
  rpc->Finish(&response, &status, (void*)1);

  void* got_tag;
  bool ok = false;
  if (m_cq->Next(&got_tag, &ok) && ok && got_tag == (void*)1) {
    if (status.ok()) {
      std::cout << "Stop response: " << response.message() << std::endl;
    } else {
      std::cout << "Stop RPC failed." << std::endl;
    }
  }
}

auto RobotControlAsyncClientImpl::AsyncStop2() -> void {
  robot::StopRequest request;

  StopCallData* call = new StopCallData();
  call->m_responder = m_stub->PrepareAsyncStop(&call->m_ctx, request, m_cq.get());
  call->m_responder->StartCall();
  call->m_responder->Finish(&call->m_response, &call->m_status, (void*)call);
}

auto RobotControlAsyncClientImpl::HandleRpcs() -> void {
  void* tag;
  bool ok;
  while (m_cq->Next(&tag, &ok)) {
    static_cast<CallData*>(tag)->Proceed(ok);
  }
}
#include "callback_control_impl.h"

#include <string>

auto RobotControlCallbackImpl::Move(grpc::CallbackServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) -> grpc::ServerUnaryReactor* {
  response->set_message("Moved to (" + std::to_string(request->x()) + ", " + std::to_string(request->y()) + ")");
  
  grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
  reactor->Finish(grpc::Status::OK);

  return reactor;
}

auto RobotControlCallbackImpl::Stop(grpc::CallbackServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) -> grpc::ServerUnaryReactor* {
  response->set_message("Robot stopped");
  
  grpc::ServerUnaryReactor* reactor = context->DefaultReactor();
  reactor->Finish(grpc::Status::OK);
  
  return reactor;
}

RobotControlCallbackServerImpl::RobotControlCallbackServerImpl() {
  m_service = std::make_unique<RobotControlCallbackImpl>();
}

auto RobotControlCallbackServerImpl::Run(const std::string& soket) -> void {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(soket, grpc::InsecureServerCredentials());
  builder.RegisterService(m_service.get());

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

  std::cout << "Server listening on " << soket << std::endl;

  server->Wait();
}
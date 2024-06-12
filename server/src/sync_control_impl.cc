#include "sync_control_impl.h"

#include <string>

auto RobotControlImpl::Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) -> grpc::Status {
  response->set_message("Moved to (" + std::to_string(request->x()) + ", " + std::to_string(request->y()) + ")");
  return grpc::Status::OK;
}

auto RobotControlImpl::Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) -> grpc::Status {
  response->set_message("Robot stopped");
  return grpc::Status::OK;
}

RobotControlSyncServerImpl::RobotControlSyncServerImpl() {
  m_service = std::make_unique<RobotControlImpl>();
}

auto RobotControlSyncServerImpl::Run(const std::string& soket) -> void {
  grpc::ServerBuilder builder;
  builder.AddListeningPort(soket, grpc::InsecureServerCredentials());
  builder.RegisterService(m_service.get());

  std::unique_ptr<grpc::Server> server = builder.BuildAndStart();

  std::cout << "Server listening on " << soket << std::endl;

  server->Wait();
}
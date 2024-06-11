#include "control_impl.h"

#include <string>

auto RobotControlImpl::Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) -> grpc::Status {
  response->set_message("Moved to (" + std::to_string(request->x()) + ", " + std::to_string(request->y()) + ")");
  return grpc::Status::OK;
}

auto RobotControlImpl::Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) -> grpc::Status {
  response->set_message("Robot stopped");
  return grpc::Status::OK;
}

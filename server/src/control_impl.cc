#include "control_impl.h"

#include <string>

grpc::Status RobotControlImpl::Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) {
  response->set_message("Moved to (" + std::to_string(request->x()) + ", " + std::to_string(request->y()) + ")");
  return grpc::Status::OK;
}

grpc::Status RobotControlImpl::Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) {
  response->set_message("Robot stopped");
  return grpc::Status::OK;
}

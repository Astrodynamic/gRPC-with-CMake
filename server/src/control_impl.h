#pragma once

#include <grpcpp/grpcpp.h>

#include "robot/control.grpc.pb.h"

class RobotControlImpl final : public robot::RobotControl::Service {
  auto Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) -> grpc::Status override;
  auto Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) -> grpc::Status override;
};
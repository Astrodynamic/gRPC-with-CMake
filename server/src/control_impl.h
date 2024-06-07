#pragma once

#include <grpcpp/grpcpp.h>

#include "robot/control.grpc.pb.h"

class RobotControlImpl final : public robot::RobotControl::Service {
  grpc::Status Move(grpc::ServerContext* context, const robot::MoveRequest* request, robot::MoveResponse* response) override;
  grpc::Status Stop(grpc::ServerContext* context, const robot::StopRequest* request, robot::StopResponse* response) override;
};
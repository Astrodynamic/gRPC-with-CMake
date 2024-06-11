#pragma once

#include <memory>
#include <string>

#include "control_impl.h"

class Server final {
 public:
  Server();

  auto Run(const std::string& address) -> void;

 private:
  std::unique_ptr<RobotControlImpl> m_service;
};
#pragma once

#include <string>
#include <memory>

#include "control_impl.h"

class Server final {
 public:
  Server();

  void Run(const std::string& address);

 private:
  std::unique_ptr<RobotControlImpl> m_service;
};
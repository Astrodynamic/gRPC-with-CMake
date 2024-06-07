#include <iostream>

#include "client.h"

int main(int argc, char** argv) {
  RobotControlClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

  std::cout << "Moving to (10, 20)" << std::endl;
  client.Move(10, 20);

  std::cout << "Stopping robot" << std::endl;
  client.Stop();

  return 0;
}

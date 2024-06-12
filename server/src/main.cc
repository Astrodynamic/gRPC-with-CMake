#include "sync_control_impl.h"
#include "async_control_impl.h"
#include "callback_control_impl.h"

int main(int argc, char** argv) {
  // RobotControlSyncServerImpl server;
  // server.Run("0.0.0.0:50051");

  RobotControlAsyncServerImpl server;
  server.Run("0.0.0.0:50051");

  // RobotControlCallbackServerImpl server;
  // server.Run("0.0.0.0:50051");
  
  return 0;
}

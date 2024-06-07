#include "server.h"

int main(int argc, char** argv) {
  Server server;
  server.Run("0.0.0.0:50051");
  return 0;
}

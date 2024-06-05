#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <iostream>

class AddressBookImpl final : public example::AddressBook::Service {
 public:
  virtual ::grpc::Status GetAddress(::grpc::ServerContext* context, const ::example::NameQuerry* request, ::example::Address* response) {
    std::cout << "Server: GetAddress for \"" << request->name() << "\"." << std::endl;

    response->set_name("Peter Peterson");
    response->set_zip("12345");
    response->set_country("Superland");

    return grpc::Status::OK;
  }
};

int main(int argc, char* argv[]) {
  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

  AddressBookImpl my_service;
  builder.RegisterService(&my_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();

  return 0;
}

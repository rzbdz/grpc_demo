//
// Created by A1134 on 2022/4/9.
//

#include "book_system_server.h"

void RunGrpcServer(const string& a, const string& b, const string& c) {
  std::string server_address("0.0.0.0:50051");
  BookSystemImpl service{a, b, c};
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  if (argc < 4) {
    cout << "usage: ./server mysql_addr sql_user sql_passwd\n";
    return 0;
  }
  RunGrpcServer(argv[1], argv[2], argv[3]);
  return 0;
}
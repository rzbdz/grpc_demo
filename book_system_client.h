//
// Created by A1134 on 2022/4/9.
//

#ifndef DIS_3_BOOK_SYSTEM_CLIENT_H
#define DIS_3_BOOK_SYSTEM_CLIENT_H

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include "book_system.grpc.pb.h"

using disc3::Book;
using disc3::BookSystem;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

class BookSystemClient {
 public:
  BookSystemClient(std::shared_ptr<Channel> channel)
      : stub_(BookSystem::NewStub(channel)) {}

  void Add(string name, int32_t id) {
    Book book;
    book.set_name(name);
    book.set_id(id);
    ClientContext context;
    google::protobuf::BoolValue res;
    stub_->Add(&context, book, &res);
    if (res.value()) {
      cout << "Add book " << name << "," << id << " success\n";
    } else {
      cout << "Add book " << name << "," << id << " fails\n";
    }
  }

  void QueryById(int32_t id) {
    ClientContext context;
    Book book;
    google::protobuf::Int32Value gid;
    gid.set_value(id);
    if (stub_->QueryById(&context, gid, &book).ok()) {
      cout << "Query book " << book.name() << "," << id << " success\n";
    } else {
      cout << "Query book " << id << " fails\n";
    }
  }
  void QueryByName(string match) {
    ClientContext context;
    Book book;
    google::protobuf::StringValue val;
    val.set_value(match);
    std::shared_ptr<ClientReader<Book> > stream(
        stub_->QueryByName(&context, val));
    cout << "Querying book with partial/complete name: `" << match << "` ...\n";
    while (stream->Read(&book)) {
      cout << "A book is returned: " << book.name() << " " << book.id() << endl;
    }
  }
  void Delete(int32_t id) {
    ClientContext context;
    google::protobuf::Int32Value val;
    google::protobuf::BoolValue res;
    res.set_value(false);
    val.set_value(id);
    stub_->DeleteById(&context, val, &res);
    if (res.value()) {
      cout << "Delete book with id = " << id << " success"
           << "\n";
    } else {
      cout << "Delete book with id = " << id << " fails"
           << "\n";
    }
  }

 private:
  std::unique_ptr<BookSystem::Stub> stub_;
};

#endif  // DIS_3_BOOK_SYSTEM_CLIENT_H

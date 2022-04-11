//
// Created by A1134 on 2022/4/9.
//

#ifndef DIS_3_BOOK_SYSTEM_SERVER_H
#define DIS_3_BOOK_SYSTEM_SERVER_H

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "book_system.grpc.pb.h"
#include "book_sql.h"
#include "cpp_sql.h"

using disc3::Book;
using disc3::BookSystem;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using grpc::StatusCode;
using std::string;
using std::unique_ptr;
using std::unordered_map;

class BookSystemImpl final : public BookSystem::Service {
 private:
  bool RemoveFromDB(int32_t id) {
    try {
      disc3::BookIdParam param{id};
      if (disc3::DeleteBook(&sql_engine_, param) != 1) {
        return false;
      }
      return true;
    } catch (std::exception &e) {
      cout << e.what() << endl;
      return false;
    }
  }
  bool InsertToDB(const ::disc3::Book *request) {
    try {
      if (disc3::AddBook(&sql_engine_, *request) != 1) {
        return false;
      }
      return true;
    } catch (std::exception &e) {
      cout << e.what() << endl;
      return false;
    }
  }
  bool GetAllFromDB() {
    try {
      disc3::Empty param{};
      auto res = disc3::GetAllBooks(&sql_engine_, param);
      if (res.empty()) {
        return false;
      }
      for (auto &r : res) {
        int32_t id = r.id();
        all_books_.emplace(id, std::make_unique<Book>(std::move(r)));
      }
      return true;
    } catch (std::exception &e) {
      cout << e.what() << endl;
      return false;
    }
  }

 public:
  BookSystemImpl(const string &ip, const string &user, const string &passwd)
      : driver_{::get_driver_instance()},
        con_{driver_->connect(ip, user, passwd)} {
    con_->setSchema("ds_course_lab");
    sql_engine_.Init(std::unique_ptr<sql::Statement>(con_->createStatement()));
    GetAllFromDB();
  }
  ~BookSystemImpl() override = default;

  Status Add(::grpc::ServerContext *context, const ::disc3::Book *request,
             ::google::protobuf::BoolValue *response) override {
    auto id = request->id();
    all_books_.emplace(id, std::make_unique<Book>(*request));
    response->set_value(true);
    InsertToDB(request);
    return Status::OK;
  }

  Status QueryById(::grpc::ServerContext *context,
                   const ::google::protobuf::Int32Value *request,
                   ::disc3::Book *response) override {
    if (all_books_.count(request->value())) {
      response->operator=(*all_books_[request->value()]);
      return Status::OK;
    } else {
      return {StatusCode ::NOT_FOUND, ""};
    }
  }

  Status QueryByName(::grpc::ServerContext *context,
                     const ::google::protobuf::StringValue *request,
                     ::grpc::ServerWriter<::disc3::Book> *writer) override {
    for (auto &b : all_books_) {
      if ((b.second->name()).find(request->value()) != std::string::npos) {
        writer->Write(*b.second);
      }
    }
    return Status::OK;
  }

  Status DeleteById(::grpc::ServerContext *context,
                    const ::google::protobuf::Int32Value *request,
                    ::google::protobuf::BoolValue *response) override {
    if (all_books_.count(request->value())) {
      if (RemoveFromDB(request->value())) {
        all_books_.erase(request->value());
        response->set_value(true);
        return Status::OK;
      } else {
        response->set_value(false);
        return {StatusCode::INTERNAL, "DB error"};
      }
    }
    return {StatusCode::NOT_FOUND, ""};
  }

 private:
  unordered_map<int32_t, unique_ptr<Book>> all_books_{};
  sql::Driver *driver_;
  std::unique_ptr<sql::Connection> con_;
  CppSql sql_engine_{};
};

#endif  // DIS_3_BOOK_SYSTEM_SERVER_H

#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "sql_handler.h"
using namespace std;

namespace disc3 {

struct BookIdParam {
  int32_t id_;
  int32_t id() const { return id_; };
  template <typename T>
  void set_id(T &&t) {
    id_ = t;
  };
};

struct Empty {};

struct BookNameParam {
  std::string name_;
  std::string name() const { return name_; };
  template <typename T>
  void set_name(T &&t) {
    name_ = t;
  };
};

// struct Book {
//   std::string name_;
//   std::string name() const { return name_; };
//   template <typename T>
//   void set_name(T &&t) {
//     name_ = t;
//   };
//   int32_t id_;
//   int32_t id() const { return id_; };
//   template <typename T>
//   void set_id(T &&t) {
//     id_ = t;
//   };
// };
//
static std::string GetBookByIdSql(CommonSqlHandler *sql_handler,
                                  const BookIdParam &param) {
  ostringstream ss;
  ss << "  select `NAME`, `ID`";
  ss << "  from `BOOKS`";
  ss << "  where `ID` = '" + to_string(param.id()) + "'";
  return ss.str();
}

static std::string GetAllBooksSql(CommonSqlHandler *sql_handler,
                                  const Empty &param) {
  ostringstream ss;
  ss << "  select `NAME`, `ID`";
  ss << "  from `BOOKS`";
  return ss.str();
}

static std::string GetBookByNameSql(CommonSqlHandler *sql_handler,
                                    const BookNameParam &param) {
  ostringstream ss;
  ss << "  select `NAME`, `ID`";
  ss << "  from `BOOKS`";
  ss << "  where `NAME` like concat('%','" +
            sql_handler->EscapeString(param.name()) + "','%')";
  return ss.str();
}

static std::string AddBookSql(CommonSqlHandler *sql_handler,
                              const Book &param) {
  ostringstream ss;
  ss << "  insert into `BOOKS`";
  ss << "  values";
  ss << "  ('" + sql_handler->EscapeString(param.name()) + "', '" +
            to_string(param.id()) + "')";
  return ss.str();
}

static std::string DeleteBookSql(CommonSqlHandler *sql_handler,
                                 const BookIdParam &param) {
  ostringstream ss;
  ss << "  delete from `BOOKS`";
  ss << "  where `ID`='" + to_string(param.id()) + "'";
  return ss.str();
}

static std::vector<Book> GetBookById(ColumnLabelSqlHandler *sql_handler,
                                     const BookIdParam &param) {
  std::vector<Book> ret;
  auto sql = GetBookByIdSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString("name"));
    ret.back().set_id(sql_handler->GetInt("id"));
  }
  return ret;
}

static std::vector<Book> GetBookById(ColumnIndexSqlHandler *sql_handler,
                                     const BookIdParam &param) {
  std::vector<Book> ret;
  auto sql = GetBookByIdSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString(1));
    ret.back().set_id(sql_handler->GetInt(2));
  }
  return ret;
}

static std::vector<Book> GetAllBooks(ColumnLabelSqlHandler *sql_handler,
                                     const Empty &param) {
  std::vector<Book> ret;
  auto sql = GetAllBooksSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString("name"));
    ret.back().set_id(sql_handler->GetInt("id"));
  }
  return ret;
}

static std::vector<Book> GetAllBooks(ColumnIndexSqlHandler *sql_handler,
                                     const Empty &param) {
  std::vector<Book> ret;
  auto sql = GetAllBooksSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString(1));
    ret.back().set_id(sql_handler->GetInt(2));
  }
  return ret;
}

static std::vector<Book> GetBookByName(ColumnLabelSqlHandler *sql_handler,
                                       const BookNameParam &param) {
  std::vector<Book> ret;
  auto sql = GetBookByNameSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString("name"));
    ret.back().set_id(sql_handler->GetInt("id"));
  }
  return ret;
}

static std::vector<Book> GetBookByName(ColumnIndexSqlHandler *sql_handler,
                                       const BookNameParam &param) {
  std::vector<Book> ret;
  auto sql = GetBookByNameSql(sql_handler, param);
  sql_handler->ExecuteQuery(sql);
  while (sql_handler->Next()) {
    ret.emplace_back();
    ret.back().set_name(sql_handler->GetString(1));
    ret.back().set_id(sql_handler->GetInt(2));
  }
  return ret;
}

static int32_t AddBook(ColumnLabelSqlHandler *sql_handler, const Book &param) {
  auto sql = AddBookSql(sql_handler, param);
  return sql_handler->ExecuteUpdate(sql);
}

static int32_t AddBook(ColumnIndexSqlHandler *sql_handler, const Book &param) {
  auto sql = AddBookSql(sql_handler, param);
  return sql_handler->ExecuteUpdate(sql);
}

static int32_t DeleteBook(ColumnLabelSqlHandler *sql_handler,
                          const BookIdParam &param) {
  auto sql = DeleteBookSql(sql_handler, param);
  return sql_handler->ExecuteUpdate(sql);
}

static int32_t DeleteBook(ColumnIndexSqlHandler *sql_handler,
                          const BookIdParam &param) {
  auto sql = DeleteBookSql(sql_handler, param);
  return sql_handler->ExecuteUpdate(sql);
}
}  // namespace disc3

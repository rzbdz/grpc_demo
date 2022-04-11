<!--encoding: UTF-8 -->

gRPC 的 Demo，作为分布式计算课程第三次作业以及继续练习基于 gRPC 的微服务实现仓库模板。

sql 生成 PO 定义及 mysql wrapper 部分魔改自项目 [cpp_easy_sql](https://github.com/liuping001/cpp_easy_sql) ，
使其能够生成支援 protobuf 生成的 message 类的 `get/set` 方法。

## RPC 图书系统。

使用 grpc 作为 RPC 中间件。 编译运行方法：

```shell
# 本机已安装 c++17 编译器。
# 本机已全局安装 grpc 库与 protoc 代码生成器。
#   如果是本地安装，需要得知安装路径，起名为 $MY_INSTALL_DIR
# 本机已安装 cmake 与 make, build-essential 组件等。
# 在根目录下：
$ mkdir build
$ cd build
# 如果已全局安装 grpc + protobuf
$ cmake .. 
# 如果本地安装
$ cmake .. -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR
# 然后
$ make bs_grpc_proto
$ make book_system_server
$ ./book_system_server&
$ make book_system_client
$ ./book_system_client
```

运行预期结果：

```shell
Powered by grpc, homework 3
-------------------AddBook-------------------
Add book Code:The Hidden Language of Computer Hardware and Software by Charles Petzold Read,10112 success
Add book Clean Code : A Handbook of Agile Software Craftsmanship by Robert C.Martin,10113 success
Add book Code Complete: A Practical Handbook of Software Construction by Steve McConnell,10114 success
Add book Algorithms by Robert Sedgewick& Kevin Wayne,10115 success
Add book Computer Science and Programming Types and Programming Languages by Benjamin,10116 success
-------------------QueryBookById-------------------
Query book Code Complete: A Practical Handbook of Software Construction by Steve McConnell,10114 success
-------------------QueryBooksByName-------------------
Querying book with partial/complete name: `Code`...
A book is returned: Code Complete: A Practical Handbook of Software Construction by Steve McConnell 10114
A book is returned: Clean Code : A Handbook of Agile Software Craftsmanship by Robert C.Martin 10113
A book is returned: Code:The Hidden Language of Computer Hardware and Software by Charles Petzold Read 10112
-------------------DeleteById-------------------
Delete book with id = 10115 success
Goodbye....
```

设置 MySQL 数据库，数据库连接逻辑参照 `BookSystemImpl` 构造函数， 其中 Schema/Database 名称定义为 `ds_course_lab`,其中只有一个 Table：

```mysql
CREATE TABLE BOOKS
(
    name VARCHAR(60),
    id   int PRIMARY KEY
);
```

对应的查询语句在 `cpp_sql/book.xml` 文件中。

## 定义 gRPC 服务类型

根据 PPT 要求，服务端至少暴露如下 RPC 接口，定义为 protobuf 的 service如下：

```protobuf
service BookSystem{
  rpc Add(Book) returns (google.protobuf.BoolValue) {}
  rpc QueryById(google.protobuf.Int32Value) returns (Book){}
  rpc QueryByName(google.protobuf.StringValue) returns (stream Book){}
  rpc DeleteById(google.protobuf.Int32Value) returns (google.protobuf.BoolValue){}
}
```

## 定义数据类型

根据 PPT 要求，数据图书有 name 和 id 两个字段：

```protobuf
message Book{
  int32 id = 1;
  string name = 2;
}
```

## 服务端设计说明


```cpp
class BookSystemImpl{
 private:
  unordered_map<int32_t, unique_ptr<Book>> all_books_{}; //简单的内存缓存
  //....(一系列同步 PO 自数据库的 helper 方法)
 public:
  //....(继承 Service 虚方法)

}
```

## 具体逻辑实现

- 服务器实现所有的 RPC 接口在文件 `book_system_server.h` 中。
- 客户端模拟进行多次查询，在文件 `book_system_client.h` 中定义辅助方法， 在文件 `book_system_client.cpp` 中定义模拟逻辑。

## 测试代码

见 `book_system_client.cpp` 中。测试添加五本图书。

```cpp
  BookSystemClient client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  cout << "Powered by grpc, homework 3\n";
  cout << "-------------------AddBook-------------------" << endl;
  vector<string> ns{
      "Code:The Hidden Language of Computer Hardware and Software by Charles "
      "Petzold Read",
      "Clean Code : A Handbook of Agile Software Craftsmanship by Robert "
      "C.Martin",
      "Code Complete: A Practical Handbook of Software Construction by Steve "
      "McConnell",
      "Algorithms by Robert Sedgewick& Kevin Wayne",
      "Computer Science and Programming Types and Programming Languages by "
      "Benjamin"};
  vector<int32_t> ids{10112, 10113, 10114, 10115, 10116};
  for (int i = 0; i < 5; i++) {
    client.Add(ns[i], ids[i]);
  }
  cout << "-------------------QueryBookById-------------------" << endl;
  client.QueryById(10114);
  cout << "-------------------QueryBooksByName-------------------" << endl;
  client.QueryByName("Code");
  cout << "-------------------DeleteById-------------------" << endl;
  client.Delete(10115);
  cout << "Goodbye...." << endl;
```

//
// Created by A1134 on 2022/4/9.
//

#include "book_system_client.h"

int main() {
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
  return 0;
}
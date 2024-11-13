#include "DataBase.hpp"
#include "__table.hpp"

struct hash {
  size_t operator()(const char* buff) {
    std::string str = buff;
    const unsigned long long k = 0x9ae16a3b2f90404fULL;
    size_t h = 0, m = 1;
    for (char it : str) {
      size_t x = (size_t) (it - 'a' + 1);
      h = (h + m * x) % 200;
      m = (m * k) % 200;
    }
    //size_t h = std::stoi(buff) / 10;
    return h;
  }
};

char* summ(int a, int b) {
    int c = a + b;
    
    // Using std::to_string
    std::string str = std::to_string(c);
    
    // Convert std::string to char*
    char* char_str = new char[str.size() + 1];
    std::strcpy(char_str, str.c_str());
    
    return char_str;
}

int main() {
  // __table<hash> t("/Users/alex/labwork11-SPLOIT47/src/DataBase/test.txt", 8);
  // t.write(summ, 20, 3);
  // t.write(summ, 2, 3);
  // t.write(summ, -10, 3);
  // std::string str = "-6";
  // std::cout << t[str.c_str()];


  DataBase db("/Users/alex/labwork11-SPLOIT47/src/DataBase");
  db.AddTable<hash>("test");
  
  db.AddModule("test", "2", "1 biba");
  // db.AddModule("test", "20", summ(10, 2));
  db.AddModule("test", "9", "9 lox");
  db.AddModule("test", "2", "2 aboba");
  db.AddModule("test", "2", "3 1");
  db.AddModule("test", "9", "4 2");

  // std::cout << db.get("test", "1") << '\n';
  // std::cout << db.get("test", "20") << '\n';
  // std::cout << db.get("test", "10") << '\n';
  //std::cout << db.get("test", "120") << '\n';
}
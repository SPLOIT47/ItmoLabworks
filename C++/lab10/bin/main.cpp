#include "src/Scheduler.hpp"
#include <iostream>
#include <map>
#include <unordered_set>
#include "src/any.hpp"
#include "src/function.hpp"

int main() {
  Scheduler scheduler;
  float b = 2.0f; float a = 1.0f; float c = -3.0f;
  auto id1 = scheduler.add([](float a, float c){ return -4 * a * c; }, a, c);
  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));

  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));

  auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));

  auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));

  // // scheduler.print();
  // // std::cout << '\n';
  scheduler.executeAll();
  std::cout << scheduler.getResult<float>(id5) << std::endl;
  std::cout << scheduler.getResult<float>(id6) << std::endl;

  // for (auto it : scheduler.adj) {
  //   std::cout << it->id() << ": " << it->adj.size() << ' ' << it->arguments.size();
  //   for (auto i : it->adj) {
  //     std::cout << i.first->id() << ' ';
  //   }
  //   std::cout << '\n';
  // }
  // std::cout << '\n';
}

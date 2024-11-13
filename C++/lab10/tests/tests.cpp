#include "gtest/gtest.h"
#include "src/Scheduler.hpp"

TEST(SchedulerTESTS, TEST1) {
  Scheduler scheduler;
  float b = 2.0f; float a = 1.0f; float c = -3.0f;
  auto id1 = scheduler.add([](float a, float c){ return -4 * a * c; }, a, c);
  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));
  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));
  auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
  auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));
  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<float>(id5), 1);
  ASSERT_EQ(scheduler.getResult<float>(id6), -3);
}

TEST(SchedulerTESTS, TEST2) {
  Scheduler scheduler;
  float b = 2.0f; float a = 1.0f; float c = -3.0f;
  auto id1 = scheduler.add([](float a, float c){ return -4 * a * c; }, a, c);
  auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.getFutureResult<float>(id1));
  auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));
  auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(abs(d)); }, b, scheduler.getFutureResult<float>(id2));
  auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.getFutureResult<float>(id3));
  auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.getFutureResult<float>(id4));
  scheduler.executeAll();
  ASSERT_ANY_THROW(scheduler.getResult<int>(id5));
  ASSERT_ANY_THROW(scheduler.getResult<int>(id6));
}

TEST(SchedulerTESTS, TEST3) {
  Scheduler scheduler;
  int a = 1; int c = 3;
  auto id1 = scheduler.add([](int a, int c){ return a * c; }, a, c);
  auto id2 = scheduler.add([](int b){return b * b;}, scheduler.getFutureResult<int>(id1));
  auto id3 = scheduler.add([](int b, int d){return b + d; }, scheduler.getFutureResult<int>(id1), scheduler.getFutureResult<int>(id2));

  scheduler.executeAll();
  ASSERT_EQ(scheduler.getResult<int>(id3), 12);
}

TEST(SchedulerTESTS, TEST4) {
  Scheduler scheduler;
  float a = 1; int c = 3;
  auto id1 = scheduler.add([](int a, int c){ return a * c; }, a, c);
  ASSERT_ANY_THROW(scheduler.executeAll());
}

TEST(SchedulerTESTS, TEST5) {
  Scheduler scheduler;
  float a = 1;
  auto id1 = scheduler.add([](int a){ return a; }, a);
  ASSERT_ANY_THROW(scheduler.executeAll());
}
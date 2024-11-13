#pragma once
#include <vector>
#include <string>
#include <utility>
#include <chrono>
#include <thread>
#include <iostream>
#include "ftxui/component/component.hpp"
#include "request.hpp"
using namespace ftxui;

class WeatherRequests;
class UI;
class JsonParse;

class WeatherForecast {
public:
  ~WeatherForecast();

  void Run();

  struct City {
    std::string name;
    std::vector<std::string> time;
    std::vector<std::string> temperature;
    std::vector<std::string>  weather;
    std::vector<std::string>  apparent_temperature;
    std::pair<std::string, std::string> coords;
  };
  
  friend WeatherRequests;
  friend UI;
  friend JsonParse;
private:
  std::vector<City*> cities;
  int days;
  int freq;
  void RefreshData();
};
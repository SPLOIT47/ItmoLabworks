#pragma once
#include <string>

class JsonParse {
public:
  static std::string GetByWMOcode(std::string code);
  static void ReadConfig(WeatherForecast& w);
};
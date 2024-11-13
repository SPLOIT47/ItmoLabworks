#pragma once
#include <string>
#include <utility>
#include "class.hpp"

class WeatherForecast;
class WeatherRequests {
public:
  static std::pair<std::string, std::string> CoordsApiRequest(const std::string& city);
  static void WeatherRequest(std::pair<std::string, std::string> coords, WeatherForecast::City& city, WeatherForecast& w);
};
#pragma once
#include "ftxui/component/component.hpp"

class UI {
  public:
  static Element CreateDayBox(size_t& city_index, size_t& time_index, WeatherForecast& w);
  static void UpdateBoxForecast(Element& box, size_t city_index, size_t time_index, WeatherForecast& w);
  static Element CreateSubBox(size_t& city_index, size_t& time_index, WeatherForecast& w);
  static void TUI(WeatherForecast& w);
};
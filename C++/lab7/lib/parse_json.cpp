#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "request.hpp"
#include "class.hpp"
#include "parse_json.hpp"


using json = nlohmann::json;

std::string JsonParse::GetByWMOcode(std::string code) {
  if (code.length() < 2) code = '0' + code;
  std::ifstream f("/Users/alex/labwork7-SPLOIT47/WMO_codes.json");
  json data = json::parse(f);
  return data[code];
}

void JsonParse::ReadConfig(WeatherForecast& w) {
  std::ifstream f("/Users/alex/labwork7-SPLOIT47/config.json");
  json data = json::parse(f);
  for (int i = 0; i < data["cities"].size(); ++i) {
    WeatherForecast::City* city = new WeatherForecast::City;
    city->name = data["cities"][i];
    w.cities.push_back(city);
  }
  w.days = data["days"];
}
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "request.hpp"
#include "class.hpp"


using json = nlohmann::json;

void JsonFormatToVectorString(std::vector<std::string>& vec, const nlohmann::json_abi_v3_11_3::json& json_data) {
  int i = 0;
  for (const auto& t : json_data) {
    if (i == 24) { i = 0; }
    if (i == 0 || i == 7 || i == 13 || i == 18) { vec.push_back(t.dump()); }
    ++i;
  }
}

std::pair<std::string, std::string> WeatherRequests::CoordsApiRequest(const std::string& city) {
  std::ifstream f("/Users/alex/labwork7-SPLOIT47/config.json");
  json parsed = json::parse(f);
  std::string url = "https://api.api-ninjas.com/v1/city?name=";
  std::string api = parsed["api_key"];
  cpr::Header headers {{"X-Api-Key", api}};
  cpr::Response r = cpr::Get(cpr::Url{url + city}, headers);
  json raw_data = json::parse(r.text);
  const auto& data = raw_data[0];
  std::string lat = std::to_string(data["latitude"].get<double>());
  std::string lon = std::to_string(data["longitude"].get<double>());
  return std::make_pair(lat, lon);
}

void  WeatherRequests::WeatherRequest(std::pair<std::string, std::string> coords, WeatherForecast::City& city, WeatherForecast& w) {
  std::string url = "https://api.open-meteo.com/v1/forecast";
  cpr::Parameters params  = cpr::Parameters{{"latitude", coords.first}, {"longitude", coords.second}, 
      {"hourly", "temperature_2m,weather_code,apparent_temperature"}, {"forecast_days", std::to_string(w.days)}};
  cpr::Response r = cpr::Get(cpr::Url{url}, params);
  if (r.status_code == 200) {
  json raw_data = json::parse(r.text);
  const auto& hourlyData = raw_data["hourly"];
  const auto& time_values = hourlyData["time"];
  const auto& temperature_values = hourlyData["temperature_2m"];
  const auto& weather_values = hourlyData["weather_code"];
  const auto& apparent_temperature_values = hourlyData["apparent_temperature"];

  JsonFormatToVectorString(city.time, time_values);
  JsonFormatToVectorString(city.temperature, temperature_values);
  JsonFormatToVectorString(city.weather, weather_values);
  JsonFormatToVectorString(city.apparent_temperature, apparent_temperature_values);
  city.coords = coords;
  }
}
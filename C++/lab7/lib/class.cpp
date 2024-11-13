#include "request.hpp"
#include "terminal.hpp"
#include "parse_json.hpp"
#include "class.hpp"

WeatherForecast::~WeatherForecast() {
  for (auto city : cities) {
    delete city;
  }
}

void WeatherForecast::Run() {
  JsonParse j;
  WeatherRequests r;
  UI u;
  j.ReadConfig(*this);
  for (auto city : cities) {
    r.WeatherRequest(r.CoordsApiRequest(city->name),*city, *this);
  }

  
  std::thread refresh_thread([this] { this->RefreshData(); });
  std::thread tui_thread([this] { UI::TUI(*this); });

  refresh_thread.join();
  tui_thread.join();
}

void WeatherForecast::RefreshData() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(this->freq));
    for (auto city : cities) {
      WeatherRequests::WeatherRequest(city->coords, *city, *this);
    }
  }
}

#include "request.hpp"
#include "parse_json.hpp"
#include "terminal.hpp"
#include "class.hpp"
#include <iostream>
#include <vector>
#include <string>  
#include <type_traits>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/component/component_options.hpp" 
#include "ftxui/component/component.hpp"
#include "ftxui/dom/table.hpp"

using namespace ftxui;

std::string ConcotinateStr(std::string& res, std::string& a,  std::string& b, std::string& c) {
  res = "";
  res += a;
  res += b;
  res += c;
  return res;
}

Element UI::CreateSubBox(size_t& city_index, size_t& time_index, WeatherForecast& w) {
  std::string res;
  std::string tmp = "temperature: ";
  std::string a_tmp = "apparent_temperature: ";
  std::string deg = "°C";

  auto box = vbox({
      text("Night"),
      text(JsonParse::GetByWMOcode(w.cities[city_index]->weather[time_index])),
      text(ConcotinateStr(res, tmp, w.cities[city_index]->temperature[time_index], deg)),
      text(ConcotinateStr(res, a_tmp, w.cities[city_index]->apparent_temperature[time_index], deg)),
    });
  ++time_index;
  return box;
};

void UI::UpdateBoxForecast(Element& box, size_t city_index, size_t time_index, WeatherForecast& w) {
  
  box = vbox({
    box,
      text(w.cities[city_index]->time[time_index].substr(1, 10)) | center,
      hbox(CreateDayBox(city_index, time_index, w)),
  });
}

Element UI::CreateDayBox(size_t& city_index, size_t& time_index,  WeatherForecast& w) {
  std::string res;
  std::string tmp = "temperature: ";
  std::string a_tmp = "apparent_temperature: ";
  std::string deg = "°C";

  Element box = hbox({
      CreateSubBox(city_index, time_index, w),
      separator(),
      CreateSubBox(city_index, time_index, w),
      separator(),
      CreateSubBox(city_index, time_index, w),
      separator(),
      CreateSubBox(city_index, time_index, w),
  }) | borderRounded;
  return box;
}

void UI::TUI(WeatherForecast& w) {
  auto screen = ScreenInteractive::Fullscreen();

  int selected_city = 0;
  int selected_day = 0;
  
  std::vector<Element> box_condition;
  int curr_condion = 0;
  int page = 0;
  auto box = vbox();
  box_condition.push_back(box);
  auto forecast_rendered = Renderer([&] { return box ;});
  
  MenuOption city_option;
  city_option.on_enter = [&] {
    int selected_date = 0;
    auto box = vbox({});
    auto renderer = Renderer([&] { return box; });
    auto window = Window({ 
      .inner = renderer,
      .height = screen.dimy() - 2,
      .width = 50
      });
    auto dates_layout = Container::Vertical({window});
    dates_layout |= CatchEvent([&] (Event event) {
      if ((event == Event::Character('=') || event == Event::Character('+')) && (selected_date / 4)< w.days) {
        box_condition.push_back(box);
        UpdateBoxForecast(box, selected_city, selected_date, w);
        selected_date += 4;
      } else if ((event == Event::Character('-') || event == Event::Character('_')) && selected_date > 3) {
        selected_date -= 4;
        box = box_condition.back();
        box_condition.pop_back();
      } else if (event == Event::Escape) {
          screen.ExitLoopClosure()();
      } 
      return 0;
    });
    screen.Loop(dates_layout);
  };

  std::vector<std::string> cities_names;
  for (const auto& city : w.cities) {
    cities_names.push_back(city->name);
  }

  auto city_connect_menu = Menu(&cities_names, &selected_city, city_option);
  auto city_renderer = Renderer(city_connect_menu, [&] {
    return vbox({city_connect_menu->Render() | vscroll_indicator | frame});
  });
  
  auto cities_window = Window({
    .title = "Cities",
    .inner = city_renderer,
    .height = 6 + w.cities.size(),
    .width = 30,
    .top = 1,
  });
  
  auto layout = cities_window;
  layout |= CatchEvent([&](Event event){
    if (event == Event::Escape) {
      screen.ExitLoopClosure()();
      exit(0);
    } 
    return 0;
  });
  screen.Loop(layout);
}


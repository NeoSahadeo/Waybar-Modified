#include "modules/boat_playback.hpp"

#include <string>

waybar::modules::BoatPlayback::BoatPlayback(const std::string& id, const Json::Value& config)
    : ALabel(config, "boat_playback", id, "{boat_format}", 1) {
  thread_ = [this] {
    dp.emit();
    thread_.sleep_for(interval_);
  };
}

std::string exec(const char* cmd) {
  std::array<char, 128> buffer;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  std::string result;
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

auto waybar::modules::BoatPlayback::update() -> void {
  auto format = format_;

  float arg_1 = std::stod(exec("playerctl position"));
  float arg_2 = std::stod(exec("playerctl metadata | grep -Poe '(?<=length).*'"));

  int sea_size = 10;
  char* boat = (char*)"𓊝";
  char* sea = (char*)"﹏";
  std::string ocean;

  float pos = sea_size * (arg_1 * 1000000 / arg_2);
  int pos_i = (int)pos;

  for (int x = 0; x < sea_size + 1; x++) {
    if (x == pos_i) {
      ocean.append(boat);
    } else {
      ocean.append(sea);
    }
  }

  label_.set_markup(fmt::format(fmt::runtime(format), fmt::arg("boat_format", ocean)));
  ALabel::update();
}

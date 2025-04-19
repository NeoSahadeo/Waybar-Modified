#pragma once

#include <fmt/format.h>

#include "ALabel.hpp"
#include "util/sleeper_thread.hpp"

namespace waybar::modules {
class BoatPlayback : public ALabel {
 public:
  BoatPlayback(const std::string&, const Json::Value&);
  virtual ~BoatPlayback() = default;
  auto update() -> void override;

 private:
  util::SleeperThread thread_;
};
}  // namespace waybar::modules

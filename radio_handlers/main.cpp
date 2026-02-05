#include <cstdint>
#include <functional>
#include <print>
#include <span>
#include <string>
#include <tuple>
#include <vector>


int main() {
  DebuggingDriver drv = DebuggingDriver{};
  std::vector<std::string> lines = {"reset", "sleep", "standby",
                                    "log abcdef hey wassup asdsadsadsad",
                                    "unknowncmd"};
  for (auto line : lines) {
    drv.Parse(std::string_view(line));
  }
}
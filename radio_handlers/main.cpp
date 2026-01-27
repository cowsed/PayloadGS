#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
class failed_fifo_exception : std::exception {};

int make_fifo(const std::filesystem::path &path) {
  int fd;
  int ret = mkfifo(path.c_str(), 0666);
  std::cout << "errno " << errno << std::endl;
  std::cout << "errno str::" << std::strerror(errno) << std::endl;
  return ret;
}



int main() {
  std::filesystem::path tx_fifo_path = "./command_fifo";
  make_fifo(tx_fifo_path);
  while (true) {
    std::ifstream tx_fifo{tx_fifo_path};
    std::string line;
    while (std::getline(tx_fifo, line)) {
      std::istringstream iss(line);
      std::cout << "Got: " << line << std::endl;
    }
    std::cerr << "reader disconnected" << std::endl;
  }
}
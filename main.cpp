#include <iostream>
#include <ostream>
#include <thread>

#include "gol/StringFormatter.hpp"
#include "gol/mat.hpp"

using namespace std::chrono_literals;

int main() {
  const gol::Stat stat{50, 50};
  gol::Mat mat{stat};

  mat.setAlive({{1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}});

  gol::StringFormatter f{stat};

  while (true) {
    std::cout << f.format(mat) << std::endl;
    mat = mat.advance();
    std::this_thread::sleep_for(50ms);
  }

  return {};
}
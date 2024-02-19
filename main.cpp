#include "gol/mat.hpp"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <fmt/core.h>
#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <unistd.h>

namespace {

namespace po = boost::program_options;

auto getOptions() -> po::options_description {
  po::options_description options{"Allowed options"};
  options.add_options()("help", "print help message")(
      "size,S", po::value<int>(), "field size");

  return options;
}

static const po::options_description opts{getOptions()};

struct ApplicationContext {
  bool showHelp{};
  int matSize{20};
};

auto parseCommandLine(int argc, char **argv) -> ApplicationContext {
  po::variables_map map;
  po::store(po::parse_command_line(argc, argv, opts), map);
  po::notify(map);

  ApplicationContext ctx;
  if (map.count("size"))
    ctx.matSize = map["size"].as<int>();

  ctx.showHelp = map.count("help") != 0;

  return ctx;
}

void printHelp(std::ostream &os) { os << opts << "\n"; }

auto clearScreen() -> void { std::cout << "\x1b[2J"; }

auto moveTo(int row, int col) -> void {
  std::cout << fmt::format("\x1b[{};{}f", row, col);
}

} // namespace

using namespace std::chrono_literals;

int main(int argc, char **argv) {
  const ApplicationContext ctx{parseCommandLine(argc, argv)};

  if (ctx.showHelp) {
    printHelp(std::cout);
    return {};
  }

  const gol::Stat stat{static_cast<gol::ColNo>(ctx.matSize),
                       static_cast<gol::RowNo>(ctx.matSize)};
  gol::Mat mat{stat};

  clearScreen();
  mat.setAlive({{1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}});

  while (true) {
    mat = mat.advance([&](const gol::Coordinates &coor, gol::State state,
                          gol::GenerationNo genNo) {
      moveTo(coor.row + 1, coor.col + 1);
      std::cout << (state == gol::State::dead ? " " : "x");
      std::cout.flush();
    });
    std::this_thread::sleep_for(500ms);
  }

  return {};
}
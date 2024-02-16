#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

namespace gol {

int modulo(int value, int base) { return (base + (value % base)) % base; }

using ColNo = int;
using RowNo = int;

struct Coordinates {
  ColNo col;
  RowNo row;
};
enum class State { live, dead };
struct Cell {
  Coordinates coor{};
  State state{State::dead};

  operator std::string() const {
    return state == State::dead ? std::string{"."} : std::string{"x"};
  }
};

using CoorContainer = std::vector<Coordinates>;

using Cells = std::vector<Cell>;
using Neightbours = std::vector<Coordinates>;

struct Stat {
  ColNo maxCols;
  RowNo maxRows;
};

class Mat {
public:
  Mat(Stat stat)
      : m_stat{std::move(stat)}, m_cells{m_stat.maxCols * m_stat.maxRows} {
    for (RowNo row = 0; row < m_stat.maxCols; ++row) {
      for (ColNo col = 0; col < m_stat.maxRows; ++col) {
        const Coordinates coor{col, row};
        m_cells.at(convertToIndex(coor)) = Cell{coor};
      }
    }
  }
  Mat(const Mat &) = default;

  auto setAlive(CoorContainer coor) -> void { setState(coor, State::live); }
  auto setDead(CoorContainer coor) -> void { setState(coor, State::dead); }

  auto setState(CoorContainer coor, State state) -> void {
    for (const auto &c : coor)
      m_cells.at(convertToIndex(c)).state = state;
  }

  Mat advance() const {
    Mat newGeneration{*this};

    for (const auto &cell : m_cells) {
      newGeneration.setState({cell.coor}, getNewState(cell));
    }

    return newGeneration;
  }

  operator std::string() const {
    std::ostringstream output;

    std::for_each(std::cbegin(m_cells), std::cend(m_cells),
                  [&output](const Cell &cell) { output << std::string{cell}; });

    return output.str();
  }

private:
  auto convertToIndex(Coordinates coor) const -> std::size_t {
    return coor.row * m_stat.maxCols + coor.col;
  }

  auto getNewState(Cell cell) const -> State {
    const auto n = getNeightbours(cell.coor);
    const auto aliveN = std::accumulate(
        std::begin(n), std::end(n), 0,
        [&](int accumulate, const Coordinates &item) {
          return accumulate + (getState(item) == State::live ? 1 : 0);
        });

    if (cell.state == State::dead) {
      return aliveN == 3 ? State::live : State::dead;
    }

    if (aliveN < 2 or aliveN > 3)
      return State::dead;

    return State::live;
  }

  auto getState(Coordinates coor) const -> State {
    return m_cells.at(convertToIndex(coor)).state;
  }

  auto getNeightbours(Coordinates coor) const -> Neightbours {
    return {
        Coordinates{modulo(coor.col - 1, m_stat.maxCols),
                    modulo(coor.row - 1, m_stat.maxRows)},
        Coordinates{coor.col, modulo(coor.row - 1, m_stat.maxRows)},
        Coordinates{modulo(coor.col + 1, m_stat.maxCols),
                    modulo(coor.row - 1, m_stat.maxRows)},
        Coordinates{modulo(coor.col - 1, m_stat.maxCols), coor.row},
        Coordinates{modulo(coor.col + 1, m_stat.maxCols), coor.row},
        Coordinates{modulo(coor.col - 1, m_stat.maxCols),
                    modulo(coor.row + 1, m_stat.maxRows)},
        Coordinates{coor.col, modulo(coor.row + 1, m_stat.maxRows)},
        Coordinates{modulo(coor.col + 1, m_stat.maxCols),
                    modulo(coor.row + 1, m_stat.maxRows)},
    };
  }

  Stat m_stat;
  Cells m_cells;
};

struct StringFormatter {
public:
  StringFormatter(Stat stat) : m_stat(stat) {}

  std::string format(Mat mat) const {
    const std::string matStr{mat};

    std::ostringstream oss;

    for (std::size_t i = 0; i < m_stat.maxRows; ++i) {
      oss << matStr.substr(i * m_stat.maxCols, m_stat.maxRows) << std::endl;
    }

    return oss.str();
  }

private:
  Stat m_stat;
};

} // namespace gol

using namespace std::chrono_literals;

int main() {
  const gol::Stat stat{50, 50};
  gol::Mat mat{stat};

  mat.setState({{1, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}}, gol::State::live);

  gol::StringFormatter f{stat};

  while (true) {
    std::cout << f.format(mat) << std::endl;
    mat = mat.advance();
    std::this_thread::sleep_for(50ms);
  }

  return {};
}
#include <cstddef>
#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "mat.hpp"

namespace gol {

std::size_t modulo(int value, int base) {
  return (base + (value % base)) % base;
}

Mat::Mat(Stat stat)
    : m_stat{std::move(stat)}, m_cells{m_stat.maxCols * m_stat.maxRows} {
  for (RowNo row = 0; row < m_stat.maxCols; ++row) {
    for (ColNo col = 0; col < m_stat.maxRows; ++col) {
      const Coordinates coor{col, row};
      m_cells.at(convertToIndex(coor)) = Cell{coor};
    }
  }
}

auto Mat::setAlive(CoorContainer coor) -> void { setState(coor, State::live); }
auto Mat::setDead(CoorContainer coor) -> void { setState(coor, State::dead); }

auto Mat::setState(CoorContainer coor, State state) -> void {
  for (const Coordinates &c : coor)
    m_cells.at(convertToIndex(c)).state = state;
}

Mat Mat::advance() const {
  Mat newGeneration{*this};

  for (const Cell &cell : m_cells) {
    newGeneration.setState({cell.coor}, getNewState(cell));
  }

  return newGeneration;
}

auto Mat::convertToIndex(Coordinates coor) const -> std::size_t {
  return coor.row * m_stat.maxCols + coor.col;
}

auto Mat::getNewState(Cell cell) const -> State {
  const Neightbours n = getNeightbours(cell.coor);
  const int aliveN = std::accumulate(
      std::begin(n), std::end(n), 0, [&](int accumulate, const Cell &item) {
        return accumulate + (item.state == State::live ? 1 : 0);
      });

  if (cell.state == State::dead) {
    return aliveN == 3 ? State::live : State::dead;
  }

  if (aliveN < 2 or aliveN > 3)
    return State::dead;

  return State::live;
}

auto Mat::getState(Coordinates coor) const -> State {
  return m_cells.at(convertToIndex(coor)).state;
}

auto Mat::getNeightbours(Coordinates coor) const -> Neightbours {
  Neightbours n;
  for (const Coordinates &coor : CoorContainer{
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
                       modulo(coor.row + 1, m_stat.maxRows)}}) {
    n.push_back({coor, m_cells.at(convertToIndex(coor)).state});
  }

  return n;
}

} // namespace gol
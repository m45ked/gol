#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

namespace gol {

using ColNo = std::size_t;
using RowNo = std::size_t;

struct Coordinates {
  ColNo col;
  RowNo row;
};

enum class State { live, dead };
struct Cell {
  Coordinates coor{};
  State state{State::dead};

  operator std::string() const { return state == State::dead ? "." : "x"; }
};

using CoorContainer = std::vector<Coordinates>;

using Cells = std::vector<Cell>;
using Neightbours = Cells;

struct Stat {
  ColNo maxCols;
  RowNo maxRows;
};

class Mat {
public:
  Mat(Stat stat);
  Mat(const Mat &) = default;
  auto setAlive(CoorContainer coor) -> void;
  auto setDead(CoorContainer coor) -> void;
  auto setState(CoorContainer coor, State state) -> void;

  Mat advance() const;

  operator std::string() const {
    std::ostringstream output;

    std::for_each(std::cbegin(m_cells), std::cend(m_cells),
                  [&output](const Cell &cell) { output << std::string{cell}; });

    return output.str();
  }

private:
  auto convertToIndex(Coordinates coor) const -> std::size_t;
  auto getNewState(Cell cell) const -> State;
  auto getState(Coordinates coor) const -> State;
  auto getNeightbours(Coordinates coor) const -> Neightbours;

  Stat m_stat;
  Cells m_cells;
};

} // namespace gol
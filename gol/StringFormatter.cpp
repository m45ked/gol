#include "StringFormatter.hpp"

#include <sstream>
#include <string>
#include <utility>

#include "mat.hpp"

namespace gol {

StringFormatter::StringFormatter(Stat stat) : m_stat(std::move(stat)) {}

std::string StringFormatter::format(const Mat &mat) const {
  const std::string matStr{mat};

  std::ostringstream oss;

  for (RowNo i = 0; i < m_stat.maxRows; ++i) {
    oss << matStr.substr(i * m_stat.maxCols, m_stat.maxRows) << std::endl;
  }

  return oss.str();
}

} // namespace gol

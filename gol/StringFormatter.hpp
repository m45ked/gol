#pragma once

#include "mat.hpp"

namespace gol {

struct StringFormatter {
public:
  StringFormatter(Stat stat);
  std::string format(const Mat &mat) const;

private:
  Stat m_stat;
};

} // namespace gol
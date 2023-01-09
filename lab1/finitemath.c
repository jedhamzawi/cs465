#include "finitemath.h"

GROUP ffadd(const GROUP group1, const GROUP group2) {
  return group1 ^ group2;
}
#include "misc.hpp"
#include <limits>
const Weapon weapons[WT_MAX] = {
	{"none", std::numeric_limits<float>::infinity(), ""},
	{"crossbow", 3.0f, "assets/scripts/projectile_cross.json"}
};
#include "misc.hpp"
#include <limits>
const Weapon weapons[WT_MAX] = {
	{"none", std::numeric_limits<float>::infinity(), "", false},
	{"crossbow", 3.0f, "assets/scripts/projectile_cross.json", false},
	{"launcher", 1.0f, "assets/scripts/projectile_holy_water.json", true},
	{"suncannon", 6.0f, "assets/scripts/projectile_sun.json", true}
};
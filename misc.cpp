#include "misc.hpp"
std::string FromWT(WeaponType t){
	switch (t) {
		case WT_CROSSBOW:
			return "crossbow";
	}
	return "none";
}
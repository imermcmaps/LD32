#ifndef LD32_MISC_HPP
#define	LD32_MISC_HPP
#include <string>
#include <Engine/Node.hpp>
enum WeaponType {
	WT_NONE,
	WT_CROSSBOW,
	WT_MAX
};
struct Weapon {
	const char* name;
	float shootDelay;
	const char* projectileFile;
};
extern const Weapon weapons[WT_MAX];

enum LD32NodeTypes {
	NT_PROJECTILE = engine::NT_END,
	NT_ENEMY,
};
#endif	/* LD32_MISC_HPP */


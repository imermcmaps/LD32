#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include "Slayer.hpp"
#include "WeaponSelector.hpp"
#include "Projectile.hpp"
#include "Enemy.hpp"
#include "Bat.hpp"
int main() {
	engine::Factory::RegisterType("slayer", engine::Factory::CreateChildNode<Slayer>);
	engine::Factory::RegisterType("selector", engine::Factory::CreateChildNode<WeaponSelector>);
	engine::Factory::RegisterType("projectile", engine::Factory::CreateChildNode<Projectile>);
	engine::Factory::RegisterType("enemy", engine::Factory::CreateChildNode<Enemy>);
	engine::Factory::RegisterType("bat", engine::Factory::CreateChildNode<Bat>);
	LD32 game;
	game.run();
	return 0;
}
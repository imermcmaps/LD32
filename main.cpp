#include "LD32.hpp"
#include <Engine/Factory.hpp>
#include "Slayer.hpp"
#include "WeaponSelector.hpp"
#include "Projectile.hpp"
#include "Damagable.hpp"
#include "Bat.hpp"
#include "Level.hpp"
#include "Vampire.hpp"
#include <Engine/Node.hpp>
int main() {
	engine::Factory::RegisterType("slayer", engine::Factory::CreateChildNode<Slayer>);
	engine::Factory::RegisterType("selector", [](Json::Value& root, engine::Node* parent){ 
		engine::Node* n = engine::Factory::CreateChildNode<WeaponSelector>(root, parent);
		if (n) {
			LD32* g = static_cast<LD32*>(n->GetScene()->GetGame());
			auto u = [&](WeaponType wt){
				auto s = static_cast<engine::SpriteNode*>(n->GetChildByID(weapons[wt].name));
				if (s) {
					if (g->IsUnlocked(wt)) {
						s->PlayAnimation("default");
					} else {
						s->PlayAnimation("disabled");
					}
				}
			};
			u(WT_CROSSBOW);
			u(WT_LAUNCHER);
			u(WT_SUNCANNON);
			static_cast<engine::SpriteNode*>(n->GetChildByID("none"))->PlayAnimation("active");
		}
		return n;
	});
	engine::Factory::RegisterType("projectile", engine::Factory::CreateChildNode<Projectile>);
	engine::Factory::RegisterType("bat", engine::Factory::CreateChildNode<Bat>);
	engine::Factory::RegisterType("vampire", engine::Factory::CreateChildNode<Vampire>);
	LD32 game;
	game.run();
	return 0;
}
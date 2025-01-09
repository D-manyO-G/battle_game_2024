#include "battle_game/core/bullets/arrow_bullet.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
ArrowBullet::ArrowBullet(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity,
					   float limit_distance)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity) , limit_distance_(limit_distance) {
}

void ArrowBullet::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
  DrawModel(0);
}

void ArrowBullet::Update() {
  position_ += velocity_ * kSecondPerTick;
  move_distance_ += glm::length(velocity_) * kSecondPerTick;
  bool should_die = move_distance_ >= limit_distance_;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }


  if (should_die == false)
  {
  	auto &units = game_core_->GetUnits();  
	  for (auto &unit : units) {
	    if (unit.first == unit_id_) {
	      continue;
	    }
	    if (unit.second->IsHit(position_)) {
	      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 5.0f * (1.0f - move_distance_ / limit_distance_));
	      should_die = true;
	    }
	  }	
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

ArrowBullet::~ArrowBullet() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet

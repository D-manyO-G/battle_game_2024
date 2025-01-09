#include "battle_game/core/bullets/cannon_ball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"

namespace battle_game::bullet {
Explosive::Explosive(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity,
					   float damage_range,
					   float detect_range)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity), damage_range_(damage_range), detect_range_(detect_range) {
}

void Explosive::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/particle3.png");
//  for (float deg = 0.0; deg < 2*pi; deg += pi/4)
//  	Set
  DrawModel(0);
}

void Explosive::Bomb() {
  auto &units = game_core_->GetUnits();
  for (auto & unit : units) {
    auto dis = glm::distance(unit.second-> GetPosition(), position_);
    if (dis <= damage_range_) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f * (1.0 - pow(dis / damage_range_ , 2)));
    }
  }
}

void Explosive::Update() {
  position_ += velocity_ * kSecondPerTick;
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }

  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (glm::distance(unit.second->GetPosition(), position_) <= detect_range_) {
      should_die = true;
    }
  }

  if (should_die) {
  	Bomb();
    game_core_->PushEventRemoveBullet(id_);
  }
}

Explosive::~Explosive() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet

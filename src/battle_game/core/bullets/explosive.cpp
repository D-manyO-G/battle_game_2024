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
  SetTransformation(position_, rotation_, glm::vec2{0.5f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture(BATTLE_GAME_ASSETS_DIR "textures/bomber1.png");
  DrawModel(0);
  const float pi = 3.1415926535f;
  const int points = 12;
  for (float deg = 0.0; deg < 2*pi; deg += pi * 2 / points)
  {
    SetTransformation(position_ + Rotate(glm::vec2{0.0f, detect_range_}, deg), 0.0f, glm::vec2{0.1f});
    SetColor({0.0f,0.0f,1.0f,0.2f});
    SetTexture(BATTLE_GAME_ASSETS_DIR "textures/bomber0.png");
  DrawModel(0);
    SetTransformation(position_ + Rotate(glm::vec2{0.0f, damage_range_}, deg + pi / points), 0.0f, glm::vec2{0.15f});
    SetColor({1.0f,0.0f,0.0f,0.2f});
    SetTexture(BATTLE_GAME_ASSETS_DIR "textures/bomber0.png");
  DrawModel(0);
  }
}

void Explosive::Bomb() {
  auto &units = game_core_->GetUnits();
  for (auto & unit : units) {
    auto dis = glm::distance(unit.second-> GetPosition(), position_);
    if (dis <= damage_range_) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 30.0f * (1.0 - pow(dis / damage_range_ , 2)));
    }
  }
}

void Explosive::Update() {
  position_ += velocity_ * kSecondPerTick;
  if (glm::length(velocity_) >= 5.0f) velocity_ = velocity_ * 0.9f;
  bool should_die = false;
  if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }
  auto player = game_core_->GetPlayer(player_id_);
  auto &input_data = player->GetInputData();
  if (input_data.key_down[GLFW_KEY_C]) {
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

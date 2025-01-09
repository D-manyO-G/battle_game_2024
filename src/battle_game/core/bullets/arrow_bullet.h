#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class ArrowBullet : public Bullet {
 public:
  ArrowBullet(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity,
             float limit_distance = 20.0f);
  ~ArrowBullet() override;
  void Render() override;
  void Update() override;

 private:
  glm::vec2 velocity_{};
  float move_distance_{0.0f};
  float limit_distance_{0.0F}; 
};
}  // namespace battle_game::bullet

#pragma once
#include "battle_game/core/bullet.h"

namespace battle_game::bullet {
class Explosive : public Bullet {
 public:
  Explosive(GameCore *core,
             uint32_t id,
             uint32_t unit_id,
             uint32_t player_id,
             glm::vec2 position,
             float rotation,
             float damage_scale,
             glm::vec2 velocity,
			 float damage_range = 5.0f,
			 float detect_range = 3.0f);
  ~Explosive() override;
  void Render() override;
  void Update() override;
  void Bomb();

 private:
  glm::vec2 velocity_{};
  float damage_range_{2.0f};
  float detect_range_{1.0f}; 
};
}  // namespace battle_game::bullet

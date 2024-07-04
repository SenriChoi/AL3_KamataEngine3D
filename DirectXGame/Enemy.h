#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <numbers>

class Player;

class Enemy {

	public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();
	WorldTransform& GetWorldTransform() { return worldTransform_; }


	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Player* player);

	private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

	static inline const float kWalkSpeed = 0.05f;
	Vector3 velocity_ = {-kWalkSpeed, 0.0f, 0.0f};

	
	static inline const float kWalkMotionAngleStart = -1 * std::numbers::pi_v<float>; // 角度
	static inline const float kWalkMotionAngleEnd = 2 * std::numbers::pi_v<float>;    // 角度
	static inline const float kWalkMotionTime = 2.0f; 

	float walkTimer_ = 0.0f;

	static inline const float kWidth = 1.99f;
	static inline const float kHeight = 1.99f;
};

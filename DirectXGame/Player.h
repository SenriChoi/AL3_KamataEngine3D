#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
#include "Model.h"
#include "cassert"
#include<numbers>
#include "Input.h"
#include "calculation.h"
#include <algorithm>


class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3&position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform();
	
	const Vector3& GetVelocity() const { return velocity_; }


private:
	WorldTransform worldTransform_;


	ViewProjection *viewProjection_= nullptr;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;
	//uint32_t textureHandle_ = 0u;
	enum class LRDirection {
		kRight,
		kLeft,
	};
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn= 0.3f;

	bool onGround_ = true;
	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 0.8f;
	static inline const float kJumpAcceleration = 0.5f;

};

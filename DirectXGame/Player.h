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
#include "MapChipField.h"

struct CollisionMapInfo {
	bool isCeiling = false;
	bool isLanding = false;
	bool isWall = false;
	Vector3 move;

};

	enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorners // 要素数
	};

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform();

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField;}
	void playerMove();
	void MapCollision(CollisionMapInfo& info);
	void MapCollisionTop(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	//void MapCollisionRight(CollisionMapInfo& info);
	//void MapCollisionLeft(CollisionMapInfo& info);
	Vector3 CornerPostion(const Vector3& center, Corner corner);

	void collisionResult(CollisionMapInfo& info);

	void isCeilingCollision(CollisionMapInfo& info);
	void isLandingCollision(CollisionMapInfo& info);
	

	 private:
	WorldTransform worldTransform_;


	ViewProjection *viewProjection_= nullptr;
	Model* model_ = nullptr;
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.5f;
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
	static inline const float kGravityAcceleration = 0.04f;
	static inline const float kLimitFallSpeed = 0.8f;
	static inline const float kJumpAcceleration = 0.51f;

	//mapchip　field

	MapChipField* mapChipField_ = nullptr;
	static inline const float kWidth = 2.0f;
	static inline const float kHeight =2.0f;

	static inline const float kBlank = 2.0f;

	// 着地时速度衰减率
	static inline const float kAttenuationLanding = 0.0f;
	// 撞墙减速率
	static inline const float kAttenuationWall = 0.0f;



};

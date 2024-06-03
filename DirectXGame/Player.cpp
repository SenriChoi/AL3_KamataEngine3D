#define NOMINMAX
#include "Player.h"


Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	viewProjection_=viewProjection;

}


void Player::Update() {
	bool landing = false;//着地FLAG

	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 2.0f) { 
			landing = true;
		}
	}

	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}
	
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 0.2f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = 0.2f;
			}
		}
	
		// 加減速
		velocity_ = Add(velocity_, acceleration);
		// 制限速度
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		velocity_.x *= (1.0f - kAttenuation);
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		velocity_ = Add(velocity_, {0.0f, kJumpAcceleration, 0.0f});
	
	}
	} else //空中
	{
		velocity_ = Add(velocity_,Vector3 (0,-kGravityAcceleration,0));
		velocity_.y=std::max(velocity_.y,-1*kLimitFallSpeed);
		if (landing) {
			worldTransform_.translation_.y = 2.0f; 
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}


	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60;
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,
		    std::numbers::pi_v<float> * 3.0f / 2.0f,
		};
		// 角度取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		float timeRatio = 1 - turnTimer_ / kTimeTurn;
		float easing = timeRatio;
		float nowRotationY = std::lerp(turnFirstRotationY_, destinationRotationY, easing);
		worldTransform_.rotation_.y = nowRotationY;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	//畫面制限(今当たり判定がないから）
	if (worldTransform_.translation_.x > 16) {
		worldTransform_.translation_.x = 16;
	}
	if (worldTransform_.translation_.x < 2) {
		worldTransform_.translation_.x = 2;
	}


	worldTransform_.UpdateMatrix();
}


void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }
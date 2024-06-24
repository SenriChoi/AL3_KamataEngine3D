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
	onGround_ = true;

}

void Player ::playerMove() {
	
		velocity_ = Add(velocity_, Vector3(0, -kGravityAcceleration, 0));
		velocity_.y = std::max(velocity_.y, -1 * kLimitFallSpeed);
	

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
	//JUMP
	if (onGround_) {
		if(Input::GetInstance()->PushKey(DIK_UP)) { velocity_ = Add(velocity_, {0.0f, kJumpAcceleration, 0.0f}); 
		onGround_ = false; // 起跳时不再着地
		}
	}
	}


void Player::Update() {
	playerMove();

	/// 衝突
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	MapCollision(collisionMapInfo);
	collisionResult(collisionMapInfo);
	isCeilingCollision(collisionMapInfo);
	isLandingCollision(collisionMapInfo);


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


// ******** 处理水平方向移动 ********
	worldTransform_.translation_ = Add(worldTransform_.translation_, Vector3(velocity_.x, 0.0f, 0.0f));

	// ******** 如果不在地面上，应用重力 ********
	if (!onGround_) {
		worldTransform_.translation_ = Add(worldTransform_.translation_, Vector3(0.0f, velocity_.y, 0.0f));
	}

	//畫面制限(今当たり判定がないから）
	if (worldTransform_.translation_.x > 50) {
		worldTransform_.translation_.x = 50;
	}
	if (worldTransform_.translation_.x < 2) {
		worldTransform_.translation_.x = 2;
	}


	worldTransform_.UpdateMatrix();
}

Vector3 Player::CornerPostion(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorners] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
};


void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }
//
void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);

};
void Player::MapCollisionTop(CollisionMapInfo& info){
	if (info.move.y <= 0) {
		return;
	}

	std::array<Vector3, kNumCorners> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}


	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右上点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {0.0f, kHeight / 2.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(worldTransform_.translation_, info.move),offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.bottom - worldTransform_.translation_.y + kBlank) - (kHeight / 2.0f);
		info.move.y = std::max(0.0f, moveY);
		info.isCeiling = true;
	}


};


void Player::MapCollisionBottom(CollisionMapInfo& info) {
	if (info.move.y >= 0) {
		return;
	}

	std::array<Vector3, kNumCorners> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPostion(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}


	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
		cameraStop = true;
	}
	// 右下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
		cameraStop = true;
	}

   if (hit) {
		// 检测到碰撞
	   Vector3 offset = {0.0f, kHeight / 2.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(worldTransform_.translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);
		float moveY = (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f) ;

		info.move.y = std::min(0.0f, moveY);
		info.isLanding = true;
	}
   else {
        info.isLanding = false; // ******** 确保未检测到碰撞时将 isLanding 设置为 false ********
    }

}

void Player::collisionResult(CollisionMapInfo& info) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, info.move); 

}

void Player::isCeilingCollision(CollisionMapInfo& info) {
	if (info.isCeiling) {
		velocity_.y = 0.0f;
	}
}

void Player::isLandingCollision(CollisionMapInfo& info) {
	if (onGround_) {
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			const float kSmallOffset = 0.01f;
			std::array<Vector3, kNumCorners> positionsNew;
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPostion(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
			}

			if (info.move.y <= 0) {
				return;
			}

			MapChipType mapChipType;
			bool hit = false;
			MapChipField::IndexSet indexSet;

			// 左下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(positionsNew[kLeftBottom], Vector3(0, kSmallOffset, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
				cameraStop = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(positionsNew[kRightBottom], Vector3(0, kSmallOffset, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
				cameraStop = true;
			}

			if (!hit) {
				onGround_ = false; // 修改为未检测到块时仍然设置为不在地面上
			}
		}
	} else {
		if (info.isLanding) {
			velocity_.y = 0.0f;
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
		} else {
			onGround_ = false; // 增加离开块时的处理逻辑
		}
	}
}



//void Player::MapCollisionBottom(CollisionMapInfo& info){};
//void Player::MapCollisionRight(CollisionMapInfo& info){};
//void Player::MapCollisionLeft(CollisionMapInfo& info){};


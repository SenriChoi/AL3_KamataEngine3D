#define NOMINMAX
#include "Player.h"


Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float>;
	viewProjection_=viewProjection;
}

void Player::Update() {
	playerMove();
	/// 衝突
	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	MapCollision(collisionMapInfo);
	collisionResult(collisionMapInfo);
	isCeilingCollision(collisionMapInfo);
	isWallCollision(collisionMapInfo);
	isLandingCollision(collisionMapInfo);


	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60;
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float>-0.5f,
		    std::numbers::pi_v<float>+0.5f,
		};
		// 角度取得
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];



		float timeRatio = 1 - turnTimer_ / kTimeTurn;
		float easing = timeRatio;
	float diff = destinationRotationY - turnFirstRotationY_;
	float nowRotationY = std::lerp(turnFirstRotationY_, turnFirstRotationY_ + diff, easing);
		worldTransform_.rotation_.y = nowRotationY;
	}

	worldTransform_.UpdateMatrix();
}

void Player ::playerMove() {

if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 旋转
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = std::numbers::pi_v<float> + 0.5f;
				turnTimer_ = 0.2f;
			}
			// 减速
			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 旋转
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = std::numbers::pi_v<float> -0.5f;
				turnTimer_ = 0.2f;
			}
			// 减速
			if (velocity_.x > 0.0f) {
				velocity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
		velocity_ = Add(velocity_, acceleration);
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} // 非按键时速度衰减
	else {
		velocity_.x *= (1.0f - kAttenuation);
	}
	// 跳跃
	if (onGround_) {
	

	/*	if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ = Add(velocity_, {0.0f, kJumpAcceleration, 0.0f});
		}*/
	}

}


Vector3 Player::CornerPostion(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorners] = {
	    {kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
        {kWidth / 2.0f, kHeight / 2.0f, 0.0f},
        {-kWidth / 2.0f, kHeight / 2.0f, 0.0f}
    };
	return Add(center, offsetTable[static_cast<uint32_t>(corner)]);
};


void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }




Vector3 Player::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

WorldTransform& Player::GetWorldTransform() { return worldTransform_; }


AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};
	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	//velocity_ =Add(velocity_, Vector3(0, 2.0f, 0));
	isDead_ = true;
}

void Player::MapCollision(CollisionMapInfo& info) {
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionRight(info);
	MapCollisionLeft(info);

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
		cameraStopY = true;
	}
	// 右下点
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
		cameraStopY = true;
	}

	if (indexSet.yIndex == 69) {
		playerClear = true;
	}
   if (hit) {
		// 检测到碰撞
	   Vector3 offset = {0.0f, kHeight / 2.0f, 0.0f};
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(worldTransform_.translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);
		//float moveY = (rect.top - worldTransform_.translation_.y) + (kHeight / 2.0f) + kBlank;

		//info.move.y = std::min(0.0f, moveY);
		//info.isLanding = true;
	

   } else {
	   info.isLanding = false;
   }


}


void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.move.x <= 0) {
		return;
	}

	std::array<Vector3, kNumCorners> positionsNew;
	for (uint32_t i = 0; i < kNumCorners; ++i) {
		positionsNew[i] = CornerPostion(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 右上角
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下角
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {kWidth / 2.0f, 0.0f, 0.0f}; 
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(Add(worldTransform_.translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.left - worldTransform_.translation_.x - kWidth / 2.0f - kBlank;
		info.move.x = std::max(0.0f, moveX);
		info.isWall = true;
	}
	else
	{
		info.isWall = false;
	}
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.move.x >= 0) {
		return;
	}

	std::array<Vector3, kNumCorners> positionsNew;
	for (uint32_t i = 0; i < kNumCorners; ++i) {
		positionsNew[i] = CornerPostion(Add(worldTransform_.translation_, info.move), static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	bool hit = false;
	MapChipField::IndexSet indexSet;

	// 左上角
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下角
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		Vector3 offset = {kWidth / 2.0f, 0.0f, 0.0f}; 
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(Subtract(Add(worldTransform_.translation_, info.move), offset));
		MapChipField::Rect rect = mapChipField_->GetRectByIndexSet(indexSet.xIndex, indexSet.yIndex);

		float moveX = rect.right - worldTransform_.translation_.x + kWidth / 2.0f + kBlank;
		info.move.x = std::min(0.0f, moveX);
		info.isWall = true;
	} else {
		info.isWall = false;
	}
}

void Player::collisionResult(CollisionMapInfo& info) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, info.move); 

}

void Player::isCeilingCollision(CollisionMapInfo& info) {
	if (info.isCeiling) {
		velocity_.y = 0.0f;
	}
	if (info.isLanding) {
		velocity_.y = 0.0f;
	}
}

void Player::isWallCollision(CollisionMapInfo& info) {
	if (info.isWall) {
		velocity_.x *= (1 - kAttenuationWall);
		cameraStopX = true;

	}
}

void Player::isLandingCollision(CollisionMapInfo& info) {
	velocity_ = Add(velocity_, {0.0f, -kGravityAcceleration, 0.0f});
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);

	if (onGround_) {
		if (velocity_.y >0.0f) {
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
				cameraStopY = true;
			}
			// 右下
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(Add(positionsNew[kRightBottom], Vector3(0, kSmallOffset, 0)));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
				cameraStopY = true;
				
			}

			if (!hit) {
				onGround_ = false; // 修改为未检测到块时仍然设置为不在地面上
			}
		}
	} else {
		if (info.isLanding) {
			onGround_ = true;
			velocity_.x *= (1.0f - kAttenuationLanding);
			velocity_.y = 0.0f;

		} 
	}
}


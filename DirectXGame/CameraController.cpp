#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <cmath>
#include <iostream>


void CameraController::Initialize(ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

void CameraController::Update() { 
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	Vector3 targetVelocity = target_->GetVelocity();
	targetPosition = Add(Add(targetWorldTransform.translation_, targetOffset_), Multiply( kVelocityBias,targetVelocity));
	viewProjection_->translation_ .x= std::lerp(viewProjection_->translation_.x, targetPosition.x, kInterpolationRate);
	viewProjection_->translation_.y = std::lerp(viewProjection_->translation_.y, targetPosition.y, kInterpolationRate);

	viewProjection_->translation_.x = std::max(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.left);
	viewProjection_->translation_.x = std::min(viewProjection_->translation_.x, targetWorldTransform.translation_.x + margin.right);
	viewProjection_->translation_.y = std::max(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.bottom);
	viewProjection_->translation_.y = std::min(viewProjection_->translation_.y, targetWorldTransform.translation_.y + margin.top);


	//移動範囲制限
	viewProjection_->translation_.x = std::max(movableArea_.left, std::min(viewProjection_->translation_.x, movableArea_.right));
	viewProjection_->translation_.y = std::max(movableArea_.bottom, std::min(viewProjection_->translation_.y, movableArea_.top));

	viewProjection_->UpdateMatrix();

}

void CameraController::Reset() {
	// ワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//カメラの座標を計算
	viewProjection_->translation_ = Add(targetWorldTransform.translation_, targetOffset_);
	viewProjection_->translation_.x = std::max(movableArea_.left, std::min(viewProjection_->translation_.x, movableArea_.right));
	viewProjection_->translation_.y = std::max(movableArea_.bottom, std::min(viewProjection_->translation_.y, movableArea_.top));
}
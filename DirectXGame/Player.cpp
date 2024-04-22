#include "Player.h"


void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_=viewProjection;
}


void Player::Update() {
	worldTransform_.TransferMatrix();
}


void Player::Draw() { 
}
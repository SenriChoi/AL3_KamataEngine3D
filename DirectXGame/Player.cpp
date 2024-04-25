#include "Player.h"
#include"WorldTransformEx.h"

Player::Player(){};

Player::~Player(){};

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	viewProjection_=viewProjection;
}


void Player::Update() {
	worldTransform_.TransferMatrix();
	\\worldTransformBlock->matWorld_ = MakeAffineMatrix(
	worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_
	);

}


void Player::Draw() { 
}
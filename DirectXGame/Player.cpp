#include "Player.h"
#include"WorldTransformEx.h"

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
worldTransform_.TransferMatrix();

}


void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); }
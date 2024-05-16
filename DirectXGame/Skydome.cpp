#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
	worldTransform_.scale_ = {1,1,1};
}

void Skydome::Update() {  } 

void Skydome::Draw() { model_->Draw(worldTransform_, *viewProjection_); }
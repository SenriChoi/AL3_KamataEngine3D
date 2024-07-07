#include "DeathParticles.h"

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;

	for (auto& worldTransfrom : worldTransforms_) {
		worldTransfrom.Initialize();
		worldTransfrom.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
}


void DeathParticles::Update() {

	counter_ += 1.0f / 60.0f;
	if (counter_ > kDuration) {
		counter_ = kDuration;
		isFinished_ = true;
	}

	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ = Add(worldTransforms_[i].translation_, velocity);
	}
	for (auto& worldTransfrom : worldTransforms_) {
		worldTransfrom.UpdateMatrix();
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();

}

void DeathParticles::Draw() {
	if (isFinished_) {
		return;
	}
	for (auto& worldTransfrom : worldTransforms_) {
		model_->Draw(worldTransfrom, *viewProjection_, &objectColor_);
	}
}
#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "cassert"

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection *viewProjection_= nullptr;
	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

};

#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "cassert"
#include<numbers>

class Player {
public:
	Player();
	~Player();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3&position);

	void Update();

	void Draw();

private:
	WorldTransform worldTransform_;
	ViewProjection *viewProjection_= nullptr;
	Model* model_ = nullptr;
	//uint32_t textureHandle_ = 0u;

};

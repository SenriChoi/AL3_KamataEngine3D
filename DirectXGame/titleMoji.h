#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>
#include <cassert>
#include <numbers>

class titleMoji {
private:
	float deltaTime;
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();
};
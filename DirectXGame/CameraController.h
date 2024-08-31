#pragma once
#define NOMINMAX
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"WorldTransformEx.h"
#include <algorithm>


class Player;
class CameraController {

	public:
	void Initialize(ViewProjection* viewProjection);
	void Update();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void SetMovableArea(Rect area){ movableArea_=area;	};

	private:
	ViewProjection* viewProjection_ = nullptr;
	Player* target_ = nullptr;
	Vector3 targetOffset_ = {0.0f,0.0f,-15.0f};
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};
	Vector3 targetPosition;
	static inline const float kInterpolationRate = 0.5f;
	static inline const float kVelocityBias = 8.0f;
	//画面内に収める
	static inline const Rect margin = {-100, 100, -100, 100};
};

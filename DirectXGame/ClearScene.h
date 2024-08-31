#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "cassert"
#include "titleMoji.h"
#include <algorithm>

class ClearScene {
private:
	bool finished_ = false;

	Model* modelClear_ = nullptr;
	Model* modelTitlePlayer_ = nullptr;
	ViewProjection viewProjection_;

	DirectXCommon* dxCommon_ = nullptr;

	titleMoji* moji_ = nullptr;
	Player* player_ = nullptr;

public:
	ClearScene();
	~ClearScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};

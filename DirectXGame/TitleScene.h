#pragma once
#include "DirectXCommon.h"
#include "cassert"
#include <algorithm>
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "titleMoji.h"

class TitleScene {
private:
	bool finished_ = false;

	Model* modelTitle_ = nullptr;
	Model* modelTitlePlayer_ = nullptr;
	ViewProjection viewProjection_;

	DirectXCommon* dxCommon_ = nullptr;

	titleMoji* moji_ = nullptr;
	Player* player_ = nullptr;


public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }

};

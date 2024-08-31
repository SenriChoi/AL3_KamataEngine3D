#include "ClearScene.h"
ClearScene::ClearScene() {}

ClearScene::~ClearScene() {
	delete modelClear_;
	delete moji_;
}

void ClearScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	modelClear_ = Model::CreateFromOBJ("ClearMoji", true);

	moji_ = new titleMoji();
	Vector3 position = {-32.0f, 10.0f, 5.0f};
	moji_->Initialize(modelClear_, &viewProjection_, position);
}

void ClearScene::Update() {
	moji_->Update();
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void ClearScene::Draw() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	moji_->Draw();
	Model::PostDraw();
}

#include "TitleScene.h"
TitleScene::TitleScene() {}


TitleScene::~TitleScene() {
	delete modelTitle_;
	delete moji_;
}


void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	modelTitle_ = Model::CreateFromOBJ("TitleMoji", true);

	moji_ = new titleMoji();
	Vector3 position = {-32.0f, 10.0f, 5.0f};
	moji_->Initialize(modelTitle_, &viewProjection_, position);


}

	
	void TitleScene::Update() {
	moji_->Update();
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

	void TitleScene::Draw() { 

		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	    Model::PreDraw(commandList);
	    moji_->Draw();
	    Model::PostDraw();
	}

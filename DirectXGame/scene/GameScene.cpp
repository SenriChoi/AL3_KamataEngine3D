#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>





GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
	delete skydemo_;

	delete modelPlayer_;

	for (std::vector<WorldTransform*>& worldTransforBlockLine : worldTransformBlock_) {
		for (WorldTransform* worldTransformBlock : worldTransforBlockLine) {
			delete worldTransformBlock;
		}
		worldTransformBlock_.clear();
	}
	delete mapChipField_;
}

void GameScene::Initialize() {


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

 // DebugCamera
	debugCamera_ = new DebugCamera(1280, 720);

	viewProjection_.Initialize();
	// テクスチャ読み込む
	textureHandle_ = TextureManager::Load("cube/cube.jpg");
	// ３Dモデルの生成
	model_ = Model::Create();
	modelSkydemo_ = Model::CreateFromOBJ("skydome", true);
	modelPlayer_ = Model::CreateFromOBJ( "player", true);

	// ワールド、ビューの初期化
	/*viewProjection_.farZ=5000;*/

	skydemo_ = new Skydome;
	skydemo_->Initialize(modelSkydemo_, &viewProjection_);


	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/block.csv");


	// player生成+初期化
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	GenerateBlocks();
}

void GameScene::Update() { 
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_SPACE)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	skydemo_->Update();
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}

		debugCamera_->Update();

	for (std::vector<WorldTransform*>& worldTransforBlockLine : worldTransformBlock_) {
		for (WorldTransform* worldTransformBlock : worldTransforBlockLine) {
			if (!worldTransformBlock)
				continue;
			worldTransformBlock->UpdateMatrix();
		}
	}

	player_->Update();

}




void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);
	



	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	/// 
	skydemo_->Draw();
	for (std::vector<WorldTransform*>& worldTransforBlockLine : worldTransformBlock_) {
		for (WorldTransform* worldTransformBlock : worldTransforBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	player_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


void GameScene::GenerateBlocks() {
	uint32_t numBlockVertical = mapChipField_->GetNumBlockVertical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
	worldTransformBlock_.resize(numBlockVertical);
	for (uint32_t i = 0; i < numBlockVertical; i++) {
		worldTransformBlock_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVertical; i++) {
		for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlock_[i][j] = worldTransform;
				worldTransformBlock_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
};
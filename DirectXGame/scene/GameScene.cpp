#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>





GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	//delete block;
	for (WorldTransform* worldTransformBlock : worldTransformBlock_) {
	delete worldTransformBlock;
	}
	worldTransformBlock_.clear();
}

void GameScene::Initialize() {
	//要素数
	const uint32_t kNumBlockHorizontal = 20;
	//ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	//要素数を変更する
	worldTransformBlock_.resize(kNumBlockHorizontal);

	//キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlock_[i] = new WorldTransform();
		worldTransformBlock_[i]->Initialize();
		worldTransformBlock_[i]->translation_.x = kBlockWidth*i;
		worldTransformBlock_[i]->translation_.y = 0.0f;
	}

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// テクスチャ読み込む
	textureHandle_ = TextureManager::Load("cube/cube.png");
	//３Dモデルの生成
	model_ = Model::Create();
	// ワールド、ビューの初期化
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//player生成+初期化
	player_ = new Player();
	player_->Initialize(model_, textureHandle_, &viewProjection_);

}

void GameScene::Update() { player_->Update(); 
	for (WorldTransform* worldTransformBlock : worldTransformBlock_) {
		worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	}
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
	for (WorldTransform* worldTransformBlock : worldTransformBlock_) {
		model_->Draw(*worldTransformBlock, viewProjection_);
	}
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
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

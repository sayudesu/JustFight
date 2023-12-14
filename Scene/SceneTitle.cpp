#include "SceneTitle.h"
#include <DxLib.h>
#include "../SlideSelect.h"
#include "SceneDebug.h"
#include "SceneMain.h"
#include "../SlideSelect.h"
#include "../Util/Game.h"
#include "../GameObject.h"
#include "../Object/Camera/Camera.h";

SceneTitle::SceneTitle():
	m_hTitle(-1),
	m_isInputController(false),
	m_bgPos(VGet(static_cast<float>(Game::kScreenWidth / 2), static_cast<float>(Game::kScreenHeight / 2), 0.0f))
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_hTitle = LoadGraph("Data/Image/UI/ゲーム難易度選択ベース2.png");

	m_select = std::make_unique<SlideSelect>();
	m_select->Init();

	// カメラインスタンス
	m_camera = std::make_unique<Camera>();
	// カメラターゲット位置初期化
	m_camera->SetTargetPos(VGet(0.0f, 0.0f, 0.0f));

	m_pStage = std::make_unique<GameObject>(
		GameObject::DataType::THREE,
		"Data/Model/洋館部屋1.mv1",
		VGet(0,0,0),
		VGet(0,0, 180 * DX_PI_F/ 180.0f),
		VGet(1, 1, 1));

	m_hBg = std::make_shared<GameObject>(
		GameObject::DataType::TWO,
		"Data/Image/UI/ゲーム難易度選択ベース2.png",
		m_bgPos,
		3.0f * DX_PI_F / 180.0f,
		0.95f
	);

	m_pStage->SetPos(VGet(0.0f, 0.0f, 0.0f));
	m_pStage->SetRotate(VGet(0.0f, 0.0f, 0.0f));
	m_pStage->Update();
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{
	char deviceName[260];
	char productName[260];
	// コントローラーの接続を確認する
	if (GetJoypadName(DX_INPUT_PAD1, &deviceName[0], &productName[0]) == 0) 
	{
		m_isInputController = false;
	}
	else
	{
		m_isInputController = true;
		return this;
	}

	m_select->Update();

	if (DxLib::CheckHitKey(KEY_INPUT_UP))
	{
		z -= 1.0f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_DOWN))
	{
		z += 1.0f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_LEFT))
	{
		x -= 1.0f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RIGHT))
	{
		x += 1.0f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_SPACE))
	{
		y += 1.0f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_LCONTROL))
	{
		y -= 1.0f;
	}

	if (DxLib::CheckHitKey(KEY_INPUT_Q))
	{
		rY -= 0.03f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_E))
	{
		rY += 0.03f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RCONTROL))
	{
		rX += 0.03f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RSHIFT))
	{
		rX -= 0.03f;
	}

	static float x1 = -25.0f;
	static float y1 =  21.0f;
	static float z1 = -27.0f;

	m_camera->SetPos(VGet(x1, y1, z1));

	const float speed = 0.4f;
	if (y1 > 10.0f)
	{
		y1 -= speed;
	}
	if (x1 < 0.0f)
	{
		x1 += speed;
	}
	if (z1 < -5.0f)
	{
		z1 += speed;
	}

	m_camera->Setting();



	m_hBg->Update();

	if (m_select->GetResult() == 0)
	{
		printfDx("まだありません\n");
	}

	if (m_select->GetResult() == 1)
	{
		return new SceneMain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
	}

	return this;
}

void SceneTitle::Draw()
{
	m_pStage->Draw();

	// 画像の背景を描画
//	DrawRotaGraph(Game::kScreenWidth/2, Game::kScreenHeight/2, 0.95f, 3.0f * DX_PI_F / 180.0f, m_hTitle, true);
	m_hBg->Draw();
#if _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
	if (m_isInputController)
	{
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
		DrawFormatString((Game::kScreenWidth - ((17 * 32)/2)) / 2, Game::kScreenHeight/2 - 32, 0x000000, "コントローラーを接続してください。");
	}	
}
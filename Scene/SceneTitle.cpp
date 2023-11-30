#include "SceneTitle.h"
#include <DxLib.h>
#include "../SlideSelect.h"
#include "SceneDebug.h"
#include "SceneMain.h"
#include "../SlideSelect.h"

#include "../GameObject.h"

SceneTitle::SceneTitle():
	m_hTitle(-1)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_hTitle = LoadGraph("Data/Image/Title.png");

	m_select = std::make_unique<SlideSelect>();
	m_select->Init();

	// どこから、どこまで見えるか
	SetCameraNearFar(1.0f, 30000.0f);
	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -100), VGet(0, 0, 0));
	// FOV
	SetupCamera_Perspective(80);

	m_pStage = std::make_unique<GameObject>(
		GameObject::DataType::THREE,
		"Data/Model/洋館部屋1.mv1",
		VGet(0,0,0),
		VGet(0,0, 180 * DX_PI_F/ 180.0f),
		VGet(1, 1, 1));

	m_pStage->Update();
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{
	m_select->Update();

	static int z = 0;
	static int y = 0;
	static int x = 0;

	static float rY = 0;
	static float rX = 0;

	if (DxLib::CheckHitKey(KEY_INPUT_UP))
	{
		z--;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_DOWN))
	{
		z++;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_LEFT))
	{
		x--;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RIGHT))
	{
		x++;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_SPACE))
	{
		y++;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_LCONTROL))
	{
		y--;
	}

	if (DxLib::CheckHitKey(KEY_INPUT_Q))
	{
		rY -= 0.1f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_E))
	{
		rY += 0.1f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RCONTROL))
	{
		rX += 0.1f;
	}
	if (DxLib::CheckHitKey(KEY_INPUT_RSHIFT))
	{
		rX -= 0.1f;
	}

	m_pStage->Move(VGet(x, y, z));
	m_pStage->Rotate(VGet(rX, rY, 180 * DX_PI_F / 180.0f));
	m_pStage->Update();

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
//	m_select->Draw();

//	DrawGraph(0, 0, m_hTitle, true);
#if _DEBUG
	DrawString(0, 0, "SceneTitle", 0xffffff);
#endif
}
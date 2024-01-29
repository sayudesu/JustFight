#include <DxLib.h>

#include "PoseScreen.h"

#include "Util/Game.h"
#include "Util/SlideSelect.h"


namespace
{
	// 背景位置
	constexpr int kPoseBgX = 200;
	constexpr int kPoseBgY = 100;
	constexpr int kPoseBgX1 = Game::kScreenWidth - 200;
	constexpr int kPoseBgY1 = Game::kScreenHeight - 100;
}

PoseScreen* PoseScreen::m_pInstance = nullptr;

void PoseScreen::Load()
{
	// インスタンス生成
	for (int i = 0; i < 3; i++)
	{
		m_pSlide[i] = new SlideSelect();
	}

	// 左右にスライド
	m_pSlide[0]->Init(1, false);
	// 上下にスライド
	m_pSlide[1]->Init(3, true);

	int a[3]{};
	a[0] = 0xff0000;
	a[1] = 0x00ff00;
	a[2] = 0x0000ff;
	for (int i = 0; i < 3; i++)
	{
		m_box.push_back
		(
			{
				kPoseBgX + 50 * (i + 1),
				kPoseBgY + 150 * (i + 1),
				kPoseBgX1 - 50,
				kPoseBgY1 + kPoseBgY * (i + 1),
				a[i],
				true
			}
		);
	}

	// 上下にスライド
	m_pSlide[2]->Init(3, true);

	for (int i = 0; i < 2; i++)
	{
		m_blendModeRate[i] = 0;
	}
}

void PoseScreen::Unload()
{
	// メモリの解放
	for (int i = 0; i < 3; i++)
	{
		delete m_pSlide[i];
		m_pSlide[i] = nullptr;
	}
}

void PoseScreen::Update()
{
	// 左右の選択の更新処理
	m_pSlide[0]->Update();
	// 選択をしている上下の更新処理
	if (m_pSlide[0]->GetSelect() == 0)
	{
		m_blendModeRate[0] = 255;
		m_blendModeRate[1] = 255 / 2;

		m_pSlide[1]->Update();

		m_pSlide[2]->Reset();
	}
	else if (m_pSlide[0]->GetSelect() == 1)
	{
		m_blendModeRate[1] = 255;
		m_blendModeRate[0] = 255 / 2;

		m_pSlide[2]->Update();

		m_pSlide[1]->Reset();
	}

	printfDx("X  = %d\n", m_pSlide[0]->GetSelect());
	printfDx("Y1 = %d\n", m_pSlide[1]->GetSelect());
	printfDx("Y2 = %d\n", m_pSlide[2]->GetSelect());
}

void PoseScreen::Draw()
{
	// 背景
	DrawBox(kPoseBgX, kPoseBgY, kPoseBgX1, kPoseBgY1, 0xffffff, true);

	// 左右の選択
	DrawBox(200, 100, Game::kScreenWidth / 2, 200, 0x000000, false);
	DrawBox(Game::kScreenWidth / 2, 100, Game::kScreenWidth - 200, 200, 0x000000, false);

	// 描画ブレンドモードをアルファブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[0]);
	DrawBox(200 - 1, 100 - 1, Game::kScreenWidth / 2 - 1, 200 - 1, 0xaaaaaa, true);
	
	// 描画ブレンドモードをアルファブレンド
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blendModeRate[1]);
	DrawBox(Game::kScreenWidth / 2 - 1, 100 - 1, Game::kScreenWidth - 200 - 1, 200 - 1, 0xaaaaaa, true);

	// 描画ブレンドモードをノーブレンドにする
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 上下の選択しを描画
	for (int i = 0; i < 3; i++)
	{
		DrawBox(m_box[i].x, m_box[i].y, m_box[i].x2, m_box[i].x2, m_box[i].color, m_box[i].fill);
	}

	DrawCircle(kPoseBgX + 50, kPoseBgY + 150 * (0 + 1), 15, 0xffffff);
	DrawCircle(kPoseBgX + 50, kPoseBgY + 150 * (1 + 1), 15, 0xffffff);
	DrawCircle(kPoseBgX + 50, kPoseBgY + 150 * (2 + 1), 15, 0xffffff);

}
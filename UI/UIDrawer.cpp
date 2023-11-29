#include "UIDrawer.h"
#include "../Util/Game.h"
#include <DxLib.h>

namespace
{

}

UIDrawer::UIDrawer()
{
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]          = LoadGraph("Data/Image/ステータスベース.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]   = LoadGraph("Data/Image/馬.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]          = LoadGraph("Data/Image/HPゲージ.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]       = LoadGraph("Data/Image/HPゲージベース.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]          = LoadGraph("Data/Image/必殺技ゲージ.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]       = LoadGraph("Data/Image/必殺技ゲージベース.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)] = LoadGraph("Data/Image/ゲージ枠.png");
}

UIDrawer::~UIDrawer()
{
}

void UIDrawer::Draw()
{
	static float frameCount = 0.0f;
	if (frameCount < 256)
	{
		frameCount += 1.0f;
	}
	// 描画位置テスト
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	// 背景、馬
	DrawGraph(
		0 + 50,
		Game::kScreenHeight - y - 50,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		true);
	DrawGraph(x - 10,
		Game::kScreenHeight - y - 109,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)], 
		true);

	// HP
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)],
		&x1,
		&y1);
	DrawGraph(
		0 + 106,
		Game::kScreenHeight - y - 28,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)],
		true);

	SetDrawArea(
		0 + 106,
		Game::kScreenHeight - y - 28,
		0 + 106 + x1 - frameCount,
		Game::kScreenHeight - y - 28 + y1);// 描画可能領域を指定

	DrawGraph(
		0 + 106,
		Game::kScreenHeight - y - 28,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)],
		true);

	SetDrawArea(
		0,
		0,
		Game::kScreenWidth,
		Game::kScreenHeight);// 描画可能領域を全体に戻す

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y - 28 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)], 
		true);

	// 必殺技
	DrawGraph(
		0 + 106,
		Game::kScreenHeight - y + 15,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)], 
		true);
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)],
		&x1, 
		&y1);

	SetDrawArea(
		0 + 106,
		Game::kScreenHeight - y + 15,
		0 + 106 + x1 - frameCount,
		Game::kScreenHeight - y + 15 + y1);// 描画可能領域を指定

	DrawGraph(
		0 + 106, 
		Game::kScreenHeight - y + 15,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)],
		true);

	SetDrawArea(
		0, 
		0, Game::kScreenWidth,
		Game::kScreenHeight);// 描画可能領域を全体に戻す

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y + 15 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)],
		true);
}

void UIDrawer::SetParam(CharacterName name, CharacterParameter param)
{

}

#include "UIDrawer.h"
#include "../Util/Game.h"
#include <DxLib.h>

namespace
{

}

UIDrawer::UIDrawer()
{
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]          = LoadGraph("Data/Image/�X�e�[�^�X�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]   = LoadGraph("Data/Image/�n.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]          = LoadGraph("Data/Image/HP�Q�[�W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]       = LoadGraph("Data/Image/HP�Q�[�W�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]          = LoadGraph("Data/Image/�K�E�Z�Q�[�W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]       = LoadGraph("Data/Image/�K�E�Z�Q�[�W�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)] = LoadGraph("Data/Image/�Q�[�W�g.png");
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
	// �`��ʒu�e�X�g
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	// �w�i�A�n
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
		Game::kScreenHeight - y - 28 + y1);// �`��\�̈���w��

	DrawGraph(
		0 + 106,
		Game::kScreenHeight - y - 28,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)],
		true);

	SetDrawArea(
		0,
		0,
		Game::kScreenWidth,
		Game::kScreenHeight);// �`��\�̈��S�̂ɖ߂�

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y - 28 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)], 
		true);

	// �K�E�Z
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
		Game::kScreenHeight - y + 15 + y1);// �`��\�̈���w��

	DrawGraph(
		0 + 106, 
		Game::kScreenHeight - y + 15,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)],
		true);

	SetDrawArea(
		0, 
		0, Game::kScreenWidth,
		Game::kScreenHeight);// �`��\�̈��S�̂ɖ߂�

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y + 15 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)],
		true);
}

void UIDrawer::SetParam(CharacterName name, CharacterParameter param)
{

}

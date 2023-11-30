#include "UIDrawer.h"
#include "../Util/Game.h"
#include <DxLib.h>
#include "../GameObject.h"

namespace
{

}

UIDrawer::UIDrawer()
{
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]					= LoadGraph("Data/Image/UI/�X�e�[�^�X�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]		    = LoadGraph("Data/Image/UI/�n.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]					= LoadGraph("Data/Image/UI/HP�Q�[�W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]				= LoadGraph("Data/Image/UI/HP�Q�[�W�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]					= LoadGraph("Data/Image/UI/�K�E�Z�Q�[�W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]              = LoadGraph("Data/Image/UI/�K�E�Z�Q�[�W�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)]        = LoadGraph("Data/Image/UI/�̊��Q�[�W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)] = LoadGraph("Data/Image/UI/�̊��Q�[�W�^��.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)]     = LoadGraph("Data/Image/UI/�̊��Q�[�W�x�[�X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)]      = LoadGraph("Data/Image/UI/�̊��Q�[�W�g.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)]        = LoadGraph("Data/Image/UI/�Q�[�W�g.png");


	// �`��ʒu�e�X�g
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	m_playerPos = VGet(0 + 50, Game::kScreenHeight - y - 50, 0);

	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)] = std::make_unique<GameObject>(
		GameObject::DataType::TWO,
		"Data/Image/UI/�X�e�[�^�X�x�[�X.png",
		m_playerPos,
		VGet(0, 0, 0),
		VGet(0, 0, 0)
		);

	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)] = std::make_unique<GameObject>(
		GameObject::DataType::TWO,
		"Data/Image/UI/�n.png",
		VGet(1000, 0, 0),
		VGet(0, 0, 0),
		VGet(0, 0, 0),
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]->Update();
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

	DrawAreaResult(
		0 + 106,
		Game::kScreenHeight - y - 28,
		0 + 106 + x1 * m_hpNum[static_cast<int>(CharacterName::PLAYER)] / 6,         //����* HP/ HPMAX
		Game::kScreenHeight - y - 28 + y1);// �`��\�̈���w��

	UpdateHp(
		0 + 106,
		Game::kScreenHeight - y - 28);

	DrawAreaALL();

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

	DrawAreaResult(
		0 + 106,
		Game::kScreenHeight - y + 15,
		0 + 106 + x1 * m_skillNum[static_cast<int>(CharacterName::PLAYER)] / 100,		   //����* HP/ HPMAX
		Game::kScreenHeight - y + 15 + y1);// �`��\�̈���w��

	UpdateSkill(
		0 + 106,
		Game::kScreenHeight - y + 15);

	DrawAreaALL();

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y + 15 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)],
		true);

	// �̊��Q�[�W�̘g
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)],
		false,
		false);

	// �̊��w�i
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)],
		false,
		false);

	// �̊�
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)],
		false,
		false);

	// �̊��Q�[�W�̒��S
	DrawRotaGraph(
		Game::kScreenWidth / 2 + 4,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)],
		false,
		false);

	DrawFormatString(Game::kScreenWidth / 2, Game::kScreenHeight - 100,0xffffff,"%d", m_fightMeterNum[static_cast<int>(CharacterName::PLAYER)]);


//	m_playerPos.x++;
//	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Move(m_playerPos);

	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]->Update();

	//m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Draw();
	//m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]->Draw();

}

void UIDrawer::SetParam(CharacterName name,int hpNum,float skillNum,int fightMeterNum)
{
	m_hpNum[static_cast<int>(name)] = hpNum;
	m_skillNum[static_cast<int>(name)] = static_cast<int>(skillNum);
	m_fightMeterNum[static_cast<int>(name)] = fightMeterNum;
}

void UIDrawer::UpdateHp(int x, int y)
{
	DrawGraph(
		x,
		y,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)],
		true);
}

void UIDrawer::UpdateSkill(int x, int y)
{
	DrawGraph(
		x,
		y,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)],
		true);
}

void UIDrawer::DrawAreaResult(int x, int y, int x1, int y1)
{
	SetDrawArea(
		x,
		y,
		x1,
		y1);// �`��\�̈��S�̂ɖ߂�
}

void UIDrawer::DrawAreaALL()
{
	SetDrawArea(
		0,
		0,
		Game::kScreenWidth,
		Game::kScreenHeight);// �`��\�̈��S�̂ɖ߂�
}

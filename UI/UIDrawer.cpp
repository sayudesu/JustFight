#include <DxLib.h>

#include "../Util/Game.h"

#include "../Util/GameObject.h"

#include "UIDrawer.h"

namespace
{

}

UIDrawer::UIDrawer()
{
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]					= LoadGraph("Data/Image/UI/Xe[^Xx[X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]		    = LoadGraph("Data/Image/UI/n.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]					= LoadGraph("Data/Image/UI/HPQ[W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]				= LoadGraph("Data/Image/UI/HPQ[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]					= LoadGraph("Data/Image/UI/KEZQ[W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]              = LoadGraph("Data/Image/UI/KEZQ[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)]        = LoadGraph("Data/Image/UI/Ì²Q[W.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)] = LoadGraph("Data/Image/UI/Ì²Q[W^ñ.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)]     = LoadGraph("Data/Image/UI/Ì²Q[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)]      = LoadGraph("Data/Image/UI/Ì²Q[Wg.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)]        = LoadGraph("Data/Image/UI/Q[Wg.png");

	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)] = LoadGraph("Data/Image/UI/Xe[^Xx[X.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::CHARACTOR)] = LoadGraph("Data/Image/UI/n.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)] = LoadGraph("Data/Image/UI/HPQ[W.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP_BG)] = LoadGraph("Data/Image/UI/HPQ[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)] = LoadGraph("Data/Image/UI/KEZQ[W.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP_BG)] = LoadGraph("Data/Image/UI/KEZQ[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::FIGHT_POWER)] = LoadGraph("Data/Image/UI/Ì²Q[W.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)] = LoadGraph("Data/Image/UI/Ì²Q[W^ñ.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::FIGHT_POWER_BG)] = LoadGraph("Data/Image/UI/Ì²Q[Wx[X.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::FIGHT_OUTSIDE)] = LoadGraph("Data/Image/UI/Ì²Q[Wg.png");
	m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE)] = LoadGraph("Data/Image/UI/Q[Wg.png");



	// `æÊueXg
	int x = 0;
	int y = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	// vC[
	{
		m_playerPos = VGet(315, Game::kScreenHeight - y + 5, 0);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/Xe[^Xx[X.png",
			m_playerPos,
			0.0f,
			1.0f
			);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)] = std::make_unique<GameObject>(
			"Data/Image/UI/n.png",
			VGet(270, -30, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
			);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPQ[W.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPQ[Wx[X.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)] = std::make_unique<GameObject>(
			"Data/Image/UI/Q[Wg.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)] = std::make_unique<GameObject>(
			"Data/Image/UI/KEZQ[W.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/KEZQ[Wx[X.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE2)] = std::make_unique<GameObject>(
			"Data/Image/UI/Q[Wg.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);
	}


	// `æÊueXg
	int x2 = 0;
	int y2 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)],
		&x2, &y2);

	// Gl~[
	{
		m_enemyPos = VGet(Game::kScreenWidth - 315,y + 5 + 50, 0);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/Xe[^Xx[X.png",
			m_enemyPos,
			0.0f,
			1.0f
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::CHARACTOR)] = std::make_unique<GameObject>(
			"Data/Image/UI/n.png",
			VGet(-270, -30, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPQ[W.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPQ[Wx[X.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE)] = std::make_unique<GameObject>(
			"Data/Image/UI/Q[Wg.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)] = std::make_unique<GameObject>(
			"Data/Image/UI/KEZQ[W.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/KEZQ[Wx[X.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE2)] = std::make_unique<GameObject>(
			"Data/Image/UI/Q[Wg.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)].get()
		);
	}


	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Update();
	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();

	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::CHARACTOR)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();
}

UIDrawer::~UIDrawer()
{
}

void UIDrawer::Draw()
{
#if false
	static float frameCount = 0.0f;
	if (frameCount < 256)
	{
		frameCount += 1.0f;
	}

	// `æÊueXg
	int x = 0;
	int y = 0;
	int x1 = 0;
	int y1 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	// wiAn
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
		0 + 106 + x1 * m_hpNum[static_cast<int>(CharacterName::PLAYER)] / 6,         //·³* HP/ HPMAX
		Game::kScreenHeight - y - 28 + y1);// `æÂ\Ìæðwè

	UpdateHp(
		0 + 106,
		Game::kScreenHeight - y - 28);

	DrawAreaALL();

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y - 28 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)], 
		true);

	// KEZ
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
		0 + 106 + x1 * m_skillNum[static_cast<int>(CharacterName::PLAYER)] / 100,		   //·³* HP/ HPMAX
		Game::kScreenHeight - y + 15 + y1);// `æÂ\Ìæðwè

	UpdateSkill(
		0 + 106,
		Game::kScreenHeight - y + 15);

	DrawAreaALL();

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y + 15 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)],
		true);

	// Ì´Q[WÌg
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)],
		false,
		false);

	// Ì´wi
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)],
		false,
		false);

	// Ì´
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)],
		false,
		false);

	// Ì´Q[WÌS
	DrawRotaGraph(
		Game::kScreenWidth / 2 + 4,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)],
		false,
		false);

	DrawFormatString(Game::kScreenWidth / 2, Game::kScreenHeight - 100,0xffffff,"%d", m_fightMeterNum[static_cast<int>(CharacterName::PLAYER)]);
#endif

//	m_playerPos.x++;
//	m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->Move(m_playerPos);

	// vC[
	{
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)          ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)   ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)       ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)          ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)       ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)          ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE) ]->Update();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)          ]->Draw();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)   ]->Draw();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)          ]->Draw();


		// ¶ã
		int dXL = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().x) -
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// Eã
		int dX = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeX()/2);

		// ã
		int dY = m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		int dX2 = dX * (m_hpNum[static_cast<int>(CharacterName::PLAYER)] + 1) / (m_hpMax[static_cast<int>(CharacterName::PLAYER)] + 1);

		// º
		int dY2 = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeY()/2);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]->Draw();

		int aaa = m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_hpNum[static_cast<int>(CharacterName::PLAYER)] / m_hpMax[static_cast<int>(CharacterName::PLAYER)], dY2, 0x007700, true);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->Draw();

		dX = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetGraphSizeX() / 2);

		dY = m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		dX2 = dX * (m_skillNum[static_cast<int>(CharacterName::PLAYER)]) / (m_skillMax[static_cast<int>(CharacterName::PLAYER)]);

		dY2 = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)       ]->Draw();

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE) ]->Draw();
		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Draw();

		aaa = m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_skillNum[static_cast<int>(CharacterName::PLAYER)] / m_skillMax[static_cast<int>(CharacterName::PLAYER)], dY2, 0xaaaa00, true);
	}

	// Gl~[
	{
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::CHARACTOR)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP_BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP_BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::CHARACTOR)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->Draw();


		// ¶ã
		int dXL = (m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().x) -
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// Eã
		int dX = (m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// ã
		int dY = m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		int dX2 = dX * (m_hpNum[static_cast<int>(CharacterName::ENEMYNORMAL)] + 1) / (m_hpMax[static_cast<int>(CharacterName::ENEMYNORMAL)] + 1);

		// º
		int dY2 = (m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP_BG)]->Draw();

		int aaa = m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::HP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_hpNum[static_cast<int>(CharacterName::ENEMYNORMAL)] / m_hpMax[static_cast<int>(CharacterName::ENEMYNORMAL)], dY2, 0x007700, true);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->Draw();

		dX = (m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetGraphSizeX() / 2);

		dY = m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		dX2 = dX * (m_skillNum[static_cast<int>(CharacterName::ENEMYNORMAL)]) / (m_skillMax[static_cast<int>(CharacterName::ENEMYNORMAL)]);

		dY2 = (m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP_BG)]->Draw();

		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Draw();

		aaa = m_image[static_cast<int>(CharacterName::ENEMYNORMAL)][static_cast<int>(HandleType::SP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_skillNum[static_cast<int>(CharacterName::ENEMYNORMAL)] / m_skillMax[static_cast<int>(CharacterName::ENEMYNORMAL)], dY2, 0xaaaa00, true);
	}

}

void UIDrawer::SetParam(CharacterName name,int hpNum,int hpMax,float skillNum, float skillMax,int fightMeterNum)
{
	m_hpNum[static_cast<int>(name)] = hpNum;
	m_hpMax[static_cast<int>(name)] = hpMax;
	m_skillNum[static_cast<int>(name)] = static_cast<int>(skillNum);
	m_skillMax[static_cast<int>(name)] = static_cast<int>(skillMax);
	m_fightMeterNum[static_cast<int>(name)] = fightMeterNum;
}

//void UIDrawer::UpdateHp(int x, int y)
//{
//	DrawGraph(
//		x,
//		y,
//		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)],
//		true);
//}

//void UIDrawer::UpdateSkill(int x, int y)
//{
//	DrawGraph(
//		x,
//		y,
//		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)],
//		true);
//}

void UIDrawer::DrawAreaResult(int x, int y, int x1, int y1)
{
	//DxLib::SetDrawArea(
	//	x,
	//	y,
	//	x1,
	//	y1);// `æÂ\ÌæðSÌÉß·
}

void UIDrawer::DrawAreaALL()
{
	//DxLib::SetDrawArea(
	//	0,
	//	0,
	//	Game::kScreenWidth,
	//	Game::kScreenHeight);// `æÂ\ÌæðSÌÉß·
}

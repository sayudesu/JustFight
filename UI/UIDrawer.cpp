#include <DxLib.h>

#include "../Util/Game.h"

#include "../Util/GameObject.h"

#include "UIDrawer.h"

namespace
{

}

UIDrawer::UIDrawer()
{
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]					= LoadGraph("Data/Image/UI/StatusBaseWhite.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)]		    = LoadGraph("Data/Image/UI/HoseWhite.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]					= LoadGraph("Data/Image/UI/HPGauge.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)]				= LoadGraph("Data/Image/UI/HPGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)]					= LoadGraph("Data/Image/UI/SpecialMovesGauge.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)]              = LoadGraph("Data/Image/UI/SpecialMovesGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)]        = LoadGraph("Data/Image/UI/BodyGauge.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)] = LoadGraph("Data/Image/UI/BodyGaugeMid.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)]     = LoadGraph("Data/Image/UI/BodyGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)]      = LoadGraph("Data/Image/UI/BodyGaugeOutSide.png");
	m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)]        = LoadGraph("Data/Image/UI/GaugeOutSide.png");

	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)] = LoadGraph("Data/Image/UI/StatusBaseBlack.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::CHARACTOR)] = LoadGraph("Data/Image/UI/HoseBlack.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)] = LoadGraph("Data/Image/UI/HPGauge.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP_BG)] = LoadGraph("Data/Image/UI/HPGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)] = LoadGraph("Data/Image/UI/SpecialMovesGauge.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP_BG)] = LoadGraph("Data/Image/UI/SpecialMovesGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::FIGHT_POWER)] = LoadGraph("Data/Image/UI/BodyGauge.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::FIGHT_POWER_CENTER)] = LoadGraph("Data/Image/UI/BodyGaugeMid.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::FIGHT_POWER_BG)] = LoadGraph("Data/Image/UI/BodyGaugeBase.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::FIGHT_OUTSIDE)] = LoadGraph("Data/Image/UI/BodyGaugeOutSide.png");
	m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE)] = LoadGraph("Data/Image/UI/GaugeOutSide.png");



	// 描画位置テスト
	int x = 0;
	int y = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)],
		&x, &y);

	// プレイヤー
	{
		m_playerPos = VGet(315, Game::kScreenHeight - y + 5, 0);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/StatusBaseWhite.png",
			m_playerPos,
			0.0f,
			1.0f
			);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::CHARACTOR)] = std::make_unique<GameObject>(
			"Data/Image/UI/HoseWhite.png",
			VGet(270, -30, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
			);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPGauge.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPGaugeBase.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)] = std::make_unique<GameObject>(
			"Data/Image/UI/GaugeOutSide.png",
			VGet(-11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP)] = std::make_unique<GameObject>(
			"Data/Image/UI/SpecialMovesGauge.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::SP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/SpecialMovesGaugeBase.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE2)] = std::make_unique<GameObject>(
			"Data/Image/UI/GaugeOutSide.png",
			VGet(-12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)].get()
		);
	}


	// 描画位置テスト
	int x2 = 0;
	int y2 = 0;
	GetGraphSize(
		m_handle[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)],
		&x2, &y2);

	// エネミー
	{
		m_enemyPos = VGet(Game::kScreenWidth - 315,y + 5 + 50, 0);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/StatusBaseBlack.png",
			m_enemyPos,
			0.0f,
			1.0f
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::CHARACTOR)] = std::make_unique<GameObject>(
			"Data/Image/UI/HoseBlack.png",
			VGet(-270, -30, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPGauge.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/HPGaugeBase.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE)] = std::make_unique<GameObject>(
			"Data/Image/UI/GaugeOutSide.png",
			VGet(11, -25, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)] = std::make_unique<GameObject>(
			"Data/Image/UI/SpecialMovesGauge.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP_BG)] = std::make_unique<GameObject>(
			"Data/Image/UI/SpecialMovesGaugeBase.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
		);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE2)] = std::make_unique<GameObject>(
			"Data/Image/UI/GaugeOutSide.png",
			VGet(12, 18, 0),
			0.0f,
			1.0f,
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)].get()
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

	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::CHARACTOR)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP_BG)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Update();
	m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();
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

	DrawAreaResult(
		0 + 106,
		Game::kScreenHeight - y - 28,
		0 + 106 + x1 * m_hpNum[static_cast<int>(CharacterName::PLAYER)] / 6,         //長さ* HP/ HPMAX
		Game::kScreenHeight - y - 28 + y1);// 描画可能領域を指定

	UpdateHp(
		0 + 106,
		Game::kScreenHeight - y - 28);

	DrawAreaALL();

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

	DrawAreaResult(
		0 + 106,
		Game::kScreenHeight - y + 15,
		0 + 106 + x1 * m_skillNum[static_cast<int>(CharacterName::PLAYER)] / 100,		   //長さ* HP/ HPMAX
		Game::kScreenHeight - y + 15 + y1);// 描画可能領域を指定

	UpdateSkill(
		0 + 106,
		Game::kScreenHeight - y + 15);

	DrawAreaALL();

	DrawGraph(
		0 + 106 - 3,
		Game::kScreenHeight - y + 15 - 3,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BAR_OUTSIDE)],
		true);

	// 体感ゲージの枠
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_OUTSIDE)],
		false,
		false);

	// 体感背景
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER_BG)],
		false,
		false);

	// 体感
	DrawRotaGraph(
		Game::kScreenWidth / 2,
		Game::kScreenHeight - 100,
		1,
		0 * DX_PI_F / 180.0f,
		m_handle[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::FIGHT_POWER)],
		false,
		false);

	// 体感ゲージの中心
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

	// プレイヤー
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


		// 左上
		int dXL = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().x) -
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// 右上
		int dX = (m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeX()/2);

		// 上
		int dY = m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::PLAYER)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		int dX2 = dX * (m_hpNum[static_cast<int>(CharacterName::PLAYER)] + 1) / (m_hpMax[static_cast<int>(CharacterName::PLAYER)] + 1);

		// 下
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

	// エネミー
	{
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::CHARACTOR)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP_BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP_BG)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Update();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Update();

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::CHARACTOR)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->Draw();


		// 左上
		int dXL = (m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().x) -
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// 右上
		int dX = (m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetGraphSizeX() / 2);

		// 上
		int dY = m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		int dX2 = dX * (m_hpNum[static_cast<int>(CharacterName::ENEMY)] + 1) / (m_hpMax[static_cast<int>(CharacterName::ENEMY)] + 1);

		// 下
		int dY2 = (m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetGraphSizeY() / 2);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP_BG)]->Draw();

		int aaa = m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::HP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_hpNum[static_cast<int>(CharacterName::ENEMY)] / m_hpMax[static_cast<int>(CharacterName::ENEMY)], dY2, 0x007700, true);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->Draw();

		dX = (m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetPos().x +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().x) +
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetGraphSizeX() / 2);

		dY = m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().y -
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		dX2 = dX * (m_skillNum[static_cast<int>(CharacterName::ENEMY)]) / (m_skillMax[static_cast<int>(CharacterName::ENEMY)]);

		dY2 = (m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetPos().y +
			m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BG)]->GetPos().y) +
			(m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetGraphSizeY() / 2);

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP_BG)]->Draw();

		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE)]->Draw();
		m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::BAR_OUTSIDE2)]->Draw();

		aaa = m_image[static_cast<int>(CharacterName::ENEMY)][static_cast<int>(HandleType::SP)]->GetGraphSizeX();
		DrawBox(dXL, dY, dXL + aaa * m_skillNum[static_cast<int>(CharacterName::ENEMY)] / m_skillMax[static_cast<int>(CharacterName::ENEMY)], dY2, 0xaaaa00, true);
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
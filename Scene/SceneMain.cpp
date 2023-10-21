#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pPlayer(nullptr),
	m_pEnemy(nullptr)
{
	
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera = std::make_unique<Camera>();
	m_pPlayer = std::make_unique<Player>(VGet(-300, 0, 0));
	m_pEnemy  = std::make_unique<Enemy> (VGet( 300, 0, 0));

	m_pCamera->Init();
	m_pPlayer->Init();
	m_pEnemy->Init();
}

void SceneMain::End()
{
	m_pPlayer->End();
	m_pEnemy->End();
}

SceneBase* SceneMain::Update()
{
	m_pPlayer->Update();
	m_pPlayer->Input();
	m_pEnemy->Update();
	m_pEnemy->Input();

	m_pCamera->Update();

	m_pCamera->SetTargetPos(m_pPlayer->GetPos());
	m_pCamera->SetLockonPos(m_pEnemy->GetPos());

	m_pCamera->SetPlayerAngle(m_pPlayer->GetAngle());

	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());

	m_pPlayer->SetDamage(false);
	// Enemyの攻撃した場合の処理
	if (m_pEnemy->GetAttackFrame() == m_pEnemy->GetAttackFrameMax())
	{	
		// ジャストガード
		if (m_pPlayer->GetJustGuardFrame() > 0 &&
			m_pPlayer->GetJustGuardFrame() < m_pPlayer->GetJustGuardFrameMax())
		{
			m_pPlayer->SetStamina(30, 0);
			printfDx("Pジャストガード成功\n");
		}
		else if (m_pPlayer->GetGuardFrame() == m_pPlayer->GetGuardFrameMax())
		{
			m_pPlayer->SetStamina(0, 30);
			printfDx("Pガード成功\n");
		}
		else
		{
			m_pPlayer->SetDamage(true);
			printfDx("Pガード失敗\n");
		}
	}

	m_pEnemy->SetDamage(false);
	// Enemyの攻撃した場合の処理
	if (m_pPlayer->GetAttackFrame() == m_pPlayer->GetAttackFrameMax())
	{
		// ジャストガード
		if (m_pEnemy->GetJustGuardFrame() > 0 &&
			m_pEnemy->GetJustGuardFrame() < m_pEnemy->GetJustGuardFrameMax())
		{
			m_pEnemy->SetStamina(30, 0);
		//	printfDx("Eジャストガード成功\n");
		}
		else if (m_pEnemy->GetGuardFrame() == m_pEnemy->GetGuardFrameMax())
		{
			m_pEnemy->SetStamina(0, 30);
		//	printfDx("Eガード成功\n");
		}
		else
		{
			m_pEnemy->SetDamage(true);
		//	printfDx("Eガード失敗\n");
		}
	}


	if (Pad::isTrigger(PAD_INPUT_1))
	{
		clsDx();
		return new SceneDebug();
	}

	return this;
}

void SceneMain::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xaaaaaa,true);

	m_pPlayer->Draw();
	m_pEnemy->Draw();

#if _DEBUG
	DEBUG::FrameMeter("P体力", 100, 50, m_pPlayer->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E体力", 100, 100, m_pEnemy->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("Pスタミナ", 100, 150, m_pPlayer->GetStamina(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("Eスタミナ", 100, 200, m_pEnemy->GetStamina(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P攻撃フレーム", 100, 250, m_pPlayer->GetAttackFrameMax(), m_pPlayer->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P防御フレーム", 100, 300, m_pPlayer->GetGuardFrameMax(), m_pPlayer->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pPlayer->GetJustGuardFrameMax(), m_pPlayer->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E攻撃フレーム", 100, 400, m_pEnemy->GetAttackFrameMax(), m_pEnemy->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E防御フレーム", 100, 450, m_pEnemy->GetGuardFrameMax(), m_pEnemy->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pEnemy->GetJustGuardFrameMax(), m_pEnemy->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::Field();
#endif

	DrawString(0, 0, "SceneMain", 0xffffff);
}


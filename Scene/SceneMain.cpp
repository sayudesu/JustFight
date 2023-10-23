#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Collision3D.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pPlayer(nullptr),
	m_pEnemy(nullptr),
	m_pColl(nullptr)
{
	
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera = std::make_unique<Camera>();
	m_pPlayer = std::make_unique<Player>(VGet(-300.0f, 0.0f, 0.0f));
	m_pEnemy  = std::make_unique<Enemy> (VGet( 300.0f, 0.0f, 0.0f));
	m_pColl   = std::make_unique<Collision3D>();

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

	// カメラにプレイヤーとエネミーの位置を渡す
	m_pCamera->SetTargetPos(m_pPlayer->GetPos());
	m_pCamera->SetLockonPos(m_pEnemy->GetPos());

	// カメラにプレイヤーの角度と位置を渡す
	m_pCamera->SetPlayerAngle(m_pPlayer->GetAngle());
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());

	// Enemyの攻撃した場合の処理
	{
		m_pPlayer->SetDamage(false);
		m_pPlayer->SetJustGuard(false);
		m_pPlayer->SetJustGuardBreak(m_pEnemy->IsJustGuard());
		m_pPlayer->SetRota(m_pEnemy->GetRot());
		// 攻撃フレームが最大数かどうか
		if (m_pEnemy->GetAttackFrame() == m_pEnemy->GetAttackFrameMax())
		{
			// ジャストガード
			if (m_pPlayer->GetJustGuardFrame() > 0 &&
				m_pPlayer->GetJustGuardFrame() < m_pPlayer->GetJustGuardFrameMax())
			{
				m_pPlayer->SetStamina(30.0f, 0.0f);
				m_pPlayer->SetJustGuard(true);
				printfDx("Pジャストガード成功\n");
				// 振動開始
				StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
			}
			else if (m_pPlayer->GetGuardFrame() == m_pPlayer->GetGuardFrameMax())
			{
				if (CheckHItPlayerSield())
				{
					m_pPlayer->SetStamina(0.0f, 30.0f);
					m_pPlayer->SetGuard(true);
					printfDx("Pガード成功\n");
				}
			}
			else
			{
				if (CheckHitPlayer())
				{
					m_pPlayer->SetDamage(true);
					// 振動開始
					StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
					printfDx("Pガード失敗\n");
				}
			}
		}

	}
	// Playerの攻撃した場合の処理
	{
		m_pEnemy->SetDamage(false);
		m_pEnemy->SetJustGuard(false);
		m_pEnemy->SetJustGuardBreak(m_pPlayer->IsJustGuard());
		if (m_pPlayer->GetAttackFrame() == m_pPlayer->GetAttackFrameMax())
		{
			// ジャストガード
			if (m_pEnemy->GetJustGuardFrame() > 0 &&
				m_pEnemy->GetJustGuardFrame() < m_pEnemy->GetJustGuardFrameMax())
			{
				m_pEnemy->SetStamina(30.0f, 0.0f);
				m_pEnemy->SetJustGuard(true);
			}
			else if (m_pEnemy->GetGuardFrame() == m_pEnemy->GetGuardFrameMax())
			{
				m_pEnemy->SetStamina(0.0f, 30.0f);
			}
			else
			{
				if (CheckHitEnemy())
				{
					m_pEnemy->SetDamage(true);
				}
				//	printfDx("Eガード失敗\n");
			}
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
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xAAAAAA,true);

	m_pPlayer->Draw();
	m_pEnemy->Draw();

#if _DEBUG
	CheckHitPlayer();
	CheckHitEnemy();
	CheckHItPlayerSield();
	CheckHItEnemySield();
	DEBUG::FrameMeter("P体力", 100, 50, m_pPlayer->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E体力", 100, 100, m_pEnemy->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("Pスタミナ", 100, 150, m_pPlayer->GetStamina(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("Eスタミナ", 100, 200, m_pEnemy->GetStamina(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P攻撃フレーム", 100, 250, m_pPlayer->GetAttackFrameMax(), m_pPlayer->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P防御フレーム", 100, 300, m_pPlayer->GetGuardFrameMax(), m_pPlayer->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pPlayer->GetJustGuardFrameMax(), m_pPlayer->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E攻撃フレーム", 100, 400, m_pEnemy->GetAttackFrameMax(), m_pEnemy->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E防御フレーム", 100, 450, m_pEnemy->GetGuardFrameMax(), m_pEnemy->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pEnemy->GetJustGuardFrameMax(), m_pEnemy->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::Field();
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

}

bool SceneMain::CheckHitEnemy()
{
	if (m_pColl->IsCheckHit(
		m_pPlayer->GetWeaponPos(), m_pEnemy->GetPos(),
		m_pPlayer->GetWeaponAttackRadius(), m_pEnemy->GetWeaponAttackRadius(),
		m_pPlayer->GetRot(), m_pEnemy->GetRot(),
		m_pPlayer->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckHitPlayer()
{
	if (m_pColl->IsCheckHit(
		m_pEnemy->GetWeaponPos(), m_pPlayer->GetPos(),
		m_pEnemy->GetWeaponAttackRadius(), m_pPlayer->GetWeaponAttackRadius(),
		m_pEnemy->GetRot(), m_pPlayer->GetRot(),
		m_pEnemy->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckHItEnemySield()
{
	if (m_pColl->IsCheckHit(
		m_pPlayer->GetWeaponPos(), m_pEnemy->GetSieldPos(),
		m_pPlayer->GetWeaponAttackRadius(), m_pEnemy->GetSieldRadius(),
		m_pPlayer->GetRot(), m_pEnemy->GetRot(),
		m_pPlayer->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckHItPlayerSield()
{
	if (m_pColl->IsCheckHit(
		m_pEnemy->GetWeaponPos(), m_pPlayer->GetSieldPos(),
		m_pEnemy->GetWeaponAttackRadius(), m_pPlayer->GetSieldRadius(),
		m_pEnemy->GetRot(), m_pPlayer->GetRot(),
		m_pEnemy->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, 0.0f)))
	{
		return true;
	}
	return false;
}


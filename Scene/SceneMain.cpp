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
	// Enemy�̍U�������ꍇ�̏���
	if (m_pEnemy->GetAttackFrame() == m_pEnemy->GetAttackFrameMax())
	{	
		// �W���X�g�K�[�h
		if (m_pPlayer->GetJustGuardFrame() > 0 &&
			m_pPlayer->GetJustGuardFrame() < m_pPlayer->GetJustGuardFrameMax())
		{
			m_pPlayer->SetStamina(30, 0);
			printfDx("P�W���X�g�K�[�h����\n");
		}
		else if (m_pPlayer->GetGuardFrame() == m_pPlayer->GetGuardFrameMax())
		{
			m_pPlayer->SetStamina(0, 30);
			printfDx("P�K�[�h����\n");
		}
		else
		{
			m_pPlayer->SetDamage(true);
			printfDx("P�K�[�h���s\n");
		}
	}

	m_pEnemy->SetDamage(false);
	// Enemy�̍U�������ꍇ�̏���
	if (m_pPlayer->GetAttackFrame() == m_pPlayer->GetAttackFrameMax())
	{
		// �W���X�g�K�[�h
		if (m_pEnemy->GetJustGuardFrame() > 0 &&
			m_pEnemy->GetJustGuardFrame() < m_pEnemy->GetJustGuardFrameMax())
		{
			m_pEnemy->SetStamina(30, 0);
		//	printfDx("E�W���X�g�K�[�h����\n");
		}
		else if (m_pEnemy->GetGuardFrame() == m_pEnemy->GetGuardFrameMax())
		{
			m_pEnemy->SetStamina(0, 30);
		//	printfDx("E�K�[�h����\n");
		}
		else
		{
			m_pEnemy->SetDamage(true);
		//	printfDx("E�K�[�h���s\n");
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
	DEBUG::FrameMeter("P�̗�", 100, 50, m_pPlayer->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E�̗�", 100, 100, m_pEnemy->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("P�X�^�~�i", 100, 150, m_pPlayer->GetStamina(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("E�X�^�~�i", 100, 200, m_pEnemy->GetStamina(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P�U���t���[��", 100, 250, m_pPlayer->GetAttackFrameMax(), m_pPlayer->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P�h��t���[��", 100, 300, m_pPlayer->GetGuardFrameMax(), m_pPlayer->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pPlayer->GetJustGuardFrameMax(), m_pPlayer->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E�U���t���[��", 100, 400, m_pEnemy->GetAttackFrameMax(), m_pEnemy->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E�h��t���[��", 100, 450, m_pEnemy->GetGuardFrameMax(), m_pEnemy->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pEnemy->GetJustGuardFrameMax(), m_pEnemy->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::Field();
#endif

	DrawString(0, 0, "SceneMain", 0xffffff);
}


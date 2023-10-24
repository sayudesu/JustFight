#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Collision3D.h"
#include "../Util/Effekseer3DDrawer.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Object/CharacterBase.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pColl(nullptr)
{
	m_pCharacter[0] = nullptr;
	m_pCharacter[1] = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera       = std::make_unique<Camera>();
	m_pCharacter[0] = std::make_unique<Player>(VGet(-300.0f, 0.0f, 0.0f));
	m_pCharacter[1] = std::make_unique<Enemy> (VGet( 300.0f, 0.0f, 0.0f));
	m_pColl         = std::make_unique<Collision3D>();
	m_pEffect[0]    = std::make_unique<Effekseer3DDrawer>();
	m_pEffect[1]    = std::make_unique<Effekseer3DDrawer>();

	m_pCamera->Init();
	m_pCharacter[0]->Init();
	m_pCharacter[1]->Init();
	m_pEffect[0]->Init("Data/Guard.efk",30.0f);
	m_pEffect[1]->Init("Data/Guard2.efk", 130.0f);
}

void SceneMain::End()
{
	m_pCharacter[0]->End();
	m_pCharacter[1]->End();
	m_pEffect[0]->End();
	m_pEffect[1]->End();
}

SceneBase* SceneMain::Update()
{
	for (auto& character : m_pCharacter)
	{
		character->Update();
		character->Input();
	}


	m_pCamera->Update();

	for (auto& effect : m_pEffect)
	{
		effect->Update();

		if (!effect->IsPlay())
		{
			effect->SetPlay(false);
		}
	}

	// �J�����Ƀv���C���[�ƃG�l�~�[�̈ʒu��n��
	m_pCamera->SetTargetPos(m_pCharacter[0]->GetPos());
	m_pCamera->SetLockonPos(m_pCharacter[1]->GetPos());

	// �J�����Ƀv���C���[�̊p�x�ƈʒu��n��
	m_pCamera->SetPlayerAngle(m_pCharacter[0]->GetAngle());
	m_pCharacter[1]->SetTargetPos(m_pCharacter[0]->GetPos());

	UpdateCharacter(m_pCharacter[0].get(), m_pCharacter[1].get());
	UpdateCharacter(m_pCharacter[1].get(), m_pCharacter[0].get());

	{
		//// Enemy�̍U�������ꍇ�̏���
	//{
	//	// �W���X�g�K�[�h�������������ǂ���
	//	m_pPlayer->SetJustGuard(false);
	//	// �W���X�g�K�[�h���ꂽ���ǂ���
	//	m_pPlayer->SetJustGuardBreak(m_pEnemy->IsJustGuard());
	//	// �G�̉�]�p�x���擾
	//	m_pPlayer->SetRota(m_pEnemy->GetRot());
	//	// �U���t���[�����ő吔���ǂ���
	//	if (m_pEnemy->GetAttackFrame() == m_pEnemy->GetAttackFrameMax() - 1)
	//	{
	//		// �W���X�g�K�[�h
	//		if (m_pPlayer->GetJustGuardFrame() > 0 &&
	//			m_pPlayer->GetJustGuardFrame() < m_pPlayer->GetJustGuardFrameMax())
	//		{
	//			// �X�^�~�i�����炷
	//			m_pPlayer->SetStamina(30.0f, 0.0f);
	//			// �W���X�g�K�[�h�������������ǂ���
	//			m_pPlayer->SetJustGuard(true);
	//			// �U���J�n
	//			StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

	//			m_pEffect[1]->SetPlay(true);
	//			m_pEffect[1]->SetAngle(m_pPlayer->GetAngle());
	//			m_pEffect[1]->SetPos(VGet(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + 100.0f, m_pPlayer->GetPos().z));

	//		}
	//		// �ʏ�K�[�h���o���邩�ǂ���
	//		else if (m_pPlayer->GetGuardFrame() == m_pPlayer->GetGuardFrameMax())
	//		{
	//			// �G�l�~�[�̍U�����v���C���[�̏��ɓ����������ǂ���
	//			if (CheckHItPlayerSield())
	//			{
	//				// �v���C���[�̃X�^�~�i�����炷
	//				m_pPlayer->SetStamina(0.0f, 10.0f);
	//				// �K�[�h�����������ǂ���
	//				m_pPlayer->SetGuard(true);

	//				m_pEffect[0]->SetPlay(true);
	//				m_pEffect[0]->SetAngle(m_pPlayer->GetAngle());
	//				m_pEffect[0]->SetPos(m_pPlayer->GetSieldPos());
	//			}
	//		}
	//		else
	//		{
	//			// �G�l�~�[�̍U�����v���C���[�ɓ����������ǂ���
	//			if (CheckHitPlayer())
	//			{
	//				// �v���C���[�Ƀ_���[�W��^����
	//				m_pPlayer->SetDamage(true);
	//				// �U���J�n+
	//				StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
	//			}
	//		}
	//	}

	//}
	//// Player�̍U�������ꍇ�̏���
	//{
	//	m_pEnemy->SetJustGuard(false);
	//	m_pEnemy->SetJustGuardBreak(m_pPlayer->IsJustGuard());
	//	if (m_pPlayer->GetAttackFrame() == m_pPlayer->GetAttackFrameMax() - 1)
	//	{
	//		// �W���X�g�K�[�h
	//		if (m_pEnemy->GetJustGuardFrame() > 0 &&
	//			m_pEnemy->GetJustGuardFrame() < m_pEnemy->GetJustGuardFrameMax())
	//		{
	//			m_pEnemy->SetStamina(30.0f, 0.0f);
	//			m_pEnemy->SetJustGuard(true);
	//		}
	//		else if (m_pEnemy->GetGuardFrame() == m_pEnemy->GetGuardFrameMax())
	//		{
	//			m_pEnemy->SetStamina(0.0f, 10.0f);
	//		}
	//		else
	//		{
	//			if (CheckHitEnemy())
	//			{
	//				m_pEnemy->SetDamage(true);
	//			}
	//			//	printfDx("E�K�[�h���s\n");
	//		}
	//	}
	//}
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

	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}

	for (auto& effect : m_pEffect)
	{
		effect->Draw();
	}

#if _DEBUG
	//CheckHitPlayer();
	//CheckHitEnemy();
	//CheckHItPlayerSield();
	//CheckHItEnemySield();
	DEBUG::FrameMeter("P�̗�", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E�̗�", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("P�X�^�~�i", 100, 150, m_pCharacter[0]->GetStamina(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("E�X�^�~�i", 100, 200, m_pCharacter[1]->GetStamina(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P�U���t���[��", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P�h��t���[��", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E�U���t���[��", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E�h��t���[��", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::Field();
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

}

bool SceneMain::CheckHit(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetWeaponAttackRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckHItSield(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetSieldPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetSieldRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

void SceneMain::UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2)
{
	// �W���X�g�K�[�h�������������ǂ���
	chara1->SetJustGuard(false);
	// �W���X�g�K�[�h���ꂽ���ǂ���
	chara1->SetJustGuardBreak(chara2->IsJustGuard());
	// �G�̉�]�p�x���擾
	chara1->SetRota(chara2->GetRot());
	// �U���t���[�����ő吔���ǂ���
	if (chara2->GetAttackFrame() == chara2->GetAttackFrameMax() - 1)
	{
		// �W���X�g�K�[�h
		if (chara1->GetJustGuardFrame() > 0 &&
			chara1->GetJustGuardFrame() < chara1->GetJustGuardFrameMax())
		{
			// �X�^�~�i�����炷
			chara1->SetStamina(30.0f, 0.0f);
			// �W���X�g�K�[�h�������������ǂ���
			chara1->SetJustGuard(true);
			// �U���J�n
			StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

			m_pEffect[1]->SetPlay(true);
			m_pEffect[1]->SetAngle(chara1->GetAngle());
			m_pEffect[1]->SetPos(VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z));

		}
		// �ʏ�K�[�h���o���邩�ǂ���
		else if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax())
		{
			// �G�l�~�[�̍U�����v���C���[�̏��ɓ����������ǂ���
			if (CheckHItSield(chara1, chara2))
			{
				// �v���C���[�̃X�^�~�i�����炷
				chara1->SetStamina(0.0f, 10.0f);
				// �K�[�h�����������ǂ���
				chara1->SetGuard(true);

				m_pEffect[0]->SetPlay(true);
				m_pEffect[0]->SetAngle(chara1->GetAngle());
				m_pEffect[0]->SetPos(chara1->GetSieldPos());
			}
		}
		else
		{
			// �G�l�~�[�̍U�����v���C���[�ɓ����������ǂ���
			if (CheckHit(chara1, chara2))
			{
				// �v���C���[�Ƀ_���[�W��^����
				chara1->SetDamage(true);
				// �U���J�n
				StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
			}
		}
	}
}


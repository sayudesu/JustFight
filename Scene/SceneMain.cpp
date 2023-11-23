#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"// �J����
#include "../Object/Player/Player.h"// �v���C���[
#include "../Object/Enemy/Enemy.h"// �G�l�~�[
#include "../Util/Collision3D.h"// �����蔻��
#include "../Util/EffekseerDrawer.h"// 3D�G�t�F�N�g
#include "../Util/Game.h"// �Q�[����{�ݒ�
#include "../Object/CharacterBase.h"// �L�����N�^�[
#include "../Util/Pad.h"// �p�b�h
#include "SceneDebug.h"// �f�o�b�O�V�[��
#include "SceneResult.h"// ���U���g�V�[��
#include "../Util/BloodDrawer.h"// ���̃G�t�F�N�g
#include "../FIeldDrawer.h"// �}�b�v�`��

#include "SceneGameOver.h"// �Q�[���I�[�o�[�V�[��
#include "SceneClear.h"// �Q�[���N���A�V�[��

#include "../Util/CharacterName.h"// �L�����N�^�[�̖��O

#include "../DEBUG.h"// �f�o�b�O�p

SceneMain::SceneMain():
	m_pCamera(nullptr)
{
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)] = nullptr;
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]  = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera               = std::make_unique<Camera>();							// �J�����N���X
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)] = std::make_shared<Player>(VGet(-300.0f, 260.0f, 0.0f));// �L�����N�^�[�N���X
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]  = std::make_shared<Enemy>(VGet(300.0f, 260.0f, 0.0f));  // �L�����N�^�[�N���X
	m_pField                = std::make_unique<FIeldDrawer>();			            // �t�B�[���h�`��N���X

	// ������
	m_pCamera->Init();
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Init();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Init();
	m_pField->Init();
}

void SceneMain::End()
{
	// �������
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->End();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->End();
	m_pField->End();

	for (int i = 0; i < m_pBlood.size(); i++)
	{
		// �f���[�g����
		delete m_pBlood[i];
		m_pBlood[i] = nullptr;
	}
}

SceneBase* SceneMain::Update()
{
	{
		// �v���C���[�̓��͏��
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Input();
		// �v���C���[�X�V����
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Update();

		// �L�����N�^�[�U�����菈��
		// �v���C���[���U�������ꍇ
		if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACK ||
			m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState() == BattleState::ATTACKTWO)
		{
			UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::PLAYER)],
				m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		}
		// �^�[�Q�b�g��HP���擾
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp());
	}
	{
		// �v���C���[�̓��͏��
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Input();
		// �v���C���[�X�V����
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Update();
		// �L�����N�^�[�U�����菈��
	// �v���C���[���U�������ꍇ
		if (m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACK ||
			m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState() == BattleState::ATTACKTWO)
		{
			UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)],
				m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
		}
		// �^�[�Q�b�g��HP���擾
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetTargetHp(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp());
	}

	{
		static int frame = 0;
		frame++;
		printfDx("state = %d frame = %d\n",m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState(), frame);
	}
	
	// �^�[�Q�b�g�̐퓬�̏�Ԃ��󂯎��
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetBattleState());
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->SetBattleState(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetBattleState());

	// �J�����̍X�V����
	m_pCamera->Update();

	// �G�̍U���\�͈͂ɂ��邩�ǂ���
	if (CheckModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]))
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(false);
	}

	{
		// ���̃G�t�F�N�g���X�V
		for (auto& blood : m_pBlood)
		{
			blood->Update();
		}
		for (int i = 0; i < m_pBlood.size(); i++)
		{
			if (m_pBlood[i]->IsGetErase())
			{
				// �f���[�g����
				delete m_pBlood[i];
				m_pBlood[i] = nullptr;
				// �v�f�̍폜
				m_pBlood.erase(m_pBlood.begin() + i);
			}
		}
	}

	// �J�����Ƀv���C���[�ƃG�l�~�[�̈ʒu��n��
	m_pCamera->SetTargetPos(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetPos());
	// �J�����Ƀv���C���[�̊p�x�ƈʒu��n��
	m_pCamera->SetPlayerAngle(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetAngle());

	{
#if true
		CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

		CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndShieldHIt(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);

		CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]);
		CheckWeaponAndBodyHit(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], m_pCharacter[static_cast<int>(CharacterName::PLAYER)]);
#endif
	}
	
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			clsDx();
			return new SceneDebug();
		}
	}
	
	{
		if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp() == 0)
		{
			static int count = 0;
			count++;
			if (count == 60 * 2)
			{
				count = 0;
				clsDx();
				return new SceneGameOver();
			}
		}
		else if(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp() == 0)
		{
			static int count = 0;
			count++;
			if (count == 60 * 2)
			{
				count = 0;
				clsDx();
				return new SceneClear();
			}
		}
	}

	return this;
}

void SceneMain::Draw()
{
	m_pField->Draw();

	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}
	
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}

	DEBUG::Field();
#if _DEBUG
	DEBUG::FrameMeter("P�̗�", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E�̗�", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("P�X�^�~�i", 100, 150, m_pCharacter[0]->GetFightingMeter(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("E�X�^�~�i", 100, 200, m_pCharacter[1]->GetFightingMeter(),  100, 15, 0xffff00);

	DEBUG::FrameMeter("P�U���t���[��", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20,0xffff00);
	DEBUG::FrameMeter("P�h��t���[��", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E�U���t���[��", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E�h��t���[��", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);

	
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

}

bool SceneMain::CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetCollPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetWeaponAttackRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetShieldPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetShieldRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetCollWeaponPos(), chara2->GetCollPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	if (Coll::IsCheckHit(
		chara1->GetPos(), chara2->GetCollPos(),
		chara1->GetModelRadius(), chara2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

// 1���U�������鑤
// 2���U�����󂯂鑤
void SceneMain::UpdateCharacter(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2)
{
	// �W���X�g�K�[�h�������������ǂ���
	chara2->SetJustGuard(false);

	// ��]�p�x���擾
	chara2->SetTargetMtxRota(chara1->GetRot());

	// �^�[�Q�b�g�̉�]�s����󂯎��
	chara2->SetTargetMtxRota(chara1->GetRot());


	// �^�[�Q�b�g�̊p�x���󂯎��
	chara2->SetTargetRota(chara1->GetAngle());

	// �^�[�Q�b�g�̈ʒu���󂯎��
	chara2->SetTargetPos(chara1->GetPos());
#if true
	// �U���t���[�����ő吔���ǂ���
	if (chara2->GetAttackFrame() == chara2->GetAttackFrameMax() - 1)
	{
	
	}
	//// �W���X�g�K�[�h
	//if (chara1->GetJustGuardFrame() > 0 &&
	//	chara1->GetJustGuardFrame() < chara1->GetJustGuardFrameMax())
	//{
	//	// �W���X�g�K�[�h���������Ă���̂ŏ��ɓ������Ă��Ă��̂ɓ������Ă��Ă�
	//	// ���G����ɂȂ�
	//	if ((CheckWeaponAndShieldHIt(chara1, chara2)) || (CheckWeaponAndBodyHit(chara1, chara2)))
	//	{
	//		// �W���X�g�K�[�h�������������ǂ���
	//		chara1->SetJustGuard(true);

	//		// �W���X�g�K�[�h���ꂽ����
	//		// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
	//		chara2->SetFightingMeter(-8.0f);
	//		// �W���X�g�K�[�h�ɐ�����������
	//		// �킢�ɕK�v�ȓ���ȃ��[�^�[�𑝂₷
	//		chara1->SetFightingMeter(8.0f);

	//		chara1->SetCollJustGuardEffect();

	//		// �U���J�n
	//		StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
	//	}
	//}  

	// �U����Ԃ�������

	// �ʏ�K�[�h���o���邩�ǂ���
	if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax()) 
	{
		//�U�������ɓ����������ǂ���
		if (CheckWeaponAndShieldHIt(chara1, chara2))
		{
			// �K�[�h�����������ǂ���
			chara1->SetGuardKnockBack(true,-20);

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			chara1->SetFightingMeter(-10.0f);

			chara1->SetCollGuardEffect();

			chara2->SetWeaponAttacksShield(true);
		}
		return;
	}
	else
	{
		// �U���������������ǂ���
		if (CheckWeaponAndBodyHit(chara1, chara2))
		{
			// �_���[�W��^����
			chara1->SetDamage(true);

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			chara1->SetFightingMeter(-10.0f);

			for (int i = 0; i < 30; i++)
			{
				m_pBlood.push_back(new BloodDrawer(VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z)));
				m_pBlood.back()->Init(i);
			}
			// �U���J�n
			StartJoypadVibration(DX_INPUT_PAD1, 1000 / 3, 1000 / 2, -1);
		}
		return;
	}
#endif
}


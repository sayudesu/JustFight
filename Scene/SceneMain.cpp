#include "SceneMain.h"
#include <DxLib.h>
#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"
#include "../Util/Collision3D.h"
#include "../Util/EffekseerDrawer.h"
#include "../Util/Game.h"
#include "../DEBUG.h"

#include "../Object/CharacterBase.h"

#include "../Util/Pad.h"
#include "SceneDebug.h"
#include "SceneResult.h"
#include "../Util/BloodDrawer.h"

#include "../FIeldDrawer.h"

#include "../AttackData.h"

#include "../Util/EffectId.h"

namespace
{
	// �L�����N�^�[�x�[�X�̔z��ԍ�
	constexpr int kPlayerNo = 0;
	constexpr int kEnemyNo = 1;
}

SceneMain::SceneMain():
	m_pCamera(nullptr),
	m_pColl(nullptr)
{
	m_pCharacter[kPlayerNo] = nullptr;
	m_pCharacter[kEnemyNo] = nullptr;
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	m_pCamera               = std::make_unique<Camera>();							// �J�����N���X
	m_pCharacter[kPlayerNo] = std::make_unique<Player>(VGet(-300.0f, 260.0f, 0.0f));// �L�����N�^�[�N���X
	m_pCharacter[kEnemyNo]  = std::make_unique<Enemy>(VGet(300.0f, 260.0f, 0.0f));  // �L�����N�^�[�N���X
	m_pColl                 = std::make_unique<Collision3D>();						// �����蔻��N���X
	m_pField                = std::make_unique<FIeldDrawer>();			            // �t�B�[���h�`��N���X

	// ������
	m_pCamera->Init();
	m_pCharacter[kPlayerNo]->Init();
	m_pCharacter[kEnemyNo]->Init();
	m_pField->Init();
}

void SceneMain::End()
{
	// �������
	m_pCharacter[kPlayerNo]->End();
	m_pCharacter[kEnemyNo]->End();
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
	// �L�����N�^�[�X�V����
	m_pCharacter[kPlayerNo]->Input();
	m_pCharacter[kPlayerNo]->Update();

	// �L�����N�^�[�̑��쏈��
	m_pCharacter[kEnemyNo]->Input();
	m_pCharacter[kEnemyNo]->Update();

	// �J�����̍X�V����
	m_pCamera->Update();

	// �G�̍U���\�͈͂ɂ��邩�ǂ���
	if (CheckModelAboutHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[1].get()))
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[kEnemyNo]->SetAttackRange(false);
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
	m_pCamera->SetTargetPos(m_pCharacter[kPlayerNo]->GetPos());

	// �J�����Ƀv���C���[�̊p�x�ƈʒu��n��
	m_pCamera->SetPlayerAngle(m_pCharacter[kPlayerNo]->GetAngle());

	// ���݂��Ɉʒu��n��
	m_pCharacter[kPlayerNo]->SetTargetPos(m_pCharacter[kEnemyNo]->GetPos());
	m_pCharacter[kEnemyNo]->SetTargetPos(m_pCharacter[kPlayerNo]->GetPos());

	// �L�����N�^�[�U�����菈��
	UpdateCharacter(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
	UpdateCharacter(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());


	{
#if _DEBUG
		CheckWeaponAndModelAboutHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndModelAboutHIt(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());

		CheckWeaponAndSieldHIt(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndSieldHIt(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());

		CheckWeaponAndBodyHit(m_pCharacter[kPlayerNo].get(), m_pCharacter[kEnemyNo].get());
		CheckWeaponAndBodyHit(m_pCharacter[kEnemyNo].get(), m_pCharacter[kPlayerNo].get());
#endif
	}
	
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			clsDx();
			return new SceneDebug();
		}
	}
	
	if (m_pCharacter[kPlayerNo]->GetHp() == 0 || m_pCharacter[kEnemyNo]->GetHp() == 0)
	{
		static int count = 0;
		count++;
		if (count == 60 * 2)
		{
			count = 0;
			clsDx();
			return new SceneResult();
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

bool SceneMain::CheckWeaponAndBodyHit(CharacterBase* chara1, CharacterBase* chara2)
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

bool SceneMain::CheckWeaponAndSieldHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetSieldPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetSieldRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 0.0f, -50.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetWeaponPos(), chara2->GetPos(),
		chara1->GetWeaponAttackRadius(), chara2->GetModelRadius(),
		chara1->GetRot(), chara2->GetRot(),
		chara1->GetWeaponAttackRelative(), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2)
{
	if (m_pColl->IsCheckHit(
		chara1->GetPos(), chara2->GetPos(),
		chara1->GetModelRadius(), chara2->GetModelRadius(),
		chara1->GetRot(), chara2->GetRot(),
		VGet(0.0f, 100.0f, 0.0f), VGet(0.0f, 100.0f, 0.0f)))
	{
		return true;
	}
	return false;
}

void SceneMain::UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2)
{
	// �W���X�g�K�[�h�������������ǂ���
	chara1->SetJustGuard(false);

	// ��]�p�x���擾
	chara1->SetTargetRota(chara2->GetRot());

	// �U���t���[�����ő吔���ǂ���
	if (chara2->GetAttackFrame() == chara2->GetAttackFrameMax() - 1)
	{
		// �W���X�g�K�[�h
		if (chara1->GetJustGuardFrame() > 0 &&
			chara1->GetJustGuardFrame() < chara1->GetJustGuardFrameMax())
		{
			// �W���X�g�K�[�h���������Ă���̂ŏ��ɓ������Ă��Ă��̂ɓ������Ă��Ă�
			// ���G����ɂȂ�
			if ((CheckWeaponAndSieldHIt(chara1, chara2)) || (CheckWeaponAndBodyHit(chara1, chara2)))
			{
				// �W���X�g�K�[�h�������������ǂ���
				chara1->SetJustGuard(true);

				// �W���X�g�K�[�h�����ۂ̃G�t�F�N�g�Đ�
				EffekseerDrawer::GetInstance().Play(
					handle, Id::JustGuard,
					EffectPlayType::NORMAL,
					VGet(chara1->GetPos().x, chara1->GetPos().y + 100.0f, chara1->GetPos().z),
					VGet(0, 0, 0),
					VGet(0, 0, 0));

				// �W���X�g�K�[�h���ꂽ����
				// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
				chara2->SetFightingMeter(-10.0f);

				// �W���X�g�K�[�h�ɐ�����������
				// �킢�ɕK�v�ȓ���ȃ��[�^�[�𑝂₷
				chara1->SetFightingMeter(30.0f);
	
				// �U���J�n
				StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);
			}
		}
		// �ʏ�K�[�h���o���邩�ǂ���
		else if (chara1->GetGuardFrame() == chara1->GetGuardFrameMax())
		{
			//�U�������ɓ����������ǂ���
			if (CheckWeaponAndSieldHIt(chara1, chara2))
			{
				// �K�[�h�����������ǂ���
				chara1->SetGuard(true);
				// �����U���������ꍇ
				if (chara2->GetMyAttackId() == AttackData::STRONG)
				{
					// �^�[�Q�b�g���X�^����Ԃɂ���
					chara1->SetStun(true);
				}

				// �K�[�h�ۂ̃G�t�F�N�g�Đ�
				EffekseerDrawer::GetInstance().Play(
					handle, Id::Guard,
					EffectPlayType::NORMAL,
					chara1->GetSieldPos(),
					VGet(0, 0, 0),
					VGet(0, chara1->GetAngle(), 0));

				// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
				chara1->SetFightingMeter(-10.0f);

			}
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
				StartJoypadVibration(DX_INPUT_PAD1, 1000/3, 1000/2, -1);
			}
		}
	}
}


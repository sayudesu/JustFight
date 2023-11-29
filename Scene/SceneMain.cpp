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
#include "../UI/UIDrawer.h";

#include "SceneGameOver.h"// �Q�[���I�[�o�[�V�[��
#include "SceneClear.h"// �Q�[���N���A�V�[��

#include "../Util/CharacterName.h"// �L�����N�^�[�̖��O

#include "../DEBUG.h"// �f�o�b�O�p

SceneMain::SceneMain():
	m_pUpdateFunc(nullptr),
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
	// �V�[���J��
	m_pUpdateFunc = &SceneMain::UpdateGamePlay;

	m_pCamera               = std::make_unique<Camera>();// �J�����N���X
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)] = std::make_shared<Player>(VGet(-300.0f, 260.0f, 0.0f));// �L�����N�^�[�N���X
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]  = std::make_shared<Enemy>(VGet(300.0f, 260.0f, 0.0f));  // �L�����N�^�[�N���X
	m_pField                = std::make_unique<FieldDrawer>();// �t�B�[���h�`��N���X
	m_pUi                   = std::make_unique<UIDrawer>();   // UI�`��N���X

	// ������
	m_pCamera->Init();
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->Init();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->Init();
	m_pField->Init();


	// ���H�p�̉�ʃn���h����ۑ�
	int sw = 0, sh = 0, bit = 0;      // ��ʕ�����ʍ����r�b�g��
	GetScreenState(&sw, &sh, &bit);   // ���ƍ������擾���Ă���
	m_tempScreen = MakeScreen(sw, sh);// ���H�p��ʂ�p�ӂ���
}

void SceneMain::End()
{
	// �������
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->End();
	m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->End();
	m_pField->End();

	DeleteGraph(m_tempScreen);

	for (int i = 0; i < m_pBlood.size(); i++)
	{
		// �f���[�g����
		delete m_pBlood[i];
		m_pBlood[i] = nullptr;
	}
}

SceneBase* SceneMain::Update()
{
	// �V�[���J��
	return (this->*m_pUpdateFunc)();
}

SceneBase* SceneMain::UpdateGamePlay()
{
	// �L�����N�^�[�̍X�V����
	UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::PLAYER)],
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)], true);
	UpdateCharacter(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)],
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)], false);

	// UI�Ƀp�����[�^�[�̏�Ԃ�n��
	m_pUi->SetParam(
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetMyId(),
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp(),
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetStrongPower(),
		m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetFightingMeter());

	// �v���C���[����O�ɏo����@�s�k
	// �G����O�ɏo����@�@�@�@�@����
	if (CheckCollMap(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]))
	{
		m_pUpdateFunc = &SceneMain::UpdateGameOver;
	}
	else if (CheckCollMap(m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]))
	{
		m_pUpdateFunc = &SceneMain::UpdateGameClear;
	}

	// �G�̍U���\�͈͂ɂ��邩�ǂ���
	if (CheckModelAboutHIt(m_pCharacter[static_cast<int>(CharacterName::PLAYER)], m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]))
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->SetAttackRange(false);
	}

	// �J�����Ƀv���C���[�ƃG�l�~�[�̈ʒu��n��
	m_pCamera->SetTargetPos(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetPos());
	// �J�����Ƀv���C���[�̊p�x�ƈʒu��n��
	m_pCamera->SetPlayerAngle(m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetAngle());
		// �J�����̍X�V����
	m_pCamera->Update();

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

	{
		if (m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->GetHp() == 0)
		{						
			//	m_pUpdateFunc = &SceneMain::UpdateGameOver;
		}
		else if (m_pCharacter[static_cast<int>(CharacterName::ENEMYNORMAL)]->GetHp() == 0)
		{			
		//	m_pUpdateFunc = &SceneMain::UpdateGameClear;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		clsDx();
		m_quakeX = 20.0f;
		m_quakeTimer = 144;
	//	return new SceneDebug();
	}

	if (--m_quakeTimer > 0) 
	{
		m_quakeX = -m_quakeX;
		m_quakeX *= 0.95f;
		--m_quakeTimer;
	}
	else
	{
		m_quakeX = 0.0f;
	}

	return this;
}

SceneBase* SceneMain::UpdateGameOver()
{
	if (Pad::IsTrigger(PAD_INPUT_1))
	{	
		clsDx();
		return new SceneGameOver();
	}

	printfDx("������");
	return this;
}

SceneBase* SceneMain::UpdateGameClear()
{

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		clsDx();
		return new SceneClear();
	}

	printfDx("����");
	return this;
}

void SceneMain::Draw()
{
	//���H�p�X�N���[���n���h�����Z�b�g
    SetDrawScreen(m_tempScreen);
	
	ClearDrawScreen();
	
	// DxLib�̎d�l��SetDrawScreen�ŃJ�����̈ʒu�Ȃǂ̐ݒ肪
	// �����������̂ł����ōĎw��
	m_pCamera->Init();

	// �}�b�v�̕`��
	m_pField->Draw();

	// �L�����N�^�[�̕`��
	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}
	
	// �����Ԃ��̕`��(��)
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}
	
#if _DEBUG
	DEBUG::Field();
	DEBUG::FrameMeter("P�̗�", 100, 50, m_pCharacter[0]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("E�̗�", 100, 100, m_pCharacter[1]->GetHp(), 6, 30, 0xffff00);
	DEBUG::FrameMeter("P�X�^�~�i", 100, 150, m_pCharacter[0]->GetFightingMeter(), 100, 15, 0xffff00);
	DEBUG::FrameMeter("E�X�^�~�i", 100, 200, m_pCharacter[1]->GetFightingMeter(), 100, 15, 0xffff00);

	DEBUG::FrameMeter("P�U���t���[��", 100, 250, m_pCharacter[0]->GetAttackFrameMax(), m_pCharacter[0]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("P�h��t���[��", 100, 300, m_pCharacter[0]->GetGuardFrameMax(), m_pCharacter[0]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 300, m_pCharacter[0]->GetJustGuardFrameMax(), m_pCharacter[0]->GetJustGuardFrame(), 30, 0xffffff);

	DEBUG::FrameMeter("E�U���t���[��", 100, 400, m_pCharacter[1]->GetAttackFrameMax(), m_pCharacter[1]->GetAttackFrame(), 20, 0xffff00);
	DEBUG::FrameMeter("E�h��t���[��", 100, 450, m_pCharacter[1]->GetGuardFrameMax(), m_pCharacter[1]->GetGuardFrame(), 30, 0xffff00);
	DEBUG::FrameMeter("              + JustGuard", 100, 450, m_pCharacter[1]->GetJustGuardFrameMax(), m_pCharacter[1]->GetJustGuardFrame(), 30, 0xffffff);
	DrawString(0, 0, "SceneMain", 0xffffff);
#endif

	SetDrawScreen(DX_SCREEN_BACK);

	if (m_quakeTimer > 0)
	{
//		GraphFilter(m_tempScreen, DX_GRAPH_FILTER_INVERT);         // �F�𔽓]������		
//		GraphFilter(m_tempScreen, DX_GRAPH_FILTER_MONO, 128, 0);   // �e�s�N�Z���̐F���q�f�a�`������YCbCr�`���ɕϊ��������� Cb Cr �̒l��u����������A�Ăтq�f�a�`���ɖ߂�
		GraphFilter(m_tempScreen, DX_GRAPH_FILTER_GAUSS, 16, 1400);// �K�E�X�łڂ���������
		SetDrawBlendMode(DX_BLENDMODE_ADD, 128);                   // ���Z��������		
		DrawGraph(m_quakeX, m_quakeX, m_tempScreen, false);        // ��ʂ�`��
		GraphFilter(m_tempScreen, DX_GRAPH_FILTER_GAUSS, 32, 1400);// �K�E�X�łڂ���������
		DrawGraph(m_quakeX, m_quakeX, m_tempScreen, false);        // ��ʂ�`��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);               // �u�����h���[�h��������Ԃɖ߂�
	}
	else
	{		
		DrawGraph(0, 0, m_tempScreen, false);// �ʏ��ʕ`��
	}

	m_pUi->Draw();
}

bool SceneMain::CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetCollPos(),
		character1->GetWeaponAttackRadius(), character2->GetWeaponAttackRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetShieldPos(),
		character1->GetWeaponAttackRadius(), character2->GetShieldRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetCollWeaponPos(), character2->GetCollPos(),
		character1->GetWeaponAttackRadius(), character2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckModelAboutHIt(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2)
{
	if (Coll::IsCheckHit(
		character1->GetPos(), character2->GetCollPos(),
		character1->GetModelRadius(), character2->GetModelRadius()))
	{
		return true;
	}
	return false;
}

bool SceneMain::CheckCollMap(std::shared_ptr<CharacterBase> character)
{
	MV1_COLL_RESULT_POLY_DIM HitPolyDim;

	// ���f���ƃJ�v�Z���Ƃ̓����蔻��
	HitPolyDim = MV1CollCheck_Capsule(
		m_pField->GetHandle(),
		-1,
		character->GetCapsulePosDown(),
		character->GetCapsulePosUp(),
		character->GetCapsuleRadius());

	// �����������ǂ����ŏ����𕪊�
	if (HitPolyDim.HitNum >= 1)
	{
		character->SetFieldHit();
		return false;
	}

	//// �����蔻����̌�n��
	//MV1CollResultPolyDimTerminate(HitPolyDim);

	//MV1_COLL_RESULT_POLY_DIM HitPolyDim;

	// ���f���ƃJ�v�Z���Ƃ̓����蔻��
	HitPolyDim = MV1CollCheck_Capsule(
		m_pField->GetHandle2(),
		-1,
		character->GetCapsulePosDown(),
		character->GetCapsulePosUp(),
		character->GetCapsuleRadius());

	// �����������ǂ����ŏ����𕪊�
	if (HitPolyDim.HitNum >= 1)
	{
		character->SetFieldHit();
		// �Q�[���I�[�o�[
	//	return true;
	}

	// �����蔻����̌�n��
	MV1CollResultPolyDimTerminate(HitPolyDim);

	return false;
}

// 1���U�������鑤
// 2���U�����󂯂鑤
void SceneMain::UpdateCharacter(std::shared_ptr<CharacterBase> character1, std::shared_ptr<CharacterBase> character2,bool isPlayer)
{
#if true

	// �v���C���[�̓��͏��
	character1->Input();
	// �v���C���[�X�V����
	character1->Update();

	// �^�[�Q�b�g��HP���擾
	character1->SetTargetHp(character2->GetHp());
	// �^�[�Q�b�g�̈ʒu���󂯎��
	character1->SetTargetPos(character2->GetPos());

	// �^�[�Q�b�g�̐퓬�̏�Ԃ��󂯎��
	character1->SetBattleState(character2->GetBattleState());

	// �W���X�g�K�[�hOFF
//	character1->SetJustGuard(false);

	// ��]�p�x���擾
	character2->SetTargetMtxRota(character1->GetRot());

	// �W���X�g�K�[�h����
	// �U�����������Ă����ꍇ
	if ((CheckWeaponAndShieldHIt(character2, character1)))
	{
		// �W���X�g�K�[�h�t���[��
		if (character1->GetJustGuardFrame() > 0 &&
			character1->GetJustGuardFrame() < character1->GetJustGuardFrameMax())
		{
			// �W���X�g�K�[�h�������������ǂ���
			character1->SetJustGuard(true);

			// �G�t�F�N�g���Đ�
			character1->SetCollJustGuardEffect();

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			character2->SetFightingMeter(-30.0f);

			// �U���J�n
			StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

			return;
		}
	}

	// �ʏ�K�[�h����
	// �ʏ�K�[�h���o���邩�ǂ���
	if (character1->GetGuardFrame() == character1->GetGuardFrameMax())
	{
		// �U����Ԃ�������
		//�U�������ɓ����������ǂ���
		if (CheckWeaponAndShieldHIt(character2, character1))
		{
			// �m�b�N�o�b�N
			character1->SetGuardKnockBack(true, -20);

			// ���U�����邽�߂̗͂𗭂߂�
			character1->SetStrongPower(20.0f);

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			character1->SetFightingMeter(-3.0f);

			character1->SetCollGuardEffect();

			character2->SetWeaponAttacksShield(true);

			return;
		}		
	}


	const bool a = character1->GetBattleState() == BattleState::ATTACK;
	const bool b = character1->GetBattleState() == BattleState::ATTACKTWO;
	const bool c = character1->GetBattleState() == BattleState::STRONGATTACK;
	// �U����^���鏈��
	if (a || b || c)
	{
		// �U���������������ǂ���
		if (CheckWeaponAndBodyHit(character1, character2))
		{
			// �_���[�W��^����
			character2->SetDamage(true);

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			character1->SetFightingMeter(-0.1f);

			// �m�b�N�o�b�N
			if (character1->GetBattleState() == BattleState::STRONGATTACK)
			{
				character2->SetGuardKnockBack(true, -30);
			}
			else
			{
				character2->SetGuardKnockBack(true, -10);
			}

			int color = 0xffffff;
			if (isPlayer)
			{
				color = 0x000000;
			}
			else
			{
				color = 0xffffff;
			}

			const bool d = character2->GetBattleState() != BattleState::GUARD;
			if (d)
			{
				for (int i = 0; i < 100; i++)
				{
					m_pBlood.push_back(new BloodDrawer(VGet(character2->GetPos().x, character2->GetPos().y + 100.0f, character2->GetPos().z), color));
					m_pBlood.back()->Init(i);
				}
			}

			// �U���J�n
			StartJoypadVibration(DX_INPUT_PAD1, 1000 / 3, 1000 / 2, -1);

			return;
		}		
	}
#endif
}


#include <DxLib.h>

#include "SceneMain.h"
#include "SceneDebug.h" // �f�o�b�O�V�[��
#include "SceneResult.h"// ���U���g�V�[��

#include "../Object/Camera/Camera.h"    // �J����
#include "../Object/Player/Player.h"    // �v���C���[
#include "../Object/Enemy/Enemy.h"      // �G�l�~�[
#include "../Object/CharacterBase.h"    // �L�����N�^�[
#include "../Object/Stage/FIeldDrawer.h"// �}�b�v�`��

#include "../Util/Collision3D.h"    // �����蔻��
#include "../Util/EffekseerDrawer.h"// 3D�G�t�F�N�g
#include "../Util/Game.h"           // �Q�[����{�ݒ�
#include "../Util/Pad.h"            // �p�b�h
#include "../Util/BloodDrawer.h"    // ���̃G�t�F�N�g
#include "../Util/CharacterName.h"  // �L�����N�^�[�̖��O
#include "../Util/BlurScreen.h";    // ��ʉ��H
#include "../Util/TutorialDrawer.h"
#include "../Util/Tips.h"
#include "../Util/SoundName.h"
#include "../Util/HitPos.h"

#include "../UI/UIDrawer.h";// UI�`��

#include "../CSVData/SoundManager.h"

SceneMain::SceneMain(DifficultyData data):
	m_pUpdateFunc(nullptr),
	m_pCamera(nullptr),
	m_hCheckmate(-1),
	m_resultData(GameResultData::NONE),
	m_frameCount(0),
	m_difficultyData(data),
	m_checkmateSize(10.0f),
	m_checkmateRota(10.0f),
	m_checkmateBgBlendRate(0),
	m_checkmatePosY(0)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{	
	// �R�[�h�̌��₷���̈וϐ���
	int player = static_cast<int>(CharacterName::PLAYER);
	int enemy = static_cast<int>(CharacterName::ENEMY);

	// �V�[���J��
	m_pUpdateFunc = &SceneMain::UpdateGamePlay;

	m_pCharacter[player] = std::make_shared<Player>(m_difficultyData, VGet(-300.0f, 300.0f, 0.0f)); // �L�����N�^�[�N���X
	m_pCharacter[enemy]  = std::make_shared<Enemy>(m_difficultyData, VGet(300.0f, 300.0f, 0.0f)); // �L�����N�^�[�N���X

	m_pCamera   = std::make_unique<Camera>();        // �J�����N���X
	m_pField    = std::make_unique<FieldDrawer>();   // �t�B�[���h�`��N���X
	m_pUi       = std::make_unique<UIDrawer>();      // UI�`��N���X
	m_pTutorial = std::make_unique<TutorialDrawer>();// �`���[�g���A���N���X

	// ������
	m_pCharacter[player]->Init();
	m_pCharacter[enemy]->Init();
	m_pCamera->Setting();
	m_pField->Init();
	m_pTutorial->Init();

	// �`�F�b�N���C�g�摜�̓ǂݍ���
	m_hCheckmate = LoadGraph("Data/Image/UI/Checkmate.png");

	// UI�Ƀp�����[�^�[�̏�Ԃ�n��
	m_pUi->SetParam(
		m_pCharacter[player]->GetMyId(),
		m_pCharacter[player]->GetHp(),
		m_pCharacter[player]->GetMaxHp(),
		m_pCharacter[player]->GetStrongPower(),
		m_pCharacter[player]->GetkStrongAttackPowerMax(),
		m_pCharacter[player]->GetFightingMeter());
	m_pUi->SetParam(
		m_pCharacter[enemy]->GetMyId(),
		m_pCharacter[enemy]->GetHp(),
		m_pCharacter[enemy]->GetMaxHp(),
		m_pCharacter[enemy]->GetStrongPower(),
		m_pCharacter[enemy]->GetkStrongAttackPowerMax(),
		m_pCharacter[enemy]->GetFightingMeter());

	// �X�N���[�����ʂ̏�����
	EffectScreen::GetInstance().BlurIReplayInit();	
}

void SceneMain::End()
{
	// �������
	m_pCharacter[static_cast<int>(CharacterName::PLAYER)]->End();
	m_pCharacter[static_cast<int>(CharacterName::ENEMY)]->End();

	m_pField->End();
	m_pTutorial->End();

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
	// BGM�̍Đ�
	SoundManager::GetInstance().Play(SoundName::PLAY, true);

	// �R�[�h�̌��₷���̈וϐ���
	int player = static_cast<int>(CharacterName::PLAYER);
	int enemy = static_cast<int>(CharacterName::ENEMY);

	// if ��Փx�����A�傾������
	// else if ��Փx������������ �`���[�g���A��
	if (m_difficultyData == DifficultyData::INTERMEDIATE||
		m_difficultyData == DifficultyData::EXPERT)
	{
		m_pCharacter[player]->Input();
	//	m_pCharacter[enemy]->Input();
	}
	else if(m_difficultyData == DifficultyData::NOIVE)
	{
		m_pCharacter[player]->InputTutorial();
		m_pCharacter[enemy]->InputTutorial();
		m_pTutorial->SetTips(Tips::MOVE);

		if (m_pCharacter[player]->GetTipsMove(Tips::ATTACK))
		{
			m_pTutorial->SetTips(Tips::ATTACK);
		}
		if (m_pCharacter[player]->GetTipsMove(Tips::GUARD))
		{
			m_pTutorial->SetTips(Tips::GUARD);
		}

	}

	// �L�����N�^�[�̍X�V����
	UpdateCharacter(m_pCharacter[player],m_pCharacter[enemy], true);
	UpdateCharacter(m_pCharacter[enemy], m_pCharacter[player], false);

	// UI�Ƀp�����[�^�[�̏�Ԃ�n��
	m_pUi->SetParam(
		m_pCharacter[player]->GetMyId(),
		m_pCharacter[player]->GetHp(),
		m_pCharacter[player]->GetMaxHp(),
		m_pCharacter[player]->GetStrongPower(),
		m_pCharacter[player]->GetkStrongAttackPowerMax(),
		m_pCharacter[player]->GetFightingMeter());
	m_pUi->SetParam(
		m_pCharacter[enemy]->GetMyId(),
		m_pCharacter[enemy]->GetHp(),
		m_pCharacter[enemy]->GetMaxHp(),
		m_pCharacter[enemy]->GetStrongPower(),
		m_pCharacter[enemy]->GetkStrongAttackPowerMax(),
		m_pCharacter[enemy]->GetFightingMeter());


	// �G�̍U���\�͈͂ɂ��邩�ǂ���
	if (CheckModelAboutHIt(m_pCharacter[player], m_pCharacter[enemy]))
	{
		m_pCharacter[enemy]->SetAttackRange(true);
	}
	else
	{
		m_pCharacter[enemy]->SetAttackRange(false);
	}

	// �J�����Ƀv���C���[�ƃG�l�~�[�̈ʒu��n��
	m_pCamera->SetTargetPos(m_pCharacter[player]->GetPos());
	// �J�����Ƀv���C���[�̊p�x�ƈʒu��n��
	m_pCamera->SetPlayerAngle(m_pCharacter[player]->GetAngle());
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

	// ���s��������
	{
		// HP��0�ɂȂ����ꍇ
		if (m_pCharacter[player]->GetHp() <= 0) // �v���C���[
		{		
			m_frameCount++;
			if (m_frameCount > 20)
			{
				m_frameCount = 0;
				m_resultData = GameResultData::OVER;
			}
		}
		else if (m_pCharacter[enemy]->GetHp() <= 0) // �G�l�~�[
		{	
			m_frameCount++;
			if (m_frameCount > 20)
			{
				m_frameCount = 0;
				m_resultData = GameResultData::CREAR;
			}
		}

		// ��O�ɏo���ꍇ
		if (CheckCollMap(m_pCharacter[player])) // �v���C���[
		{
			m_resultData = GameResultData::OVER;
		}
		else if (CheckCollMap(m_pCharacter[enemy])) // �G�l�~�[
		{
			m_resultData = GameResultData::CREAR;
		}

		// �Q�[���̃N���A,�I�[�o�[�����̊m�F
		if (m_resultData != GameResultData::NONE)
		{
			m_pUpdateFunc = &SceneMain::UpdateGameResult;
			m_frameCount = 0;
		}
	}

	// ��ʐU���X�V����
	EffectScreen::GetInstance().QuakeUpdate();

	return this;
}

SceneBase* SceneMain::UpdateGameResult()
{
	// �w��t���[���̌�Ƀ��U���g��ʂɈړ�����
	m_frameCount++;

	// �{�^�����������ꍇ
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// BGM�̒�~
		SoundManager::GetInstance().Stop(SoundName::PLAY);
		return new SceneResult(m_resultData, m_difficultyData);
	}

	// �w�肵���t���[���܂ŃJ�E���g���i�ނ�
	if (m_frameCount >= 60 * 3)
	{
		// BGM�̒�~
		SoundManager::GetInstance().Stop(SoundName::PLAY);
		return new SceneResult(m_resultData, m_difficultyData);
	}

	// �摜�̈ʒu�𓮂����v�Z
	m_checkmatePosY = cosf(static_cast<float>(m_frameCount) * 0.07f) * 100.0f + static_cast<float>(Game::kScreenHeight) / 2.0f - 160.0f;

	// �\�߂ł��������T�C�Y��1�ɂ���
	if (m_checkmateSize > 1.0f)
	{
		m_checkmateSize -= 1.0f;
	}
	// �\�ߕύX�����p�x��0�ɂ���
	if (m_checkmateRota > 0.0f)
	{
		m_checkmateRota -= 0.5f;
	}

	// �u�����h����ύX
	if (m_checkmateBgBlendRate < 128)
	{
		m_checkmateBgBlendRate += 5;
	}

	return this;
}


void SceneMain::Draw()
{
	// �V������ʂ̍쐬
	EffectScreen::GetInstance().BlurPreRenderBlurScreen();
	
	// ��ʂ��N���A�ɂ���
	EffectScreen::GetInstance().ClearScreen();
	
	// DxLib�̎d�l��SetDrawScreen�ŃJ�����̈ʒu�Ȃǂ̐ݒ肪
	// �����������̂ł����ōĎw��
	m_pCamera->Setting();

	// ��Ɠ��l������������̂ł�����x�Đݒ肷��
	EffekseerDrawer::GetInstance().EffekseerSync();

	// �}�b�v�̕`��
	m_pField->Draw();
	
	// �����Ԃ��̕`��(��)
	for (auto& blood : m_pBlood)
	{
		blood->Draw();
	}
	// �L�����N�^�[�̕`��
	for (auto& character : m_pCharacter)
	{
		character->Draw();
	}

	// �u���[���ʁA��ʂ̐U�����ʂ�`�悷��
	EffectScreen::GetInstance().BlurPostRenderBlurScreen();
	
#if false	
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

	////// ��ʃG�t�F�N�g�̍X�V����
	//EffectScreen::GetInstance().ScreenBack();
	//EffectScreen::GetInstance().QuakePostRenderBlurScreen();

	// UI�̕`��
	m_pUi->Draw();

	if (m_difficultyData == DifficultyData::NOIVE)
	{
		// �`���[�g���A���p�`��
		m_pTutorial->Draw();
	}

	// ���s�������ꍇ�`�悷��
	if (m_pUpdateFunc == &SceneMain::UpdateGameResult)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_checkmateBgBlendRate);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawRotaGraph(Game::kScreenWidth/2, m_checkmatePosY, m_checkmateSize, m_checkmateRota, m_hCheckmate, true);
	}

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
	character->IsCheckHitWall(false,HitPos::NONE);
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
		// �����������L�����N�^�[�ɂ�n��
		character->SetFieldHit();

		if (fabs(HitPolyDim.Dim->Normal.x) > 0.9f)
		{					
			if (HitPolyDim.Dim->Position->x < character->GetPos().x + character->GetModelRadius() / 2)
			{
				printfDx("�� = X+\n");
				character->IsCheckHitWall(true,HitPos::XP);
			}			
		}
		if (fabs(HitPolyDim.Dim->Normal.z) > 0.9f)
		{
			printfDx("HIT == ");

			if (HitPolyDim.Dim->Position->z < character->GetPos().z + character->GetModelRadius() / 2)
			{
				printfDx("�� = Z+\n");
				character->IsCheckHitWall(true, HitPos::ZP);
			}
		}
	}

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
		// �Q�[���I�[�o�[
	//	return true;
		// �����������L�����N�^�[�ɂ�n��
		character->SetFieldHit();
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

	// �K�[�h�������Ȃ����
	character1->SetWeaponAttacksShield(false);

	// �W���X�g�K�[�h����
	// �U�����������Ă����ꍇ
	if (CheckWeaponAndShieldHIt(character2, character1))
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
			character2->SetFightingMeter(-110.0f);

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
		// �U�������ɓ����������ǂ���
		if (CheckWeaponAndShieldHIt(character2, character1))
		{
			// �m�b�N�o�b�N
			character1->SetGuardKnockBack(true, -20);

			// ���U�����邽�߂̗͂𗭂߂�
			character1->SetStrongPower(20.0f);

			// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
		//	character1->SetFightingMeter(-3.0f);

			character1->SetCollGuardEffect();

			character1->SetWeaponAttacksShield(true);

			return;
		}		
	}
	else
	{
		const bool isAttack = character1->GetBattleState()       == BattleState::ATTACK;
		const bool isAttackTow = character1->GetBattleState()    == BattleState::ATTACKTWO;
		const bool isStrongAttack = character1->GetBattleState() == BattleState::STRONGATTACK;

		// �U����^���鏈��
		if (isAttack || isAttackTow || isStrongAttack)
		{
			// �U���������������ǂ���
			if (CheckWeaponAndBodyHit(character1, character2))
			{
				// �_���[�W��^����
				character2->SetDamage(true);

				// �킢�ɕK�v�ȓ���ȃ��[�^�[�����炷
			//	character1->SetFightingMeter(-0.1f);

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
	}

#endif
}


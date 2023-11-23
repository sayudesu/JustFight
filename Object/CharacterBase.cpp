#include "CharacterBase.h"
#include "../Util/MyLoadModel.h"
#include <cmath>
#include "../Util/EffekseerDrawer.h"
#include "../GameObject.h"

namespace
{
	// �J�v�Z���`��̔��a
	float kCapsuleRadius = 64.0f;

	// �d��
	float kGravity = 32.0f;
}

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_effectHandle(-1),
	m_rotMtx({}),
	m_targetRotMtx({}),
	m_pos(pos),
	m_targetPos    (VGet(0,0,0)),
	m_vecWeapon    (VGet(0, 0, 0)),
	m_vecShield    (VGet(0, 0, 0)),
	m_tempWeaponPos(VGet(0, 0, 0)),
	m_tempFightingMeter(0),
	m_angle         (0.0f),
	m_isAttack      (false),
	m_isStrongAttack(false),
	m_isGuard       (false),
	m_isJustGuard   (false),
	m_isAway	    (false),
	m_isResultGuard (false),
	m_isResultDamage(false),
	m_isWeaponAttacksShield(false),
	m_isStun        (false),
	m_isAttackRange (false),
	m_targetHp      (0),
	m_attackFrame   (0),
	m_attackGapFrame(0),
	m_guardFrame    (0),
	m_justGuardFrame(0),
	m_stunFrame     (0),
	m_recoilFrame   (0),
	m_attackAfterStopFrame(0),
	m_isSceneChange(false),
	m_comboAttack   (0)
{
	// �����o�֐��̏���
	m_pFunc = &CharacterBase::Idle;

	// ���g�̔��ʗpID��������
	m_myId = CharacterName::NONE;

	// ���݂̍s�����L�^
	m_battleState       = BattleState::NONE;// ���g
	m_targetBattleState = BattleState::NONE;// �^�[�Q�b�g
	m_tempTargetBattleState = BattleState::NONE;// �^�[�Q�b�g
	
	// ���g�̃p���[���[�^�[
	m_parameter = {};
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	// ���f���̑��Έʒu
	m_vecWeapon = m_parameter.weaponRelativePos;
	m_vecShield = m_parameter.shieldRelativePos;
	// ���f���̊p�x
	testV1.z = 90 * DX_PI_F / 180.0f;

	m_hp = m_parameter.hpMax;
	m_vec = m_parameter.knockBackPos;
	m_fightingMeter = m_parameter.fightingMeterMax;

	VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
	m_weaponPos = VAdd(m_pos, move);

	// �I�u�W�F�N�g�̐���
	m_my = new GameObject(
		m_parameter.fileName.c_str(),
		m_pos,
		VGet(testV1.x, m_angle, testV1.z),
		VGet(40.0f, 40.0f, 40.0f));

	// ����I�u�W�F�N�g�̐���
	m_weapon = new GameObject(
		"Data/Model/SwordCollTest.mv1",
		m_weaponPos,
		VGet(testV1.x, m_angle, testV1.z),
		VGet(3.0f, 3.0f, 3.0f));

	// ���I�u�W�F�N�g�̐���
	m_shield = new GameObject(
		"Data/Model/Shield.mv1",
		VGet(100, m_weaponPos.y, 100),
		VGet(testV1.x, m_angle, testV1.z),
		VGet(3.0f, 3.0f, 3.0f));

	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::End()
{
	// �������
	delete m_my;
	m_my = nullptr;
	delete m_weapon;
	m_weapon = nullptr;
	delete m_shield;
	m_shield = nullptr;
}

void CharacterBase::Update()
{
	(this->*m_pFunc)();
}

void CharacterBase::Draw()
{
	// �L�����N�^�[���f��
	m_my->Draw();
	// ���탂�f��
	m_weapon->Draw();
	// �����f��
	m_shield->Draw();

#if _DEBUG
	// �J�v�Z���̕`��
	DrawCapsule3D(m_capsuleUpDown, m_capsuleUpPos, kCapsuleRadius, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), FALSE);
#endif
}

void CharacterBase::TargetMove()
{
	float tempPosY = m_pos.y;
	// �������Z�o
	VECTOR m_dir = VSub(m_targetPos, m_pos);

	// �v���C���[����G�l�~�[�܂ł̊p�x�����߂�
	const float angle = atan2(m_dir.y, m_dir.x);

	// ���ݓG�������Ă�������̃x�N�g���𐶐�����
	const MATRIX enemyRotMtx = MGetRotY(angle);

	// �΂߂ɂȂ����Ƃ�((1, 1, 0)�Ȃ�)�ɂ������񒷂��P�ɖ߂�(���K��)
	if (VSquareSize(m_dir) > 0)
	{
		m_dir = VNorm(m_dir);
	}

	// ���x�����߂�
	const VECTOR velecity = VScale(m_dir, 5.0f);

	// �ʒu��ς���
	m_pos = VAdd(m_pos, velecity);

	m_pos.y = tempPosY;

	// �����𑪂�
	m_targetRange.x = sqrt(pow(m_pos.x - m_targetPos.x, 2.0f) + pow(m_pos.x - m_targetPos.x, 2.0f));
	m_targetRange.z = sqrt(pow(m_pos.z - m_targetPos.z, 2.0f) + pow(m_pos.z - m_targetPos.z, 2.0f));
}

CharacterName CharacterBase::GetMyId()
{
	return m_myId;
}

BattleState CharacterBase::GetBattleState()
{
	return m_battleState;
}

void CharacterBase::Idle()
{
	// 
	SetFightingMeter(0.03f);

	// �R���{������
	m_comboAttack = 0;


	// ��������̈ʒu�ɖ߂�
	{
		bool isEndX = false;
		bool isEndZ = false;
		if (m_vecWeapon.x > m_parameter.weaponRelativePos.x)
		{
			m_vecWeapon.x -= 10.0f;
		}
		else
		{
			isEndX = true;
			m_vecWeapon.x = m_parameter.weaponRelativePos.x;
		}

		if (m_vecWeapon.z < m_parameter.weaponRelativePos.z)
		{
			m_vecWeapon.z += 30.0f;
		}
		else
		{
			isEndZ = true;
			m_vecWeapon.z = m_parameter.weaponRelativePos.z;
		}

		if (isEndX && isEndZ)
		{
			m_isAttack = false;
			m_isStrongAttack = false;
		}

		if (m_vecWeapon.y < m_parameter.weaponRelativePos.y)
		{
			m_vecWeapon.y -= 10.0f;
		}
		else
		{
			m_vecWeapon.y = m_parameter.weaponRelativePos.y;
		}
	}

	// �������̈ʒu�ɖ߂�
	{
		if (m_vecShield.x < m_parameter.shieldRelativePos.x)
		{
			m_vecShield.x += 30.0f;
		}
		else
		{
			m_vecShield.x = m_parameter.shieldRelativePos.x;
		}

		if (m_vecShield.y < m_parameter.shieldRelativePos.y)
		{
			m_vecShield.y -= 30.0f;
		}
		else
		{
			m_vecShield.y = m_parameter.shieldRelativePos.y;
		}
	}

	if (m_hp == 0)
	{
		m_pFunc = &CharacterBase::Losers;
		m_vecWeapon.y = 100.0f;
	}

	if (m_targetHp == 0)
	{
		m_pFunc = &CharacterBase::Winner;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}

	// ����̊p�x��ς���
	if (test2 > 0.0f)
	{
		m_isAttack = true;
		test2 -= 00.5f;
	}
	else
	{
		m_isAttack = false;
		// ���݂̍s�����L�^
		m_battleState = BattleState::IDLE;
		test2 = 0.0f;
	}

	if (m_targetBattleState == BattleState::IDLE)
	{
		m_tempTargetBattleState = BattleState::NONE;
	}

	test3 = test2;

	// ���݂�HP�𒲐�
	HitPoint();

	// �ʒu���̍X�V
	UpdatePos();
}

// �U�������ꍇ
void CharacterBase::Attack()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::ATTACK;

	// ���̃R���{�U���ɐ؂�ւ���
	if (m_comboAttack == 2)
	{
		test2 = -(90 * 3) * DX_PI / 180.0f;
		test3 = test2;
		// �U�����J�n����
		m_isAttack = true;
		// �d����ԗp�t���[�������Z�b�g
		m_attackAfterStopFrame = 0;
		// �U���t���[�������Z�b�g
		m_attackFrame = 0;
		// �V�[���J�ڗp
		m_isSceneChange = false;
		// ���̍U���Ɉڍs����
		m_pFunc = &CharacterBase::AttackTwo;
		return;
	}

	// ���퓮����
	if (!m_isSceneChange)
	{
		test2 = MoveByFrame(test3, (90 * 3) * DX_PI / 180.0f, m_attackFrame, m_parameter.attackFrameMax);
		m_vecWeapon.z = MoveByFrame(m_tempWeaponPos.z, -30.0f, m_attackFrame, m_parameter.attackFrameMax);
		m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.x, 0.0f, m_attackFrame, m_parameter.attackFrameMax);
		// �U�����̃t���[������Z
		m_attackFrame++;
	}

	// �ő�t���[���ɓ��B������
	if (m_attackFrame == m_parameter.attackFrameMax)
	{
		m_isSceneChange = true;
	}
	
	// �V�[����؂�ւ��鎖���ł���Ȃ�
	if (m_isSceneChange)
	{	
		// �U�����Ă��Ȃ�
		m_isAttack = false;

		// �U����̍d�����
		m_attackAfterStopFrame++;
		if (m_attackAfterStopFrame == m_parameter.attackAfterStopFrameMax)
		{
			// �d����ԗp�t���[�������Z�b�g
			m_attackAfterStopFrame = 0;			
			// �U���t���[�������Z�b�g
			m_attackFrame = 0;
			// �R���{�I��
			m_comboAttack = 0;
			// �V�[���J�ڗp
			m_isSceneChange = false;
			// �V�[���@��
			m_pFunc = &CharacterBase::Idle;
		}

	}

	// �X�^�����
	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}

	// ���݂�HP�𒲐�
	HitPoint();
	// �ʒu���̍X�V
	UpdatePos();

	m_my->Move(m_pos);
	m_my->Rotate(VGet(0.0f, m_angle + ((90) * DX_PI_F / 180.0f), 0.0f));
	m_my->Update();
}

// �U���R���{2
void CharacterBase::AttackTwo()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::ATTACKTWO;

	// ���퓮����
	if (!m_isSceneChange)
	{
		test2 = MoveByFrame(test3, -((90 * 5) * DX_PI / 180.0f), m_attackFrame, m_parameter.attackFrameMax);
		m_vecWeapon.z = MoveByFrame(m_tempWeaponPos.z, -30.0f, m_attackFrame, m_parameter.attackFrameMax);
		m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.x, 0.0f, m_attackFrame, m_parameter.attackFrameMax);
		// �U�����̃t���[������Z
		m_attackFrame++;
	}

	// �ő�t���[���ɓ��B������	
	if (m_attackFrame == m_parameter.attackFrameMax)
	{
		m_isSceneChange = true;
	}

	// �V�[����؂�ւ��鎖���ł���Ȃ�
	if (m_isSceneChange)
	{
		// �U����̍d�����
		m_attackAfterStopFrame++;
		if (m_attackAfterStopFrame == m_parameter.attackAfterStopFrameMax)
		{
			test3 = 0;
			// �U���I��
			m_isAttack = false;
			// �d����ԗp�t���[�������Z�b�g
			m_attackAfterStopFrame = 0;
			// �U���t���[�������Z�b�g
			m_attackFrame = 0;
			// �R���{�I��
			m_comboAttack = 0;
			// �V�[���J�ڗp
			m_isSceneChange = false;
			// �V�[���@��
			m_pFunc = &CharacterBase::Idle;
		}
	}

	// ���݂�HP�𒲐�
	HitPoint();

	// �X�^�����
	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}

	// ����
	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		m_weaponPos = VAdd(m_pos, move);
		m_weapon->Move(m_weaponPos);
		m_weapon->Rotate(VGet((90 * 3) * DX_PI / 180.0f, m_angle - test2, 0));
		m_weapon->Update();
	}
	// ��
	{
		VECTOR move = VTransform(m_vecShield, m_rotMtx);
		move = VAdd(m_pos, move);
		m_shield->Move(move);
		m_shield->Rotate(VGet(0, m_angle, 0));
		m_shield->Update();
	}

	m_my->Move(m_pos);
	m_my->Rotate(VGet(0.0f, m_angle + ((90) * DX_PI_F / 180.0f), 0.0f));
	m_my->Update();

	m_capsuleUpDown = m_pos;
	m_capsuleUpDown.y = m_pos.y + 50.0f;

	m_capsuleUpPos = m_capsuleUpDown;
	m_capsuleUpPos.y = m_capsuleUpDown.y + 150.0f;

	// �m�b�N�o�b�N���ꂽ�ꍇ
	KnockBack();
}

// ���U�������ꍇ
void CharacterBase::StrongAttack()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::STRONGATTACK;

#if false
	int slideX = 0;
	int slideY = 0;
	int slideZ = 0;
	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	// �n�߂Ɍ��悤�̌��ɓ���������
	if (m_attackGapFrame < m_parameter.strongAttackFrameGapMax  )
	{
		m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z, 100.0f, m_attackGapFrame, m_parameter.strongAttackFrameGapMax  );
		m_attackGapFrame++;
		m_tempWeaponPos = m_vecWeapon;
		slideX = GetRand(6);
		slideY = GetRand(6);
		slideZ = GetRand(6);
	}
	// �U�������铮��
	if (m_attackGapFrame == m_parameter.strongAttackFrameGapMax  )
	{
		slideX = 0;
		slideY = 0;
		slideZ = 0;
		if (m_attackFrame < m_parameter.attackFrameMax)
		{
			m_vecWeapon.z = MoveByFrame(m_tempWeaponPos.z, -200.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.x, 0.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			m_attackFrame++;
		}
	}

	if (m_attackFrame + m_attackGapFrame == m_parameter.strongAttackRotalFrame )
	{
		m_attackFrame = 0;
		m_attackGapFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}
#endif	

	// ���퓮����
	if (!m_isSceneChange)
	{
		if (m_attackGapFrame < m_parameter.strongAttackFrameGapMax)
		{
			m_vecWeapon.x = MoveByFrame(0.0f, 0.0f, m_attackGapFrame, m_parameter.strongAttackFrameGapMax);
			m_vecWeapon.y = MoveByFrame(0.0f, 150.0f, m_attackGapFrame, m_parameter.strongAttackFrameGapMax);
			m_vecWeapon.z = MoveByFrame(0.0f, -100.0f, m_attackGapFrame, m_parameter.strongAttackFrameGapMax);

			// �U�����̃t���[������Z
			m_attackGapFrame++;
		}
		if (m_attackGapFrame == m_parameter.strongAttackFrameGapMax)
		{
			test1 = MoveByFrame(0.0f, (90) * DX_PI_F / 180.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			//m_vecWeapon.y = MoveByFrame(m_parameter.weaponRelativePos.y, 500.0f, m_attackFrame, m_parameter.strongAttackFrameMax);
			//m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z, -300.0f, m_attackFrame, m_parameter.strongAttackFrameMax);

			//// �U�����̃t���[������Z
			m_attackFrame++;
		}
	}

	// �ő�t���[���ɓ��B������
	if (m_attackFrame == m_parameter.strongAttackFrameMax + 60 * 3)
	{
		m_isSceneChange = true;
	}

	//// �I�u�W�F�N�g�̏��
	//{
	//	VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
	//	m_weaponPos = VAdd(m_pos, move);
	//	m_weapon->Move(m_weaponPos);

	//	m_weapon->Rotate(VGet(m_angle - test1, m_angle, 0.0f));
	//	m_weapon->Update();
	//}


	// �V�[����؂�ւ��鎖���ł���Ȃ�
	if (m_isSceneChange)
	{
		test1 = 0.0f;
		// �U���t���[�������Z�b�g
		m_attackFrame = 0;
		m_attackGapFrame = 0;
		// �V�[���J�ڗp
		m_isSceneChange = false;
		// �V�[���@��
		m_pFunc = &CharacterBase::Idle;
	}

	// ���݂�HP�𒲐�
	HitPoint();
	UpdatePos();

	m_my->Move(m_pos);
	m_my->Rotate(VGet(0.0f, m_angle + ((90) * DX_PI_F / 180.0f), 0.0f));
	m_my->Update();
}

// �K�[�h�����ꍇ
void CharacterBase::Guard()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::GUARD;

	// �ő�t���[�����ɖڕW�n�_�܂ňړ�����
	if (m_guardFrame < m_parameter.guardFrameMax )
	{
		m_vecShield.x = MoveByFrame(m_parameter.shieldRelativePos.x, 0.0f, m_guardFrame, m_parameter.guardFrameMax);
		m_guardFrame++;

		// �W���X�g�K�[�h�̃t���[��
		// + 1 �̓W���X�g�K�[�h�͈̔͊O������
		if (m_justGuardFrame < m_parameter.justGuardFrameMax + 1)
		{
			m_justGuardFrame++;
		}
		
	}
	else
	{
		m_guardFrame = m_parameter.guardFrameMax ;
		m_vecShield.x = 0.0f;
	}


	// �K�[�h����߂��ꍇ
	if (!m_isGuard)
	{
		m_guardFrame = 0;
		m_justGuardFrame = 0;
		m_pFunc = &CharacterBase::Idle;
	}

	if (m_isStun)
	{
		m_pFunc = &CharacterBase::Stun;
	}
	// ���݂�HP�𒲐�
	HitPoint();
	// �ʒu���̍X�V
	UpdatePos();
}

// �W���X�g�K�[�h�����ꍇ
void CharacterBase::JustGuard()
{
	m_vecWeapon = m_parameter.weaponRelativePos;
	m_pFunc = &CharacterBase::Idle;

	// �ʒu���̍X�V
	UpdatePos();
}

// �X�^�������ꍇ
void CharacterBase::Stun()
{
	// ���݂̍s�����L�^
	m_battleState = BattleState::STUN;
	// �X�^����Ԃ̃t���[�����J�E���g
	m_stunFrame++;

	if (m_vecWeapon.y < m_parameter.weaponRelativePos.y + 60.0f)
	{
		m_vecWeapon.y += 10.0f;
	}

	// �t���[���̃��Z�b�g
	m_attackFrame    = 0;
	m_attackGapFrame = 0;
	m_guardFrame     = 0;
	m_justGuardFrame = 0;

	if (/*m_parameter.stunFrameMax*/1 < m_stunFrame)
	{
		m_stunFrame = 0;
		m_isStun = false;
		m_pFunc = &CharacterBase::Idle;
	}

	// �ʒu���̍X�V
	UpdatePos();
}

void CharacterBase::Winner()
{
	// �ʒu���̍X�V
	UpdatePos();

	m_weapon->Move(VGet(90 * DX_PI_F / 180.0f, m_angle, 0));
}

void CharacterBase::Losers()
{
	static float m_jumpAcc = 60.0f;

	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		move = VAdd(m_pos, move);
		static VECTOR Wpos = move;
		// �d��
		m_jumpAcc += -2.0f;
		if (Wpos.y < 300.0f)
		{
			m_jumpAcc = 0.0f;
		}
		Wpos.y += m_jumpAcc;

		m_weapon->Move(Wpos);
	}
}

void CharacterBase::HitPoint()
{
	// �U�����󂯂���
	if (m_isResultDamage)
	{
		// ���݂̍U���ƑO��̍U�����Ⴄ�ꍇ
		if (m_targetBattleState != m_tempTargetBattleState)
		{
			// �̗͂����炷
			m_hp--;

			// �ꎞ�I�ɍŌ�̍U���̎�ނ��擾
			m_tempTargetBattleState = m_targetBattleState;
		}

		// �U���𖳌���
		m_isResultDamage = false;
	}
}

void CharacterBase::KnockBack()
{
	// �K�[�h������������
	// ���Ƀm�b�N�o�b�N
	if (m_isResultGuard)
	{
		// �m�b�N�o�b�N�p�̃x�N�g���͑��Έʒu���猩��Z�}�C�i�X�����Ȃ̂�0.0f�ɂȂ�܂ő����Ă���
		if (m_vecKnockBack < 0.0f)
		{
			m_vecKnockBack += 1.0f;
			VECTOR move = VTransform(VGet(0, 0, m_vecKnockBack), m_targetRotMtx);
			m_pos = VAdd(m_pos, move);
		}
		else
		{
			m_isResultGuard = false;
		}
	}
}

void CharacterBase::UpdatePos(int shiftX, int shiftY, int shiftZ)
{
	// �d�͌�ŏ����̈ʒu��ς��܂�
	if (m_isGravity)
	{
		m_pos.y -= kGravity;
	}
	m_isGravity = true;

	// ����
	{
		VECTOR move = VTransform(m_vecWeapon, m_rotMtx);
		m_weaponPos = VAdd(m_pos, move);
		m_weapon->Move(m_weaponPos);
		m_weapon->Rotate(VGet(90 * DX_PI / 180.0f, m_angle - test2, 0.0f));
		m_weapon->Update();
	}
	// ��
	{
		VECTOR move = VTransform(m_vecShield, m_rotMtx);
		move = VAdd(m_pos, move);
		m_shield->Move(move);
		m_shield->Rotate(VGet(0, m_angle, 0));
		m_shield->Update();
	}	

	// �L�����N�^�[�̈ʒu
	m_my->Move(m_pos);
	m_my->Rotate(VGet(0.0f, m_angle + ((90) * DX_PI_F / 180.0f), 0.0f));
	m_my->Update();

	m_capsuleUpDown = m_pos;
	m_capsuleUpDown.y = m_pos.y + 50.0f;

	m_capsuleUpPos = m_capsuleUpDown;
	m_capsuleUpPos.y = m_capsuleUpDown.y + 150.0f;

	// �m�b�N�o�b�N���ꂽ�ꍇ
	KnockBack();
}

void CharacterBase::WeaponAttacksShield()
{
//	// �ʒu  =  ���Έʒu�@+ �������������W - ���Έʒu�@*�@�@���݂̃t���[���@/�@�ő�t���[���@�@
////	m_vecSield.x = m_parameter.sieldRelativePos.x + (0.0f - m_parameter.sieldRelativePos.x) * (float(m_guardFrame) / m_parameter.guardFrameMax );
//	m_recoilFrame++;
//
//	if (m_recoilFrame < 30)
//	{
//		m_vecWeapon.z = MoveByFrame(m_parameter.weaponRelativePos.z,100.0f, m_recoilFrame, 30);
//		m_vecWeapon.x = MoveByFrame(m_parameter.weaponRelativePos.z,-100.0f, m_recoilFrame, 30);
//	}
//
//
//	if (m_recoilFrame > 30)
//	{
//		m_recoilFrame = 0;
//		m_pFunc = &CharacterBase::Idle;
//	}
//
//	// �ʒu���̍X�V
//	UpdatePos();
}

float CharacterBase::MoveByFrame(const float relativePos, const float EndPos, const int nowFrame, const int maxFrame)
{
	// �ʒu = ���Έʒu + (�������������W - ���Έʒu) * (���݂̃t���[�� / �ő�t���[��)�@�@
	const float resultPos = relativePos + (EndPos - relativePos) * (float(nowFrame) / maxFrame);
	return resultPos;
}

void CharacterBase::SetAngle(float angle)
{
	m_angle = angle;
}

void CharacterBase::SetRotMtx(MATRIX rotMtx)
{
	m_rotMtx = rotMtx;
}

VECTOR CharacterBase::GetPos()const
{
	return m_pos;
}

VECTOR CharacterBase::GetCollPos() const
{
	return VGet(m_pos.x, m_pos.y + 100.0f, m_pos.z);
}

VECTOR CharacterBase::GetCollWeaponPos() const
{
	return m_weapon->GetCollPos(3);
}

VECTOR CharacterBase::GetShieldPos() const
{
	VECTOR move = VTransform(m_vecShield, m_rotMtx);
	move = VAdd(m_pos, move);
	return move;
}

VECTOR CharacterBase::GetCapsulePosDown()
{
	return m_capsuleUpDown;
}

VECTOR CharacterBase::GetCapsulePosUp()
{	
	return m_capsuleUpPos;
}

float CharacterBase::GetCapsuleRadius()
{
	return kCapsuleRadius;
}

MATRIX CharacterBase::GetRot()const
{
	return m_rotMtx;
}

float CharacterBase::GetAngle() const
{
	return m_angle;
}

float CharacterBase::GetModelRadius() const
{
	return m_parameter.modelRadius ;
}

float CharacterBase::GetWeaponAttackRadius() const
{
	return m_parameter.weaponAttackRadius;
}

VECTOR CharacterBase::GetWeaponAttackRelative() const
{
	return m_parameter.weaponAttackPos;
}

float CharacterBase::GetShieldRadius() const
{
	return m_parameter.shieldRadius;
}

VECTOR CharacterBase::GetShieldRelative() const
{
	return m_parameter.shieldRelativePos;
}

void CharacterBase::SetTargetPos(VECTOR pos)
{
	m_targetPos = pos;
}

void CharacterBase::SetTargetHp(const int hp)
{
	m_targetHp = hp;
}

void CharacterBase::SetCollGuardEffect()
{
	// �K�[�h�ۂ̃G�t�F�N�g�Đ�
	EffekseerDrawer::GetInstance().Play(
		m_effectHandle, Id::Guard,
		EffectPlayType::NORMAL,
		GetShieldPos(),
		VGet(1, 1, 1),
		VGet(0, m_angle, 0));
}

void CharacterBase::SetCollJustGuardEffect()
{
	// �W���X�g�K�[�h�����ۂ̃G�t�F�N�g�Đ�
	EffekseerDrawer::GetInstance().Play(
		m_effectHandle, Id::JustGuard,
		EffectPlayType::NORMAL,
		VGet(GetShieldPos().x, GetShieldPos().y + 100.0f, GetShieldPos().z),
		VGet(1, 1, 1),
		VGet(0, 0, 0));
}

int CharacterBase::GetAttackFrame()const
{
	const int totalFrame = m_attackFrame + m_attackGapFrame;
	return totalFrame;
}

int CharacterBase::GetGuardFrame()const
{
	return m_guardFrame;
}

int CharacterBase::GetJustGuardFrame()const
{
	return m_justGuardFrame;
}

int CharacterBase::GetAttackFrameMax()const
{
	if (m_isAttack)
	{
		return m_parameter.attackTotalFrame;
	}

	if (m_isStrongAttack)
	{
		return m_parameter.strongAttackTotalFrame ;
	}

	return -1;
}

int CharacterBase::GetGuardFrameMax()const
{
	return m_parameter.guardFrameMax ;
}

int CharacterBase::GetJustGuardFrameMax()const
{
	return m_parameter.justGuardFrameMax;
}

int CharacterBase::GetStunFrameMax() const
{
	return m_parameter.stunFrameMax;
}

int CharacterBase::GetHp()const
{
	return m_hp;
}

float CharacterBase::GetFightingMeter()const
{
	return m_fightingMeter;
}

bool CharacterBase::IsJustGuard() const
{
	return m_isJustGuard;
}

bool CharacterBase::IsStun() const
{
	return m_isStun;
}

bool CharacterBase::IsAttackRange() const
{
	return m_isAttackRange;
}

void CharacterBase::SetFieldHit()
{
	m_isGravity = false;	
}

void CharacterBase::SetWeaponAttacksShield(const bool isShieldHit)
{
	m_isWeaponAttacksShield = isShieldHit;
}

void CharacterBase::SetBattleState(BattleState state)
{	
	m_targetBattleState = state;
}

void CharacterBase::SetTargetMtxRota(MATRIX rot)
{
	m_targetRotMtx = rot;
}

void CharacterBase::SetAttackRange(const bool isRange)
{
	m_isAttackRange = isRange;
}

void CharacterBase::SetDamage(bool isDamage)
{
	m_isResultDamage = isDamage;
}

void CharacterBase::SetGuardKnockBack(bool isGuard,float vec)
{
	m_isResultGuard = isGuard;
	m_vecKnockBack = vec;
}

void CharacterBase::SetJustGuard(bool isJustGuard)
{
	m_isJustGuard = isJustGuard;
}

void CharacterBase::SetStun(bool isStun)
{
	m_isStun = isStun;
}

void CharacterBase::SetFightingMeter(const float fightingMeter)
{
	m_tempFightingMeter = fightingMeter;

	FightingMeter();
}

void CharacterBase::FightingMeter()
{
	m_fightingMeter += m_tempFightingMeter;

	// �킢�ɕK�v�ȃ��[�^�[���ő�l�ɂȂ�ƍő�l�ŌŒ�
	if (m_fightingMeter > m_parameter.fightingMeterMax)
	{
		m_fightingMeter = m_parameter.fightingMeterMax;
	}
	// �킢�ɕK�v�ȃ��[�^�[���ŏ��l�ɂȂ�ƍŏ��l�ŌŒ�
	if (m_fightingMeter < 0.0f)
	{
		m_isStun = true;
		m_fightingMeter = 0.0f;
	}
}

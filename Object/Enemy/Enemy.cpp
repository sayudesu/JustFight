#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// �p�x�����t���[���P�ʂŒx�点��
	constexpr int kDelayFrameAngle = 18;
	constexpr int kGuardFrameCountMax = 60 * 2;
}

namespace
{
	// �v���C���[�̈ړ���
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// ���
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}


Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos),
	m_isCheckGuard(false),
	m_isResultGuard(false),
	m_isCheckAttack(false),
	m_isAttackResult(false),
	m_isCheckStrongAttack(false),
	m_isStrongAttackResult(false),
	m_isMoveLeft(false),
	m_isMoveRight(false)
{
	m_pFunc = &Enemy::Idle;

	m_isAttack = false;
	m_isGuard = false;

	// ���g���G�l�~�[�ł���ƌ��߂�
	m_myId = CharacterName::ENEMYNORMAL;

	m_parameter.fileName = "Data/Model/Knight_B.mv1";
	// �p�����[�^�[����
	m_parameter.attackFrameMax = 30;
	m_parameter.attackFrameGapMax = 0;
	m_parameter.attackTotalFrame = m_parameter.attackFrameMax + m_parameter.attackFrameGapMax;

	m_parameter.attackAfterStopFrameMax = 60;

	m_parameter.strongAttackFrameMax = 5;
	m_parameter.strongAttackFrameGapMax = 60;
	m_parameter.strongAttackTotalFrame = m_parameter.strongAttackFrameMax + m_parameter.strongAttackFrameGapMax;

	m_parameter.guardFrameMax = 20;
	m_parameter.justGuardFrameMax = 15;

	m_parameter.stunFrameMax = 60 * 3;

	m_parameter.hpMax = 6;
	m_parameter.fightingMeterMax = 100.0f;

	m_parameter.weaponRelativePos = { -80.0f, 100.0f, 0.0f };
	m_parameter.shieldRelativePos = { 100.0f, 100.0f, -50.0f };

	m_parameter.weaponAttackRadius = 100.0f;
	m_parameter.shieldRadius = 50.0f;
	m_parameter.modelRadius = 180.0f;

	m_parameter.weaponAttackPos = { 0.0f, 0.0f, -210.0f };
	m_parameter.knockBackPos = { 0.0f,0.0f ,-20.0f };

	m_parameter.weaponBackSpeed = 30.0f;
	m_parameter.shieldBackSpeed = 30.0f;
}

Enemy::~Enemy()
{
}

//void Enemy::Input()
//{
//	DINPUT_JOYSTATE input;
//	// ���͏�Ԃ��擾
//	GetJoypadDirectInputState(DX_INPUT_PAD2, &input);
//
//	if (m_isCameraLockon)
//	{
//		// �J�����̉�]�p�x�𒲐�
//		if (input.Rx > 30)
//		{
//			m_angle += 0.05f;
//		}
//		if (input.Rx < -30)
//		{
//			m_angle -= 0.05f;
//		}
//	}
//	else
//	{
//		const VECTOR direction = VSub(m_targetPos, m_pos);
//		m_angle = atan2f(-direction.x, -direction.z);
//	}
//
//	// angle�����N���X�ɓn��
//	SetAngle(m_angle);
//	// �v���C���[�̐i�s����
//	MATRIX rotMtx = MGetRotY(m_angle);
//	// ��]�s������N���X�ɓn��
//	SetRotMtx(rotMtx);
//	if (!IsStun())
//	{
//		// �ړ�or���
//		if (m_isAway)
//		{
//			static VECTOR away = kVecAwayZ;
//			static int frameCount = 0;
//			int frameCountMax = 10;
//
//			if (frameCount < frameCountMax)
//			{
//				m_awayVec.x = (m_awayRelativePos.x) * (float(frameCount) / frameCountMax);
//				m_awayVec.z = (m_awayRelativePos.z) * (float(frameCount) / frameCountMax);
//				frameCount++;
//				VECTOR move = VTransform(m_awayVec, rotMtx);
//				m_pos = VAdd(m_pos, move);
//			}
//			else
//			{
//
//				m_isAway = false;
//				frameCount = 0;
//			}
//		}
//		else
//		{
//			m_isUp = false;
//			m_isDown = false;
//			m_isRight = false;
//			m_isLeft = false;
//
//			if (Pad::IsPress(PAD_INPUT_UP,1))
//			{
//				m_isUp = true;
//
//				m_pos = AddMoving(kVecZ, rotMtx, m_pos);
//
//				MoveAway(0.0f, -60.0f, rotMtx);
//			}
//			else if (Pad::IsPress(PAD_INPUT_DOWN, 1))
//			{
//				m_isDown = true;
//
//				m_pos = SubMoving(kVecZ, rotMtx, m_pos);
//
//				MoveAway(0.0f, 60.0f, rotMtx);
//			}
//			if (Pad::IsPress(PAD_INPUT_RIGHT, 1))
//			{
//				m_isRight = true;
//
//				m_pos = AddMoving(kVecX, rotMtx, m_pos);
//
//				MoveAway(-60.0f, 0.0f, rotMtx);
//			}
//			else if (Pad::IsPress(PAD_INPUT_LEFT, 1))
//			{
//				m_isLeft = true;
//
//				m_pos = SubMoving(kVecX, rotMtx, m_pos);
//
//				MoveAway(60.0f, 0.0f, rotMtx);
//			}
//
//			if ((!(m_isUp) && !(m_isDown) && !(m_isLeft) && !(m_isRight)))
//			{
//				MoveAway(0.0f, 60.0f, rotMtx);
//			}
//		}
//
//		// �U��or�h��
//		{
//			// �ʏ�U��
//			if (Pad::IsTrigger(PAD_INPUT_6, 1) &&
//				!m_isAttack &&
//				!m_isStrongAttack &&
//				!m_isGuard)
//			{
//				m_isAttack = true;
//				m_comboAttack++;
//				if (m_comboAttack == 1)
//				{
//					m_pFunc = &Enemy::Attack;
//				}
//			}
//
//			// ���U��
//			if (input.Z < -100 &&
//				!m_isAttack &&
//				!m_isStrongAttack &&
//				!m_isGuard)
//			{
//				m_isStrongAttack = true;
//				m_pFunc = &Enemy::StrongAttack;
//			}
//
//			// �h��
//			if (Pad::IsPress(PAD_INPUT_5, 1) &&
//				!m_isAttack &&
//				!m_isStrongAttack)
//			{
//				m_isGuard = true;
//				m_pFunc = &Enemy::Guard;
//			}
//			else
//			{
//				m_isGuard = false;
//			}
//		}
//	}
//
//	{
//		// �J�����̑���ύX
//		static int frameCount = 0;
//		if (input.Buttons[9] == 128)
//		{
//			frameCount++;
//			if (frameCount == 1)
//			{
//				m_isCameraLockon = (!m_isCameraLockon);
//			}
//		}
//		else
//		{
//			frameCount = 0;
//		}
//	}
//
//
//	if (Pad::IsTrigger(PAD_INPUT_4, 1))
//	{
//		//SetStun(true);
//		SetFightingMeter(-10000.0f);
//	}
//}
//
//VECTOR Enemy::AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
//{
//	VECTOR move = VTransform(RelativePos, rotMtx);
//	move = VAdd(pos, move);
//	return move;
//}
//
//VECTOR Enemy::SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
//{
//	VECTOR move = VTransform(RelativePos, rotMtx);
//	move = VSub(m_pos, move);
//	return move;
//}
//
//void Enemy::MoveAway(float x, float z, MATRIX rotMtx)
//{
//	// ����̉�����
//	{
//		if (Pad::IsTrigger(PAD_INPUT_3, 1))
//		{
//			m_isAway = true;
//			m_awayRelativePos.x = x;
//			m_awayRelativePos.z = z;
//			m_awayVec.x = 0.0f;
//			m_awayVec.y = 0.0f;
//			m_awayVec.z = 0.0f;
//		}
//	}
//}

void Enemy::Input()
{
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	const VECTOR direction = VSub(m_targetPos, m_pos);

	const float angle = atan2f(-direction.x, -direction.z);

	m_delayFrameAngle.push_front(angle);
	if (m_delayFrameAngle.size() > kDelayFrameAngle)
	{
		m_delayFrameAngle.pop_back();
	}

	SetAngle(m_delayFrameAngle.back());

	// �v���C���[�̐i�s����
	MATRIX rotMtx = MGetRotY(m_delayFrameAngle.back());

	SetRotMtx(rotMtx);
#if true	
	if (!IsStun())
	{
		// ��苗���߂Â��ƃ����_���ō��E�ړ����n�߂�
		if (m_targetRange.x + m_targetRange.z < 1000.0f)
		{
			MoveLeftAndRight(rotMtx);
			//m_isGuard = false;
		}

		// �U���\�͈͂ɓ����Ă��邩�ǂ���
		if (IsAttackRange())
		{
			static int guardFrameCount = 0;		

			// �U��or�h�䂵�Ă��Ȃ������玟�̍s�������߂�
			if (!m_isAttack && !m_isGuard && !m_isStrongAttack)
			{
				// �^�[�Q�b�g�̏�Ԃœ����ύX����
				BattleType();
			}

			// �U�����ǂ���
			if (m_isAttackResult)
			{
				m_isAttackResult = false;
				m_isAttack = true;
				m_pFunc = &Enemy::Attack;
			}
			// �U�������Ă��Ȃ��ꍇ
			if (!m_isAttack)
			{
				m_isAttack = false;
			}

			// ���U�����ǂ���
			if (m_isStrongAttackResult && !m_isGuard)
			{
				m_isStrongAttackResult = false;
				m_isCheckStrongAttack = true;
				m_pFunc = &Enemy::StrongAttack;
			}
			// ���U�������Ă��Ȃ��ꍇ
			if (!m_isCheckStrongAttack)
			{
				m_isCheckStrongAttack = false;
			}

			if (m_isResultGuard && !m_isAttack)
			{
				m_isGuard = true;
				guardFrameCount++;
				m_pFunc = &Enemy::Guard;		
			}
			if (guardFrameCount == kGuardFrameCountMax)
			{
				guardFrameCount = 0;
				m_isCheckGuard = false;
				m_isGuard = false;
				m_isResultGuard = false;
			}

		}
		else
		{
			TargetMove();
		}
	}
	else
	{
		m_isGuard = false;
	}
#endif
}

void Enemy::MoveLeftAndRight(MATRIX mtxRot)
{
	if (GetRand(100) == 0)
	{
		m_isMoveRight = true;
		m_isMoveLeft = false;
	}
	if (GetRand(100) == 1)
	{
		m_isMoveLeft = true;
		m_isMoveRight = false;
	}
	if (GetRand(100) == 2)
	{
		m_isMoveLeft = false;
		m_isMoveRight = false;
	}
	int moveType = 0;
	if (m_isMoveLeft)
	{
		moveType = 1;
	}
	if (m_isMoveRight)
	{
		moveType = -1;
	}

	const VECTOR move = VTransform(VGet(moveType * 10, 0, 0), mtxRot);
	m_pos = VAdd(m_pos, move);
}

void Enemy::BattleType()
{
	// �^�[�Q�b�g�̏�Ԃœ����ύX����
	// �^�[�Q�b�g���U�����Ă���ꍇ
	if (m_targetBattleState == BattleState::ATTACK)
	{
		m_isCheckGuard = true;
		m_isResultGuard = true;
	}
	// �^�[�Q�b�g���X�^�����Ă���ꍇ
	if (m_targetBattleState == BattleState::STUN)
	{
		m_isCheckStrongAttack = true;
		m_isStrongAttackResult = true;
	}
	// �^�[�Q�b�g���h�䂵�Ă���ꍇ
	if (m_targetBattleState == BattleState::GUARD)
	{
		m_isCheckAttack = true;
		m_isAttackResult = true;
	}
	// �A�C�h����Ԃ̏ꍇ
	if (m_targetBattleState == BattleState::IDLE)
	{
		m_isCheckAttack = true;
		m_isAttackResult = true;
	}
}


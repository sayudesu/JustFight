#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// �p�x�����t���[���P�ʂŒx�点��
	constexpr int kDelayFrameAngle = 18;
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

	// �p�����[�^�[����
	m_parameter.attackFrameMax = 5 * 9;
	m_parameter.attackFrameGapMax = 30;
	m_parameter.attackRotalFrame = m_parameter.attackFrameMax + m_parameter.attackFrameGapMax;

	m_parameter.attackAfterStopFrameMax = 60;

	m_parameter.strongAttackFrameMax = 5;
	m_parameter.strongAttackFrameGapMax = 60;
	m_parameter.strongAttackRotalFrame = m_parameter.strongAttackFrameMax + m_parameter.strongAttackFrameGapMax;

	m_parameter.guardFrameMax = 20;
	m_parameter.justGuardFrameMax = 15;

	m_parameter.stunFrameMax = 60 * 3;

	m_parameter.hpMax = 6;
	m_parameter.fightingMeterMax = 100.0f;

	m_parameter.weaponRelativePos = { -80.0f, 100.0f, 0.0f };
	m_parameter.sieldRelativePos = { 100.0f, 100.0f, -50.0f };

	m_parameter.weaponAttackRadius = 100.0f;
	m_parameter.sieldRadius = 50.0f;
	m_parameter.modelRadius = 180.0f;

	m_parameter.weaponAttackPos = { 0.0f, 0.0f, -210.0f };
	m_parameter.knockBackPos = { 0.0f,0.0f ,-20.0f };

	m_parameter.weaponBackSpeed = 30.0f;
	m_parameter.sieldBackSpeed = 30.0f;
}

Enemy::~Enemy()
{
}

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
			const int guardFrame = 60;			

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


			if (guardFrameCount == guardFrame)
			{
				guardFrameCount = 0;
				m_isCheckGuard = false;
				m_isGuard = false;
				m_isResultGuard = false;
			}
			if (m_isResultGuard && !m_isAttack)
			{
				m_isGuard = true;
				guardFrameCount++;
				m_pFunc = &Enemy::Guard;
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


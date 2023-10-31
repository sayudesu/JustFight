#include "Enemy.h"
#include "../../Util/Pad.h"

namespace
{
	// �p�x�����t���[���P�ʂŒx�点��
	constexpr int kDelayFrameAngle = 18;
}

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos)
{
	m_pFunc = &Enemy::Idle;

	m_isAttack = false;
	m_isGuard = false;

	// ���g���G�l�~�[�ł���ƌ��߂�
	m_myId = CharacterName::ENEMY;
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
			int moveType = 0;
			static bool isLeft = false;
			static bool isRight = false;
			if (GetRand(100) == 0)
			{
				isRight = true;
				isLeft = false;
			}
			if (GetRand(100) == 1)
			{
				isLeft = true;
				isRight = false;
			}
			if (GetRand(100) == 2)
			{
				isLeft = false;
				isRight = false;
			}
			if (isLeft)
			{
				moveType = 1;
			}
			if (isRight)
			{
				moveType = -1;
			}
			const VECTOR move = VTransform(VGet(moveType * 10, 0, 0), rotMtx);
			m_pos = VAdd(m_pos, move);
		}

		if (GetStamina() > 20.0f)
		{
			if (IsAttackRange())
			{
				static bool isAttack = false;
				static bool isAttackResult = false;

				static bool isStrongAttack = false;
				static bool isStrongAttackResult = false;

				static bool isGuard  = false;
				static bool isGuardResult = false;

				static int guardFrameCount = 0;
				const int guardFrame = 60;

				// �����ōU�����邩�����߂�
				if (!isAttack &&
					!m_isAttack &&
					!m_isStrongAttack &&
					!m_isGuard)
				{
					if (GetRand(30) == 0)
					{
						isAttack = true;
						isAttackResult = true;
					}
				}

				// �����ŋ��U�����邩�����߂�
				if (!isStrongAttack &&
					!m_isAttack &&
					!m_isStrongAttack &&
					!m_isGuard)
				{
					if (GetRand(90) == 0)
					{
						isStrongAttack = true;
						isStrongAttackResult = true;
					}
				}

				// �����Ŗh�䂷�邩�����߂�
				if (!isGuard &&
					!m_isAttack &&
					!m_isStrongAttack)
				{
					if (GetRand(30) == 0)
					{
						isGuard = true;
						isGuardResult = true;
					}
				}

				// �U�����ǂ���
				if (isAttackResult)
				{
					isAttackResult = false;
					m_isAttack = true;
					m_attackId = AttackData::NORMAL;
					m_pFunc = &Enemy::Attack;
				}
				// �U�������Ă��Ȃ��ꍇ
				if (!m_isAttack)
				{
					isAttack = false;
				}

				// ���U�����ǂ���
				if (isStrongAttackResult && !m_isGuard)
				{
					isStrongAttackResult = false;
					m_isStrongAttack = true;
					m_attackId = AttackData::STRONG;
					m_pFunc = &Enemy::StrongAttack;
				}
				// ���U�������Ă��Ȃ��ꍇ
				if (!m_isStrongAttack)
				{
					isStrongAttack = false;
				}


				if (guardFrameCount == guardFrame)
				{
					guardFrameCount = 0;
					isGuard = false;
					m_isGuard = false;
					isGuardResult = false;
				}
				if (isGuardResult && !m_isAttack)
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
}


#include <corecrt_math.h>

#include "Player.h"

#include "../../Util/Pad.h"

#include "../../CSVData/SoundManager.h"
#include "../../Util/SoundName.h"


namespace
{
	// �v���C���[�̈ړ���
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// ���
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}

Player::Player(DifficultyData data,VECTOR pos):
	CharacterBase(data,pos),
	m_awayVec(VGet(0,0,0)),
	m_awayRelativePos(VGet(0,0,0)),
	m_isUp(false),
	m_isDown(false),
	m_isLeft(false),
	m_isRight(false),
	m_isCameraLockon(false),
	m_isMove(false)
{
	// �����ҋ@��ԂŒ�~
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;

	m_isAway = false;

	m_angle = 0.0f;

	// ���g���v���C���[�ł���ƌ��߂�
	m_myId = CharacterName::PLAYER;

	for (int i = 0; i < static_cast<int>(Tips::MAX); i++)
	{
		m_isTipsMove[i] = false;
	}
	m_isTipsMove[static_cast<int>(Tips::MOVE)] = true;

	m_parameter.fileName = "Data/Model/Knight_W.mv1";
	// �p�����[�^�[����
	m_parameter.attackFrameMax = 20;
	m_parameter.attackFrameGapMax = 0;
	m_parameter.attackTotalFrame = m_parameter.attackFrameMax + m_parameter.attackFrameGapMax;

	m_parameter.attackAfterStopFrameMax = 60;

	m_parameter.strongAttackFrameMax = 60;
	m_parameter.strongAttackFrameGapMax = 25;
	m_parameter.strongAttackTotalFrame = m_parameter.strongAttackFrameMax + m_parameter.strongAttackFrameGapMax;

	m_parameter.guardFrameMax = 5;
	m_parameter.justGuardFrameMax = 5;

	m_parameter.stunFrameMax = 60 * 3;

	m_parameter.hpMax = 6;
	m_parameter.fightingMeterMax = 100;

	m_parameter.weaponRelativePos = { -80.0f, 100.0f, 0.0f };
	m_parameter.shieldRelativePos = { 100.0f, 100.0f, 10.0f };

	m_parameter.weaponAttackRadius = 100.0f;
	m_parameter.shieldRadius = 50.0f;
	m_parameter.modelRadius = 180.0f;

	m_parameter.weaponAttackPos = { 0.0f, 0.0f, -210.0f };
	m_parameter.knockBackPos = { 0.0f,0.0f ,-20.0f };

	m_parameter.weaponBackSpeed = 30.0f;
	m_parameter.shieldBackSpeed = 30.0f;
}

Player::~Player()
{
}

void Player::Input()
{
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_padInput);

	// �������w��
	Direction();

	// �X�^�����Ă��Ȃ�������
	if (!IsStun())
	{
		// �ړ�
		InputMove();
		// �U��
		InputAttack();
		// �h��
		InputGuard();
	}

#if _DEBUG
	{
		// �J�����̑���ύX
		static int frameCount = 0;
		if (m_padInput.Buttons[9] == 128)
		{
			frameCount++;
			if (frameCount == 1)
			{
				m_isCameraLockon = (!m_isCameraLockon);
			}
		}
		else
		{
			frameCount = 0;
		}
	}
#endif
}

void Player::InputTutorial()
{
	// �������w��
	Direction();

	// �G�Ƃ̋����𑪂�
	m_targetRange.x = sqrt(pow(m_pos.x - m_targetPos.x, 2.0f) + pow(m_pos.x - m_targetPos.x, 2.0f));
	m_targetRange.z = sqrt(pow(m_pos.z - m_targetPos.z, 2.0f) + pow(m_pos.z - m_targetPos.z, 2.0f));

	if (m_isMove)
	{
		m_isTipsMove[static_cast<int>(Tips::ATTACK)] = true;
	}

	// �����邩�ǂ���
	if (m_isTipsMove[static_cast<int>(Tips::MOVE)])
	{
		InputMove();
	}

	// �U�����ł��邩�ǂ���
	if (m_isTipsMove[static_cast<int>(Tips::ATTACK)])
	{
		// �U���ł���͈͂ɂ��邩�ǂ���
		if (m_targetRange.x + m_targetRange.z < 500.0f)
		{
			InputAttack();
			m_isTipsMove[static_cast<int>(Tips::GUARD)] = true;
		}
	}

	// �K�[�h�ł��邩�ǂ���
	if (m_isTipsMove[static_cast<int>(Tips::GUARD)])
	{
		InputGuard();
	}

}

void Player::Direction()
{
	if (m_isCameraLockon)
	{
		// �J�����̉�]�p�x�𒲐�
		if (m_padInput.Rx > 30)
		{
			m_angle += 0.05f;
		}
		if (m_padInput.Rx < -30)
		{
			m_angle -= 0.05f;
		}
	}
	else
	{
		const VECTOR direction = VSub(m_targetPos, m_pos);
		m_angle = atan2f(-direction.x, -direction.z);
	}
	// angle�����N���X�ɓn��
	SetAngle(m_angle);
	// �v���C���[�̐i�s����
	m_platerRotMtx = MGetRotY(m_angle);
	// ��]�s������N���X�ɓn��
	SetRotMtx(m_platerRotMtx);
}

void Player::InputMove()
{
	m_isMove = false;
	// �ړ�or���
	if (m_isAway)
	{
		static VECTOR away = kVecAwayZ;
		static int frameCount = 0;
		int frameCountMax = 10;

		if (frameCount < frameCountMax)
		{
			m_awayVec.x = (m_awayRelativePos.x) * (float(frameCount) / frameCountMax);
			m_awayVec.z = (m_awayRelativePos.z) * (float(frameCount) / frameCountMax);
			frameCount++;
			VECTOR move = VTransform(m_awayVec, m_platerRotMtx);
			m_pos = VAdd(m_pos, move);
		}
		else
		{

			m_isAway = false;
			frameCount = 0;
		}
	}
	else
	{
		m_isUp = false;
		m_isDown = false;
		m_isRight = false;
		m_isLeft = false;

		if (Pad::IsPress(PAD_INPUT_UP))
		{
			m_isUp = true;
			m_isMove = true;

			m_pos = AddMoving(kVecZ, m_platerRotMtx, m_pos);

			MoveAway(0.0f, -60.0f, m_platerRotMtx);
		}
		else if (Pad::IsPress(PAD_INPUT_DOWN))
		{
			m_isDown = true;
			m_isMove = true;

			m_pos = SubMoving(kVecZ, m_platerRotMtx, m_pos);

			MoveAway(0.0f, 60.0f, m_platerRotMtx);
		}
		if (Pad::IsPress(PAD_INPUT_RIGHT))
		{
			m_isRight = true;
			m_isMove = true;

			m_pos = AddMoving(kVecX, m_platerRotMtx, m_pos);

			MoveAway(-60.0f, 0.0f, m_platerRotMtx);
		}
		else if (Pad::IsPress(PAD_INPUT_LEFT))
		{
			m_isLeft = true;
			m_isMove = true;

			m_pos = SubMoving(kVecX, m_platerRotMtx, m_pos);

			MoveAway(60.0f, 0.0f, m_platerRotMtx);
		}

		if ((!(m_isUp) && !(m_isDown) && !(m_isLeft) && !(m_isRight)))
		{
			MoveAway(0.0f, 60.0f, m_platerRotMtx);
		}
	}
}

void Player::InputAttack()
{
	// �ʏ�U��
	if (Pad::IsTrigger(PAD_INPUT_6) &&
		!m_isAttack &&
		!m_isStrongAttack &&
		!m_isGuard &&
		!m_isJustGuard)
	{
		m_isAttack = true;
		m_comboAttack++;
		if (m_comboAttack == 1)
		{
			m_pFunc = &Player::Attack;
		}
	}

	// ���U��
	if (m_padInput.Z < -100 &&
		!m_isAttack &&
		!m_isStrongAttack &&
		!m_isGuard &&
		!m_isJustGuard &&
		(GetStrongPower() >= 100.0f))
	{
		m_isStrongAttack = true;
		m_pFunc = &Player::StrongAttack;
	}
}

void Player::InputGuard()
{
	// �h��
	if (Pad::IsPress(PAD_INPUT_5))
	{
		if (!m_isAttack && !m_isStrongAttack && !m_isJustGuard && !m_isJustGuard)
		{
			// 
			//SetStrongPowerReset();
			m_isGuard = true;
			m_pFunc = &Player::Guard;
		}
	}
	else
	{
		m_isGuard = false;
	}
}


VECTOR Player::AddMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
{
	VECTOR move = VTransform(RelativePos, rotMtx);
	move = VAdd(pos, move);
	return move;
}

VECTOR Player::SubMoving(const VECTOR RelativePos, const MATRIX rotMtx, const VECTOR pos)
{
	VECTOR move = VTransform(RelativePos, rotMtx);
	move = VSub(m_pos, move);
	return move;
}

void Player::MoveAway(float x, float z, MATRIX rotMtx)
{
	// ����̉�����
	{
		if (Pad::IsTrigger(PAD_INPUT_3))
		{
			SoundManager::GetInstance().Play(SoundName::SPEEDMOVE);
			m_isAway = true;
			m_awayRelativePos.x = x;
			m_awayRelativePos.z = z;
			m_awayVec.x = 0.0f;
			m_awayVec.y = 0.0f;
			m_awayVec.z = 0.0f;
		}
	}
}

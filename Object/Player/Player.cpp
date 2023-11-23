#include "Player.h"
#include "../../Util/Pad.h"
#include <corecrt_math.h>

namespace
{
	// �v���C���[�̈ړ���
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// ���
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}

Player::Player(VECTOR pos):
	CharacterBase(pos),
	m_awayVec(VGet(0,0,0)),
	m_awayRelativePos(VGet(0,0,0)),
	m_isUp(false),
	m_isDown(false),
	m_isLeft(false),
	m_isRight(false),
	m_isCameraLockon(false)
{
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;

	m_isAway = false;

	m_angle = 0.0f;

	// ���g���v���C���[�ł���ƌ��߂�
	m_myId = CharacterName::PLAYER;

	m_parameter.fileName = "Data/Model/Knight_W.mv1";
	// �p�����[�^�[����
	m_parameter.attackFrameMax = 20;
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

Player::~Player()
{
}

void Player::Input()
{
	if (CheckHitKey(KEY_INPUT_N))
	{
		test2 += 0.1f;
	}
	if (CheckHitKey(KEY_INPUT_M))
	{
		test2 -= 0.1f;
	}

	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	if (m_isCameraLockon)
	{
		// �J�����̉�]�p�x�𒲐�
		if (input.Rx > 30)
		{
			m_angle += 0.05f;
		}
		if (input.Rx < -30)
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
	MATRIX rotMtx = MGetRotY(m_angle);
	// ��]�s������N���X�ɓn��
	SetRotMtx(rotMtx);
	if (!IsStun())
	{
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
				VECTOR move = VTransform(m_awayVec, rotMtx);
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

				m_pos = AddMoving(kVecZ, rotMtx, m_pos);

				MoveAway(0.0f, -60.0f, rotMtx);
			}
			else if (Pad::IsPress(PAD_INPUT_DOWN))
			{
				m_isDown = true;

				m_pos = SubMoving(kVecZ, rotMtx, m_pos);

				MoveAway(0.0f, 60.0f, rotMtx);
			}
			if (Pad::IsPress(PAD_INPUT_RIGHT))
			{
				m_isRight = true;

				m_pos = AddMoving(kVecX, rotMtx, m_pos);

				MoveAway(-60.0f, 0.0f, rotMtx);
			}
			else if (Pad::IsPress(PAD_INPUT_LEFT))
			{
				m_isLeft = true;

				m_pos = SubMoving(kVecX, rotMtx, m_pos);

				MoveAway(60.0f, 0.0f, rotMtx);
			}

			if ((!(m_isUp) && !(m_isDown) && !(m_isLeft) && !(m_isRight)))
			{
				MoveAway(0.0f, 60.0f, rotMtx);
			}
		}

		// �U��or�h��
		{
			// �ʏ�U��
			if (Pad::IsTrigger(PAD_INPUT_6) &&
				!m_isAttack                 &&
				!m_isStrongAttack           &&
				!m_isGuard)
			{
				m_isAttack = true;
				m_comboAttack++;
				if (m_comboAttack == 1)
				{
					m_pFunc = &Player::Attack;
				}
			}

			// ���U��
			if (input.Z < -100 &&
				!m_isAttack &&
				!m_isStrongAttack &&
				!m_isGuard)
			{
				m_isStrongAttack = true;
				m_pFunc = &Player::StrongAttack;
			}

			// �h��
			if (Pad::IsPress(PAD_INPUT_5) &&
				!m_isAttack               &&
				!m_isStrongAttack)
			{
				m_isGuard = true;
				m_pFunc = &Player::Guard;
			}
			else
			{
				m_isGuard = false;
			}
		}
	}

	{
		// �J�����̑���ύX
		static int frameCount = 0;
		if (input.Buttons[9] == 128)
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


	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		//SetStun(true);
		SetFightingMeter(-10000.0f);
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
			m_isAway = true;
			m_awayRelativePos.x = x;
			m_awayRelativePos.z = z;
			m_awayVec.x = 0.0f;
			m_awayVec.y = 0.0f;
			m_awayVec.z = 0.0f;
		}
	}
}

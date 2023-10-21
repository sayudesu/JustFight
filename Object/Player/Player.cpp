#include "Player.h"
#include "../../Util/Pad.h"

namespace
{
	// �v���C���[�̈ړ���
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };
}

Player::Player(VECTOR pos):
	CharacterBase(pos)
{
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;

	m_angle = 0.0f;
}

Player::~Player()
{
}

void Player::Input()
{
	DINPUT_JOYSTATE input;
	// ���͏�Ԃ��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// �J�����̉�]�p�x�𒲐�
	if (input.Rx > 30)
	{
		m_angle += 0.05f;
	}
	if (input.Rx < -30)
	{
		m_angle -= 0.05f;
	}

	SetAngle(m_angle);

	// �v���C���[�̐i�s����
	MATRIX rotMtx = MGetRotY(m_angle);
	if (Pad::isPress(PAD_INPUT_UP))
	{
		VECTOR move = VTransform(kVecZ, rotMtx);
		m_pos = VAdd(m_pos, move);
	}
	if (Pad::isPress(PAD_INPUT_DOWN))
	{
		VECTOR move = VTransform(kVecZ, rotMtx);
		m_pos = VSub(m_pos, move);
	}
	if (Pad::isPress(PAD_INPUT_RIGHT))
	{
		VECTOR move = VTransform(kVecX, rotMtx);
		m_pos = VAdd(m_pos, move);
	}
	if (Pad::isPress(PAD_INPUT_LEFT))
	{
		VECTOR move = VTransform(kVecX, rotMtx);
		m_pos = VSub(m_pos, move);
	}
	SetRotMtx(rotMtx);

	if (!m_isJustGuard)
	{
		if (Pad::isTrigger(PAD_INPUT_6) && !m_isGuard)
		{
			m_isAttack = true;
			m_pFunc = &Player::Attack;
		}

		if (Pad::isPress(PAD_INPUT_5) && !m_isAttack)
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

float Player::GetAngle()const
{
	return m_angle;
}

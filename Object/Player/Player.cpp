#include "Player.h"
#include "../../Util/Pad.h"

namespace
{
	// プレイヤーの移動量
	constexpr VECTOR kVecZ{ 0.0f,0.0f,-10.0f };
	constexpr VECTOR kVecX{ -10.0f,0.0f,0.0f };

	// 回避
	constexpr VECTOR kVecAwayZ{ 0.0f,0.0f,-25.0f };
}

Player::Player(VECTOR pos):
	CharacterBase(pos),
	m_awayVec(VGet(0,0,0)),
	m_awayRelativePos(VGet(0,0,0)),
	m_isAway(false),
	m_isUp(false),
	m_isDown(false),
	m_isLeft(false),
	m_isRight(false)
{
	m_pFunc = &Player::Idle;

	m_isAttack = false;
	m_isGuard  = false;
	m_isJustGuard = false;
	m_isJustGuardBreak = false;

	m_angle = 0.0f;
}

Player::~Player()
{
}

void Player::Input()
{
	DINPUT_JOYSTATE input;
	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//// カメラの回転角度を調整
	//if (input.Rx > 30)
	//{
	//	m_angle += 0.05f;
	//}
	//if (input.Rx < -30)
	//{
	//	m_angle -= 0.05f;
	//}

	//SetAngle(m_angle);

	//// プレイヤーの進行方向
	//MATRIX rotMtx = MGetRotY(m_angle);

	//SetRotMtx(rotMtx);

	const VECTOR direction = VSub(m_targetPos, m_pos);
	const float angle = atan2f(-direction.x, -direction.z);

	SetAngle(angle);

	// プレイヤーの進行方向
	MATRIX rotMtx = MGetRotY(angle);

	SetRotMtx(rotMtx);

	if (m_isAway)
	{
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
	}
	else
	{
		m_isUp = false;
		m_isDown = false;
		m_isRight = false;
		m_isLeft = false;

		if (Pad::isPress(PAD_INPUT_UP))
		{
			m_isUp = true;

			m_pos = AddMoving(kVecZ, rotMtx, m_pos);

			MoveAway(0.0f, -60.0f, rotMtx);
		}
		else if (Pad::isPress(PAD_INPUT_DOWN))
		{
			m_isDown = true;

			m_pos = SubMoving(kVecZ, rotMtx, m_pos);

			MoveAway(0.0f, 60.0f, rotMtx);
		}
		if (Pad::isPress(PAD_INPUT_RIGHT))
		{
			m_isRight = true;

			m_pos = AddMoving(kVecX, rotMtx, m_pos);

			MoveAway(-60.0f, 0.0f, rotMtx);
		}
		else if (Pad::isPress(PAD_INPUT_LEFT))
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

	if (!m_isJustGuardBreak)
	{
		//if (GetStamina() > 20.0f)
		{
			if (Pad::isTrigger(PAD_INPUT_6) &&
				!m_isAttack &&
				!m_isGuard)
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
		//else
		//{
		//	m_isGuard = false;
		//}
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
	// 回避の仮実装
	{
		if (Pad::isTrigger(PAD_INPUT_3))
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

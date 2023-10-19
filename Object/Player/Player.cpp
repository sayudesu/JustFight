#include "Player.h"
#include "../../Util/Pad.h"

Player::Player(VECTOR pos):
	CharacterBase(pos)
{
	m_pFunc = &CharacterBase::Idle;

	m_isAttack = false;
	m_isGuard  = false;
}

Player::~Player()
{
}

void Player::Input()
{
	if (Pad::isTrigger(PAD_INPUT_6) && !m_isGuard)
	{
		m_isAttack = true;
		m_pFunc = &CharacterBase::Attack;
	}

	if (Pad::isPress(PAD_INPUT_5) && !m_isAttack)
	{
		m_isGuard = true;
		m_pFunc = &CharacterBase::Guard;
	}
	else
	{
		m_isGuard = false;
	}
}

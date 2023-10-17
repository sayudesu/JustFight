#include "Player.h"
#include "../../Util/Pad.h"

Player::Player(VECTOR pos):
	CharacterBase(pos)
{
	m_pFunc = &CharacterBase::UpdateInput;
}

Player::~Player()
{
}

void Player::UpdateInput()
{
	if (Pad::isTrigger(PAD_INPUT_6))
	{
		printfDx("Attack\n");
	}
	if (Pad::isPress(PAD_INPUT_5))
	{
		printfDx("Guard\n");
	}

}

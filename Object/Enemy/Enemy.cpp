#include "Enemy.h"
#include "../../Util/Pad.h"

Enemy::Enemy(VECTOR pos) :
	CharacterBase(pos)
{

}

Enemy::~Enemy()
{
}

void Enemy::Input()
{

	if (GetRand(100) == 0)
	{
		m_isAttack = true;
		m_pFunc = &CharacterBase::Attack;
	}

	//if (Pad::isPress(PAD_INPUT_3) && !m_isAttack)
	//{
	//	m_isGuard = true;
	//	m_pFunc = &CharacterBase::Guard;
	//}
	//else
	//{
	//	m_isGuard = false;
	//}
}
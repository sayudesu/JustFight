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
//	SetPlayerOnlyAngle(m_angle);
//	MATRIX rotMtx = MGetRotY(m_angle);
//	SetRotMtx(rotMtx);
	if (GetRand(100) == 0)
	{
		m_isAttack = true;
		m_pFunc = &Enemy::Attack;
	}
	//if (GetRand(50) == 0)
	//{
	//	m_isGuard = true;
	//	m_pFunc = &CharacterBase::Guard;
	//}
	//else
	//{
	//	m_isGuard = false;
	//}
}
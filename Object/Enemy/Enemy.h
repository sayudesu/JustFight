#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Enemy : public CharacterBase
{
public:
	Enemy(VECTOR pos);
	virtual ~Enemy();
	void Input()override;

	void SetTargetPos(VECTOR pos);
private:
	float m_angle = 0.0f;
	VECTOR m_targetPos = VGet(0, 0, 0);
};


#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Enemy : public CharacterBase
{
public:
	Enemy(VECTOR pos);
	virtual ~Enemy();
	void UpdateInput()override;
private:
};


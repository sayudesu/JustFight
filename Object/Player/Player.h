#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Player : public CharacterBase
{
public:
	Player(VECTOR pos);
	virtual ~Player();
	
	// �{�^������
	void Input()override;

public:
	float GetAngle()const;
};


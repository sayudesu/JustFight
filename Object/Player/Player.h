#pragma once
#include "../CharacterBase.h"
#include <DxLib.h>

class Effekseer3DDrawer;
class Player : public CharacterBase
{
public:
	Player(VECTOR pos);
	virtual ~Player();
	
	// ボタン入力
	void Input()override;

public:
	float GetAngle()const;
private:
	void MoveAway();

	Effekseer3DDrawer* m_effect;
};


#pragma once
#include "EnemyBase.h"

#include "../../Util/DifficultyData.h"

class EnemyCcavalry : public EnemyBase
{
public:
	EnemyCcavalry(DifficultyData data, VECTOR pos);
	virtual ~EnemyCcavalry();

	void Input()override;
	void Update()override;
	void Draw()override;

private:

};


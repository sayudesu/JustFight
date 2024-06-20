#pragma once

#include <DxLib.h>
#include "../../Util/DifficultyData.h"

#include "../CharacterBase.h"

class EnemyBase : public CharacterBase
{
	EnemyBase(DifficultyData data, VECTOR pos) :
		CharacterBase(data,pos)
	{

	}
};


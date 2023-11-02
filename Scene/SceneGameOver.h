#pragma once
#include "SceneBase.h"
class SceneGameOver : public SceneBase
{
public:
	SceneGameOver();
	virtual~SceneGameOver();

	virtual void Init();
	virtual void End();
	virtual SceneBase* Update()override;
	virtual void Draw();
private:
};


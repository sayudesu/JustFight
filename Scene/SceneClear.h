#pragma once
#include "SceneBase.h"
class SceneClear : public SceneBase
{
public:
	SceneClear();
	virtual~SceneClear();

	virtual void Init();
	virtual void End();
	virtual SceneBase* Update();
	virtual void Draw();
private:
};


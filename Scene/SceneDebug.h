#pragma once
#include "SceneBase.h"
#include <vector>

class SceneDebug final : public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug() {}

	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	std::vector<int>m_handle;
	int m_selectScene;
	int m_resultSelectScene;
};
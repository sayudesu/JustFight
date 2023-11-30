#pragma once
#include "SceneBase.h"
#include <memory>

class SlideSelect;
class GameObject;
class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	int m_hTitle;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
};


#pragma once
#include "SceneBase.h"

class SceneTitle final : public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	// �摜�n���h��
	int m_hTitle;
	int m_hBg;

	// �A���t�@�l
	int m_blendAlpha;
};


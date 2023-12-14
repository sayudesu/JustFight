#pragma once
#include "SceneBase.h"
#include <memory>

class SlideSelect;
class GameObject;
class Camera;

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
	// �w�i�摜
	std::shared_ptr<GameObject> m_hBg;

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
	// �R���g���[���[��ڑ����Ă邩�ǂ���
	bool m_isInputController;

	// 
	float z = 0;
	float y = 0;
	float x = 0;

	float rY = 0;
	float rX = 0;

	// �w�i�摜�ʒu
	VECTOR m_bgPos
	
};


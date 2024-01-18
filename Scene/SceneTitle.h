#pragma once
#include <DxLib.h>
#include <memory>
#include "SceneBase.h"

class SlideSelect;
class GameObject;
class Camera;

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
	int m_hTitle;
	// �w�i�摜
	std::shared_ptr<GameObject> m_hBg;
	std::shared_ptr<GameObject> m_hSelect;
	std::shared_ptr<GameObject> m_hDecoration;
	// ��Փx
	std::shared_ptr<GameObject> m_hNovice;       // �ȒP
	std::shared_ptr<GameObject> m_hIntermediate; // ����
	std::shared_ptr<GameObject> m_hExpert;       // ���
	// �G�̉摜
	std::shared_ptr<GameObject> m_hImageNovice;       // �ȒP
	std::shared_ptr<GameObject> m_hImageIntermediate; // ����
	std::shared_ptr<GameObject> m_hImageExpert;       // ���

	std::shared_ptr<GameObject> m_hImageDifficultyBg; // �G�̔w�i

	std::shared_ptr<GameObject> m_hArrow[2];

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
	// �R���g���[���[��ڑ����Ă邩�ǂ���
	bool m_isInputController;


	// �J�������w��̈ʒu�ɒ�~�������ǂ���x.y.z���ꂼ�ꂪ
	bool m_isCameraStop[3];

	// �w�i�摜�ʒu
	VECTOR m_bgPos;

	// ���̃T�C�Y
	float m_arrowSize[2];

	// �J�����̍��W
	float m_cameraPosX;
	float m_cameraPosY;
	float m_cameraPosZ;
};


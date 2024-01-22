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
	// ”wŒi‰æ‘œ
	std::shared_ptr<GameObject> m_hBg;
	std::shared_ptr<GameObject> m_hSelect;
	std::shared_ptr<GameObject> m_hDecoration;
	// “ïˆÕ“x
	std::shared_ptr<GameObject> m_hNovice;       // ŠÈ’P
	std::shared_ptr<GameObject> m_hIntermediate; // ’†‹‰
	std::shared_ptr<GameObject> m_hExpert;       // “ï‚µ‚¢
	// “G‚Ìƒ‚ƒfƒ‹
	std::shared_ptr<GameObject> m_hModel[3];

	std::shared_ptr<GameObject> m_hImageDifficultyBg; // “G‚Ì”wŒi

	std::shared_ptr<GameObject> m_hArrow[2];

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
	// ƒRƒ“ƒgƒ[ƒ‰[‚ğÚ‘±‚µ‚Ä‚é‚©‚Ç‚¤‚©
	bool m_isInputController;

	// ƒJƒƒ‰‚ªw’è‚ÌˆÊ’u‚É’â~‚µ‚½‚©‚Ç‚¤‚©x.y.z‚»‚ê‚¼‚ê‚ª
	bool m_isCameraStop[3];
	int m_cameraStopCount;

	// ”wŒi‰æ‘œˆÊ’u
	VECTOR m_bgPos;

	// “G‚Ì”wŒi‰æ‘œ
	int m_tempSelect;
	bool m_isEnemyBgShake;
	int m_enemyBgshakeCount;
	float m_firstEnemyBgX;
	float m_firstEnemyBgY;
	float m_enemyBgShakeX;
	float m_enemyBgShakeY;

	// –îˆó
	float m_arrowSize[2];

	float m_arrowPosX[2];
	float m_arrowPosY[2];

	float m_firstArrowPosX[2];
	float m_firstArrowPosY[2];

	float m_arrowShakeX[2];
	float m_arrowShakeY[2];

	// “Gƒ‚ƒfƒ‹
	float m_modelRot[3];

	// ƒJƒƒ‰‚ÌÀ•W
	float m_cameraPosX;
	float m_cameraPosY;
	float m_cameraPosZ;
};


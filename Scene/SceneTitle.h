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
	// 背景画像
	std::shared_ptr<GameObject> m_hBg;
	std::shared_ptr<GameObject> m_hSelect;
	std::shared_ptr<GameObject> m_hDecoration;
	// 難易度
	std::shared_ptr<GameObject> m_hNovice;       // 簡単
	std::shared_ptr<GameObject> m_hIntermediate; // 中級
	std::shared_ptr<GameObject> m_hExpert;       // 難しい
	// 敵のモデル
	std::shared_ptr<GameObject> m_hModel[3];

	std::shared_ptr<GameObject> m_hImageDifficultyBg; // 敵の背景

	std::shared_ptr<GameObject> m_hArrow[2];

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
	// コントローラーを接続してるかどうか
	bool m_isInputController;

	// カメラが指定の位置に停止したかどうかx.y.zそれぞれが
	bool m_isCameraStop[3];
	int m_cameraStopCount;

	// 背景画像位置
	VECTOR m_bgPos;

	// 敵の背景画像
	int m_tempSelect;
	bool m_isEnemyBgShake;
	int m_enemyBgshakeCount;
	float m_firstEnemyBgX;
	float m_firstEnemyBgY;
	float m_enemyBgShakeX;
	float m_enemyBgShakeY;

	// 矢印
	float m_arrowSize[2];

	float m_arrowPosX[2];
	float m_arrowPosY[2];

	float m_firstArrowPosX[2];
	float m_firstArrowPosY[2];

	float m_arrowShakeX[2];
	float m_arrowShakeY[2];

	// 敵モデル
	float m_modelRot[3];

	// カメラの座標
	float m_cameraPosX;
	float m_cameraPosY;
	float m_cameraPosZ;
};


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
	// 敵の画像
	std::shared_ptr<GameObject> m_hImageNovice;       // 簡単
	std::shared_ptr<GameObject> m_hImageIntermediate; // 中級
	std::shared_ptr<GameObject> m_hImageExpert;       // 難しい

	std::shared_ptr<GameObject> m_hImageDifficultyBg; // 敵の背景

	std::shared_ptr<GameObject> m_hArrow[2];

	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SlideSelect> m_select;

	std::unique_ptr<GameObject>m_pStage;
	
	// コントローラーを接続してるかどうか
	bool m_isInputController;


	// カメラが指定の位置に停止したかどうかx.y.zそれぞれが
	bool m_isCameraStop[3];

	// 背景画像位置
	VECTOR m_bgPos;

	// 矢印のサイズ
	float m_arrowSize[2];

	// カメラの座標
	float m_cameraPosX;
	float m_cameraPosY;
	float m_cameraPosZ;
};


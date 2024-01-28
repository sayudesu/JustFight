#pragma once

#include <vector>

#include "SceneBase.h"

#include "../Util/GameResultData.h"

class SceneResult final : public SceneBase
{
public:
	SceneResult(GameResultData data);
	virtual ~SceneResult();

	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	// ゲームのリザルトデーターを取得する
	GameResultData m_resultData;

	// 画像の読み込み
	int m_hImageResultBg;
	int m_hImageResult;
	// 画像の角度
	float m_imageAngle;

	std::vector<int> m_posX[2];
	std::vector<int> m_posY[2];

	std::vector<int> m_vec[2];
};


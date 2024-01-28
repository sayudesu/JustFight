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
	// �Q�[���̃��U���g�f�[�^�[���擾����
	GameResultData m_resultData;

	// �摜�̓ǂݍ���
	int m_hImageResultBg;
	int m_hImageResult;
	// �摜�̊p�x
	float m_imageAngle;

	std::vector<int> m_posX[2];
	std::vector<int> m_posY[2];

	std::vector<int> m_vec[2];
};


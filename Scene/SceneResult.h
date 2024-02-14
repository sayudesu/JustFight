#pragma once

#include <vector>
#include <memory>
#include <string>

#include "SceneBase.h"

#include "../Util/GameResultData.h"
#include "../Util/DifficultyData.h"

class GameObject;
class Camera;
class SceneResult final : public SceneBase
{
public:
	SceneResult(GameResultData resultData, DifficultyData data);
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

	std::unique_ptr<GameObject> m_pPlayer;
	std::unique_ptr<GameObject> m_pEnemy;

	std::unique_ptr<Camera> m_pCamera;

	std::string m_enemyPath;
};


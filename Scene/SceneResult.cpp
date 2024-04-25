#include <DxLib.h>

#include "SceneResult.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include "SceneLevelSelect.h"

#include "../Util/Game.h"
#include "../Util/Pad.h"
#include "../Util/GameObject.h"

#include "../CSVData/SoundManager.h"
#include "../Util/SoundName.h"

#include "../CSVData/FontManager.h"
#include "../Util/FontSize.h"

#include "../CSVData/ModelManager.h"
#include "../\Util/ModelName.h"

#include "../Object/Camera/Camera.h"

namespace
{
	//const VECTOR kWinnerPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 - 500.0f, Game::kScreenHeight / 2, 0.5f));
	//const VECTOR kLoserPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 + 500.0f, Game::kScreenHeight / 2, 0.5f));

	// 3D�I�u�W�F�N�g�̊p�x
	const VECTOR kWinnerRota = VGet(0, 0 * DX_PI_F / 180.0f, 0);
	const VECTOR kLoserRota = VGet(
		0,
		180 * DX_PI_F / 180.0f,
		-90 * DX_PI_F / 180.0f);
	// 3D�I�u�W�F�N�g�̃T�C�Y
	const VECTOR k3DModelSize = VGet(12, 12, 12);
}

SceneResult::SceneResult(GameResultData resultData, DifficultyData data):
	m_imageAngle(0.0f),
	m_y(0.0f)
{
	// ���U���g�f�[�^���擾����
	m_resultData = resultData;

	// ���������G�̃��f���̃p�X���擾����	
	if (data == DifficultyData::NOIVE)
	{
		m_enemyPath = ModelManager::GetInstance().ModelType(ModelName::Pawn_B);
	}
	if (data == DifficultyData::INTERMEDIATE)
	{
		m_enemyPath = ModelManager::GetInstance().ModelType(ModelName::Knight_B);
	}
	if (data == DifficultyData::EXPERT)
	{
		m_enemyPath = ModelManager::GetInstance().ModelType(ModelName::Queen_B);
	}

	m_pCamera = std::make_unique<Camera>();

	m_pCamera->SetPos(VGet(0.0f,0.0f,-10000.0f));
	m_pCamera->Setting();
}

SceneResult::~SceneResult()
{
}

void SceneResult::Init()
{
	// �R���X�g���N�^�ŃJ�����̏���ݒ��X�N���[�����W����3D��ԍ��W�ւ̕ϊ����s��
	const VECTOR kWinnerPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 - 500.0f, Game::kScreenHeight / 2 + 150.0f, 0.5f));
	const VECTOR kLoserPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 + 400.0f, Game::kScreenHeight / 2, 0.5f));

	VECTOR playerScreenToWorldPos{};
	VECTOR playerRota{};
	
	VECTOR enemyScreenToWorldPos{};
	VECTOR enemyRota{};

	m_hImageResultBg = LoadGraph("Data/Image/UI/�Q�[����Փx�I���x�[�X2.png");
	if (m_resultData == GameResultData::CREAR)
	{
		m_hImageResult = LoadGraph("Data/Image/UI/Win.png");

		playerScreenToWorldPos = kWinnerPos;
		enemyScreenToWorldPos = kLoserPos;
		playerRota = kWinnerRota;
		enemyRota = kLoserRota;

		for (int i = 0; i < 100; i++)
		{
			m_posX[0].push_back(GetRand(Game::kScreenWidth));
			m_posY[0].push_back(GetRand(Game::kScreenHeight) + Game::kScreenHeight);

			m_vec[0].push_back(1);
		}
	}
	else if (m_resultData == GameResultData::OVER)
	{
		m_hImageResult = LoadGraph("Data/Image/UI/Lose.png");

		playerScreenToWorldPos = kLoserPos;
		enemyScreenToWorldPos = kWinnerPos;
		playerRota = kLoserRota;
		enemyRota = kWinnerRota;

		for (int i = 0; i < 100; i++)
		{
			m_posX[1].push_back(GetRand(Game::kScreenWidth));
			m_posY[1].push_back(0 - GetRand(Game::kScreenHeight));

			m_vec[1].push_back(1);
		}
	}

	// �v���C���[�I�u�W�F�N�g�C���X�^���X����
	m_pPlayer = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Knight_W),
		playerScreenToWorldPos,
		playerRota,
		k3DModelSize
		);
	// �G�l�~�[�I�u�W�F�N�g�C���X�^���X����
	m_pEnemy = std::make_unique<GameObject>(
		m_enemyPath,
		enemyScreenToWorldPos,
		enemyRota,
		k3DModelSize
		);
}

void SceneResult::End()
{
	DeleteGraph(m_hImageResult);
}

SceneBase* SceneResult::Update()
{
	m_pPlayer->Update();
	m_pEnemy->Update();

	static int timer = 0;
	m_y = cos(timer * 0.07f) * 5.0f;
	timer++;

	// �s�k�����ꍇ�͉摜���X����
	if (m_resultData == GameResultData::OVER)
	{
		SoundManager::GetInstance().Play(SoundName::LOSE,true);

		if (m_imageAngle < 0.1f)
		{
			m_imageAngle += 0.001f;
		}

		for (int i = 0; i < 100; i++)
		{
			m_vec[1][i] = GetRand(5) + 1;
			m_posY[1][i] += m_vec[1][i];
		}
	}

	if (m_resultData == GameResultData::CREAR)
	{
		SoundManager::GetInstance().Play(SoundName::WIN, true);

		for (int i = 0; i < 100; i++)
		{
			m_vec[0][i] = GetRand(5) + 1;
			m_posY[0][i] -= m_vec[0][i];
		}
	}

	// �I��
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		// �T�E���h�Ǘ�
		SoundManager::GetInstance().Stop(SoundName::WIN);
		SoundManager::GetInstance().Stop(SoundName::LOSE);
		SoundManager::GetInstance().Play(SoundName::SELECT);
		timer = 0;

		// �V�[���̐؂�ւ�
		return new SceneLevelSelect();
	}
	
	return this;
}

void SceneResult::Draw()
{
#if _DEBUG
	DrawString(0, 0, "SceneResult", 0xffffff);
#endif
	
	// �w�i��`��
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, 0.0f, m_hImageResultBg, true);
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2 + m_y, 1, m_imageAngle, m_hImageResult, true);

	// �L�����N�^�[�̕`��
	m_pPlayer->Draw();
	m_pEnemy->Draw();

	// �{�^������
	FontManager::GetInstance().DrawString(Game::kScreenWidth / 2 - 470.0f + 5, Game::kScreenHeight / 2 + 200 + 5, "A �{�^���Ń^�C�g���ɖ߂�", 0x000000, FontSize::GENEITERAMIN_MEDIUM);
	FontManager::GetInstance().DrawString(Game::kScreenWidth / 2 - 470.0f, Game::kScreenHeight / 2 + 200, "A �{�^���Ń^�C�g���ɖ߂�", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
}
#include <DxLib.h>
#include "SceneDebug.h"
#include "SceneMain.h"
#include "SceneTitle.h"

#include "../SlideSelect.h"
#include "../SlideSelect.h"

#include "../Util/Game.h"

#include "../GameObject.h"

#include "../Object/Camera/Camera.h"

#include "../DifficultyData.h"

#include "../CSVData/ModelManager.h"
#include "../ModelName.h"

namespace 
{
	constexpr float kImageAngle = 0.0f * DX_PI_F / 180.0f;
	constexpr float kImagePosX = -450.0f;
	constexpr float kImagePosY = 0.0f;
	constexpr float kImageSize = 0.95f;

	constexpr float kImageArrowSizeChangeSpeed = 0.002f;
}

SceneTitle::SceneTitle():
	m_hTitle(-1),
	m_isInputController(false),
	m_bgPos(VGet(static_cast<float>(Game::kScreenWidth / 2), -static_cast<float>(Game::kScreenHeight / 2), 0.0f))
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	m_arrowSize[0] = kImageSize;
	m_arrowSize[1] = kImageSize;

	// �J�����̈ʒu�����������ǂ������m�F���邽�߂̏�����
	m_isCameraStop[0] = false;
	m_isCameraStop[1] = false;
	m_isCameraStop[2] = false;

	m_hTitle = LoadGraph("Data/Image/UI/�Q�[����Փx�I���x�[�X2.png");

	m_select = std::make_unique<SlideSelect>();
	m_select->Init(2);

	// �J�����̍��W
	m_cameraPosX = 100.0f;
	m_cameraPosY = 300.0f;
	m_cameraPosZ = 100.0f;

	// �J�����C���X�^���X
	m_camera = std::make_unique<Camera>();
	// �J�����^�[�Q�b�g�ʒu������
	m_camera->SetTargetPos(VGet(0.0f, -100.0f, -10.0f));

	// 3D�I�u�W�F�N�g
	{
		// �}�b�v
		m_pStage = std::make_unique<GameObject>(
			ModelManager::GetInstance().ModelType(ModelName::MAP3),
			VGet(0,0,0),
			VGet(0,0, 180 * DX_PI_F/ 180.0f),
			VGet(1, 1, 1));		
	}

	// 2D�I�u�W�F�N�g
	{
		// �w�i
		m_hBg = std::make_shared<GameObject>(
			"Data/Image/UI/�Q�[����Փx�I���x�[�X2.png",
			m_bgPos,
			kImageAngle,
			kImageSize
		);

		// �I�𕶎�
		m_hSelect = std::make_shared<GameObject>(
			"Data/Image/UI/Select Difficulty.png",
			VGet(kImagePosX, -300.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		// �I�𕶎��̉��̏���
		m_hDecoration = std::make_shared<GameObject>(
			"Data/Image/UI/����.png",
			VGet(kImagePosX, -250.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		// ��Փx
		m_hNovice = std::make_shared<GameObject>(
			"Data/Image/UI/Novice.png",
			VGet(kImagePosX, 30.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);
		
		m_hIntermediate = std::make_shared<GameObject>(
			"Data/Image/UI/Intermediate.png",
			VGet(kImagePosX, 30.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		m_hExpert = std::make_shared<GameObject>(
			"Data/Image/UI/Expert.png",
			VGet(kImagePosX, 30.0f, 0),
			kImageAngle,
			kImageSize,
			m_hBg.get()
		);

		m_hImageNovice = std::make_shared<GameObject>(
			"Data/Image/UI/���|�[��.png",
			VGet(kImagePosX + 800.0f, 30.0f, 0),
			kImageAngle,
			kImageSize + 1.0f,
			m_hBg.get()
		);

		m_hImageIntermediate = std::make_shared<GameObject>(
			"Data/Image/UI/�n��.png",
			VGet(kImagePosX + 800.0f, 30.0f, 0),
			kImageAngle,
			kImageSize + 1.0f,
			m_hBg.get()
		);

		m_hImageExpert = std::make_shared<GameObject>(
			"Data/Image/UI/���L���O.png",
			VGet(kImagePosX + 800.0f, 30.0f, 0),
			kImageAngle,
			kImageSize + 1.0f,
			m_hBg.get()
		);

		m_hImageDifficultyBg = std::make_shared<GameObject>(
			"Data/Image/UI/CharactorBg.png",
			VGet(kImagePosX + 800.0f, 30.0f, 0),
			kImageAngle,
			kImageSize + 2.0f,
			m_hBg.get()
		);

		// �������
		m_hArrow[0] = std::make_shared<GameObject>(
			"Data/Image/UI/���.png",
			VGet(kImagePosX, -120.0f, 0),
			kImageAngle + 180.0f * DX_PI_F / 180.0f,
			m_arrowSize[0],
			m_hBg.get()
		);

		// ��󉺌���
		m_hArrow[1] = std::make_shared<GameObject>(
			"Data/Image/UI/���.png",
			VGet(kImagePosX, 180.0f, 0),
			kImageAngle,
			m_arrowSize[1],
			m_hBg.get()
		);

		m_pStage->SetPos(VGet(0.0f, 0.0f, 0.0f));
		m_pStage->SetRotate(VGet(0.0f, 0.0f, 0.0f));
		m_pStage->Update();
	}
}

void SceneTitle::End()
{
	DeleteGraph(m_hTitle);
}

SceneBase* SceneTitle::Update()
{
	char deviceName[260]{};
	char productName[260]{};
#if false
	// �R���g���[���[�̐ڑ����m�F����
	if (GetJoypadName(DX_INPUT_PAD1, &deviceName[0], &productName[0]) == 0) 
	{
		m_isInputController = false;
	}
	else
	{
		m_isInputController = true;
		return this;
	}
#endif

	m_select->Update();



	m_camera->SetPos(VGet(m_cameraPosX, m_cameraPosY, m_cameraPosZ));

	// �J������x,y,z���ꂼ���~���������`�F�b�N����
	int count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (m_isCameraStop[i])
		{
			count++;
		}

	}

	// �J���������ׂĒ�~���Ă�����
	if (count == 3)
	{
		// ��ʂ̒��S�Ɉړ�
		if (m_bgPos.y <= static_cast<float>(Game::kScreenHeight) / 2)
		{
			m_hBg->SetPos(m_bgPos);
			m_bgPos.y += 60.0f;
		}
	}

	static float speed = 0.01f;
	speed = (speed * 1.07f);

	if (m_cameraPosX > 0.0f)
	{
		m_cameraPosX -= speed;
	}
	else
	{
		m_cameraPosX = 0.0f;
		m_isCameraStop[0] = true;
	}

	if (m_cameraPosY > 32.0f)
	{
		m_cameraPosY -= speed;
	}
	else
	{
		m_cameraPosY = 32.0f;
		m_isCameraStop[1] = true;
	}
	
	if (m_cameraPosZ > 10.0f)
	{
		m_cameraPosZ -= speed;
	}
	else
	{
		m_cameraPosZ = 10.0f;
		m_isCameraStop[2] = true;
	}

	m_camera->Setting();

	m_hBg->Update();
	m_hSelect->Update();
	m_hDecoration->Update();
	// ��Փx
	m_hNovice->Update();
	m_hIntermediate->Update();
	m_hExpert->Update();

	m_hImageNovice->Update();
	m_hImageIntermediate->Update();
	m_hImageExpert->Update();

	m_hImageDifficultyBg->Update();

	m_hArrow[0]->Update();
	m_hArrow[1]->Update();

	// ��Փx����
	// �ア(�`���[�g���A��)
	if (m_select->GetResult() == 0)
	{		
		return new SceneMain(DifficultyData::NOIVE);
	}
	// ����
	if (m_select->GetResult() == 1)
	{
		return new SceneMain(DifficultyData::INTERMEDIATE);
	}
	// ����
	if (m_select->GetResult() == 2)
	{
		return new SceneMain(DifficultyData::EXPERT);
	}

#if _DEBUG
	if (DxLib::CheckHitKey(KEY_INPUT_Z))
	{
		return new SceneDebug();
	}
#endif


	return this;
}

void SceneTitle::Draw()
{
	m_pStage->Draw();

	m_hBg->Draw();
	m_hSelect->Draw();
	m_hDecoration->Draw();

	m_hImageDifficultyBg->Draw();

	// ��Փx
	if (m_select->GetSelect() == 0)
	{
		m_hNovice->Draw();
		m_hImageNovice->Draw();
	}
	else if (m_select->GetSelect() == 1)
	{
		m_hIntermediate->Draw();
		m_hImageIntermediate->Draw();
	}
	else if (m_select->GetSelect() == 2)
	{
		m_hExpert->Draw();		
		m_hImageExpert->Draw();
	}
	
	m_hArrow[0]->Draw();
	m_hArrow[1]->Draw();

	// �R���g���[���[
	if (m_isInputController)
	{
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
		DrawFormatString((Game::kScreenWidth - ((17 * 32)/2)) / 2, Game::kScreenHeight/2 - 32, 0x000000, "�R���g���[���[��ڑ����Ă��������B");
	}	
}
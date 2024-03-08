#include <DxLib.h>

#include "SceneTitle.h"
#include "SceneLevelSelect.h"

#include "../Util/Game.h"
#include "../Util/Pad.h"
#include "../CSVData/FontManager.h"
#include "../Util/FontSize.h"

#include "../Object/Camera/Camera.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/Enemy.h"

#include "../Util/GameObject.h"

#include "../Util/ModelName.h"
#include "../CSVData/ModelManager.h"

namespace
{
	// 3D�I�u�W�F�N�g�̊p�x
	const VECTOR kWinnerRota = VGet(0, 0 * DX_PI_F / 180.0f, 0);
	const VECTOR kWinnerRota2 = VGet(0, 180 * DX_PI_F / 180.0f, 0);
}

SceneTitle::SceneTitle():
	m_hTitle(-1),
	m_hBg(-1),
	m_blendAlpha(0)
{
	// �摜�̓ǂݍ���
	m_hTitle = LoadGraph("Data/Image/Title.png");
	m_hBg    = LoadGraph("Data/Image/UI/�Q�[����Փx�I���x�[�X2.png");
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{

	// �R���X�g���N�^�ŃJ�����̏���ݒ��X�N���[�����W����3D��ԍ��W�ւ̕ϊ����s��
	const VECTOR kWinnerPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 - 700.0f, Game::kScreenHeight / 2 + 150.0f, 0.5f));
	const VECTOR kLoserPos = ConvScreenPosToWorldPos(VGet(Game::kScreenWidth / 2 + 700.0f, Game::kScreenHeight / 2 + 150.0f, 0.5f));

	// �v���C���[�I�u�W�F�N�g�C���X�^���X����
	m_pPlayer = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Knight_W),
		kWinnerPos,
		kWinnerRota,
		VGet(12,12,12)
		);
	// �G�l�~�[�I�u�W�F�N�g�C���X�^���X����
	m_pEnemy = std::make_unique<GameObject>(
		ModelManager::GetInstance().ModelType(ModelName::Knight_B),
		kLoserPos,
		kWinnerRota2,
		VGet(12, 12, 12)
		);

	m_pPlayer->Update();
	m_pEnemy->Update();
}

void SceneTitle::End()
{
	
}

SceneBase* SceneTitle::Update()
{	

	// �^�C�g���摜�̈ړ��̌v�Z
	static float timer = 0.0f;
	m_blendAlpha = cos(timer * 0.06f) * 255.0f;
	timer++;

	// ���f���̉�]�̌v�Z
	static float rota = 0.0f;
	float rotaModel = cos(rota * 0.001f) * 15.0f;
	rota++;

	// �{�^�����������ꍇ
	if (Pad::IsTrigger(PAD_INPUT_1) || 
		Pad::IsTrigger(PAD_INPUT_2) || 
		Pad::IsTrigger(PAD_INPUT_3) || 
		Pad::IsTrigger(PAD_INPUT_4))
	{
		timer = 0;
		rota = 0;
		return new SceneLevelSelect();
	}

	// 3D���f���̊p�x�ύX
	m_pPlayer->SetRotate(VGet(kWinnerRota.x, kWinnerRota.y + rotaModel, kWinnerRota.z));
	m_pEnemy->SetRotate(VGet(kWinnerRota2.x, kWinnerRota2.y + rotaModel, kWinnerRota2.z));

	// 3D���f���̍X�V����
	m_pPlayer->Update();
	m_pEnemy->Update();

	return this;
}

void SceneTitle::Draw()
{
	// �w�i
	DrawExtendGraph(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_hBg, true);

	// �^�C�g���`��
	DrawRotaGraph(Game::kScreenWidth / 2, Game::kScreenHeight / 2, 1, 0.0f, m_hTitle, true);

	// 3D���f���`��
	m_pPlayer->Draw();
	m_pEnemy->Draw();

	// �{�^���U���`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(m_blendAlpha));
	FontManager::GetInstance().DrawString(Game::kScreenWidth/2 - 272 + 12 + 5, Game::kScreenHeight/2 + 256 + 5, "Press any key", 0x111111, FontSize::GENEITERAMIN_MEDIUM);
	FontManager::GetInstance().DrawString(Game::kScreenWidth/2 - 272 + 12, Game::kScreenHeight/2 + 256, "Press any key", 0xffffff, FontSize::GENEITERAMIN_MEDIUM);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
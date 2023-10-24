#include <DxLib.h>

#include "Util/Game.h"
#include "Scene/SceneManager.h"

#include "EffekseerForDXLib.h"

// �v���O������WinMain����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// window���[�h����window�T�C�Y��ύX���邩�ǂ���
	SetWindowSizeChangeEnableFlag(Game::kWindowSizeChange);
	// ���O�e�L�X�g���쐬���邩�ǂ���
	SetOutApplicationLogValidFlag(Game::kCreateLogText);

	// DirectX9���g�p����悤�ɂ���B(DirectX11����)
	// Effekseer���g�p����ɂ͕K���ݒ肷��B
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{
		// �G���[���N�����璼���ɏI��
		return -1;			
	}

	// Effekseer������������B
// �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
	if (Effkseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	// 3D�֘A�̐ݒ�
	// Z�o�b�t�@��L���ɂ���B
	SetUseZBuffer3D(true);
	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(true);
	// �|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(true);

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* pScene = new SceneManager;

	pScene->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		pScene->Update();

		pScene->Draw();

		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	pScene->End();

	// Effekseer���I������B
	Effkseer_End();

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();				

	// �\�t�g�̏I�� 
	return 0;				
}
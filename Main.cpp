#include <DxLib.h>
#include <EffekseerForDXLib.h> 

#include "Util/Game.h"
#include "Scene/SceneManager.h"

#include "Util/EffekseerDrawer.h"

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

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	// DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
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
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
		return -1;
	}

	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	// DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
	// �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
	// �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
//	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// 3D�֘A�̐ݒ�
	// Z�o�b�t�@��L���ɂ���B
	SetUseZBuffer3D(true);
	// Z�o�b�t�@�ւ̏������݂�L���ɂ���B
	// Effekseer���g�p����ꍇ�A2D�Q�[���ł�Z�o�b�t�@���g�p����B
	SetWriteZBuffer3D(true);
	// �|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(true);

	// ���ʂ��킩��Ղ��悤�ɕW�����C�g�𖳌���
	SetLightEnable(true);

	float lightRange = 2100.0f;
	float lightRangeMainUp = 700.0f;
	float lightRangeUp = 800.0f;

	const VECTOR lightPosUp    = VGet(0.0f,        lightRangeMainUp, 0.0f);
	const VECTOR lightPosLeft  = VGet(lightRange,  lightRangeUp, 0.0f);
	const VECTOR lightPosRight = VGet(-lightRange, lightRangeUp, 0.0f);
	const VECTOR lightPosNear  = VGet(0.0f,        lightRangeUp, -lightRange);
	const VECTOR lightPosFar   = VGet(0.0f,        lightRangeUp, lightRange);

	// �|�C���g�^�C�v�̃��C�g�n���h�����쐬
	const int lightHandleUp = CreatePointLightHandle(
		lightPosUp,
		2000.0f,
		0.0f,
		0.002f,
		0.0f);

	// �|�C���g�^�C�v�̃��C�g�n���h�����쐬
	const int lightHandleLeft = CreatePointLightHandle(
		lightPosLeft,
		2000.0f,
		0.0f,
		0.002f,
		0.0f);

	// �|�C���g�^�C�v�̃��C�g�n���h�����쐬
	const int lightHandleRight = CreatePointLightHandle(
		lightPosRight,
		2000.0f,
		0.0f,
		0.002f,
		0.0f);

	// �|�C���g�^�C�v�̃��C�g�n���h�����쐬
	const int lightHandleNear = CreatePointLightHandle(
		lightPosNear,
		2000.0f,
		0.0f,
		0.002f,
		0.0f);

	// �|�C���g�^�C�v�̃��C�g�n���h�����쐬
	const int lightHandleFar = CreatePointLightHandle(
		lightPosFar,
		2000.0f,
		0.0f,
		0.002f,
		0.0f);

	// ���C�g�n���h���̕������w���̃v���X�����ɂ���
	SetLightDirectionHandle(lightHandleUp, VGet(0.0f, -1.0f, 0.0f));

	SetLightDirectionHandle(lightHandleLeft, VGet(1.0f, -1.0f, 0.0f));
	SetLightDirectionHandle(lightHandleRight, VGet(-1.0f, -1.0f, 0.0f));

	SetLightDirectionHandle(lightHandleNear, VGet(0.0f, -1.0f, 1.0f));
	SetLightDirectionHandle(lightHandleFar, VGet(0.0f, -1.0f, -1.0f));

	SceneManager* pScene = new SceneManager();

	pScene->Init();

	// �ǂݍ���
	EffekseerDrawer::GetInstance().Load();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		pScene->Update();
		EffekseerDrawer::GetInstance().Update();

		pScene->Draw();
		EffekseerDrawer::GetInstance().Draw();
		
#if _DEBUG
		DrawSphere3D(lightPosUp, 30, 30, 0xaaaa11, 0xaaaa11, true);
		DrawSphere3D(lightPosLeft, 30, 30, 0xaaaa11, 0xaaaa11, true);
		DrawSphere3D(lightPosRight, 30, 30, 0xaaaa11, 0xaaaa11, true);
		DrawSphere3D(lightPosNear, 30, 30, 0xaaaa11, 0xaaaa11, true);
		DrawSphere3D(lightPosFar, 30, 30, 0xaaaa11, 0xaaaa11, true);
#endif
		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	
	EffekseerDrawer::GetInstance().Destroy();
	EffekseerDrawer::GetInstance().Unload();

	// ���C�g�n���h���̍폜
	DeleteLightHandle(lightHandleUp);
	DeleteLightHandle(lightHandleLeft);
	DeleteLightHandle(lightHandleRight);
	DeleteLightHandle(lightHandleNear);
	DeleteLightHandle(lightHandleFar);

	pScene->End();

	// Effekseer���I������B
	Effkseer_End();

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();				

	// �\�t�g�̏I�� 
	return 0;				
}
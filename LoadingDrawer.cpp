#include "LoadingDrawer.h"
#include <DxLib.h>

LoadingDrawer::LoadingDrawer()
{
}

LoadingDrawer::~LoadingDrawer()
{
}

void LoadingDrawer::SetUseASync(bool aSync)
{
	SetUseASyncLoadFlag(aSync);
}

bool LoadingDrawer::CheckHandle(int handle)
{
	if (CheckHandleASyncLoad(handle))
	{
		return true;
	}

	return false;
}

void LoadingDrawer::Draw()
{
	//// �񓯊��ǂݍ��ݒ��̃f�[�^������ꍇ�������Ȃ�
	//int total = 0;
	//int loadding = 0;
	//for (auto& handle : m_handle)
	//{
	//	// �ǂݍ��݂��I����Ă���f�[�^�̉��Z
	//	total++;
	//	if (CheckHandleASyncLoad(handle))
	//	{
	//		// �ǂݍ��݂��I����Ă��Ȃ��f�[�^�̐������Z
	//		loadding++;
	//	}
	//}
	//if (loadding > 0)
	//{
	//	// �܂��ǂݍ��݂��I����Ă��Ȃ�
	//	// �ǂݍ���ł��Ȃ��f�[�^�̊���
	//	float rate = static_cast<float>(loadding) / static_cast<float>(total);
	//	rate = 1.0f - rate;

	//	// �f�[�^�T�C�Y����e�ɂ���ă��[�h���Ԃ��ς�邵�A
	//	// ���܂���Windows�����ŏd���������n�߂����烍�[�h���d���Ȃ�,��
	//	// ���[�h���Ԃ͊m��o���Ȃ��̂ł����܂Ŗڈ�

	//	DrawFormatString(100, 50, 0xffffff, "Loading.. %.1f%%", rate * 100);

	//	return;
	//}
}

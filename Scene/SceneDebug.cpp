#include "SceneDebug.h"
#include <DxLib.h>
#include "SceneMain.h";
#include "SceneResult.h";
#include "SceneTitle.h";
#include "../Util/Pad.h"

namespace
{
	constexpr int kTest = 100;
}

SceneDebug::SceneDebug()
{
}

void SceneDebug::Init()
{
	// �񓯊��ǂݍ��ݗL���ɂ���
	SetUseASyncLoadFlag(true);
	//// ���\�[�X�̓ǂݍ���
	//for (int i = 0; i < kTest; i++)
	//{
	//	m_handle.push_back(LoadGraph("Data/test.bmp"));
	//}
	//m_handle.push_back(LoadGraph("Data/test2.bmp"));
	// �񓯊��ǂݍ��ݖ����ɂ���
	SetUseASyncLoadFlag(false);

//	int x;
//	int y;
//	int result = GetGraphSize(m_handle.back(), &x, &y);

}

void SceneDebug::End()
{
	//for (int i = 0; i < kTest; i++)
	//{
	//	DeleteGraph(m_handle[i]);
	//}
}

SceneBase* SceneDebug::Update()
{
	// �񓯊��ǂݍ��ݒ��̃f�[�^������ꍇ�������Ȃ�
	for (auto& handle : m_handle)
	{
		if (CheckHandleASyncLoad(handle))
		{
			// handle�̃��\�[�X��ǂݍ��ݒ�
			return this;
		}
	}

	if (Pad::isTrigger(PAD_INPUT_1))
	{
		return new SceneMain();
	}
	if (Pad::isTrigger(PAD_INPUT_2))
	{
		return new SceneResult();
	}
	if (Pad::isTrigger(PAD_INPUT_3))
	{
		return new SceneTitle();
	}

	return this;
}

void SceneDebug::Draw()
{
	// �񓯊��ǂݍ��ݒ��̃f�[�^������ꍇ�������Ȃ�
	int total = 0;
	int loadding = 0;
	for (auto& handle : m_handle)
	{
		// �ǂݍ��݂��I����Ă���f�[�^�̉��Z
		total++;
		if (CheckHandleASyncLoad(handle))
		{
			// �ǂݍ��݂��I����Ă��Ȃ��f�[�^�̐������Z
			loadding++;
		}
	}
	if (loadding > 0)
	{
		// �܂��ǂݍ��݂��I����Ă��Ȃ�
		// �ǂݍ���ł��Ȃ��f�[�^�̊���
		float rate = static_cast<float>(loadding) / static_cast<float>(total);
		rate = 1.0f - rate;

		// �f�[�^�T�C�Y����e�ɂ���ă��[�h���Ԃ��ς�邵�A
		// ���܂���Windows�����ŏd���������n�߂����烍�[�h���d���Ȃ�,��
		// ���[�h���Ԃ͊m��o���Ȃ��̂ł����܂Ŗڈ�

		DrawFormatString(100, 50, 0xffffff, "Loading.. %.1f%%", rate * 100);

		return;
	}

	//for (int i = 0; i < kTest; i++)
	//{
	//	DrawExtendGraph(100 + i, 100 + i, i + 1000, i + 1000, m_handle[i], false);
	//}

//	DrawExtendGraph(1000,100,300,300,m_handle.back(), false);

#if _DEBUG
	DrawString(0, 0, "SceneDebug", 0xffffff);
#endif
}


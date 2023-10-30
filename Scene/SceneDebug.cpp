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
	// 非同期読み込み有効にする
	SetUseASyncLoadFlag(true);
	//// リソースの読み込み
	//for (int i = 0; i < kTest; i++)
	//{
	//	m_handle.push_back(LoadGraph("Data/test.bmp"));
	//}
	//m_handle.push_back(LoadGraph("Data/test2.bmp"));
	// 非同期読み込み無効にする
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
	// 非同期読み込み中のデータがある場合何もしない
	for (auto& handle : m_handle)
	{
		if (CheckHandleASyncLoad(handle))
		{
			// handleのリソースを読み込み中
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
	// 非同期読み込み中のデータがある場合何もしない
	int total = 0;
	int loadding = 0;
	for (auto& handle : m_handle)
	{
		// 読み込みが終わっているデータの加算
		total++;
		if (CheckHandleASyncLoad(handle))
		{
			// 読み込みが終わっていないデータの数を加算
			loadding++;
		}
	}
	if (loadding > 0)
	{
		// まだ読み込みが終わっていない
		// 読み込んでいないデータの割合
		float rate = static_cast<float>(loadding) / static_cast<float>(total);
		rate = 1.0f - rate;

		// データサイズや内容によってロード時間が変わるし、
		// たまたまWindowsが裏で重い処理を始めたからロードが重くなる,等
		// ロード時間は確定出来ないのであくまで目安

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


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
	//// 非同期読み込み中のデータがある場合何もしない
	//int total = 0;
	//int loadding = 0;
	//for (auto& handle : m_handle)
	//{
	//	// 読み込みが終わっているデータの加算
	//	total++;
	//	if (CheckHandleASyncLoad(handle))
	//	{
	//		// 読み込みが終わっていないデータの数を加算
	//		loadding++;
	//	}
	//}
	//if (loadding > 0)
	//{
	//	// まだ読み込みが終わっていない
	//	// 読み込んでいないデータの割合
	//	float rate = static_cast<float>(loadding) / static_cast<float>(total);
	//	rate = 1.0f - rate;

	//	// データサイズや内容によってロード時間が変わるし、
	//	// たまたまWindowsが裏で重い処理を始めたからロードが重くなる,等
	//	// ロード時間は確定出来ないのであくまで目安

	//	DrawFormatString(100, 50, 0xffffff, "Loading.. %.1f%%", rate * 100);

	//	return;
	//}
}

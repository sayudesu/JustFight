#include "FIeldDrawer.h"

FIeldDrawer::FIeldDrawer():
	m_handle(-1),
	m_pos(VGet(0,0,0))
{
}

FIeldDrawer::~FIeldDrawer()
{
}

void FIeldDrawer::Init()
{
	// モデルの読み込み
	m_handle = MV1LoadModel("Data/Model/Arena.mv1");

	// 位置を設定
	MV1SetPosition(m_handle, m_pos);
	// サイズの変更
	MV1SetScale(m_handle, VGet(2.0f, 2.0f, 2.0f));
}

void FIeldDrawer::End()
{
	// 解放
	MV1DeleteModel(m_handle);
}

void FIeldDrawer::Draw()
{
	// 描画
	MV1DrawModel(m_handle);
}

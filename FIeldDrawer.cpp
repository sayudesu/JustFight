#include "FIeldDrawer.h"
#include "Util/GameObject.h"

#include "CSVData/ModelManager.h"
#include "Util/ModelName.h"

namespace
{
	// オブジェクトのサイズ
	constexpr float kChessSize = 0.6;
	constexpr float kMapSize = 40.0f;
}

FieldDrawer::FieldDrawer():
	m_chessModel(nullptr),
	m_mapModel(nullptr),
	m_pos(VGet(0,0,0))
{
}

FieldDrawer::~FieldDrawer()
{
}

void FieldDrawer::Init()
{
	m_chessModel = new GameObject(
		ModelManager::GetInstance().ModelType(ModelName::MAP2),
		m_pos,
		VGet(0, 0, 0),
		VGet(kChessSize, kChessSize, kChessSize));

	m_mapModel = new GameObject(
		ModelManager::GetInstance().ModelType(ModelName::MAP3),
		VGet(m_pos.x, m_pos.y, m_pos.z),
		VGet(0, 0, 0),
		VGet(kMapSize, kMapSize, kMapSize));

	m_chessModel->Update();
	m_mapModel->Update();
}

void FieldDrawer::End()
{
	// 解放
	delete m_chessModel;
	m_chessModel = nullptr;

	delete m_mapModel;
	m_mapModel = nullptr;
}

void FieldDrawer::Draw()
{
	// 描画
	m_chessModel->Draw();
	m_mapModel->Draw();


#if _DEBUG
	VECTOR start = VGet(1200, 300, 1200);
	VECTOR end = VGet(-1200, 300, 1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(-1200, 300, -1200);
	end = VGet(1200, 300, -1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(1200, 300, -1200);
	end = VGet(1200, 300, 1200);
	DrawLine3D(start, end, 0xffffff);

	start = VGet(-1200, 300, 1200);
	end = VGet(-1200, 300, -1200);
	DrawLine3D(start, end, 0xffffff);
	// 軸
	VECTOR Xp = ConvWorldPosToScreenPos(VGet(1000, 300, 0));
	VECTOR Xm = ConvWorldPosToScreenPos(VGet(-1000, 300, 0));

	VECTOR Zp = ConvWorldPosToScreenPos(VGet(0, 300, 1000));
	VECTOR Zm = ConvWorldPosToScreenPos(VGet(0, 300, -1000));

	DrawString(Xp.x, Xp.y, "X+", 0xffff00);
	DrawString(Xm.x, Xm.y, "X-", 0xffffff);
	DrawString(Zp.x, Zp.y, "Z+", 0xffffff);
	DrawString(Zm.x, Zm.y, "Z-", 0xffffff);
#endif
}

int FieldDrawer::GetHandle()
{
	return m_chessModel->GetHandle();
}

int FieldDrawer::GetHandle2()
{
	return m_mapModel->GetHandle();
}

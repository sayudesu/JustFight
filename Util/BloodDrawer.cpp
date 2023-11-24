#include "BloodDrawer.h"
#include "game.h"

namespace
{
	// 重力
	constexpr float kGravity = 0.4f;
}

BloodDrawer::BloodDrawer(VECTOR pos, int color)
{
	// パーティクルの初期位置
	m_pos = pos;
	m_vec = VGet(0, 0, 0);
	m_isErase = false;
	m_color = color;
	m_pFunc = &BloodDrawer::FirstMove;

}

BloodDrawer::~BloodDrawer()
{
}

void BloodDrawer::Init(int no)
{		
	// 移動量
	if (no % 2 == 0)
	{
		m_vec.x = (-GetRand(10));
		m_vec.z = (-GetRand(10));
	}
	else
	{
		m_vec.x = (GetRand(10));
		m_vec.z = (GetRand(10));
	}
	m_vec.y = static_cast<float>(GetRand(12) + 1);
}

void BloodDrawer::Update()
{
	(this->*m_pFunc)();
}

void BloodDrawer::Draw()
{
	// ３Ｄ空間上に球を描画する
	DrawSphere3D(m_pos, 2, 3, m_color, m_color, true);

}

bool BloodDrawer::IsGetErase()
{
	return m_isErase;
}

void BloodDrawer::FirstMove()
{
	// 重力を与える
	m_vec.y -= kGravity;
	// ベクトルの加算
	m_pos = VAdd(m_pos, m_vec);

	if (m_pos.y < -100.0f)
	{
		m_isErase = true;
	}
}

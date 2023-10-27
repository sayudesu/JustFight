#pragma once
#include <DxLib.h>

class Animation2D
{
public:
	Animation2D();
	virtual~Animation2D();

	virtual void Init(const char* fileName, int chipX, int chipY);
	virtual void End();
	virtual void Update();
	virtual void Draw();

public:
	virtual void SetAnim(bool isAnim);
	// ワールド座標をスクリーン座標に変換してから座標を受け取る
	virtual void SetPos3D(VECTOR pos);
private:
	// 描画する画像用ハンドル
	int m_handle;

	// 描画座標
	VECTOR m_pos;

	// チップ番号
	int m_chipX;
	int m_chipY;

	// 画像からアニメーションさせたい一部分の座標
	int m_amimX;
	int m_amimY;

	// アニメーション速度
	int m_animFrame;

	// アニメーションを再生するかどうか
	bool m_isAnimStart;
};


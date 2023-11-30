#pragma once
class EffectScreen
{
public:
	EffectScreen();

	~EffectScreen();

	// MakeScreenして画面サイズを取得する
	void Init();
	//	画面に描かれたものを消去する
	void ClearScreen();
	// 描画を裏の画面にする
	void ScreenBack();

	// ブラー
	void BlurIReplayInit(int alpha);
	void BlurRelease();
	void BlurPreRenderBlurScreen();
	void BlurPostRenderBlurScreen();

	// 揺らし
	void QuakeReplayInit();
	void QuakeUpdate();
	void QuakeRelease();
	void QuakePreRenderBlurScreen();
	void QuakePostRenderBlurScreen();

	//// カラー
	//void ColorReplayInit();
	//void ColorRelease();
	//void ColorPreRenderBlurScreen();
	//void ColorPostRenderBlurScreen();

private:
	enum class EffectNo
	{
		BLUR_0,
		BLUR_1,
		BLUR_2,
		BLUR_3,
		QUAKE,
		COLOR,
		MAX,
	};
	// 共通変数
	int m_screen[static_cast<int>(EffectNo::MAX)];

	// ブラー専用
	int m_current;
	int m_alpha;
	int m_notBlendDraw;

	// 画面揺れ用
	float m_quakeX;
	int m_quakeTimer;
};


#pragma once
class EffectScreen
{
private:
	EffectScreen() = default;
	static EffectScreen* m_pInstanceSound;
public:
	virtual~EffectScreen() = default;
	// コピーコンストラクタの禁止
	EffectScreen(const EffectScreen&) = delete;
	EffectScreen& operator=(const EffectScreen&) = delete;
	EffectScreen(EffectScreen&&) = delete;
	EffectScreen& operator=(EffectScreen&&) = delete;

	// インスタンスの作成
	static EffectScreen& GetInstance()
	{
		if (!m_pInstanceSound)
		{
			m_pInstanceSound = new EffectScreen();
		}
		return *m_pInstanceSound;
	}

	// 解放処理
	static void Destroy()
	{
		delete m_pInstanceSound;
		m_pInstanceSound = nullptr;
	}
public:

	// MakeScreenして画面サイズを取得する
	void Load();
	void Unload();
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


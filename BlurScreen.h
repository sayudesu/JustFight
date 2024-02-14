#pragma once

class EffectScreen
{
private:
	EffectScreen() = default;
	static EffectScreen* m_pInstance;
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
		if (!m_pInstance)
		{
			m_pInstance = new EffectScreen();
		}
		return *m_pInstance;
	}

	// 解放処理
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
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
	void BlurIReplayInit();
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
	enum class DamageEffectNo
	{
		QUAKE,
		COLOR,
		MAX,
	};
	// 攻撃をされた場合の画面処理
	int m_damageScreen[static_cast<int>(DamageEffectNo::MAX)];

	// 画面揺れ用
	float m_quakeX;
	int m_quakeFrame;

	enum class ScreenEffectNo
	{
		ONE,
		TWO,

		MAX
	};

	// 攻撃をされた場合の画面処理
	int m_blurScreen[static_cast<int>(ScreenEffectNo::MAX)];

	// ブラー専用
	int m_current;
	int m_alpha;
	int m_notBlendDraw;


	int m_blendAddRate = 0;
};


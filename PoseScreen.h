#pragma once

#include <memory>
#include <vector>

class SlideSelect;

class PoseScreen
{
private:
	PoseScreen() = default;
	static PoseScreen* m_pInstance;
public:
	virtual~PoseScreen() = default;
	// コピーコンストラクタの禁止
	PoseScreen(const PoseScreen&) = delete;
	PoseScreen& operator=(const PoseScreen&) = delete;
	PoseScreen(PoseScreen&&) = delete;
	PoseScreen& operator=(PoseScreen&&) = delete;

	// インスタンスの作成
	static PoseScreen& GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new PoseScreen();
		}
		return *m_pInstance;
	}

	// 解放処理
	static void Destroy()
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}

	// ロード
	void Load();
	// アンロード
	void Unload();
public:
	void Update();
	void Draw();
private:
	struct Box
	{
		int x;
		int y;
		int x2;
		int y2;

		int color;
		bool fill;
	};

//	std::unique_ptr<SlideSelect> m_pSlide;
	SlideSelect* m_pSlide[3];

	// アルファブレンド率
	int m_blendModeRate[2];

	std::vector<Box> m_box;
};


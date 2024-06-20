#pragma once

#include <memory>
#include <vector>

class SceneBase;
class SceneManager final
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void End();

	void Update();
	void Draw();
public:
	void StartFade();
private:
	void InitFade();
	void UpdateFadeIn();
	void UpdateFadeOut();
	void DrawFade();
private:
	std::unique_ptr<SceneBase>	m_pScene;
	SceneBase* m_pTempScene;

	bool m_isLoading;

	int m_hFade[26];
	bool m_isSceneSet;

	// フェイドをスタートする
	bool m_isFade;
	bool m_isInitFade;

	int m_blendRate;
	bool m_fadeIn = false;
	bool m_fadeOut = false;
	bool m_isInit = false;
};
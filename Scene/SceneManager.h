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
private:
	void InitFade();
	void UpdateFade();
	void DrawFade();
private:
	std::unique_ptr<SceneBase>	m_pScene;
	SceneBase* m_pTempScene;

	int m_handle[2];

	std::vector<float> m_x[2];
	std::vector<float> m_y[2];
	std::vector<float> m_rota;
	std::vector<bool> m_isRota;
	bool m_isReverce[2];

	bool m_isLoading;

	int m_hFade[26];
	int m_animFadeFrameCount;
	int m_animArray;
//	float size[2][6][26];
	bool m_isSceneSet;

#if _DEBUG
	int m_updateTime = 0;
	int m_drawTime = 0;
#endif	
};
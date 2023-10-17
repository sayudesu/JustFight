#pragma once

#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void Init();
	void End();

	void Update();
	void Draw();
private:
	std::unique_ptr<SceneBase>	m_pScene;
};
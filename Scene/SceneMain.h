#pragma once
#include "SceneBase.h"
#include <memory>

class Camera;
class Player;
class Enemy;

class SceneMain : public SceneBase
{
public:
	SceneMain();
	virtual ~SceneMain();

	virtual void Init() override;
	virtual void End() override;

	virtual SceneBase* Update() override;
	virtual void Draw() override;
private:
	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Enemy>  m_pEnemy;
};
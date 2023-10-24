#pragma once
#include "SceneBase.h"
#include <memory>

namespace
{
	constexpr int kEffectNum = 2;
}

class Camera;
class Player;
class Enemy;
class Collision3D;
class Effekseer3DDrawer;

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
	bool CheckHitEnemy();
	bool CheckHitPlayer();
	bool CheckHItEnemySield();
	bool CheckHItPlayerSield();
private:
	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<Player> m_pPlayer;
	std::unique_ptr<Enemy>  m_pEnemy;
	std::unique_ptr<Collision3D>  m_pColl;

	std::unique_ptr<Effekseer3DDrawer> m_pEffect[kEffectNum];
};
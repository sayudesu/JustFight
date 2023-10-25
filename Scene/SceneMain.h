#pragma once
#include "SceneBase.h"
#include <memory>

namespace
{
	constexpr int kCharacterNum = 2;

	constexpr int kEffectNum = 2;
}

class Camera;
class CharacterBase;
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
	// 当たり判定
	bool CheckWeaponAndBodyHit(CharacterBase* chara1, CharacterBase* chara2);
	bool CheckWeaponAndSieldHIt(CharacterBase* chara1, CharacterBase* chara2);
	// キャラクターの更新処理
	void UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2);
private:
	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	std::unique_ptr<Collision3D>  m_pColl;

	std::unique_ptr<Effekseer3DDrawer> m_pEffect[kEffectNum];
};
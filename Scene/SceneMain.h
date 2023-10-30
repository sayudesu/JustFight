#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include "../CharacterName.h"

namespace
{
	constexpr int kCharacterNum = 2;

	constexpr int kEffectNum = 3;
}

class Camera;
class CharacterBase;
class Collision3D;
class Effekseer3DDrawer;
class BloodDrawer;
class FIeldDrawer;

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
	// 武器と体の判定
	bool CheckWeaponAndBodyHit(CharacterBase* chara1, CharacterBase* chara2);
	// 武器と盾の判定
	bool CheckWeaponAndSieldHIt(CharacterBase* chara1, CharacterBase* chara2);
	// 武器と体範囲の判定
	bool CheckWeaponAndModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2);
	// 体範囲と体範囲の判定
	bool CheckModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2);

	// キャラクターの更新処理
	void UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2);
private:
	// カメラクラス
	std::unique_ptr<Camera> m_pCamera;
	// キャラクタークラス
	std::unique_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	// 判定クラス
	std::unique_ptr<Collision3D>  m_pColl;
	// エフェクトクラス
	std::unique_ptr<Effekseer3DDrawer> m_pEffect[kEffectNum];
	// 血描画クラス
	std::vector<BloodDrawer*>m_pBlood;
	// フィールドクラス
	std::unique_ptr<FIeldDrawer>m_pField;

	CharacterName m_name;
};
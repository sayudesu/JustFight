#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include "../Util/CharacterName.h"

namespace
{
	constexpr int kCharacterNum = 2;

	constexpr int kEffectNum = 3;
}

class Camera;
class CharacterBase;
class BloodDrawer;
class FieldDrawer;

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
	bool CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// 武器と盾の判定
	bool CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// 武器と体範囲の判定
	bool CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// 体範囲と体範囲の判定
	bool CheckModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);

	// 地面のモデルとプレイヤーの判定
	void CheckCollMap(std::shared_ptr<CharacterBase> character);

	// キャラクターの更新処理
	void UpdateCharacter(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
private:
	// カメラクラス
	std::unique_ptr<Camera> m_pCamera;
	// キャラクタークラス
	std::shared_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	// 血描画クラス
	std::vector<BloodDrawer*>m_pBlood;
	// フィールドクラス
	std::unique_ptr<FieldDrawer>m_pField; 
};
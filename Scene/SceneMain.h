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
class UIDrawer;
class EffectScreen;

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
	// シーン遷移
	// ゲームメイン
	SceneBase* UpdateGamePlay();
	// ゲームオーバー
	SceneBase* UpdateGameOver();
	// ゲームクリア
	SceneBase* UpdateGameClear();
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
	bool CheckCollMap(std::shared_ptr<CharacterBase> character);

	// キャラクターの更新処理
	void UpdateCharacter(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2,bool isPlayer);
private:
	// メンバー関数ポインタ
	SceneBase* (SceneMain::* m_pUpdateFunc)() = &SceneMain::UpdateGamePlay;
	// カメラクラス
	std::unique_ptr<Camera> m_pCamera;
	// キャラクタークラス
	std::shared_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	// 血描画クラス
	std::vector<BloodDrawer*>m_pBlood;
	// フィールドクラス
	std::unique_ptr<FieldDrawer>m_pField; 
	// UIクラス
	std::unique_ptr<UIDrawer> m_pUi;

	// テスト
	EffectScreen* m_pBS = nullptr;

	int m_quakeTimer = 0;//画面揺れタイマー
	float m_quakeX = 0.0f;//横揺れパラメータ
	int m_tempScreen;
};
#pragma once
#include <memory>
#include <vector>

#include "SceneBase.h"

#include "../Util/CharacterName.h"

#include "../DifficultyData.h"
#include "../GameResultData.h"

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
	SceneMain(DifficultyData data);
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
	SceneBase* UpdateGameResult();
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

	// 勝敗はついた場合の画像
	int m_hCheckmate;

	// 勝敗の結果
	GameResultData m_resultData;
	// クリア後カウント
	int m_frameCount;
};
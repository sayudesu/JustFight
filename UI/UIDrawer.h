#pragma once
#include "../Util/CharacterParameter.h";
#include "../Util/CharacterName.h";
#include <vector>
#include <memory>

class GameObject;
class UIDrawer
{
public:
	UIDrawer();
	~UIDrawer();

	void Draw();
public:
	/// <summary>
	/// パラメーターを受け取ります
	/// </summary>
	/// <param name="name">名前を指定</param>
	/// <param name="param">名前で指定したキャラのパラメーター</param>
	void SetParam(CharacterName name, int hpNum, float skillNum, int fightMeterNum);
private:
	void UpdateHp(int x,int y);
	void UpdateSkill(int x, int y);
	void DrawAreaResult(int x,int y,int x1,int y1);
	void DrawAreaALL();
private:
	enum class HandleType
	{
		BG,// 背景

		CHARACTOR,// キャラクターの画像

		HP,   // 体力
		HP_BG,// 体力の背景

		SP,   // スキル
		SP_BG,// スキルの背景

		BAR_OUTSIDE,// 枠

		FIGHT_POWER,       // 体幹ゲージ
		FIGHT_POWER_CENTER,// 体幹ゲージの中心
		FIGHT_POWER_BG,    // 体幹背景
		FIGHT_OUTSIDE,     // 体感ゲージ枠

		MAX,// 最大数
	};

	// 画像用ハンドル
	int m_handle[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::MAX)];

	// キャラクターごとのパラメーター
	int m_hpNum[static_cast<int>(CharacterName::MAX)];
	int m_skillNum[static_cast<int>(CharacterName::MAX)];
	int m_fightMeterNum[static_cast<int>(CharacterName::MAX)];

	std::unique_ptr<GameObject> m_image[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::MAX)];

	VECTOR m_playerPos;
};


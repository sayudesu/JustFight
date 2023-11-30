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
	/// �p�����[�^�[���󂯎��܂�
	/// </summary>
	/// <param name="name">���O���w��</param>
	/// <param name="param">���O�Ŏw�肵���L�����̃p�����[�^�[</param>
	void SetParam(CharacterName name, int hpNum, float skillNum, int fightMeterNum);
private:
	void UpdateHp(int x,int y);
	void UpdateSkill(int x, int y);
	void DrawAreaResult(int x,int y,int x1,int y1);
	void DrawAreaALL();
private:
	enum class HandleType
	{
		BG,// �w�i

		CHARACTOR,// �L�����N�^�[�̉摜

		HP,   // �̗�
		HP_BG,// �̗͂̔w�i

		SP,   // �X�L��
		SP_BG,// �X�L���̔w�i

		BAR_OUTSIDE,// �g

		FIGHT_POWER,       // �̊��Q�[�W
		FIGHT_POWER_CENTER,// �̊��Q�[�W�̒��S
		FIGHT_POWER_BG,    // �̊��w�i
		FIGHT_OUTSIDE,     // �̊��Q�[�W�g

		MAX,// �ő吔
	};

	// �摜�p�n���h��
	int m_handle[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::MAX)];

	// �L�����N�^�[���Ƃ̃p�����[�^�[
	int m_hpNum[static_cast<int>(CharacterName::MAX)];
	int m_skillNum[static_cast<int>(CharacterName::MAX)];
	int m_fightMeterNum[static_cast<int>(CharacterName::MAX)];

	std::unique_ptr<GameObject> m_image[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::MAX)];

	VECTOR m_playerPos;
};


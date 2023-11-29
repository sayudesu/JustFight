#pragma once
#include "../Util/CharacterParameter.h";
#include "../Util/CharacterName.h";
#include <vector>

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
	void SetParam(CharacterName name, CharacterParameter param);
private:
	enum class HandleType
	{
		BG,// �w�i

		CHARACTOR,// �L�����N�^�[�̉摜

		HP,// �̗�
		HP_BG,// �̗͂̔w�i

		SP,// �X�L��
		SP_BG,// �X�L���̔w�i

		BAR_OUTSIDE,// �g

		Max,// �ő吔
	};
	// �摜�p�n���h��
	int m_handle[static_cast<int>(CharacterName::MAX)][static_cast<int>(HandleType::Max)];
};


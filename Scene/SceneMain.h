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
	// �����蔻��
	// ����Ƒ̂̔���
	bool CheckWeaponAndBodyHit(CharacterBase* chara1, CharacterBase* chara2);
	// ����Ə��̔���
	bool CheckWeaponAndSieldHIt(CharacterBase* chara1, CharacterBase* chara2);
	// ����Ƒ͈̔͂̔���
	bool CheckWeaponAndModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2);
	// �͈̔͂Ƒ͈̔͂̔���
	bool CheckModelAboutHIt(CharacterBase* chara1, CharacterBase* chara2);

	// �L�����N�^�[�̍X�V����
	void UpdateCharacter(CharacterBase* chara1, CharacterBase* chara2);
private:
	// �J�����N���X
	std::unique_ptr<Camera> m_pCamera;
	// �L�����N�^�[�N���X
	std::unique_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	// ����N���X
	std::unique_ptr<Collision3D>  m_pColl;
	// ���`��N���X
	std::vector<BloodDrawer*>m_pBlood;
	// �t�B�[���h�N���X
	std::unique_ptr<FIeldDrawer>m_pField;

	// �I�u�W�F�N�g�̖��O���m�F����
	CharacterName m_name;

	int handle;
};
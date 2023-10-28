#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

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
class Animation2D;
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
	// �L�����N�^�[�N���X�̈ꎞ�ۑ�
	CharacterBase* m_pTempCharacter;
	// ����N���X
	std::unique_ptr<Collision3D>  m_pColl;
	// �G�t�F�N�g�N���X
	std::unique_ptr<Effekseer3DDrawer> m_pEffect[kEffectNum];
	// ���`��N���X
	std::vector<BloodDrawer*>m_pBlood;
	// �X�^���`��N���X
	std::unique_ptr<Animation2D>m_pStun;
	// �t�B�[���h�N���X
	std::unique_ptr<FIeldDrawer>m_pField;
};
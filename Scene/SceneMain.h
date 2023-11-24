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
	// �V�[���J��
	// �Q�[�����C��
	SceneBase* UpdateGamePlay();
	// �Q�[���I�[�o�[
	SceneBase* UpdateGameOver();
	// �Q�[���N���A
	SceneBase* UpdateGameClear();
private:
	// �����蔻��
	// ����Ƒ̂̔���
	bool CheckWeaponAndBodyHit(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// ����Ə��̔���
	bool CheckWeaponAndShieldHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// ����Ƒ͈̔͂̔���
	bool CheckWeaponAndModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// �͈̔͂Ƒ͈̔͂̔���
	bool CheckModelAboutHIt(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
	// �n�ʂ̃��f���ƃv���C���[�̔���
	bool CheckCollMap(std::shared_ptr<CharacterBase> character);

	// �L�����N�^�[�̍X�V����
	void UpdateCharacter(std::shared_ptr<CharacterBase> chara1, std::shared_ptr<CharacterBase> chara2);
private:
	// �����o�[�֐��|�C���^
	SceneBase* (SceneMain::* m_pUpdateFunc)() = &SceneMain::UpdateGamePlay;
	// �J�����N���X
	std::unique_ptr<Camera> m_pCamera;
	// �L�����N�^�[�N���X
	std::shared_ptr<CharacterBase> m_pCharacter[kCharacterNum];
	// ���`��N���X
	std::vector<BloodDrawer*>m_pBlood;
	// �t�B�[���h�N���X
	std::unique_ptr<FieldDrawer>m_pField; 
};
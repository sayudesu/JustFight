#pragma once
#include <DxLib.h>

class CharacterBase;

/// <summary>
/// �w�i���̕`��ƍX�V
/// </summary>
class Stage
{
public:
	Stage();
	~Stage();

	int GetModelHandle() const { return modelHandle; }

	void Load();			// ����������
	void Unload();			// ��n������

	// �����蔻������āA�␳��̃|�W�V������Ԃ�
	VECTOR CheckCollision(CharacterBase& player, const VECTOR& moveVector);

	void Draw();

private:
	static const int MaxHitColl = 2048;	// ��������R���W�����|���S���̍ő吔

	static constexpr float	DefaultSize = 800.0f;	// ���͂̃|���S�����o�Ɏg�p���鋅�̏����T�C�Y
	static constexpr float	HitWidth = 200.0f;	// �����蔻��J�v�Z���̔��a
	static constexpr float	HitHeight = 700.0f;	// �����蔻��J�v�Z���̍���
	static constexpr int	HitTryNum = 16;		// �ǉ����o�������̍ő厎�s��
	static constexpr float	HitSlideLength = 5.0f;		// ��x�̕ǉ����o�������ŃX���C�h�����鋗��

	int							modelHandle;		// ���f���n���h��

	// HACK: �ǂ�XZ���ʂɐ����ł���O��Ő��藧���Ă���B����ȊO��u���ƃo�O��
	int							wallNum;			// �ǃ|���S���Ɣ��f���ꂽ�|���S���̐�
	int							floorNum;			// ���|���S���Ɣ��f���ꂽ�|���S���̐�

	MV1_COLL_RESULT_POLY* wall[MaxHitColl];	// �ǃ|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��
	MV1_COLL_RESULT_POLY* floor[MaxHitColl];	// ���|���S���Ɣ��f���ꂽ�|���S���̍\���̂̃A�h���X��ۑ����Ă������߂̃|�C���^�z��

	// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f���A�ۑ�����
	void AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition);

	// �ǃ|���S���Ƃ̓�������`�F�b�N���A�␳���ׂ��ړ��x�N�g����Ԃ�
	VECTOR CheckHitWithWall(CharacterBase& player, const VECTOR& checkPosition);

	// ���|���S���Ƃ̓�������`�F�b�N���A�␳���ׂ��ړ��x�N�g����Ԃ�
	VECTOR CheckHitWithFloor(CharacterBase& player, const VECTOR& checkPosition);
};


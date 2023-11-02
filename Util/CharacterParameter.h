#pragma once
#include <DxLib.h>
const struct CharacterParameter
{
	int attackFrameMax;   // �U���ő�t���[��
	int attackFrameGapMax;// �U���\���̍ő�t���[��
	int attackRotalFrame; // �U���t���[���̍��v

	int strongAttackFrameMax;   // ���U���ő�t���[��
	int strongAttackFrameGapMax;// ���U���\���̍ő�t���[��
	int strongAttackRotalFrame; // ���U���t���[���̍��v

	int guardFrameMax;    // �K�[�h�ő�t���[��
	int justGuardFrameMax;// �W���X�g�K�[�h�ő�t���[��

	int stunFrameMax;// �X�^����Ԃ̍ő�t���[��

	int hpMax;           // �ő�̗�
	int fightingMeterMax;// �ő�퓬�ɕK�v�ȃ��[�^�[�̍ő�l

	VECTOR weaponRelativePos;// ����
	VECTOR sieldRelativePos; // ��

	float weaponAttackRadius;// ����̔��a
	float sieldRadius;       // ���̔��a
	float modelRadius;       // ���f���̓����蔻��p�͈�

	VECTOR weaponAttackPos;// ����̍U�����̔���̑��Έʒu
	VECTOR knockBackPos;  // �K�[�h���Ă��鎞�ɍU�����󂯂��ꍇ�̃m�b�N�o�b�N�ňړ����鑊�Έʒu

	float weaponBackSpeed;// �������̈ʒu�ɖ߂鑬�x
	float sieldBackSpeed; // �������̈ʒu�ɖ߂鑬�x
}; 
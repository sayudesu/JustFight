#pragma once

// �퓬���
enum class BattleState
{
	NONE,         // ��
	IDLE,         // �ҋ@���
	JUMP,         // �W�����v���
	ATTACK,       // �U��
	ATTACKTWO,    // �R���{�U��
	STRONGATTACK, // �����U��
	GUARD,        // �h��
	JUSTGUARD,    // �h��
	STUN,         // �X�^��
};
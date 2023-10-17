#include "CharacterBase.h"

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_lanceHnadle(-1),
	m_shieldHnadle(-1),
	m_pos(pos),
	m_vec(VGet(0,0,0))
{
	m_pFunc = &CharacterBase::UpdateInput;
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	m_lanceHnadle  = MV1LoadModel("Data/Model/Lance.mv1");
	m_shieldHnadle = MV1LoadModel("Data/Model/Shield.mv1");
	MV1SetPosition(m_lanceHnadle,  VGet(0, 0, 0));
	MV1SetPosition(m_shieldHnadle, VGet(0, 0, 0));

	MV1SetScale(m_shieldHnadle, VGet(3, 3, 3));

}

void CharacterBase::End()
{
	MV1DeleteModel(m_lanceHnadle);
	MV1DeleteModel(m_shieldHnadle);
}

void CharacterBase::Update()
{
	(this->*m_pFunc)();
}

void CharacterBase::UpdateInput()
{
}

void CharacterBase::Attack()
{
}

void CharacterBase::Guard()
{
}

void CharacterBase::Draw()
{
	// ‘Ì
	DrawCapsule3D(m_pos, VGet(m_pos.x, m_pos.y + 200.0f, m_pos.z), 40.0f, 8, 0x00ff00, 0xffffff, true);
	// 
	MV1DrawModel(m_lanceHnadle);
	// 
	MV1DrawModel(m_shieldHnadle);
}

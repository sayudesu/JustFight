#include "CharacterBase.h"

CharacterBase::CharacterBase(VECTOR pos):
	m_pFunc(nullptr),
	m_pos(pos),
	m_vec(VGet(0,0,0))
{
}

CharacterBase::~CharacterBase()
{
}

void CharacterBase::Init()
{
	m_pFunc = &CharacterBase::UpdateInput;
}

void CharacterBase::End()
{
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
	DrawCapsule3D(m_pos, VGet(m_pos.x, m_pos.y + 300.0f, m_pos.z), 40.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), true);
}

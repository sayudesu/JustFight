#pragma once
#include <DxLib.h>
#include <string>
#include <cmath>

#include "Util/MyLoadModel.h";

class GameObject
{
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="name"></param>
    /// <param name="pos"></param>
    /// <param name="angle"></param>
    /// <param name="size"></param>
    /// <param name="parent"></param>
    GameObject(const TCHAR* name, const VECTOR& pos, VECTOR angle, VECTOR size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle(angle), parent(parent)
    {
        m_handle = MV1LoadModel(name);
        MV1SetScale(m_handle, size);
    }
    ~GameObject()
    {
        MV1DeleteModel(m_handle);
    }

    void Update()
    {
        VECTOR relativePos = m_pos;
        if (parent != nullptr && (!m_isParentEscape))
        {
            // 親オブジェクトが存在する場合、親の座標と角度に基づいて相対的な位置を計算
            MATRIX rotationMatrixX = MGetRotX(m_angle.x);
            MATRIX rotationMatrixY = MGetRotY(m_angle.y);
            MATRIX rotationMatrixZ = MGetRotZ(m_angle.z);

            // 回転行列を順番に掛けて相対的な位置を計算
            MATRIX combinedRotationMatrix = MMult(MMult(rotationMatrixX, rotationMatrixY), rotationMatrixZ);
            relativePos = VTransform(relativePos, combinedRotationMatrix);

            relativePos = VAdd(relativePos, parent->GetPos());

            // 親オブジェクトが存在する場合、親の座標に基づいて相対的な位置を計算
            relativePos = VSub(m_pos, parent->GetPos());
            relativePos = VTransform(m_pos, MMult(MMult(MGetRotX(parent->GetAngle().x), MGetRotY(parent->GetAngle().y)), MGetRotZ(parent->GetAngle().z)));
            m_childPos = VAdd(relativePos, parent->GetPos());

        }

        MV1SetPosition(m_handle, m_pos);
        MV1SetRotationXYZ(m_handle, m_angle);
        m_tempPos = m_pos;
        if (parent != nullptr && (!m_isParentEscape))
        {
            MV1SetPosition(m_handle, m_childPos);
            m_tempPos = m_childPos;
        }
    }

    void Draw()
    {
        MV1SetRotationXYZ(m_handle, m_angle);
        DrawSphere3D(m_tempPos, 32.0f, 16, GetColor(255, 0, 0), GetColor(128, 0, 0), TRUE);
        MV1DrawModel(m_handle);
    }

    void Move(VECTOR pos)
    {
        m_pos = pos;
    }

    void Rotate(VECTOR angle)
    {
        m_angle = angle;
    }

    VECTOR GetPos()
    {
        return m_pos;
    }

    VECTOR GetAngle()
    {
        return m_angle;
    }

    void SetParentEscape(bool isEscape)
    {
        m_isParentEscape = isEscape;
    }

private:
    std::string name = {};
    int m_handle = -1;
    VECTOR m_pos = { 0,0,0 };
    VECTOR m_childPos = { 0,0,0 };
    VECTOR m_tempPos = { 0,0,0 };
    VECTOR angle = { 0,0,0 };
    VECTOR m_angle = { 0,0,0 };
    VECTOR m_size = {0,0,0};
    GameObject* parent = nullptr;
    bool m_isParentEscape = false;
};


#pragma once
#include <DxLib.h>
#include <string>
#include "Util/MyLoadModel.h";

class GameObject
{
public:
    /// <summary>
    /// オブジェクトを作る為に初期情報を指定します
    /// </summary>
    /// <param name="name"></param>
    /// <param name="pos"></param>
    /// <param name="angle"></param>
    /// <param name="size"></param>
    GameObject(std::string name, VECTOR pos, VECTOR angle, VECTOR size) :
        name(name), m_pos(pos), m_angle(angle), m_size(size)
    {
        m_handle = MyModel3D::Load(name.c_str());
        MV1SetScale(m_handle, size);
    }
    ~GameObject()
    {
        MyModel3D::End(m_handle);
    }

    void Move(VECTOR pos) 
    {
        m_pos = pos;
        MV1SetPosition(m_handle, m_pos);
    }

    void Rotate(VECTOR angle)
    {
        m_angle = angle;
        MV1SetRotationXYZ(m_handle, m_angle);
    }

    void Draw()
    {

        DrawFormatString(10, 10, GetColor(255, 255, 255),
            "%s: X=%.2f, Y=%.2f, Z=%.2f, AngleX=%.2f, AngleY=%.2f, AngleZ=%.2f",
            name.c_str(), m_pos.x, m_pos.y, m_pos.z, m_angle.x, m_angle.y, m_angle.z);

        MV1DrawModel(m_handle);
    }

private:
    std::string name;
    int m_handle;
    VECTOR m_pos;
    VECTOR m_angle;
    VECTOR m_size;
};


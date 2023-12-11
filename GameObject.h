#pragma once
#include <DxLib.h>
#include <string>
#include <cmath>

#include "Util/MyLoadModel.h";

class GameObject
{
public:
    enum DataType
    {
        THREE,
        TWO,
    };
public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="name"></param>
    /// <param name="pos"></param>
    /// <param name="angle"></param>
    /// <param name="size"></param>
    /// <param name="parent"></param>
    GameObject(DataType type ,std::string name, const VECTOR& pos, VECTOR angle, VECTOR size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle(angle), parent(parent)
    {
        // 3D�I�u�W�F�N�g��������
        if (type == DataType::THREE)
        {
            m_handle = MV1LoadModel(name.c_str());
            MV1SetScale(m_handle, size);
        }
        // 2D�I�u�W�F�N�g��������
        else if (type == DataType::TWO)
        {
            m_handle = LoadGraph(name.c_str());
        }
        m_dataType = type;
    }

    ~GameObject()
    {
        if (m_dataType == DataType::THREE)
        {
            MV1DeleteModel(m_handle);
        }
        else if (m_dataType == DataType::TWO)
        {
            DeleteGraph(m_handle);
        }
    }

    void Update()
    {
        VECTOR relativePos = m_pos;
        if (parent != nullptr && (!m_isParentEscape))
        {
            // �e�I�u�W�F�N�g�����݂���ꍇ�A�e�̍��W�Ɗp�x�Ɋ�Â��đ��ΓI�Ȉʒu���v�Z
            MATRIX rotationMatrixX = MGetRotX(m_angle.x);
            MATRIX rotationMatrixY = MGetRotY(m_angle.y);
            MATRIX rotationMatrixZ = MGetRotZ(m_angle.z);

            // ��]�s������ԂɊ|���đ��ΓI�Ȉʒu���v�Z
            MATRIX combinedRotationMatrix = MMult(MMult(rotationMatrixX, rotationMatrixY), rotationMatrixZ);
            relativePos = VTransform(relativePos, combinedRotationMatrix);

            relativePos = VAdd(relativePos, parent->GetPos());

            // �e�I�u�W�F�N�g�����݂���ꍇ�A�e�̍��W�Ɋ�Â��đ��ΓI�Ȉʒu���v�Z
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
#if false
       // ���C���ʒu
       DrawSphere3D(m_tempPos, 32.0f, 16, GetColor(0, 0, 255), GetColor(128, 0, 0), false);
       DrawSphere3D(MV1GetFramePosition(m_handle, 3), 32.0f, 16, GetColor(255, 0, 0), GetColor(128, 0, 0), false);
#endif
       // �`��
       if (m_dataType == DataType::THREE)
       {
           MV1DrawModel(m_handle);  
       }
       else if (m_dataType == DataType::TWO)
       {
           DrawGraph(m_tempPos.x, m_tempPos.y, m_handle, true);
       }
       
    }

    // �ʒu���󂯎��
    void SetPos(VECTOR pos)
    {
        m_pos = pos;
    }

    // �p�x���󂯎��
    void SetRotate(VECTOR angle)
    {
        m_angle = angle;
    }

    // �n���h���f�[�^��n��
    int GetHandle()
    {
        return m_handle;
    }

    // �ʒu��n��
    VECTOR GetPos()
    {
        return m_pos;
    }

    // �p�x��n��
    VECTOR GetAngle()
    {
        return m_angle;
    }

    // ���f���̃t���[���C���f�b�N�X���󂯎�肻�̈ʒu��Ԃ�
    VECTOR GetCollPos(int modelFrameIndex)
    {
        return MV1GetFramePosition(m_handle, modelFrameIndex);
    }

    // �e�I�u�W�F�N�g���甲���o��
    void SetParentEscape(bool isEscape)
    {
        if (parent != nullptr)
        {
            m_isParentEscape = isEscape;
            return;
        }
        return;
    }

private:
    int m_handle = -1;
    VECTOR m_pos = { 0,0,0 };
    VECTOR m_childPos = { 0,0,0 };
    VECTOR m_tempPos = { 0,0,0 };
    VECTOR angle = { 0,0,0 };
    VECTOR m_angle = { 0,0,0 };
    VECTOR m_size = {0,0,0};
    GameObject* parent = nullptr;
    bool m_isParentEscape = false;

    DataType m_dataType;
};


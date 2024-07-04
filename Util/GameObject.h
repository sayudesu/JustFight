#pragma once
#include <DxLib.h>
#include <string>
#include <cmath>

class GameObject
{
private:
    enum class DataType
    {
        THREE,
        TWO,
    };
public:

    // 3D���f���̏ꍇ�̃R���X�g���N�^ �I�[�o�[���[�h
    GameObject(std::string name, const VECTOR& pos, VECTOR angle, VECTOR size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle(angle), m_pParent(parent)
    {        
        m_dataType = DataType::THREE;

        // ���f���̓ǂݍ���
        m_handle = MV1LoadModel(name.c_str());
        // �ʒu�̒���
        MV1SetPosition(m_handle, pos);
        // �T�C�Y�̒���
        MV1SetScale(m_handle, size);        
    }

    // 2D���f���̏ꍇ�̃R���X�g���N�^ �I�[�o�[���[�h
    GameObject(std::string name, const VECTOR& pos, float angle, float size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle2D(angle), m_pParent(parent)
    {
        m_dataType = DataType::TWO;

        // �摜�̓ǂݍ���
        m_handle = LoadGraph(name.c_str());
        // �摜�T�C�Y���擾����
        GetGraphSizeF(m_handle, &m_sizeX, &m_sizeY);
        // �T�C�Y�̒���
        m_size2D = size;
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

        // �e�I�u�W�F�N�g�������ꍇ�A�e�I�u�W�F�N�g�𔲂���ꍇ
        if (m_pParent != nullptr && (!m_isParentEscape))
        {
            // �e�I�u�W�F�N�g�����݂���ꍇ�A�e�̍��W�Ɗp�x�Ɋ�Â��đ��ΓI�Ȉʒu���v�Z
            MATRIX rotationMatrixX = MGetRotX(m_angle.x);
            MATRIX rotationMatrixY = MGetRotY(m_angle.y);
            MATRIX rotationMatrixZ = MGetRotZ(m_angle.z);

            // ��]�s������ԂɊ|���đ��ΓI�Ȉʒu���v�Z
            MATRIX combinedRotationMatrix = MMult(MMult(rotationMatrixX, rotationMatrixY), rotationMatrixZ);
            relativePos = VTransform(relativePos, combinedRotationMatrix);

            relativePos = VAdd(relativePos, m_pParent->GetPos());

            // �e�I�u�W�F�N�g�����݂���ꍇ�A�e�̍��W�Ɋ�Â��đ��ΓI�Ȉʒu���v�Z
            relativePos = VSub(m_pos, m_pParent->GetPos());
            relativePos = VTransform(m_pos, MMult(MMult(MGetRotX(m_pParent->GetAngle().x), MGetRotY(m_pParent->GetAngle().y)), MGetRotZ(m_pParent->GetAngle().z)));
            m_childPos = VAdd(relativePos, m_pParent->GetPos());

        }

        // ���f���̈ʒu���w��
        MV1SetPosition(m_handle, m_pos);
        // ���f���̊p�x���w��
        MV1SetRotationXYZ(m_handle, m_angle);

        // �ʒu���ꎞ�I�ɋL�^
        m_tempPos = m_pos;

        // �e�I�u�W�F�N�g�������ꍇ�A�e�I�u�W�F�N�g�𔲂���ꍇ
        if (m_pParent != nullptr && (!m_isParentEscape))
        {
            // �q�I�u�W�F�N�g�̈ʒu
            MV1SetPosition(m_handle, m_childPos);
            // �ʒu���ꎞ�I�ɋL�^
            m_tempPos = m_childPos;
        }
    }

    void Draw()
    {       
       // �`��
       if (m_dataType == DataType::THREE)
       {
           MV1DrawModel(m_handle);  
       }
       else if (m_dataType == DataType::TWO)
       {
           DrawRotaGraphF(m_tempPos.x, m_tempPos.y, m_size2D, m_angle2D, m_handle, true);
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

    // �T�C�Y���󂯎��
    void SetSize(float size)
    {
        m_size2D = size;
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

    float GetSize2D()
    {
        return m_size2D;
    }

    // 2D�摜��X�̃T�C�Y��n��
    float GetGraphSizeX()
    {
        if (m_dataType == DataType::TWO)
        {
            return m_sizeX;
        }

        return -1;
    }

    // 2D�摜��Y�̃T�C�Y��n��
    float GetGraphSizeY()
    {
        if (m_dataType == DataType::TWO)
        {
            return m_sizeY;
        }

        return -1;
    }

    // ���f���̃t���[���C���f�b�N�X���󂯎�肻�̈ʒu��Ԃ�
    VECTOR GetCollPos(int modelFrameIndex)
    {
        return MV1GetFramePosition(m_handle, modelFrameIndex);
    }

    // �e�I�u�W�F�N�g���甲���o��
    void SetParentEscape(bool isEscape)
    {
        if (m_pParent != nullptr)
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
    VECTOR m_angle = { 0,0,0 };
    VECTOR m_size = {0,0,0};
    GameObject* m_pParent = nullptr;
    bool m_isParentEscape = false;

    // 2D�p
    float m_angle2D = 0;
    float m_size2D = 0;
    float m_sizeX = 0.0f;
    float m_sizeY = 0.0f;

    DataType m_dataType{};
};


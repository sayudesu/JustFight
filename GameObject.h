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
        m_pos(pos), m_angle(angle), m_pParent(parent)
    {
        // 3Dオブジェクトだったら
        if (type == DataType::THREE)
        {
            m_handle = MV1LoadModel(name.c_str());
            MV1SetScale(m_handle, size);
        }
        // 2Dオブジェクトだったら
        else if (type == DataType::TWO)
        {
            m_handle = LoadGraph(name.c_str());
        }
        m_dataType = type;
    }

    GameObject(DataType type, std::string name, const VECTOR& pos, float angle, float size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle2D(angle), m_pParent(parent)
    {
        m_handle = LoadGraph(name.c_str());
        m_size2D = size;
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
        if (m_pParent != nullptr && (!m_isParentEscape))
        {
            // 親オブジェクトが存在する場合、親の座標と角度に基づいて相対的な位置を計算
            MATRIX rotationMatrixX = MGetRotX(m_angle.x);
            MATRIX rotationMatrixY = MGetRotY(m_angle.y);
            MATRIX rotationMatrixZ = MGetRotZ(m_angle.z);

            // 回転行列を順番に掛けて相対的な位置を計算
            MATRIX combinedRotationMatrix = MMult(MMult(rotationMatrixX, rotationMatrixY), rotationMatrixZ);
            relativePos = VTransform(relativePos, combinedRotationMatrix);

            relativePos = VAdd(relativePos, m_pParent->GetPos());

            // 親オブジェクトが存在する場合、親の座標に基づいて相対的な位置を計算
            relativePos = VSub(m_pos, m_pParent->GetPos());
            relativePos = VTransform(m_pos, MMult(MMult(MGetRotX(m_pParent->GetAngle().x), MGetRotY(m_pParent->GetAngle().y)), MGetRotZ(m_pParent->GetAngle().z)));
            m_childPos = VAdd(relativePos, m_pParent->GetPos());

        }

        MV1SetPosition(m_handle, m_pos);
        MV1SetRotationXYZ(m_handle, m_angle);
        m_tempPos = m_pos;
        if (m_pParent != nullptr && (!m_isParentEscape))
        {
            MV1SetPosition(m_handle, m_childPos);
            m_tempPos = m_childPos;
        }
    }

    void Draw()
    {       
#if false
       // メイン位置
       DrawSphere3D(m_tempPos, 32.0f, 16, GetColor(0, 0, 255), GetColor(128, 0, 0), false);
       DrawSphere3D(MV1GetFramePosition(m_handle, 3), 32.0f, 16, GetColor(255, 0, 0), GetColor(128, 0, 0), false);
#endif
       // 描画
       if (m_dataType == DataType::THREE)
       {
           MV1DrawModel(m_handle);  
       }
       else if (m_dataType == DataType::TWO)
       {
           DrawRotaGraphF(m_tempPos.x, m_tempPos.y, m_size2D, m_angle2D, m_handle, true);
       //    DrawGraph(m_tempPos.x, m_tempPos.y, m_handle, true);
       }
       
    }

    // 位置を受け取る
    void SetPos(VECTOR pos)
    {
        m_pos = pos;
    }

    // 角度を受け取る
    void SetRotate(VECTOR angle)
    {
        m_angle = angle;
    }

    // サイズを受け取る
    void SetSize(float size)
    {
        m_size2D = size;
    }

    // ハンドルデータを渡す
    int GetHandle()
    {
        return m_handle;
    }

    // 位置を渡す
    VECTOR GetPos()
    {
        return m_pos;
    }

    // 角度を渡す
    VECTOR GetAngle()
    {
        return m_angle;
    }

    // モデルのフレームインデックスを受け取りその位置を返す
    VECTOR GetCollPos(int modelFrameIndex)
    {
        return MV1GetFramePosition(m_handle, modelFrameIndex);
    }

    // 親オブジェクトから抜け出す
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

    // 2D用
    float m_angle2D = 0;
    float m_size2D = 0;

    DataType m_dataType;
};


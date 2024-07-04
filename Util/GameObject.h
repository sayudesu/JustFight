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

    // 3Dモデルの場合のコンストラクタ オーバーロード
    GameObject(std::string name, const VECTOR& pos, VECTOR angle, VECTOR size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle(angle), m_pParent(parent)
    {        
        m_dataType = DataType::THREE;

        // モデルの読み込み
        m_handle = MV1LoadModel(name.c_str());
        // 位置の調整
        MV1SetPosition(m_handle, pos);
        // サイズの調整
        MV1SetScale(m_handle, size);        
    }

    // 2Dモデルの場合のコンストラクタ オーバーロード
    GameObject(std::string name, const VECTOR& pos, float angle, float size, GameObject* parent = nullptr) :
        m_pos(pos), m_angle2D(angle), m_pParent(parent)
    {
        m_dataType = DataType::TWO;

        // 画像の読み込み
        m_handle = LoadGraph(name.c_str());
        // 画像サイズを取得する
        GetGraphSizeF(m_handle, &m_sizeX, &m_sizeY);
        // サイズの調整
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

        // 親オブジェクトが無い場合、親オブジェクトを抜ける場合
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

        // モデルの位置を指定
        MV1SetPosition(m_handle, m_pos);
        // モデルの角度を指定
        MV1SetRotationXYZ(m_handle, m_angle);

        // 位置を一時的に記録
        m_tempPos = m_pos;

        // 親オブジェクトが無い場合、親オブジェクトを抜ける場合
        if (m_pParent != nullptr && (!m_isParentEscape))
        {
            // 子オブジェクトの位置
            MV1SetPosition(m_handle, m_childPos);
            // 位置を一時的に記録
            m_tempPos = m_childPos;
        }
    }

    void Draw()
    {       
       // 描画
       if (m_dataType == DataType::THREE)
       {
           MV1DrawModel(m_handle);  
       }
       else if (m_dataType == DataType::TWO)
       {
           DrawRotaGraphF(m_tempPos.x, m_tempPos.y, m_size2D, m_angle2D, m_handle, true);
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

    float GetSize2D()
    {
        return m_size2D;
    }

    // 2D画像のXのサイズを渡す
    float GetGraphSizeX()
    {
        if (m_dataType == DataType::TWO)
        {
            return m_sizeX;
        }

        return -1;
    }

    // 2D画像のYのサイズを渡す
    float GetGraphSizeY()
    {
        if (m_dataType == DataType::TWO)
        {
            return m_sizeY;
        }

        return -1;
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
    float m_sizeX = 0.0f;
    float m_sizeY = 0.0f;

    DataType m_dataType{};
};


using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TipsDrawer : MonoBehaviour
{
    // 画像データ.
    public Sprite _sprite; 
    // 下にスライドさせる場合の速度.
    public float _slideDownSpeed;
    // 上にスライドさせる場合の速度.
    public float _slideUpSpeed;

    // 画像クラスの初期化
    private Image _image = null;
    // トランスフォームクラスの初期化
    private RectTransform rectTransform = null;

    // スライド用の速度
    private float _slideSpeed = 0.0f;

    // スライドの方向
    private bool _isDownSlider = false;
    private bool _isUpSlider = false;

    void Start()
    {
        // Canvasを取得または作成.
        Canvas canvas = GetComponent<Canvas>();
        canvas = gameObject.AddComponent<Canvas>();

        // パネルにImageコンポーネントを追加.
        _image = gameObject.AddComponent<Image>();
        // 画像をアタッチ.
        _image.sprite = _sprite;

        // RectTransformを取得して設定.
        rectTransform = _image.rectTransform;

        // 16:9アスペクト比に設定.
        float targetAspectRatio = 16f / 9f;
        rectTransform.sizeDelta = new Vector2(1080, 1080 / targetAspectRatio); // 16:9アスペクト比を維持した高さ.
        // 画面外の上に配置.
        rectTransform.anchorMin        = new Vector2(0.5f, 0.5f);
        rectTransform.anchorMax        = new Vector2(0.5f, 0.5f);
        rectTransform.pivot            = new Vector2(0.5f, 0.5f);
        rectTransform.anchoredPosition = new Vector2(0.0f, 12.0f);
        rectTransform.localScale       = new Vector3(0.015f, 0.015f, 0.015f);
    }

    private void FixedUpdate()
    {
        
        // if 下にスライドの場合
        // else if 上にスライド
        if(_isDownSlider)
        {
            // Y軸で下にスライドさせるのでマイナス.
            _slideSpeed = (-_slideDownSpeed);
            // 画面の中心まで移動するとスライドを止める.
            if(rectTransform.anchoredPosition.y <= 1.0f)
            {
                _isDownSlider = false;
                _slideSpeed = 0.0f;
            }
        }
        else if(_isUpSlider)
        {
            _slideSpeed = _slideUpSpeed;
            if (rectTransform.anchoredPosition.y >= 12.0f)
            {
                _isUpSlider = false;
                _slideSpeed = 0.0f;
            }
        }


        // 画像をスライドさせる.
        rectTransform.anchoredPosition += new Vector2(0.0f, _slideSpeed);
         
    }

    // 下にスライドさせる場合.
    public void IsDownSlider()
    {
        _isDownSlider = true;
    }
    // 上にスライドさせる場合.
    public void IsUpSlider()
    {
        _isUpSlider = true;
    }
}

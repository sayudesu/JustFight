using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TipsDrawer : MonoBehaviour
{
    // �摜�f�[�^.
    public Sprite _sprite; 
    // ���ɃX���C�h������ꍇ�̑��x.
    public float _slideDownSpeed;
    // ��ɃX���C�h������ꍇ�̑��x.
    public float _slideUpSpeed;

    // �摜�N���X�̏�����
    private Image _image = null;
    // �g�����X�t�H�[���N���X�̏�����
    private RectTransform rectTransform = null;

    // �X���C�h�p�̑��x
    private float _slideSpeed = 0.0f;

    // �X���C�h�̕���
    private bool _isDownSlider = false;
    private bool _isUpSlider = false;

    void Start()
    {
        // Canvas���擾�܂��͍쐬.
        Canvas canvas = GetComponent<Canvas>();
        canvas = gameObject.AddComponent<Canvas>();

        // �p�l����Image�R���|�[�l���g��ǉ�.
        _image = gameObject.AddComponent<Image>();
        // �摜���A�^�b�`.
        _image.sprite = _sprite;

        // RectTransform���擾���Đݒ�.
        rectTransform = _image.rectTransform;

        // 16:9�A�X�y�N�g��ɐݒ�.
        float targetAspectRatio = 16f / 9f;
        rectTransform.sizeDelta = new Vector2(1080, 1080 / targetAspectRatio); // 16:9�A�X�y�N�g����ێ���������.
        // ��ʊO�̏�ɔz�u.
        rectTransform.anchorMin        = new Vector2(0.5f, 0.5f);
        rectTransform.anchorMax        = new Vector2(0.5f, 0.5f);
        rectTransform.pivot            = new Vector2(0.5f, 0.5f);
        rectTransform.anchoredPosition = new Vector2(0.0f, 12.0f);
        rectTransform.localScale       = new Vector3(0.015f, 0.015f, 0.015f);
    }

    private void FixedUpdate()
    {
        
        // if ���ɃX���C�h�̏ꍇ
        // else if ��ɃX���C�h
        if(_isDownSlider)
        {
            // Y���ŉ��ɃX���C�h������̂Ń}�C�i�X.
            _slideSpeed = (-_slideDownSpeed);
            // ��ʂ̒��S�܂ňړ�����ƃX���C�h���~�߂�.
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


        // �摜���X���C�h������.
        rectTransform.anchoredPosition += new Vector2(0.0f, _slideSpeed);
         
    }

    // ���ɃX���C�h������ꍇ.
    public void IsDownSlider()
    {
        _isDownSlider = true;
    }
    // ��ɃX���C�h������ꍇ.
    public void IsUpSlider()
    {
        _isUpSlider = true;
    }
}

#include <DxLib.h>

#include "Stage.h"

#include "Object/CharacterBase.h"

#include "Util/BattleState.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Stage::Stage()
	:modelHandle(-1),
	wallNum(0),
	floorNum(0),
	wall { nullptr },
	floor{ nullptr }
{
	// �����Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Stage::~Stage()
{
	Unload();
}

/// <summary>
/// ���[�h
/// </summary>
void Stage::Load()
{
	// �X�e�[�W���f���̓ǂݍ���
	modelHandle = MV1LoadModel("Map.mv1");
	//	modelHandle = MV1LoadModel("Arena.mv1");

	MV1SetScale(modelHandle, VGet(7, 7, 7));

	// ���f���S�̂̃R���W�������̃Z�b�g�A�b�v
	MV1SetupCollInfo(modelHandle, -1);
}

/// <summary>
/// �A�����[�h
/// </summary>
void Stage::Unload()
{
	// �X�e�[�W���f���̌�n��
	if (modelHandle >= 0)
	{
		MV1DeleteModel(modelHandle);
		modelHandle = -1;
	}
}

/// <summary>
/// �����蔻������āA�␳�����ړ���̃|�W�V������Ԃ�
/// </summary>
VECTOR Stage::CheckCollision(CharacterBase& model, const VECTOR& moveVector)
{
	VECTOR oldPos = model.GetPos();			// �ړ��O�̍��W	
	VECTOR nextPos = VAdd(oldPos, moveVector);		// �ړ���̍��W

	// HACK: �X�e�[�W�|���S������������ꍇ�A�������J��Ԃ������ɂȂ�
	{
		// �v���C���[�̎��͂ɂ���X�e�[�W�|���S�����擾����
		// ( ���o����͈͈͂ړ��������l������ )
		auto hitDim = MV1CollCheck_Sphere(modelHandle, -1, oldPos, DefaultSize + VSize(moveVector));

		// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f���A�ۑ�����
		AnalyzeWallAndFloor(hitDim, oldPos);

		// �ǃ|���S���Ƃ̓�������`�F�b�N���A�ړ��x�N�g����␳����
		nextPos = CheckHitWithWall(model, nextPos);

		// ���|���S���Ƃ̓�������`�F�b�N���A�ړ��x�N�g����␳����
		nextPos = CheckHitWithFloor(model, nextPos);

		// ���o�����v���C���[�̎��͂̃|���S�������J������
		MV1CollResultPolyDimTerminate(hitDim);
	}

	return nextPos;
}

/// <summary>
/// �`��
/// </summary>
void Stage::Draw()
{
	MV1DrawModel(modelHandle);
}

/// <summary>
/// ���o���ꂽ�|���S�����ǃ|���S��( �w�y���ʂɐ����ȃ|���S�� )�����|���S��( �w�y���ʂɐ����ł͂Ȃ��|���S�� )���𔻒f���A�ۑ�����
/// </summary>
void Stage::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const VECTOR& checkPosition)
{
	// �ǃ|���S���Ə��|���S���̐�������������
	wallNum = 0;
	floorNum = 0;

	// ���o���ꂽ�|���S���̐������J��Ԃ�
	for (int i = 0; i < hitDim.HitNum; i++)
	{
		// �w�y���ʂɐ������ǂ����̓|���S���̖@���̂x�������O�Ɍ���Ȃ��߂����ǂ����Ŕ��f����
		if (hitDim.Dim[i].Normal.y < 0.000001f && hitDim.Dim[i].Normal.y > -0.000001f)
		{
			// �ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[�̂x���W�{�P�D�O����荂���|���S���̂ݓ����蔻����s��
			if (hitDim.Dim[i].Position[0].y > checkPosition.y + 1.0f ||
				hitDim.Dim[i].Position[1].y > checkPosition.y + 1.0f ||
				hitDim.Dim[i].Position[2].y > checkPosition.y + 1.0f)
			{
				// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if (wallNum < MaxHitColl)
				{
					// �|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɕۑ�����
					wall[wallNum] = &hitDim.Dim[i];
					wallNum++;
				}
			}
		}
		else
		{
			// �|���S���̐����񋓂ł�����E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if (floorNum < MaxHitColl)
			{
				// �|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				floor[floorNum] = &hitDim.Dim[i];
				floorNum++;
			}
		}
	}
}

/// <summary>
/// �ǃ|���S���Ƃ̓�������`�F�b�N���A�␳���ׂ��ړ��x�N�g����Ԃ�
/// </summary>
VECTOR Stage::CheckHitWithWall(CharacterBase& model, const VECTOR& checkPosition)
{
	VECTOR fixedPos = checkPosition;

	// �ǂ̐������������瑁�����^�[��
	if (wallNum == 0)
	{
		return fixedPos;
	}

	// �ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
	for (int k = 0; k < HitTryNum; k++)
	{
		// ������\���̂���ǃ|���S����S�Č���
		bool isHitWall = false;
		for (int i = 0; i < wallNum; i++)
		{
			// i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			auto poly = wall[i];

			// �v���C���[�Ɠ������Ă���Ȃ�
			if (HitCheck_Capsule_Triangle(fixedPos, VAdd(fixedPos, VGet(0.0f, HitHeight, 0.0f)), HitWidth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
			{
				// �K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
				// �ړ���̈ʒu���X�V�i�ړ���̏ꏊ��␳�j
				fixedPos = VAdd(fixedPos, VScale(poly->Normal, HitSlideLength));

				// �ړ������ǃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
				for (int j = 0; j < wallNum; j++)
				{
					// �������Ă����烋�[�v�𔲂���
					poly = wall[j];
					if (HitCheck_Capsule_Triangle(fixedPos, VAdd(fixedPos, VGet(0.0f, HitHeight, 0.0f)), HitWidth, poly->Position[0], poly->Position[1], poly->Position[2]) == TRUE)
					{
						isHitWall = true;
						break;
					}
				}

				// �S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
				if (isHitWall == false)
				{
					break;
				}
			}
		}

		// �S���̃|���S���ŉ����o�������݂�O��
		// �S�Ă̕ǃ|���S���ƐڐG���Ȃ��Ȃ����烋�[�v���甲����
		if (isHitWall == false)
		{
			break;
		}
	}

	return fixedPos;
}

/// <summary>
/// ���|���S���Ƃ̓�������`�F�b�N���A�␳���ׂ��ړ��x�N�g����Ԃ�
/// </summary>
VECTOR Stage::CheckHitWithFloor(CharacterBase& model, const VECTOR& checkPosition)
{
	VECTOR fixedPos = checkPosition;

	// ���̐������������瑁�����^�[��
	if (floorNum == 0)
	{
		return fixedPos;
	}

	// �W�����v�����㏸���̏ꍇ�͏����𕪊�
	if (model.GetBattleState() == BattleState::JUMP && model.GetJumpPower() > 0.0f)
	{
		// �V��ɓ����Ԃ��鏈�����s��
		// ��ԒႢ�V��ɂԂ���ׂ̔���p�ϐ���������
		bool isHitRoof = false;
		float minY = 0.0f;

		// ���|���S���̐������J��Ԃ�
		for (int i = 0; i < floorNum; i++)
		{
			auto poly = floor[i];	// i�Ԗڂ̏��|���S���̃A�h���X

			// ���悩�瓪�̍����܂ł̊ԂŃ|���S���ƐڐG���Ă��邩�ǂ����𔻒�
			HITRESULT_LINE lineResult;	// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
			lineResult = HitCheck_Line_Triangle(fixedPos, VAdd(fixedPos, VGet(0.0f, HitHeight, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);

			// �ڐG���Ă��Ȃ������牽�����Ȃ�
			if (lineResult.HitFlag == TRUE)
			{
				// ���Ƀ|���S���ɓ������Ă��āA�����܂Ō��o�����V��|���S����荂���ꍇ�͉������Ȃ�
				if (!(isHitRoof == true && minY < lineResult.Position.y))
				{
					// �|���S���ɓ��������t���O�𗧂Ă�
					isHitRoof = true;

					// �ڐG�����x���W��ۑ�����
					minY = lineResult.Position.y;
				}
			}
		}

		// �ڐG�����|���S���������
		if (isHitRoof == true)
		{
			// �ڐG�����ꍇ�̓v���C���[�̂x���W��ڐG���W�����ɍX�V
			fixedPos.y = minY - HitHeight;
		//	model.OnHitRoof();
		}
	}
	// ���~�����W�����v���ł͂Ȃ��ꍇ�̏���
	else
	{
		bool isHitFloor = false;
		float maxY = 0.0f;

		// ���|���S���̐������J��Ԃ�
		for (int i = 0; i < floorNum; i++)
		{
			auto poly = floor[i];	// i�Ԗڂ̏��|���S���̃A�h���X

			// �W�����v�����ǂ����ŏ����𕪊�
			HITRESULT_LINE lineResult;	// �����ƃ|���S���Ƃ̓����蔻��̌��ʂ�������\����
			if (model.GetBattleState() == BattleState::JUMP)
			{
				// �W�����v���̏ꍇ�͓��̐悩�瑫���菭���Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�
				lineResult = HitCheck_Line_Triangle(VAdd(fixedPos, VGet(0.0f, HitHeight, 0.0f)), VAdd(fixedPos, VGet(0.0f, -1.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
			}
			else
			{
				// �����Ă���ꍇ�͓��̐悩�炻�������Ⴂ�ʒu�̊Ԃœ������Ă��邩�𔻒�( �X�΂ŗ�����ԂɈڍs���Ă��܂�Ȃ��� )
				lineResult = HitCheck_Line_Triangle(VAdd(fixedPos, VGet(0.0f, HitHeight, 0.0f)), VAdd(fixedPos, VGet(0.0f, -40.0f, 0.0f)), poly->Position[0], poly->Position[1], poly->Position[2]);
			}

			// ���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
			if (lineResult.HitFlag == TRUE)
			{
				if (!(isHitFloor == true && maxY > lineResult.Position.y))
				{
					// �ڐG�����x���W��ۑ�����
					isHitFloor = true;
					maxY = lineResult.Position.y;
				}
			}
		}

		// ���|���S���ɓ�������
		if (isHitFloor == true)
		{
			// �ڐG�����|���S���ň�ԍ����x���W���v���C���[�̂x���W�ɂ���
			fixedPos.y = maxY;

			// ���ɓ���������
			model.SetHitFall();
		}
		else
		{
			// ���R���W�����ɓ������Ă��Ȃ��Ċ��W�����v��Ԃł͂Ȃ������ꍇ�͗������
		//	model.OnFall();
		}
	}

	return fixedPos;
}

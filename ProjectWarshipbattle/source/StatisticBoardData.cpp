#include "StatisticBoardData.h"

StatisticBoardData::StatisticBoardData()
{
	boardData[7] = { 0 };
	press = false;
	for (int i = 0; i < 7; i++)
		newHighScore[i] = false;
}

StatisticBoardData::~StatisticBoardData()
{
	handleList = nullptr;
}

void StatisticBoardData::Inif(int *handle) {
	handleList = handle;
}

void StatisticBoardData::Read(bool Res) {
	win = Res;

	FILE *filePointer;//�t�@�C���|�C���^�[
	char fileName[100] = "Data//TopScore//TopScore.txt";

	/*�����ۑ����ꂽ�f�[�^������ꍇ�͂��̂܂ܓǂݍ���*/
	if ((fopen_s(&filePointer, fileName, "r")) == 0) {

		for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
			fscanf_s(filePointer, "%lf", &boardData[i]);
		fclose(filePointer);
	}

	fopen_s(&filePointer, fileName, "w");

	/*�v�Z�̕���*/
	if (S_data.shootCount == 0)/*�[���̎����������\���ł���*/
		S_data.shootCount = 1;
	S_data.hitRate = (double)S_data.hitCount / (double)S_data.shootCount;

	/*�L�^���X�V*/
	boardData[TOTAL_KILL] += (double)S_data.killed;
	boardData[TOTAL_MOVE] += S_data.movedDis;
	boardData[TOTAL_DAMAGE] += (double)S_data.damage;
	boardData[TOTAL_DAMAGE_RECIEVED] += (double)S_data.damageRecieved;
	if (S_data.damage > MAX_DAMAGE) {
		boardData[MAX_DAMAGE] = S_data.damage;
		newHighScore[MAX_DAMAGE] = true;
	}
	if (S_data.movedDis >
		boardData[MAX_MOVE]) {
		boardData[MAX_MOVE] = S_data.movedDis;
		newHighScore[MAX_MOVE] = true;
	}
	if (S_data.hitRate > boardData[MAX_HITRATE]) {
		boardData[MAX_HITRATE] = S_data.hitRate;
		newHighScore[MAX_HITRATE] = true;
	}

	for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
		fprintf_s(filePointer, "%lf\n", boardData[i]);

	fclose(filePointer);//�t�@�C�������
}

bool StatisticBoardData::Update() {
	if (!press && frameCount >= 10)
		if (CheckHitKeyAll() || GetMouseInput() != 0) {
			press = true;
			pressFrame = frameCount;
			rewind(stdin);
		}

	frameCount++;
	Draw();
	if (frameCount >= 300 || (press && frameCount - pressFrame >= 30)) {
		if (CheckHitKeyAll() && GetInputChar(TRUE) || GetMouseInput() != 0)
			return true;
	}
	return false;
}

/*���̕����̍��W�͒��ڂ��Ă��܂����̂ŁA�𑜓x���ς�鎞�ɕύX����K�v������*/
void StatisticBoardData::Draw() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	/*�`��F*/
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DxLib::SetFontSize(40);

	/*�w�i��`��*/
	DxLib::DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*(handleList + StatisticBoard::SB_BACKGROUND), FALSE);

	if (win)
		DxLib::DrawExtendGraph(40, 520, 460, 691,
			*(handleList + StatisticBoard::WIN), TRUE);
	else
		DxLib::DrawExtendGraph(40, 520, 460, 691,
			*(handleList + StatisticBoard::LOSE), TRUE);

	/*����̃f�[�^��`��*/
	if (frameCount >= 0 || press) {//��x�N���b�N���܂����A�܂��͎��Ԃ��߂��܂�����
		if (frameCount < seconds * 1 && !press) {
			SetTrans(frameCount - seconds + 100);//���ߓx���ǂ�ǂ�オ��悤��
		}
		DxLib::DrawFormatString(410, 110, Cr, "%4.1lf%%", S_data.hitRate * 100);
		ResetTrans();//���ߓx�����Z�b�g
	}
	if (frameCount >= seconds  || press) {
		if (frameCount < seconds * 2 && !press) {
			SetTrans(frameCount - seconds * 1 + 100);
		}
		DxLib::DrawFormatString(410, 220, Cr, "%4d", S_data.damage);
		ResetTrans();
	}
	if (frameCount >= seconds * 2 || press) {
		if (frameCount < seconds * 3 && !press) {
			SetTrans(frameCount - seconds * 2 + 100);
		}
		DxLib::DrawFormatString(410, 325, Cr, "%4.1lf�C��", S_data.movedDis);
		ResetTrans();
	}
	if (frameCount >= seconds * 3 || press) {
		if (frameCount < seconds * 4 && !press) {
			SetTrans(frameCount - seconds * 3 + 100);
		}
		DxLib::DrawFormatString(410, 415, Cr, "%4d��", S_data.killed);
		ResetTrans();
	}

	/*�L�^��`��*/
	if (frameCount >= seconds * 4 || press) {
		DxLib::SetFontSize(26);
		long long int num = (frameCount - seconds * 4) / seconds;
		if (num > 6 || press)num = 6;
		for (int i = TOTAL_KILL; i <= TOTAL_KILL + (int)num; i++) {
			if (frameCount - seconds * 4 < seconds * 5 + seconds * i && !press) {
				SetTrans(frameCount - seconds * 4 + seconds * i + 100);
				DxLib::DrawFormatString(1080, 115 + i * 75, Cr, "%-14.1lf", boardData[i]);
			}
			if (newHighScore[i]) {
				DrawStar({ 645,110 + i * 75 }, { 695 , 160 + i * 75 });
				DxLib::DrawFormatString(1080, 115 + i * 75, 
					((frameCount % 60>30)?
						CrBox.ReferColor(GORSE):
						CrBox.ReferColor(RED_ORANGE)
						), "%-14.1lf", boardData[i]);
			}
			
			ResetTrans();
		}
	}
	DxLib::ScreenFlip();
}

void StatisticBoardData::DrawStar(Coordinate2D<int> coord, Coordinate2D<int> coord2) {
	DrawExtendGraph(coord.x, coord.z,
		coord2.x, coord2.z, *(handleList + ((frameCount % 60 > 30) ? +
		StatisticBoard::NEW_LIGHT :
		StatisticBoard::NEW_DARK)), TRUE);
}
#include "StatisticBoardData.h"

StatisticBoardData::StatisticBoardData()
{
	boardData[7] = { 0 };
	press = false;
	for (int i = 0; i < 7; i++)
		newHighScore[i] = false;
	frameCount = 0;
	CrNum = COLOR_MARK::MY_SIN;
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

	FILE *filePointer;//ファイルポインター
	char fileName[100] = "Data//TopScore//TopScore.txt";

	/*もし保存されたデータがある場合はそのまま読み込む*/
	if ((fopen_s(&filePointer, fileName, "r")) == 0) {

		for (int i = TOTAL_KILL; i <= MAX_HITRATE; i++)
			fscanf_s(filePointer, "%lf", &boardData[i]);
		fclose(filePointer);
	}

	fopen_s(&filePointer, fileName, "w");

	/*計算の部分*/
	if (S_data.shootCount == 0)/*ゼロの時も正しく表示できる*/
		S_data.shootCount = 1;
	S_data.hitRate = (double)S_data.hitCount / (double)S_data.shootCount * 100;

	/*記録を更新*/
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

	fclose(filePointer);//ファイルを閉じる
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

/*この部分の座標は直接してしましたので、解像度が変わる時に変更する必要がある*/
void StatisticBoardData::Draw() {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	/*描画色*/
	DxLib::SetFontSize(40);

	/*背景を描く*/
	DxLib::DrawExtendGraph(0, 0, Screen::SCREEN_X, Screen::SCREEN_Z,
		*(handleList + StatisticBoard::SB_BACKGROUND), FALSE);

	if (win)
		DxLib::DrawExtendGraph(40, 520, 460, 691,
			*(handleList + StatisticBoard::WIN), TRUE);
	else
		DxLib::DrawExtendGraph(40, 520, 460, 691,
			*(handleList + StatisticBoard::LOSE), TRUE);

	/*今回のデータを描く*/
	if (frameCount >= 0 || press) {//一度クリックしました、または時間が過ぎましたら
		if (frameCount < seconds * 1 && !press) {
			SetTrans(frameCount - seconds + 100);//透過度がどんどん上がるように
		}
		DxLib::DrawFormatString(410, 110, CrBox.ReferColor(WHITE), 
			"%4.1lf%%", S_data.hitRate);
		ResetTrans();//透過度をリセット
	}
	if (frameCount >= seconds  || press) {
		if (frameCount < seconds * 2 && !press) {
			SetTrans(frameCount - seconds * 1 + 100);
		}
		DxLib::DrawFormatString(410, 220, CrBox.ReferColor(WHITE),
			"%4d", S_data.damage);
		ResetTrans();
	}
	if (frameCount >= seconds * 2 || press) {
		if (frameCount < seconds * 3 && !press) {
			SetTrans(frameCount - seconds * 2 + 100);
		}
		DxLib::DrawFormatString(410, 325, CrBox.ReferColor(WHITE), 
			"%4.1lf海里", S_data.movedDis);
		ResetTrans();
	}
	if (frameCount >= seconds * 3 || press) {
		if (frameCount < seconds * 4 && !press) {
			SetTrans(frameCount - seconds * 3 + 100);
		}
		DxLib::DrawFormatString(410, 415, CrBox.ReferColor(WHITE), 
			"%4d隻", S_data.killed);
		ResetTrans();
	}

	/*記録を描く*/
	if (frameCount >= seconds * 4 || press) {
		DxLib::SetFontSize(26);
		long long int num = (frameCount - seconds * 4) / seconds;
		if (num > 6 || press)num = 6;
		for (int i = TOTAL_KILL; i <= TOTAL_KILL + (int)num; i++) {
			if (frameCount - seconds * 4 > //経過した時間
				seconds * i ||
				press) {
				if (!press)
					SetTrans(frameCount - seconds * 4 + seconds * i + 100);
				DxLib::DrawFormatString(1060, 115 + i * 75, CrBox.ReferColor(WHITE),
					"%9.0lf%-s", boardData[i], RecordChar[i]);
			}
			if (newHighScore[i]) {
				if (frameCount % 60 == 0) {
					CrNum = rand() % 16;
				}
				DrawStar({ 645,110 + i * 75 }, { 695 , 160 + i * 75 });
				DxLib::DrawFormatString(1060, 115 + i * 75,
						CrBox.ReferColor(CrNum), "%9.0lf%-s", boardData[i],
						RecordChar[i]);
			}
			ResetTrans();
		}
	}

	if (frameCount >= seconds * 11 || press) {
		press = true;
		DrawFormatString(520, 640, ((frameCount % 60 > 30) ?
			CrBox.ReferColor(DROVER) :
			CrBox.ReferColor(RED_ORANGE)), "Press To Continue");
	}
	DxLib::ScreenFlip();
}

void StatisticBoardData::DrawStar(Coordinate2D<int> coord, Coordinate2D<int> coord2) {
	DrawExtendGraph(coord.x, coord.z,
		coord2.x, coord2.z, *(handleList + ((frameCount % 60 > 30) ? +
		StatisticBoard::NEW_LIGHT :
		StatisticBoard::NEW_DARK)), TRUE);
}
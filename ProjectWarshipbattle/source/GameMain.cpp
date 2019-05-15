/****************************************************************/
/*���̃Q�[����DxLib�𗘗p���ĊJ�����܂���                       */
/*https://dxlib.xsrv.jp/                                        */
/*�Q�[�����:                                                   */
/*���:�� �r�F                                                  */
/*�w�Z:�匴���r�W�l�X���w�Z                                 */
/*�w��:�Q�[������R�[�X                                         */
/*��͉摜��wiki�ɍڂ������̂ł�                                */
/*�A�b�v���[�h��:https://commons.wikimedia.org/wiki/User:Alexpl */
/*UI���:���r�F                                                 */
/*                                                              */
/*�X�V��:2019.5.15                                             */
/****************************************************************/
#include"DxLib.h"
#include"HeaderForMain.h"

void GameMain();
void SingleGame_Mission_Progress();
void SingleGame_DeathMatch_Progress();
int MainMenu();

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);//�E�C���h���[�h�ɕύX
	SetWaitVSyncFlag(FALSE);//���������𖳌��ɂ���
	SetMainWindowText("WarshipBattle");
	SetGraphMode(Screen::SCREEN_X, Screen::SCREEN_Z, Screen::COLOR);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	/*�����ɂ̓E�C���h�[�Y���b�Z�[�W�̖�肪����A
	DXLIB�̏ꍇ�͖��O��ς���Ή����ł��邪�A
	WINAPI�̏ꍇ�͂ق��̏��������Ȃ��Ƃ����Ȃ�*/

	SetAlwaysRunFlag(TRUE);
	srand((unsigned)time(NULL));//�����̏�����

	MainMenuController MMC;
	int choice = -1;//�I���𖳌��ɂ���
	bool endGame = false;//�Q�[���I�[�o�[�t���O���I�t�ɂ���

	while (!endGame) {

		choice = MainMenu();

		if (ProcessMessage() == -1)
			break;

		//���j���[���������R�}���h�ɍ��킹�Ċ֐����Ăяo��
		switch (choice) {
		/*�܂����[�h�@�\�������Ȃ��������߁A������̃{�^�����������烁�C���Q�[�����n�܂�*/
		case ButtonEvent::NEW_GAME:
		case ButtonEvent::CONTINUE_GAME :
		case ButtonEvent::LOAD_GAME:
			SingleGame_DeathMatch_Progress(); break;	
		case ButtonEvent::GAME_OVER:endGame = true; break;
		}
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}

int MainMenu(){
	MainMenuController MMC;
	int choice = -1;

		MMC.Inif();//���j���[����������
		MMC.DrawTitle();//�^�C�g���`��
		MMC.DrawLoading();
		choice = -1;//�I����������

		while (choice == -1) {
			MMC.DrawMainMenu();//���C�����j���[��`��
			choice = MMC.CheckChoice();//�I�����m�F�E�o�[��ԍX�V
									   //���C�����j���[����E�o
			if (CheckHitKey(KEY_INPUT_ESCAPE) == TRUE &&
				GetInputChar(TRUE)) {
				choice = ButtonEvent::GAME_OVER;
			}
			if (ProcessMessage() == -1)
				break;
		}

		MMC.DrawLoading();//���̊֐��͂ق��̕����Ɉړ�����
		MMC.FREE();//�����������
		return choice;
}

void SingleGame_DeathMatch_Progress() {
	
	//�Q�[���Ǘ��̕��i��錾
	IngameDataManagement IDM;
	TeamDeathMatchControl TDMC;

	IDM.Inif();//������
	IDM.FormationBoard();//�Ґ���ʂ�`��
	IDM.FreeFormationBoard();//�Ґ��Ɏg�����������Ȃǂ��������

	/*�Q�[�����C���v���O���X*/
	if (ProcessMessage() == 0) {
		while (!TDMC.GameOver(IDM.TeamDestroyed())) {//��`�[�����S�ł܂ŌJ��Ԃ�
			IDM.Update();
			if (ProcessMessage() == -1)//�G���[����
				break;
			if (IDM.ReferEndGame())//���̌����ŃQ�[�����I����������
				break;
		}
		IDM.DrawLoading();//���[�h��ʂ�`��
		/*���ʔ��\*/
		IDM.DrawStatisticBoard();//���v��ʂ�`��
		/*���������*/
	}
	IDM.Free();
}
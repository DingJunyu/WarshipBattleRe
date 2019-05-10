#pragma once
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"PictureLoader.h"
#include"DxLib.h"
#include"SoundLoader.h"
#include"ShipMain.h"
#include<cmath>
#include<vector>
/*����̃Q�[���ł́A���ׂĂ̑�����}�E�X�łł���悤�ɂ������ł��B
���C�����j���[�ɂ��A�Q�[����ʂɂ��g����N���X�ł��B
�ڕW�Ƃ��Ă͓����\���𗘗p���āA
�C���^�[�t�F�[�X��ύX����ΈႤ���g�𗘗p�ł���悤�ɂ������ł�*/
/*�������[�_�[�A�摜���[�_�[�͂��ׂĊO��������܂��̂ŁA�Đ錾���邱�Ƃ��T���܂�*/
/*�߂�l�Ȃǂ͂��ׂ�DefinedData.h�ɒ�`���܂��B*/
/*�Ȃ��A���ڐG��Ȃ�������UserInterface�N���X�Ɏ������܂��B
���̃N���X�ł́A�N���b�N�ł���p�[�c�̕`��Ɩ��߂�Ԃ��̓�̋@�\���������܂��B*/

class ClickableUserInterface
{
public:
	ClickableUserInterface() {
		menuOpened = false;
		lastClickTime = GetTickCount();
	}
	~ClickableUserInterface();

	void IngameInif(PictureLoader *PL, SoundLoader *SL);
	void ChooseInif(PictureLoader *PL, SoundLoader *SL);
	void InifShipList(std::vector<ShipMain> *shipList, bool left);
	void CheckShipList(std::vector<ShipMain> *shipList, bool left);
	
	/*�}�E�X�̍��W������ăR�}���h�����*/
	int CheckChoice();
	void Draw();
	void DrawNeedInput(bool lock, int num);
	/*�ŏ��Ɍ�����̂�������悤�ɐݒ肷��*/
	void SetFormationMenuStatus();
	void CloseFormationMenu();
	void SetNormalStatus();
	/*���j���[���g���邩�̃X�C�b�`*/
	void LetMeSeeMenu();
	void CloseMenu();
	bool CheckMenuOpened() { return menuOpened; }
	/*�ˌ������{�^��*/
	void SetShootMenu(bool status);
	void DrawChoosedShip(bool lock, int num);

	/*�����ꂽ���Ԃ��L�^*/
	void SetClickTime() { lastClickTime = GetTickCount(); }

	/*�A���������ł��Ȃ����A�������Ƃ��L���ł��邩�𔻒f����*/
	bool ReferClickable() {
		if (lastClickTime + inputDelay <= GetTickCount())
			return true;
		return false;
	}

	/*���������*/
	void Free();

private:
	/*���̕ύX���ł���悤�ɂ��܂���*/
	void MemorySecure(int);
	void MemoryFree(int);
	void SetThisOne(int num, double x, double z, double mt);

	/*�񎦂̓_�ŗp�J�E���g*/
	int countForTitle;

	bool menuOpened;//���j���[�̏�Ԃ�ۑ�����
	unsigned int lastClickTime;//�O�񉟂��ꂽ���Ԃ��L�^
	const int inputDelay = 200;//�A��������h�~

	ButtonCollection BC;

	/*SaveForCui�ɎQ�l���Ă�������*/
	/*�{�^���̃f�[�^��ۑ����Ă���*/
	double **buttonPosition;
};
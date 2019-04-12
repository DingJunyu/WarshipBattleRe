#pragma once
#include"DxLib.h"
#include"PictureLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"ShipMain.h"
#include"Lamp.h"
#include<cmath>
class UserInterface
{
public:
	UserInterface() :
		matStartX(0), matStartZ(Screen::SCREEN_Z / 4.25),
		leftSCMStartX(0), leftSCMStartZ(0),
		rightSCMStartX(0), rightSCMStartZ(0), mapMatSize(Screen::SCREEN_X / 8),
		mapPaperSize(mapMatSize - 10) {}
	~UserInterface();

	void InifUI(PictureLoader *PL);

	void DrawUI();
	void DrawUINeedInput(ShipMain *ship);

	void DrawShipOnTheMap(double X, double Z, bool enemy);

	void SetMat(int *M) { mat = M; }
	

private:
	void DrawPictureByLeftUp(int SN,double X,double Z,double Multiple);
	void DrawPictureByCenterOnX(int SN, double X, double Z, double Multiple);
	void DrawPictureByCenter(int SN, double X, double Z, double Multiple);
	void DrawRotatePicture(int SN, double X, double Z, double Multiple,
		double Radian);

	/*UI�̔w�i����*/
	int *mat;
	int *shipCardMat;
	int *miniMapMat;
	int *miniMapPaper;

	/*UI�w�i�����̍��W*/
	double matStartX;
	double matStartZ;
	double leftSCMStartX;
	double leftSCMStartZ;
	double rightSCMStartX;
	double rightSCMStartZ;

	/*UI�̔w�i�����̃T�C�Y*/
	double mapMatSize;
	double mapPaperSize;

	/*�e�p�[�c�̃n���h��*/
	int *handleList[UI_LIST::AMOUNT];
	/*�����v�̐錾*/
	Lamp lamps[LAMP_LIST::LAMP_AMOUNT];
	
};


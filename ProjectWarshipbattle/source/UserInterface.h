#pragma once
#include"DxLib.h"
#include"PictureLoader.h"
#include"DefinedData.h"
#include"ButtonCollection.h"
#include"ShipMain.h"
#include"Lamp.h"
#include"Camera.h"
#include<cmath>
class UserInterface
{
public:
	UserInterface() :
		matStartX(0), matStartZ(Screen::SCREEN_Z / 4.25),
		leftSCMStartX(0), leftSCMStartZ(0),
		rightSCMStartX(0), rightSCMStartZ(0), mapMatSize(Screen::SCREEN_X / 8),
		mapPaperSize(mapMatSize - 10) {
		radian_returnTheLockCircle = 0;
	}
	~UserInterface();

	void InifUI(PictureLoader *PL);

	void DrawUI();
	void DrawUINeedInput(ShipMain *ship);

	void DrawUIUnderShip(bool lock,Coordinate2D<double> coord,Camera camera, double radian);

	void DrawShipOnTheMap(double X, double Z, bool enemy);

	void SetMat(int *M) { mat = M; }
	

private:
	void DrawPictureByLeftUp(int SN,double X,double Z,double Multiple);
	void DrawPictureByCenterOnX(int SN, double X, double Z, double Multiple);
	void DrawPictureByCenter(int SN, double X, double Z, double Multiple);
	void DrawRotatePicture(int SN, double X, double Z, double Multiple,
		double Radian);
	void DrawRotatePicture2(int SN, double X, double Z, double Multiple,
		double Radian);
	

	/*UIの背景部分*/
	int *mat;
	int *shipCardMat;
	int *miniMapMat;
	int *miniMapPaper;

	/*UI背景部分の座標*/
	double matStartX;
	double matStartZ;
	double leftSCMStartX;
	double leftSCMStartZ;
	double rightSCMStartX;
	double rightSCMStartZ;

	/*UIの背景部分のサイズ*/
	double mapMatSize;
	double mapPaperSize;
	
	/*○を回すようにするradian*/
	double radian_returnTheLockCircle;
	void DrawCircle(Coordinate2D<double> coord, Camera camera);
	void DrawMyCircle(Camera camera, double radian);

	/*各パーツのハンドル*/
	int *handleList[UI_LIST::AMOUNT];
	/*ランプの宣言*/
	Lamp lamps[LAMP_LIST::LAMP_AMOUNT];
	
};


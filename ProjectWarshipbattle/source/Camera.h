#pragma once
#include"DefinedData.h"
#include"IncludeForAll.h"
const double ZOOM_RATE = 0.018f;
class Camera
{
public:
	Camera(int CT): cameraType(CT) {
		zoomRatio = Screen::DEFAULT_MULTIPLE;
	}
	~Camera();

	//カメラ座標問い合わせ
	double ReferCameraX() { return cameraX; }
	double ReferCameraZ() { return cameraZ; }
	double ReferRealCameraX() { return realCameraX; }
	double ReferRealCameraZ() { return realCameraZ; }
	double ReferPrintOutX(double PS) { return fmod(cameraX, PS); }
	double ReferPrintOutZ(double PS) { return fmod(cameraZ, PS); }
	double ReferPrintOutX2(double PS) { return fmod(realCameraX, PS); }
	double ReferPrintOutZ2(double PS) { return fmod(realCameraZ, PS); }

	void SetCameraX(double CX) { cameraX = CX; }
	void SetCameraZ(double CZ) { cameraZ = CZ; }

	//データ設置関数
	double SetScreenSizeX(double SSX) { screenSizeX = SSX; }
	double SetScreenSizeZ(double SSZ) { screenSizeZ = SSZ; }
	double SetMapSizeX(double MSX) { mapSizeX = MSX; }
	double SetMapSizeY(double MSZ) { mapSIzeZ = MSZ; }

	//カメラ座標設定
	void GetXZ(double X, double Z) { 
		GetX(X); GetZ(Z);
		//カメラの左上の座標を計算する
		realCameraX = cameraX - Screen::SCREEN_X / 2 * 1/zoomRatio;
		realCameraZ = cameraZ - Screen::SCREEN_Z / 2 * 1/zoomRatio;
	}
	//カメラ倍率設定
	void ZoomIn();
	void ZoomOut();
	//カメラ倍率問い合わせ
	double ReferZoomRatio() { return zoomRatio; }

private:
	void GetX(double X) { cameraX = X; }
	void GetZ(double Z) { cameraZ = Z; }

	//カメラの中心座標
	double cameraX;
	double cameraZ;

	//カメラの左上の座標
	double realCameraX;
	double realCameraZ;

	int cameraType;

	double mapSizeX;
	double mapSIzeZ;

	double screenSizeX;
	double screenSizeZ;

	double zoomRatio;
};


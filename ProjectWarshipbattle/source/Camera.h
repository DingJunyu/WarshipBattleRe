#pragma once
#include"DefinedData.h"
#include"IncludeForAll.h"
const double ZOOM_RATE = 1.0f;
class Camera
{
public:
	Camera(int CT): cameraType(CT) {}
	~Camera();

	//�J�������W�₢���킹
	double ReferCameraX() { return cameraX; }
	double ReferCameraZ() { return cameraZ; }
	double ReferRealCameraX() { return realCameraX; }
	double ReferRealCameraZ() { return realCameraZ; }
	double ReferPrintOutX(double PS) { return fmod(cameraX, PS); }
	double ReferPrintOutZ(double PS) { return fmod(cameraZ, PS); }

	void SetCameraX(double CX) { cameraX = CX; }
	void SetCameraZ(double CZ) { cameraZ = CZ; }

	//�f�[�^�ݒu�֐�
	double SetScreenSizeX(double SSX) { screenSizeX = SSX; }
	double SetScreenSizeZ(double SSZ) { screenSizeZ = SSZ; }
	double SetMapSizeX(double MSX) { mapSizeX = MSX; }
	double SetMapSizeY(double MSZ) { mapSIzeZ = MSZ; }

	//�J�������W�ݒ�
	void GetXZ(double X, double Z) { 
		GetX(X); GetZ(Z);
		//�J�����̍���̍��W���v�Z����
		realCameraX = cameraX - Screen::SCREEN_X / 2;
		realCameraZ = cameraZ - Screen::SCREEN_Z / 2;
	}
	//�J�����{���ݒ�
	void ZoomIn() { zoomRatio += ZOOM_RATE; }
	void ZoomOut() { zoomRatio -= ZOOM_RATE; }
	//�J�����{���₢���킹
	double ReferZoomRatio() { return zoomRatio; }

private:
	void GetX(double X) { cameraX = X; }
	void GetZ(double Z) { cameraZ = Z; }

	//�J�����̒��S���W
	double cameraX;
	double cameraZ;

	//�J�����̍���̍��W
	double realCameraX;
	double realCameraZ;

	int cameraType;

	double mapSizeX;
	double mapSIzeZ;

	double screenSizeX;
	double screenSizeZ;

	double zoomRatio;
};


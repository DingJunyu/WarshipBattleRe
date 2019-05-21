#include "Camera.h"

Camera::~Camera()
{
}

void Camera::ZoomIn() {
	zoomRatio += ZOOM_RATE;
	if (zoomRatio > Screen::MAX_MULTIPLE)
		zoomRatio = Screen::MAX_MULTIPLE;
}

void Camera::ZoomOut() {
	zoomRatio -= ZOOM_RATE;
	if (zoomRatio < Screen::MIN_MULTIPLE)
		zoomRatio = Screen::MIN_MULTIPLE;
}
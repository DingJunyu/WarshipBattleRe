#include "FlagShipAI.h"

FlagShipAI::~FlagShipAI()
{
}

void FlagShipAI::SetWayPoint() {
	wayPoint.x = nextPosOnMapX + (double)(rand() % randRange);
	wayPoint.z = nextPosOnMapZ + (double)(rand() % randRange);

	if (myPos.x > 0 && myPos.z > 0) {
		int target = rand() % 3;
		switch (target) {
		case 0:wayPoint.ChangeX(); break;//第四象限に移す
		case 1:wayPoint.ChangeZ(); break;//第二象限に移す
		case 2:wayPoint.ChangeXandZ(); break;//第三象限に移す
		}
	}
}
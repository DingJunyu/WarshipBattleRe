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
		case 0:wayPoint.ChangeX(); break;//‘ælÛŒÀ‚ÉˆÚ‚·
		case 1:wayPoint.ChangeZ(); break;//‘æ“ñÛŒÀ‚ÉˆÚ‚·
		case 2:wayPoint.ChangeXandZ(); break;//‘æOÛŒÀ‚ÉˆÚ‚·
		}
	}
}
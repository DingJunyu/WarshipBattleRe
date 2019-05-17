#include "ErrorMessegeContainer.h"

ErrorMessege Fleet_Empty() {
	return ErrorMessege("戦艦数足りない(少なくとも一チームは一台が必要だ",
		{ 50,650 }, { 600,680 });
}

ErrorMessege FlagShip_Empty() {
	return ErrorMessege("フラグシープが必要だ",
		{ 50,650 }, { 350,680 });
}
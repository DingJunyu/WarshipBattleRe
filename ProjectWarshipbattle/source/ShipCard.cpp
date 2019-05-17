#include "ShipCard.h"

ShipCard::~ShipCard()
{
}

void ShipCard::DrawCard(Coordinate2D<double> coord){
	const double & multiple = MathAndPhysics::ShipCardRate;
	const double & numMultiple = MathAndPhysics::FantasyNumRate;

	DrawExtendGraph((int)coord.x, (int)coord.z,
		(int)(coord.x + multiple * length),
		(int)(coord.z + multiple * width),
		*barHandle, TRUE);

	ship.DrawCard(coord);

	DrawExtendGraph((int)(coord.x + multiple * length * 0.985),
		(int)(coord.z + multiple * width / 4),
		(int)(coord.x + multiple * length * 0.985 + numL * numMultiple),
		(int)(coord.z + multiple * width / 4 + numW * numMultiple),
		*(fantasyNumber + NumberChoosed), TRUE);

	if (flag) {
		DrawExtendGraph((int)coord.x,(int)coord.z,
			(int)(coord.x+0.1*flagL),(int)(coord.z+0.1*flagW),
			*flagHandle,TRUE
		);	
	}
}

void ShipCard::DrawBar(Coordinate2D<int> coord) {
	Coordinate2D<int> realCoord;
	if (coord.x + barLength < Screen::SCREEN_X)
		realCoord.x = coord.x;
	else
		realCoord.x = coord.x - barLength;
	if (coord.z + barHeight < Screen::SCREEN_Z)
		realCoord.z = coord.z;
	else
		realCoord.z = coord.z - barHeight;

	SetTrans(200);
	DrawBox(realCoord.x + 2, realCoord.z + 2, realCoord.x + barLength + 2,
		realCoord.z + barHeight + 2, GetColor(128, 139, 150), TRUE);
	SetTrans(160);
	DrawBox(realCoord.x, realCoord.z, realCoord.x + barLength,
		realCoord.z + barHeight, GetColor(33, 47, 61), TRUE);

	ResetTrans();

	unsigned int Cr = GetColor(255, 255, 255);

	SetFontSize(20);

	const char *ShipTypeName[9] = {
	" ",
	"‹ì’€ŠÍ",
	"Œy„—mŠÍ",
	"d„—mŠÍ",
	"í“¬„—mŠÍ",
	"íŠÍ",
	"‹ó•ê",
	"Œy‹ó•ê",
	"Œì‰q‹ó•ê"
	};

	realCoord.x += 5; realCoord.z += 5;

	DrawFormatString(realCoord.x, realCoord.z, Cr, "%s",
		ShipTypeName[ship.ReferShipType() / 1000]);
	DrawFormatString(realCoord.x, realCoord.z + 1 * nextString, Cr, 
		"%s",ship.ReferName());

	DrawFormatString(realCoord.x, realCoord.z + 2 * nextString, Cr,
		"Å‘åHP:%d", ship.ReferMaxHP());

	DrawFormatString(realCoord.x, realCoord.z + 3 * nextString, Cr, 
		"Žå–C”F%d@ƒ_ƒ[ƒW:%2d", 
		ship.ReferWeaponCount(true),ship.ReferDamage());

	DrawFormatString(realCoord.x, realCoord.z + 4 * nextString, Cr,
		"Å‘å‘¬“xF%2.1f ŠC—¢",
		ship.ReferMaxSpeedOnZ() * 47);

	DrawFormatString(realCoord.x, realCoord.z + 5 * nextString, Cr,
		"ƒgƒ“”F%.0f ƒgƒ“",
		ship.ReferMass());
}

bool ShipCard::Clicked(Coordinate2D<int> Coord, Coordinate2D<int> Mouse) {
	const double & multiple = MathAndPhysics::ShipCardRate;

	if (Mouse.x > Coord.x &&
		Mouse.x<Coord.x + multiple * length &&
		Mouse.z>Coord.z &&
		Mouse.z < Coord.z + multiple * width
		) {
		flag = true;
		return true;
	}
	return false;
}

bool ShipCard::CheckMousePos(Coordinate2D<int> Coord, Coordinate2D<int> Mouse) {

	const double & multiple = MathAndPhysics::ShipCardRate;

	if (Mouse.x > Coord.x &&
		Mouse.x<Coord.x + multiple * length &&
		Mouse.z>Coord.z &&
		Mouse.z < Coord.z + multiple * width
		) {
		return true;
	}
	return false;
}
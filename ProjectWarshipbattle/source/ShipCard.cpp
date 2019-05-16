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


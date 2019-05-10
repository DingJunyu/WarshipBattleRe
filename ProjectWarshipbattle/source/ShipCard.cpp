#include "ShipCard.h"

ShipCard::~ShipCard()
{
}

void ShipCard::DrawCard(Coordinate2D<double> coord){
	const double & multiple = MathAndPhysics::ShipCardRate;

	DrawExtendGraph((int)coord.x, (int)coord.z,
		(int)(coord.x + multiple * length),
		(int)(coord.z + multiple * width),
		*barHandle, TRUE);

	ship.DrawCard(coord);
}


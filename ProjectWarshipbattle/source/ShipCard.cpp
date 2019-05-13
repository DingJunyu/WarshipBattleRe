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
}


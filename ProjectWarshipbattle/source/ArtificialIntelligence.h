#pragma once
#include"DefinedData.h"
#include"ShipMain.h"
#include"OtherFunctions.h"
#include"AI_Action.h"
/*Ç±ÇÃÉNÉâÉXÇ…ÇÕÇ`ÇhÇÃçsìÆÇé¿åªÇ∑ÇÈ*/
class ArtificialIntelligence
{
public:
	ArtificialIntelligence();
	~ArtificialIntelligence();

	void SetStatus(ShipMain ship);
	void Move(ShipMain me,ShipMain target);
	void InBattle();

	void SetFlagShip() { flagShip = true; }
	bool ReferFlagShip() { return flagShip; }

	double ReferRadianNeededNow() { return radianNeededNow; }

private:
	const double disToFront = 400;

	double range;
	double targetDis;
	double wayPointDis;
	Coordinate2D<double> wayPoint;
	Coordinate2D<double> myPos;
	bool flagShip;
	double nowRadian;
	double targetRadian;
	double radianNeededNow;
	double outPutRate;

	void SetTargetPos(Coordinate2D<double> target,double radian);
	void SetMyPos(Coordinate2D<double> pos) { myPos = pos; }
	void SetNowRadian(double rad) { nowRadian = rad; }
	void CalTargetRadian();
	void SetRadianNeeded();
	void CalDistance(Coordinate2D<double> coord);
	void CalWaypointDis();
};


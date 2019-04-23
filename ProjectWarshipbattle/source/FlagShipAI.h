#pragma once
#include"DefinedData.h"
class FlagShipAI
{
public:
	FlagShipAI();
	~FlagShipAI();

	void LetUsGo();//移動関数-ステータスによって決めた行動を行う
	void LetUsFight();//戦闘関数

private:
	/*移動部分*/
	void SetWayPoint();//ウェイポイント設定関数

	/*計算部分*/
	void CalRadianNeeded();//ウェイポイントへ行くため必要な角度の差
	void CalDistanceToOpponent();//相手との距離を計算する

	/*戦闘部分*/
	void ChooseTarget();//敵を選ぶ

};


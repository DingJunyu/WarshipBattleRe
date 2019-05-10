#pragma once
#include"ShipMain.h"
#include"DefinedData.h"
class ShipCard
{
public:
	ShipCard(int* BarHandle) :barHandle(BarHandle) {
		GetGraphSize(*barHandle, &length, &width);
	}
	~ShipCard();

	void DrawCard(Coordinate2D<double> coord);//�����͕��ʂ̍��W
	void DrawBar(Coordinate2D<double> coord);//�����̓}�E�X�̍��W

	ShipMain ship;
private:
	
	int NumberChoosed;//���̑D�̖{��
	
	bool pointed;//�}�E�X�������Ɏw���Ă��邩
	
	int *barHandle;
	int length, width;
};
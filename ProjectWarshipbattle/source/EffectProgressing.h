#pragma once
class EffectProgressing
{
public:
	EffectProgressing();
	~EffectProgressing();
private:
	double coordX;
	double coordY;
	
	int produceTime;
	int continueTime;

	int effectType;

	int *effectHandle;

};


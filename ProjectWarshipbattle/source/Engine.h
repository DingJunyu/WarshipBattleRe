#pragma once
#include<cmath>
class Engine
{
public:
	Engine(double MaxOutput, double BaseIncreaseRate) :
		maxOutput(MaxOutput), baseIncreaseRate(BaseIncreaseRate)
	{
		increaseDownWithOutputPercentage = .05;
		currentOutput = 0;
		outPutPercentage = 0;
	}
	Engine() {
		increaseDownWithOutputPercentage = .05;
		currentOutput = 0;
		outPutPercentage = 0;
	}
	~Engine();

	//�₢���킹
	double ReferOutput() { return currentOutput; }
	double ReferMaxOutput() { return maxOutput; }
	double ReferOutputPercentage() { return outPutPercentage; }

	//�f�[�^����
	/*�ő�o�́A�o�͑�����*/
	void Inif(double MaxOutput, double BaseIncreaseRate) {
		maxOutput = MaxOutput;
		baseIncreaseRate = BaseIncreaseRate;
	}
	void SetOutPutPercentage(double Percentage) {
		outPutPercentage = Percentage;
	}
	
	//�v�Z
	void Update();//�G���W���̏�Ԃ��X�V����

private:
	double maxOutput;//�ő�o��
	double currentOutput;//���ݏo��
	double outPutPercentage;//�ڕW�o�̓p�[�Z���e�[�W
	double baseIncreaseRate;//�o�͑�����
	double increaseDownWithOutputPercentage;//���������l
};


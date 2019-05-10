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

	//問い合わせ
	double ReferOutput() { return currentOutput; }
	double ReferMaxOutput() { return maxOutput; }
	double ReferOutputPercentage() { return outPutPercentage; }

	//データ入力
	/*最大出力、出力増加率*/
	void Inif(double MaxOutput, double BaseIncreaseRate) {
		maxOutput = MaxOutput;
		baseIncreaseRate = BaseIncreaseRate;
	}
	void SetOutPutPercentage(double Percentage) {
		outPutPercentage = Percentage;
	}
	
	//計算
	void Update();//エンジンの状態を更新する

private:
	double maxOutput;//最大出力
	double currentOutput;//現在出力
	double outPutPercentage;//目標出力パーセンテージ
	double baseIncreaseRate;//出力増加率
	double increaseDownWithOutputPercentage;//増加制限値
};


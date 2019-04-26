#include "Engine.h"

Engine::~Engine()
{
}

void Engine::Update() {
	if (outPutPercentage > 0) {
		//増大する時は大きければ大きいほど増加速度が下がる
		if (currentOutput <= maxOutput * outPutPercentage) {
			if (currentOutput < 0)//反対になる時は早めに増大する
				currentOutput += 0.1;
			else
				/*基本増加率、制限に近づくと増加率が下がる*/
				currentOutput += baseIncreaseRate * increaseDownWithOutputPercentage *
				(1 - (currentOutput + 0.01) / maxOutput);
		}
		else {//出力パーセンテージは今の状況より高い時に出力がゆっくり減少する
				currentOutput -= baseIncreaseRate * increaseDownWithOutputPercentage *
				(currentOutput + 0.01) / maxOutput;
		}
	}
	if (outPutPercentage < 0) {
		if (currentOutput >= maxOutput * outPutPercentage) {
			if (currentOutput > 0)//反対になる時は早めに減少する
				currentOutput -= 0.1;
			else
				/*基本増加率、制限に近づくと増加率が下がる*/
				currentOutput -= baseIncreaseRate * increaseDownWithOutputPercentage*
				abs(1 - (currentOutput - 0.01)) / maxOutput;
		}
		else {//出力パーセンテージは今の状況より高い時に出力がゆっくり減少する
				currentOutput += baseIncreaseRate * increaseDownWithOutputPercentage *
				abs(currentOutput - 0.01) / maxOutput;
		}
	}
	if (outPutPercentage == 0) {//出力は０の時に固定単位で減少
		if (currentOutput > 0.1)
			currentOutput -= 0.1;
		if (currentOutput < -0.1)
			currentOutput += 0.1;
		if (currentOutput >= -.1&&currentOutput <= .1)
			currentOutput = 0;
	}
}

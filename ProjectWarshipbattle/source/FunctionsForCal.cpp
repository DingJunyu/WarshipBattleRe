#include "FunctionsForCal.h"

void DrawNumberInt(int num, int *numGraphicHandle) {
	int numAsList[100];
	int numLength = 0;
	while (1) {
		numAsList[numLength] = num % 10;
		num /= 10;
		if (num < 10) {
			numAsList[numLength] = num;
			break;
		}
		numLength++;
	}
}
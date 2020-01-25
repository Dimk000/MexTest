#include <iostream>
#include <vector>
//#include <tuple>
using namespace std;
#include "mex.h"


void Modulate(float pulseTime, float timeStep, float pulseNumbers, int Uref, float Uin, float* R, int* dout) {
	//vector<float> R((unsigned int)(pulseNumbers / timeStep + 1), 0);
	//vector<int> dout{ 1 };
	R[0] = 0;
	dout[0] = 1;

	int Compare, i, j;
	Compare = 1;
	i = 1;
	j = 1;
	float currentTime = timeStep;
	float currentT0 = pulseTime;
	while (currentTime <= pulseNumbers) {
		if (Compare == 1) {
			R[i] = R[i - 1] + (Uin + Uref) * timeStep;
		}
		else {
			R[i] = R[i - 1] + (Uin - Uref) * timeStep;
		}
		if (currentTime >= currentT0) {
			if (R[i] > 0)
				Compare = 0;
			else
				Compare = 1;
			currentT0 += pulseTime;
			dout[j] = Compare;
			j++;
		}
		i++;
		currentTime += timeStep;
	}
	return;
}

void mexFunction(int nlhs, mxArray *plhs[],
	int nrhs, const mxArray *prhs[])
{
	int Uref, *dout;
	float pulseTime, timeStep, pulseNumbers, Uin, *R;

	pulseTime = mxGetScalar(prhs[0]);
	timeStep = mxGetScalar(prhs[1]);
	pulseNumbers = mxGetScalar(prhs[2]);
	Uref = mxGetScalar(prhs[3]);
	Uin = mxGetScalar(prhs[4]);

	plhs[0] = mxCreateNumericMatrix(1, (unsigned int)(pulseNumbers / timeStep + 1), mxSINGLE_CLASS, mxREAL);
	plhs[1] = mxCreateNumericMatrix(1, (unsigned int)pulseNumbers + 1, mxINT32_CLASS, mxREAL);
	R = (float*)mxGetData(plhs[0]);
	dout = (int*)mxGetData(plhs[1]);

	Modulate(pulseTime, timeStep, pulseNumbers, Uref, Uin, R, dout);

	return;
}

//#include <iostream>
//#include <vector>
//#include <tuple>
//
//#ifndef CLASSC_MODULATORS_H
//#define CLASSC_MODULATORS_H
//
//using namespace std;
//
//class Modulator
//{
//public:
//	int Uref, dCoeff;
//	float pulseTime, timeStep, pulseNumbers;
//	vector<float>Uin;
//	Modulator(float, float, float, int, vector<float>);
//	//    Modulator(float, float, float, int, float, int);
//	tuple <vector<float>, vector<int>> Modulate();
//	//    vector<float> Modulate(float);
//};
//
//#endif //CLASSC_MODULATORS_H

#pragma once

#include"Drawer.h"


struct GaussParam
{
	double A;
	double s;
	double x0;
	GaussParam() :A(0), s(0), x0(0) {}
	GaussParam(double a, double s, double x0) :A(a), s(s), x0(x0) {}
};


class PB
{
private:
	GaussParam gParam[5];
	int N;
	double fd;

	Drawer sDrw;
protected:
	vector<double> CreateSignal(GaussParam* gP);
	double gauss(GaussParam gP, double t);

public:
	PB();
	void SetN(int n);
	void SetFd(double fd);
	void SetSDrwHWND(HWND hWnd);
	void SetGParam(GaussParam g1, GaussParam g2, GaussParam g3, GaussParam g4, GaussParam g5);
	void test();
};
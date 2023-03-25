#pragma once

#include<random>
#include<time.h>
#include<fstream>
#include"Drawer.h"
#include"cmplx.h"

const double pi2 = 8 * atan(1);
static CListBox* lg;

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
	double tau;
	Drawer sDrw;
	Drawer spdDrw;
	vector<double> sgn;
	vector<double>Spd;
	ofstream log;
protected:
	vector<double> CreateSignal(GaussParam* gP);
	double gauss(GaussParam gP, double t);
	template<typename T> vector<cmplx>toCmplx(vector<T>& data);
	vector<double>CreateSPD(vector<cmplx>& data);
	vector<cmplx>PB::Feinup(vector<double>& spd);
	vector<cmplx>init(vector<double>& spd);
	void removeNegative(vector<cmplx>& data);
	void PB::correctAbs(vector<cmplx>& data, vector<double>& spd);
	double PB::mistake(vector<cmplx>& data, vector<cmplx>& real);
	double PB::estimate(vector<cmplx>& data, vector<double>& real);
	vector<double> PB::GetReal(vector<cmplx>& data);

	double CalcE(vector<cmplx>& data);
	double CalcE(vector<double>& data);

public:
	PB();
	void SetTau(double tau);
	void SetN(int n);
	void SetFd(double fd);
	void SetSPDDrwHWND(HWND hWnd);
	void SetSDrwHWND(HWND hWnd);
	void SetGParam(GaussParam g1, GaussParam g2, GaussParam g3, GaussParam g4, GaussParam g5);
	void test();
	int ctr;
	bool NeedToDraw;
	vector<vector<double>>toDraw;
	void Draw();

};
#include"pch.h"
#include"PB.h"


PB::PB()
{
	N = 0;
	fd = 0;
}
void PB::SetN(int n)
{
	N = n;
}
void PB::SetFd(double Fd)
{
	fd = Fd;
}
void PB::SetSDrwHWND(HWND hWnd)
{
	sDrw.Create(hWnd);
}
void PB::SetGParam(GaussParam g1, GaussParam g2, GaussParam g3, GaussParam g4, GaussParam g5)
{
	gParam[0] = g1;
	gParam[1] = g2;
	gParam[2] = g3;
	gParam[3] = g4;
	gParam[4] = g5;
}


vector<double> PB::CreateSignal(GaussParam* gP)
{
	vector<double>res;
	for (int i = 0; i < N; i++)
	{
		double temp = 0;
		for (int g = 0; g < 5; g++)
		{
			temp += gauss(gP[g], i / fd);
		}
		res.push_back(temp);
	}
	return res;
}


double PB::gauss(GaussParam gPar, double t)
{
	return gPar.A * exp(-(t - gPar.x0) * (t - gPar.x0) / gPar.s / gPar.s);
}


void PB::test()
{
	auto sign = CreateSignal(gParam);
	sDrw.DrawGraph(sign, fd);
}
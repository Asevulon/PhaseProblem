#include"pch.h"
#include"PB.h"


PB::PB()
{
	N = 0;
	fd = 0;
	srand(time(NULL));
	log.open("log.txt");

}
void PB::SetN(int n)
{
	N = n;
}
void PB::SetTau(double Tau)
{
	tau = Tau;
}
void PB::SetFd(double Fd)
{
	fd = Fd;
}
void PB::SetSDrwHWND(HWND hWnd)
{
	sDrw.Create(hWnd);
}
void PB::SetSPDDrwHWND(HWND hWnd)
{
	spdDrw.Create(hWnd);
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
	sgn = sign;

	auto csign = toCmplx(sign);
	fourea(N, csign, -1);

	auto spd = CreateSPD(csign);
	spdDrw.DrawSpectr(spd, fd, -100);
	Spd = spd;

	auto S = Feinup(spd);
	res = GetReal(S);
	sDrw.DrawTwoSignals(sign, res, fd);
	log.close();
	

}
vector<double> PB::GetReal(vector<cmplx>& data)
{
	vector<double> res;
	for (auto& item : data)res.push_back(item.re);
	return res;
}

template<typename T> vector<cmplx> PB::toCmplx(vector<T>& data)
{
	vector<cmplx>res;
	for (auto& item : data)res.push_back(cmplx(item, 0));
	return res;
}


vector<double>PB::CreateSPD(vector<cmplx>& data)
{
	vector<double>res;
	for (auto& item : data)res.push_back(item.abs());
	return res;
}


vector<cmplx>PB::Feinup(vector<double>& spd)
{
	//init

	vector<cmplx>S = init(spd);
	ctr = 0;
	auto S_1 = S;
	while (1)
	{
		fourea(N, S, 1);

		removeNegative(S);
		if (NeedToDraw)
		{
			NeedToDraw = false;
			toDraw.push_back(GetReal(S));
		}
		if (mistake(S, S_1) < tau)break;
		S_1 = S;
		fourea(N, S, -1);
		correctAbs(S, spd);
		ctr++;
	}


	return S;
}


vector<cmplx>PB::init(vector<double>& spd)
{
	vector<cmplx>S;

	for (auto& item : spd)
	{
		double y = pi2 * double(rand()) / double(RAND_MAX);
		cmplx c;
		c.re = item * cos(y);
		c.im = item * sin(y);
		S.push_back(c);
	}
	return S;
}

void PB::removeNegative(vector<cmplx>& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		data[i].im = 0;
		if (data[i].re < 0)data[i].re = 0;
	}
}
void PB::correctAbs(vector<cmplx>& data, vector<double>& spd)
{
	for (int i = 0; i < data.size(); i++)
	{
		double y = data[i].phase();
		data[i].re = spd[i] * cos(y);
		data[i].im = spd[i] * sin(y);
	}
}


double PB::mistake(vector<cmplx>& data, vector<cmplx>& data2)
{
	double res = 0;
	for (int i = 0; i < data.size(); i++)
	{
		double temp = data[i].re - data2[i].re;
		res += temp * temp;
	}
	return res;
}


double PB::estimate(vector<cmplx>& data, vector<double>& real)
{
	double mist = 0;
	for (int i = 0; i < data.size(); i++)
	{
		double temp = data[i].re - real[i];
		mist += temp * temp;
	}
	double Es = 0;
	for (auto& item : real)Es += item * item;
	return 100. * mist / Es;
}


void PB::Draw()
{
	sDrw.DrawTwoSignals(sgn, toDraw[0], fd);
	toDraw.erase(toDraw.begin());
}


double PB::CalcE(vector<cmplx>& data)
{
	double res = 0;
	for (auto& item : data)res += item.re * item.re + item.im * item.im;
	return res;
}
double PB::CalcE(vector<double>& data)
{
	double res = 0;
	for (auto& item : data)res += item * item;
	return res;
}


void PB::RedrawFromData()
{
	if (sgn.empty())return;
	if (res.empty())return;
	sDrw.DrawTwoSignals(sgn, res, fd);
}


void PB::fixShift()
{
	int maxs = GetMaxId(sgn);
	int maxr = GetMaxId(res);
	int shift = maxs - maxr;


	if (shift == 0)
	RedrawFromData();


	vector<double>resMod;


	if (shift > 0)
	{
		for (int i = N - shift - 1; i < N; i++)resMod.push_back(res[i]);
		for (int i = 0; i < N - shift - 1; i++)resMod.push_back(res[i]);
	}
	else
	{
		for (int i = -shift; i < N; i++)resMod.push_back(res[i]);
		for (int i = 0; i < -shift; i++)resMod.push_back(res[i]);
	}
	res = resMod;
	RedrawFromData();
}


int PB::GetMaxId(vector<double>& data)
{
	int res = 0;
	for (int i = 1; i < data.size(); i++)
	{
		if (data[res] < data[i])res = i;
	}
	return res;
}


void PB::Mirror()
{
	vector<double>resMod;
	for (int i = N - 1; i > -1; i--)resMod.push_back(res[i]);
	res = resMod;
	RedrawFromData();
}


void PB::Fixing()
{
	fixShift();
	if (mistake(res, sgn) > 1)
	{
		Mirror();
		fixShift();
	}
}


double PB::mistake(vector<double>& data, vector<double>& data2)
{
	double res = 0;
	for (int i = 0; i < data.size(); i++)
	{
		double temp = data[i] - data2[i];
		res += temp * temp;
	}
	return res;
}


double PB::estimate()
{
	double mist = 0;
	for (int i = 0; i < res.size(); i++)
	{
		double temp = res[i] - sgn[i];
		mist += temp * temp;
	}
	double Es = 0;
	for (auto& item : sgn)Es += item * item;
	return 100. * mist / Es;
}
#pragma once

#include <afxwin.h>
#include <vector>
#include<algorithm>
using namespace std;


class Drawer
{
	// ������������� ������� ���������.
	CRect frame;
	// ��������� �� ������ ����, ������� ���������.
	CWnd * wnd;
	// �������� ���������, ����������� � ������� ���������.
	CDC * dc;
	// �������� ���������, ����������� � ������.
	CDC memDC;
	// ������ ��� ��������� ��������� memDC.
	CBitmap bmp;
	// ���� ��� ������������ ��������� ������������� ������.
	bool init;

protected:
	double convert_range(double data, double outmax, double outmin, double inmax, double inmin);
	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);


public:
	// ������������������� ������ ������ �� ������ HWND.
	void Create(HWND hWnd);
	
	void DrawGraph(vector<double> signal, double fd);
	void DrawSpectr(vector<double> signal, double fd, double SG);
	void DrawOverGraph(vector<double>& signal, double fd, int r = 0, int g = 255, int b = 0);
	void DrawOverSpectr(vector<double>& signal, double fd, int r = 0, int g = 255, int b = 0);
	void DrawTwoSignals(vector<double> signal1, vector<double> signal2, double fd);
	void DrawTwoSpecters(vector<double> signal1, vector<double> signal2, double fd, double SG);
	void DrawTwoSpecters(vector<double>& signal1, vector<double>& signal2, double fd, int shift);
	void DrawTwoRoflSpecters(vector<double>& signal1, vector<double>& signal2, double fd, int shift, int wlen);
	void DrawFourSpecters(vector<double> spectr, vector<double> rect, vector<double> trian, vector<double> rofl, double fd);
	void DrawThreeSignals(vector<double> signal1, vector<double> signal2, vector<double> signal3, /*vector<double> signal4,*/ double fd);
	void DrawStat(vector<double> stat, double left, double right);

	void test(int i);
	void Redraw();
	void Clear();
	void DrawFrom(Drawer drv);
};

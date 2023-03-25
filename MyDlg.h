
// MyDlg.h: файл заголовка
//

#pragma once

#define TIMERVAL 100
#include"PB.h"


// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHASEPROBLEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double A2;
	double A3;
	double A4;
	double A5;
	double x01;
	double x02;
	double x03;
	double x04;
	double x05;
	double s1;
	double s2;
	double s3;
	double s4;
	double s5;
	int N;
	double fd;
	afx_msg void OnBnClickedOk();
	PB pb;
	HANDLE thPb;
	double tau;

	DWORD tid;
	afx_msg void MyDlg::OnTimer(UINT_PTR idEvent);
	CListBox log;
	float stime;
};


DWORD WINAPI thrPB(LPVOID);
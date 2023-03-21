
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "PhaseProblem.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHASEPROBLEM_DIALOG, pParent)
	, A1(1)
	, A2(2)
	, A3(3)
	, A4(4)
	, A5(5)
	, x01(1)
	, x02(3)
	, x03(5)
	, x04(7)
	, x05(9)
	, s1(0.125)
	, s2(0.125)
	, s3(0.125)
	, s4(0.125)
	, s5(0.125)
	, N(1024)
	, fd(100)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, A2);
	DDX_Text(pDX, IDC_EDIT3, A3);
	DDX_Text(pDX, IDC_EDIT4, A4);
	DDX_Text(pDX, IDC_EDIT5, A5);
	DDX_Text(pDX, IDC_EDIT6, x01);
	DDX_Text(pDX, IDC_EDIT7, x02);
	DDX_Text(pDX, IDC_EDIT8, x03);
	DDX_Text(pDX, IDC_EDIT9, x04);
	DDX_Text(pDX, IDC_EDIT10, x05);
	DDX_Text(pDX, IDC_EDIT11, s1);
	DDX_Text(pDX, IDC_EDIT12, s2);
	DDX_Text(pDX, IDC_EDIT13, s3);
	DDX_Text(pDX, IDC_EDIT14, s4);
	DDX_Text(pDX, IDC_EDIT15, s5);
	DDX_Text(pDX, IDC_EDIT16, N);
	DDX_Text(pDX, IDC_EDIT17, fd);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	pb.SetSDrwHWND(GetDlgItem(IDC_PICTURE_SIGNAL)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	TerminateThread(thPb, 0);
	UpdateData();
	pb.SetFd(fd);
	pb.SetN(N);
	pb.SetGParam(
		GaussParam(A1, s1, x01),
		GaussParam(A2, s2, x02),
		GaussParam(A3, s3, x03),
		GaussParam(A4, s4, x04),
		GaussParam(A5, s5, x05)
	);
	thPb = CreateThread(NULL, NULL, thrPB, this, NULL, NULL);
	
}

DWORD WINAPI thrPB(LPVOID lp)
{
	MyDlg* dlg = (MyDlg*)lp;
	
	dlg->pb.test();

	return 0;
}
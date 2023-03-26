
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
	, A1(4)
	, A2(2)
	, A3(3.5)
	, A4(2.5)
	, A5(3.5)
	, x01(150)
	, x02(380)
	, x03(600)
	, x04(800)
	, x05(920)
	, s1(3)
	, s2(3)
	, s3(3)
	, s4(3)
	, s5(3)
	, N(1024)
	, fd(1)
	, tau(1e-6)
	, compare(false)
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
	DDX_Text(pDX, IDC_EDIT18, tau);
	DDX_Control(pDX, IDC_LIST1, log);
	DDX_Control(pDX, IDC_EDIT19, EstCtr);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &MyDlg::OnBnClickedCheck1)
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
	pb.SetSPDDrwHWND(GetDlgItem(IDC_PICTURE_SPD)->GetSafeHwnd());

	lg = &log;

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
	log.ResetContent();
	stime = clock();
	UpdateData();
	pb.SetTau(tau);
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
	CString str;
	str.Format(L"Начат расчет...");
	log.InsertString(-1, str);
	tid = SetTimer(1, TIMERVAL, NULL);

}

DWORD WINAPI thrPB(LPVOID lp)
{
	MyDlg* dlg = (MyDlg*)lp;
	
	dlg->pb.test();
	dlg->KillTimer(dlg->tid);
	CString str;
	str.Format(L"Выполнено!");
	dlg->log.InsertString(-1, str);
	str.Format(L"Число итераций: %d", dlg->pb.ctr);
	dlg->log.InsertString(-1, str);
	str.Format(L"Время выполнения: %.1f", (clock() - dlg->stime)/1000.);
	dlg->log.InsertString(-1, str);
	dlg->log.SetTopIndex(dlg->log.GetCount() - 1);

	double est = dlg->pb.estimate();
	str.Format(L"Оценка точности: %.4f", str);
	str += CString('%');
	dlg->EstCtr.SetWindowTextW(str);

	if (dlg->compare)dlg->pb.Fixing();
	else dlg->pb.RedrawFromData();

	return 0;
}




afx_msg void MyDlg::OnTimer(UINT_PTR idEvent)
{
	CString str;
	str.Format(L"Число итераций: %d", pb.ctr);
	log.InsertString(-1, str);
	
	str.Format(L"Текущее время выполнения: %.1f", (clock() - stime)/1000.);
	log.InsertString(-1, str);
	str.Format(L" ");
	log.InsertString(-1, str);
	log.SetTopIndex(log.GetCount() - 1);

	pb.NeedToDraw = true;
	while (!pb.toDraw.empty())
	{
		pb.Draw();
		str.Format(L"Отрисовано!");
		log.InsertString(-1, str);

	}
}

void MyDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	pb.fixShift();
}


void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	pb.Mirror();
}


void MyDlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	compare = !compare;
}

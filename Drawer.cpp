#include"pch.h"
#include"Drawer.h"


void Drawer::Create(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;


	memDC.FillSolidRect(frame, RGB(255, 255, 255));
}


double Drawer::convert_range(double data, double outmax, double outmin, double inmax, double inmin)
{
	double k = (outmax - outmin) / (inmax - inmin);
	return  (data - inmin) * k + outmin;
}
vector<double> Drawer::convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}


void Drawer::DrawGraph(vector<double> signal, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = *min_element(signal.begin(), signal.end());
	double data_y_max = *max_element(signal.begin(), signal.end());

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.2f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);


		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawSpectr(vector<double> signal, double fd, double SG)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CPen sg_pen(PS_SOLID, 2, RGB(0, 175, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = *max_element(signal.begin(), signal.end());


	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);


		memDC.LineTo(x, signal[i]);
	}


	memDC.SelectObject(&sg_pen);
	SG = convert_range(SG, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(actual_right, SG);
	memDC.LineTo(actual_left, SG);


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawOverGraph(vector<double>& signal, double fd, int r, int g, int b)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(r, g, b));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = *min_element(signal.begin(), signal.end());
	double data_y_max = *max_element(signal.begin(), signal.end());

	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawOverSpectr(vector<double>& signal, double fd, int r, int g, int b)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(r, g, b));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = *max_element(signal.begin(), signal.end());


	memDC.SelectObject(&data_pen);


	signal = convert_range(signal, actual_top, actual_bottom, data_y_max, data_y_min);


	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawTwoSignals(vector<double> signal1, vector<double> signal2, double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 4, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 100, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = min(*min_element(signal1.begin(), signal1.end()), *min_element(signal2.begin(), signal2.end()));
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}


	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < signal2.size(); i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawTwoSpecters(vector<double> signal1, vector<double> signal2, double fd, double SG)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 3, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 200));
	CPen sg_pen(PS_SOLID, 2, RGB(0, 175, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));



	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}

	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	memDC.SelectObject(&sg_pen);
	SG = convert_range(SG, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(actual_right, SG);
	memDC.LineTo(actual_left, SG);


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawTwoSpecters(vector<double>& signal1, vector<double>& signal2, double fd, int shift)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 3, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));



	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}

	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(shift*len, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < signal2.size(); i++)
	{
		double x = convert_range((shift + i) * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawTwoRoflSpecters(vector<double>& signal1, vector<double>& signal2, double fd, int shift, int wlen)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 3, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 0, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end()));



	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}

	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(shift * len, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < signal2.size(); i++)
	{
		double x = convert_range((shift + i*wlen) * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

void Drawer::DrawThreeSignals(vector<double> signal1, vector<double> signal2, vector<double> signal3, /*vector<double> signal4,*/ double fd)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 4, RGB(239, 137, 36));
	CPen data_pen2(PS_SOLID, 3, RGB(165, 3, 249));
	CPen data_pen3(PS_SOLID, 3, RGB(0, 174, 254));
	CPen data_pen4(PS_SOLID, 3, RGB(51, 173, 22));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = signal1.size();
	double data_x_min = 0;
	double data_x_max = (N - 1) / fd;
	double data_y_min = min(min(*min_element(signal1.begin(), signal1.end()), *min_element(signal2.begin(), signal2.end())),
							*min_element(signal3.begin(), signal3.end()));
	double data_y_max = max(max(*max_element(signal1.begin(), signal1.end()), *max_element(signal2.begin(), signal2.end())),
							*max_element(signal3.begin(), signal3.end()));

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	signal1 = convert_range(signal1, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal1[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal1[i]);
	}


	memDC.SelectObject(&data_pen2);
	signal2 = convert_range(signal2, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(0, actual_right, actual_left, data_x_max, data_x_min), signal2[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal2[i]);
	}


	memDC.SelectObject(&data_pen3);
	signal3 = convert_range(signal3, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), signal3[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal3[i]);
	}


	/*memDC.SelectObject(&data_pen4);
	signal4 = convert_range(signal4, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), signal4[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i / fd, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, signal4[i]);
	}*/


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawFourSpecters(vector<double> spectr, vector<double> rect, vector<double> trian, vector<double> rofl, double fd)
{
	if (!init) return;
	//if (spectr.empty())return;
	//if (rect.empty())return;
	//if (trian.empty())return;
	//if (rofl.empty())return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen1(PS_SOLID, 4, RGB(239, 137, 36));
	CPen data_pen2(PS_SOLID, 3, RGB(165, 3, 249));
	CPen data_pen3(PS_SOLID, 3, RGB(0, 174, 254));
	CPen data_pen4(PS_SOLID, 3, RGB(51, 173, 22));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	int N = spectr.size();
	double data_x_min = 0;
	double data_x_max = fd;
	double data_y_min = 0;
	double data_y_max = max(
		max(*max_element(spectr.begin(), spectr.end()), *max_element(rect.begin(), rect.end())),
		max(*max_element(trian.begin(), trian.end()), *max_element(rofl.begin(), rofl.end()))
							);



	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen1);
	spectr = convert_range(spectr, actual_top, actual_bottom, data_y_max, data_y_min);
	double len = fd / (N - 1);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), spectr[0]);
	for (int i = 0; i < N; i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, spectr[i]);
	}


	len = fd / (rect.size() - 1);
	memDC.SelectObject(&data_pen2);
	rect = convert_range(rect, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), rect[0]);
	for (int i = 0; i < rect.size(); i++)
	{
		double x = convert_range( i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, rect[i]);
	}


	len = fd / (trian.size() - 1);
	memDC.SelectObject(&data_pen3);
	trian = convert_range(trian, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), trian[0]);
	for (int i = 0; i < trian.size(); i++)
	{
		double x = convert_range(i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, trian[i]);
	}


	len = fd / (rofl.size() - 1);
	memDC.SelectObject(&data_pen4);
	rofl = convert_range(rofl, actual_top, actual_bottom, data_y_max, data_y_min);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), rofl[0]);
	for (int i = 0; i < rofl.size(); i++)
	{
		double x = convert_range( i * len, actual_right, actual_left, data_x_max, data_x_min);
		memDC.LineTo(x, rofl[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawStat(vector<double> stat, double left, double right)
{
	if (!init) return;

	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	double data_x_min = left;
	double data_x_max = right;
	double data_y_min = *min_element(stat.begin(), stat.end());
	double data_y_max = *max_element(stat.begin(), stat.end());

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.2f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	stat = convert_range(stat, actual_top, actual_bottom, data_y_max, data_y_min);


	double len = (right - left) / (stat.size() - 1);
	memDC.MoveTo(convert_range(data_x_min, actual_right, actual_left, data_x_max, data_x_min), stat[0]);
	for (int i = 0; i < stat.size(); i++)
	{
		double x = convert_range(left + i * len , actual_right, actual_left, data_x_max, data_x_min);


		memDC.LineTo(x, stat[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}


void Drawer::test(int i)
{
	if (!init) return;

	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	CString str;
	str.Format(L"%i", i);

	memDC.TextOutW(0, 0, str);
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::Clear()
{
	memDC.FillSolidRect(frame, RGB(255, 255, 255));
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::Redraw()
{
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawFrom(Drawer drv)
{
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &drv.memDC, 0, 0, SRCCOPY);
}

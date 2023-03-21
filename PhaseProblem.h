
// PhaseProblem.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CPhaseProblemApp:
// Сведения о реализации этого класса: PhaseProblem.cpp
//

class CPhaseProblemApp : public CWinApp
{
public:
	CPhaseProblemApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPhaseProblemApp theApp;

#pragma once

#include <duiwnd.h>
#include <wke.h>
#include <wkewebview.h>

namespace DuiEngine
{
	class CDuiWebkit: public CDuiWindow
		,public wke::wkeWebView
	{
	public:
		CDuiWebkit(void);
		~CDuiWebkit(void);
	};
}

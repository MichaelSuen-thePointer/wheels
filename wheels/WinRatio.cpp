#include "WinRatio.h"

namespace pl
{
namespace windows
{

void WinScroll::GetInfo(SCROLLINFO& Info)
{
	memset(&Info, 0, sizeof(Info));
	Info.cbSize = sizeof(Info);
	Info.fMask = SIF_ALL;
	SendMessageW(_Handle, SBM_GETSCROLLINFO, FALSE, reinterpret_cast<LPARAM>(&Info));
}

void WinScroll::SetInfo(SCROLLINFO& Info)
{
	Info.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	SendMessageW(_Handle, SBM_SETSCROLLINFO, TRUE, reinterpret_cast<LPARAM>(&Info));
}

WinScroll::WinScroll(WinContainer* Parent, bool Horizontal)
	: WinControl()
	, _LineSize(1)
{
	_CreateWindow(0,
				  WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | (Horizontal ? SBS_HORZ : SBS_VERT),
				  WC_SCROLLBAR,
				  Parent);
	SetMax(99);
	SetPageSize(10);
}

WinScroll::~WinScroll()
{}

LRESULT WinScroll::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_VSCROLL_DISPATCHED:
	case WM_HSCROLL_DISPATCHED:
		switch (LOWORD(wParam))
		{
		case SB_ENDSCROLL:
			OnEndScroll(this);
			break;
		case SB_LEFT:
			SetPosition(GetMin());
			OnScrollMin(this);
			OnPositionChanged(this);
			break;
		case SB_RIGHT:
			SetPosition(GetMax());
			OnScrollMax(this);
			OnPositionChanged(this);
			break;
		case SB_LINELEFT:
			SetPosition(GetPosition() - GetLineSize());
			OnArrowDec(this);
			OnPositionChanged(this);
			break;
		case SB_LINERIGHT:
			SetPosition(GetPosition() + GetLineSize());
			OnArrowInc(this);
			OnPositionChanged(this);
			break;
		case SB_PAGELEFT:
			SetPosition(GetPosition() - GetPageSize());
			OnPageDec(this);
			OnPositionChanged(this);
			break;
		case SB_PAGERIGHT:
			SetPosition(GetPosition() + GetPageSize());
			OnPageInc(this);
			OnPositionChanged(this);
			break;
		case SB_THUMBPOSITION:
		{
			SCROLLINFO Info;
			GetInfo(Info);
			SetPosition(Info.nTrackPos);
			OnThumbUp(this);
			OnPositionChanged(this);
			break;
		}
		case SB_THUMBTRACK:
		{
			SCROLLINFO Info;
			GetInfo(Info);
			SetPosition(Info.nTrackPos);
			OnTracking(this);
			OnPositionChanged(this);
			break;
		}
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProcedure);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProcedure);
	}
	return Result;
}

int WinScroll::GetMin()
{
	SCROLLINFO Info;
	GetInfo(Info);
	return Info.nMin;
}

void WinScroll::SetMin(int Value)
{
	SCROLLINFO Info;
	GetInfo(Info);
	Info.nMin = Value;
	SetInfo(Info);
}

int WinScroll::GetMax()
{
	SCROLLINFO Info;
	GetInfo(Info);
	return Info.nMax;
}

void WinScroll::SetMax(int Value)
{
	SCROLLINFO Info;
	GetInfo(Info);
	Info.nMax = Value;
	SetInfo(Info);
}

int WinScroll::GetPageSize()
{
	SCROLLINFO Info;
	GetInfo(Info);
	return Info.nPage;
}

void WinScroll::SetPageSize(int Value)
{
	SCROLLINFO Info;
	GetInfo(Info);
	Info.nPage = Value;
	SetInfo(Info);
}

int WinScroll::GetPosition()
{
	SCROLLINFO Info;
	GetInfo(Info);
	return Info.nPos;
}

void WinScroll::SetPosition(int Value)
{
	SCROLLINFO Info;
	GetInfo(Info);
	Info.nPos = Value;
	SetInfo(Info);
}

void WinTrack::GetTickVisible(bool& Top, bool& Bottom)
{
	if (GetStyle(TBS_NOTICKS))
	{
		Top = false;
		Bottom = false;
	}
	else if (GetStyle(TBS_BOTH))
	{
		Top = true;
		Bottom = true;
	}
	else if (GetStyle(TBS_LEFT) || GetStyle(TBS_TOP))
	{
		Top = true;
		Bottom = false;
	}
	else
	{
		Top = false;
		Bottom = true;
	}
}

void WinTrack::SetTickVisible(bool Top, bool Bottom)
{
	SetStyle(TBS_LEFT | TBS_TOP, false);
	SetStyle(TBS_RIGHT | TBS_BOTTOM, false);
	SetStyle(TBS_NOTICKS, false);
	SetStyle(TBS_BOTH, false);
	if (Top)
	{
		if (Bottom)
		{
			SetStyle(TBS_BOTH, true);
		}
		else
		{
			SetStyle(TBS_LEFT | TBS_TOP, true);
		}
	}
	else
	{
		if (Bottom)
		{
			SetStyle(TBS_RIGHT | TBS_BOTTOM, true);
		}
		else
		{
			SetStyle(TBS_NOTICKS, true);
		}
	}
}

WinTrack::WinTrack(WinContainer* Parent, bool Horizontal)
	: WinControl()
{
	_CreateWindow(0,
				  WS_TABSTOP | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | (Horizontal ? TBS_HORZ : TBS_VERT) | TBS_BOTTOM | TBS_RIGHT,
				  TRACKBAR_CLASSW,
				  Parent);
	SendMessageW(_Handle, TBM_SETUNICODEFORMAT, TRUE, 0);
	SetMax(10);
}
WinTrack::~WinTrack()
{}

LRESULT WinTrack::ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProc)
{
	LRESULT Result = 0;
	switch (Message)
	{
	case WM_VSCROLL_DISPATCHED:
	case WM_HSCROLL_DISPATCHED:
		switch (LOWORD(wParam))
		{
		case TB_ENDTRACK:
			OnEndScroll(this);
			break;
		case TB_TOP:
			OnScrollMin(this);
			OnPositionChanged(this);
			break;
		case TB_BOTTOM:
			OnScrollMax(this);
			OnPositionChanged(this);
			break;
		case TB_LINEUP:
			OnArrowDec(this);
			OnPositionChanged(this);
			break;
		case TB_LINEDOWN:
			OnArrowInc(this);
			OnPositionChanged(this);
			break;
		case TB_PAGEUP:
			OnPageDec(this);
			OnPositionChanged(this);
			break;
		case TB_PAGEDOWN:
			OnPageInc(this);
			OnPositionChanged(this);
			break;
		case TB_THUMBPOSITION:
			OnThumbUp(this);
			OnPositionChanged(this);
			break;
		case TB_THUMBTRACK:
			OnTracking(this);
			OnPositionChanged(this);
			break;
		}
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
		break;
	default:
		Result = WinControl::ProcessMessage(Message, wParam, lParam, CallDefaultProc);
	}
	return Result;
}

bool WinTrack::GetShowLeftTopTick()
{
	bool Top, Bottom;
	GetTickVisible(Top, Bottom);
	return Top;
}

void WinTrack::SetShowLeftTopTick(bool Value)
{
	bool Top, Bottom;
	GetTickVisible(Top, Bottom);
	Top = Value;
	SetTickVisible(Top, Bottom);
}

bool WinTrack::GetShowRightBottomTick()
{
	bool Top, Bottom;
	GetTickVisible(Top, Bottom);
	return Bottom;
}

void WinTrack::SetShowRightBottomTick(bool Value)
{
	bool Top, Bottom;
	GetTickVisible(Top, Bottom);
	Bottom = Value;
	SetTickVisible(Top, Bottom);
}

void WinTrack::GetAdditionalTicks(std::vector<int>& TickPos)
{
	int Count = GetTicksCount() - 2;
	if (Count <= 0)
	{
		TickPos.clear();
	}
	else
	{
		TickPos.reserve(Count);
		DWORD* Ticks = reinterpret_cast<DWORD*>(SendMessageW(_Handle, TBM_GETPTICS, 0, 0));
		TickPos.insert(TickPos.end(), Ticks, Ticks + Count);
	}
}

WinProgress::WinProgress(WinContainer* Parent, bool Horizontal)
	: _MarqueeRunning(true)
	, _MarqueeInterval(5000)
{
	_CreateWindow(0,
				  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | (Horizontal ? 0 : PBS_VERTICAL),
				  PROGRESS_CLASSW,
				  Parent);
	SetMax(100);
	SetStep(5);
	InternalSetMarquee();
}

WinProgress::~WinProgress()
{}

}
}

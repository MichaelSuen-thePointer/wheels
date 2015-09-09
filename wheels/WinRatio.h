#pragma once
#ifndef WINRATIO_H
#define WINRATIO_H

#include "WinClass.h"
#include "commctrl.h"

namespace pl
{
namespace windows
{

class WinScroll: public WinControl
{
protected:
	int _LineSize;

	void GetInfo(SCROLLINFO& Info);
	void SetInfo(SCROLLINFO& Info);
public:
	WinScroll(WinContainer* Parent, bool Horizontal);
	~WinScroll();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnScrollMin;
	NotifyEvent OnScrollMax;
	NotifyEvent OnPageDec;
	NotifyEvent OnPageInc;
	NotifyEvent OnArrowDec;
	NotifyEvent OnArrowInc;
	NotifyEvent OnTracking;
	NotifyEvent OnThumbUp;
	NotifyEvent OnPositionChanged;
	NotifyEvent OnEndScroll;

	int GetMin();
	void SetMin(int Value);

	int GetMax();
	void SetMax(int Value);

	int GetLineSize();
	void SetLineSize(int Value);

	int GetPosition();
	void SetPosition(int Value);
};

class WinTrack: public WinControl
{
protected:
	void GetTickVisible(bool& Top, bool& Bottom);
	void SetTickVisible(bool Top, bool Bottom);
public:
	WinTrack(WinContainer* Parent, bool Horizontal);
	~WinTrack();

	LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

	NotifyEvent OnScrollMin;
	NotifyEvent OnScrollMax;
	NotifyEvent OnPageDec;
	NotifyEvent OnPageInc;
	NotifyEvent OnArrowDec;
	NotifyEvent OnArrowInc;
	NotifyEvent OnTracking;
	NotifyEvent OnThumbUp;
	NotifyEvent OnPositionChanged;
	NotifyEvent OnEndScroll;

	bool GetShowLeftTopTick();
	void SetShowLeftTopTick(bool Value);

	bool GetShowRightBottomTick();
	void SetShowRightBottomTick(bool Value);

	bool GetEnableSelection();
	void SetEnableSelection(bool Value);

	bool SetAutoTicks();
	void GetAutoTicks(bool Value);

	int GetMax();
	void SetMax(int Value);

	int GetMin();
	void SetMin(int Value);

	int GetLineSize();
	void SetLineSize(int Value);

	int GetPageSize();
	void SetPageSize(int Value);

	int GetPosition();
	void SetPosition(int Value);

	int GetSelStart();
	void SetSelStart(int Value);

	int GetSelEnd();
	void SetSelEnd(int Value);

	void ClearSelection();

	void ClearTicks();
	int GetTicksCount();
	void GetAdditionalTicks(std::vector<int>& TickPos);
	int GetTickPos(int Index);
	void SetTick(int Position);
	void BuildAutoTicks(int Value);
};

class WinProgress: public WinControl
{
protected:
	bool _MarqueeRunning;
	int _MarqueeInterval;

	void InternalSetMarque();
public:
	WinProgress(WinContainer* Parent, bool Horizontal);
	~WinProgress();

	bool GetMarquee();
	void SetMarquee(bool Value);

	bool GetSmooth();
	void SetSmooth(bool Value);

	bool GetSmoothReverse();
	void SetSmoothReverse(bool Value);

	COLORREF GetBarColor();
	void SetBarColor(COLORREF Value);

	COLORREF GetBackgroundColor();
	void SetBackgroundColor(COLORREF Value);

	int GetPosition();
	void SetPosition(int Value);

	int GetMin();
	void SetMin(int Value);

	int GetMax();
	void SetMax(int Value);

	int GetStep();
	void SetStep(int Value);

	bool GetMarqueeRunning();
	void SetMarqueeRunning(bool Value);

	int GetMarqueeInterval();
	void SetMarqueeInterval(int Value);
};
}
}
#endif // !WINRATIO_H

inline
int WinScroll::GetLineSize()
{
	return _LineSize;
}

inline
void WinScroll::SetLineSize(int Value)
{
	_LineSize = Value;
}

inline
bool WinTrack::GetEnableSelection()
{
	return GetStyle(TBS_ENABLESELRANGE);
}

inline
void WinTrack::SetEnableSelection(bool Value)
{
	SetStyle(TBS_ENABLESELRANGE, Value);
}

inline
bool WinTrack::GetAutoTicks()
{
	return GetStyle(TBS_AUTOTICKS);
}

inline
void WinTrack::SetAutoTicks(bool Value)
{
	SetStyle(TBS_AUTOTICKS, Value);
}

inline
int WinTrack::GetMin()
{
	return SendMessageW(_Handle, TBM_GETRANGEMIN, 0, 0);
}

inline
void WinTrack::SetMin(int Value)
{
	SendMessageW(_Handle, TBM_SETRANGEMIN, TRUE, Value);
}

inline
int WinTrack::GetMax()
{
	return SendMessageW(_Handle, TBM_GETRANGEMAX, 0, 0);
}

inline
void WinTrack::SetMax(int Value)
{
	SendMessageW(_Handle, TBM_SETRANGEMAX, TRUE, Value);
}

inline
int WinTrack::GetLineSize()
{
	return SendMessageW(_Handle, TBM_GETLINESIZE, 0, 0);
}

inline
void WinTrack::SetLineSize(int Value)
{
	SendMessageW(_Handle, TBM_SETLINESIZE, 0, Value);
}

inline
int WinTrack::GetPageSize()
{
	return SendMessageW(_Handle, TBM_GETPAGESIZE, 0, 0);
}

inline
void WinTrack::SetPageSize(int Value)
{
	SendMessageW(_Handle, TBM_SETPAGESIZE, 0, Value);
}

inline
int WinTrack::GetPosition()
{
	return SendMessageW(_Handle, TBM_GETPOS, 0, 0);
}

inline
void WinTrack::SetPosition(int Value)
{
	SendMessageW(_Handle, TBM_SETPOS, TRUE, Value);
}

inline
int WinTrack::GetSelStart()
{
	return SendMessageW(_Handle, TBM_GETSELSTART, 0, 0);
}

inline
void WinTrack::SetSelStart(int Value)
{
	SendMessageW(_Handle, TBM_SETSELSTART, TRUE, Value);
}

inline
int WinTrack::GetSelEnd()
{
	return SendMessageW(_Handle, TBM_GETSELEND, 0, 0);
}

inline
void WinTrack::SetSelEnd(int Value)
{
	SendMessageW(_Handle, TBM_SETSELEND, TRUE, Value);
}

inline
void WinTrack::ClearSelection()
{
	SendMessageW(_Handle, TBM_CLEARSEL, TRUE, 0);
}

inline
void WinTrack::ClearTicks()
{
	SendMessageW(_Handle, TBM_CLEARTICS, TRUE, 0);
}

inline
int WinTrack::GetTicksCount()
{
	return SendMessageW(_Handle, TBM_GETNUMTICS, 0, 0);
}

inline
int WinTrack::GetTickPos(int Index)
{
	return SendMessageW(_Handle, TBM_GETTIC, Index, 0);
}

inline
void WinTrack::SetTick(int Position)
{
	SendMessageW(_Handle, TBM_SETTIC, 0, Position);
}

inline
void WinTrack::BuildAutoTicks(int Value)
{
	SendMessageW(_Handle, TBM_SETTICFREQ, Value, 0);
}

inline
void WinProgress::InternalSetMarquee()
{
	SendMessageW(_Handle, PBM_SETMARQUEE, (_MarqueeRunning ? TRUE : FALSE), _MarqueeInterval / 100);
}

inline
bool WinProgress::GetMarquee()
{
	return GetStyle(PBS_MARQUEE);
}

inline
void WinProgress::SetMarquee(bool Value)
{
	SetStyle(PBS_MARQUEE, Value);
}

inline
bool WinProgress::GetSmooth()
{
	return GetStyle(PBS_SMOOTH);
}

inline
void WinProgress::SetSmooth(bool Value)
{
	SetStyle(PBS_SMOOTH, Value);
}

inline
bool WinProgress::GetSmoothReverse()
{
	return GetStyle(PBS_SMOOTHREVERSE);
}

inline
void WinProgress::SetSmoothReverse(bool Value)
{
	SetStyle(PBS_SMOOTHREVERSE, Value);
}

inline
COLORREF WinProgress::GetBarColor()
{
	return SendMessageW(_Handle, PBM_GETBARCOLOR, 0, 0);
}

inline
void WinProgress::SetBarColor(COLORREF Value)
{
	SendMessageW(_Handle, PBM_SETBARCOLOR, 0, Value);
}

inline
COLORREF WinProgress::GetBackgroundColor()
{
	return SendMessageW(_Handle, PBM_GETBKCOLOR, 0, 0);
}

inline
void WinProgress::SetBackgroundColor(COLORREF Value)
{
	SendMessageW(_Handle, PBM_SETBKCOLOR, 0, Value);
}

inline
int WinProgress::GetPosition()
{
	return SendMessageW(_Handle, PBM_GETPOS, 0, 0);
}

inline
void WinProgress::SetPosition(int Value)
{
	SendMessageW(_Handle, PBM_SETPOS, Value, 0);
}

inline
int WinProgress::GetMin()
{
	return SendMessageW(_Handle, PBM_GETRANGE, TRUE, 0);
}

inline
void WinProgress::SetMin(int Value)
{
	SendMessageW(_Handle, PBM_SETRANGE32, Value, GetMax());
}

inline
int WinProgress::GetMax()
{
	return SendMessageW(_Handle, PBM_GETRANGE, FALSE, 0);
}

inline
void WinProgress::SetMax(int Value)
{
	SendMessageW(_Handle, PBM_SETRANGE32, GetMin(), Value);
}

inline
int WinProgress::GetStep()
{
	return SendMessageW(_Handle, PBM_GETSTEP, 0, 0);
}

inline
void WinProgress::SetStep(int Value)
{
	SendMessageW(_Handle, PBM_SETSTEP, Value, 0);
}

inline
bool WinProgress::GetMarqueeRunning()
{
	return _MarqueeRunning;
}

inline
void WinProgress::SetMarqueeRunning(bool Value)
{
	_MarqueeRunning = Value;
	InternalSetMarquee();
}

inline
int WinProgress::GetMarqueeInterval()
{
	return _MarqueeInterval;
}

inline
void WinProgress::SetMarqueeInterval(int Value)
{
	_MarqueeInterval = Value;
	InternalSetMarquee();
}



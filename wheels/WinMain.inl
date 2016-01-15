inline
HINSTANCE WinApplication::GetInstance()
{
	return _Instance;
}

inline
void WinApplication::DoEvents()
{
	ProcessMessage(false);
}

inline
WinForm* WinApplication::GetMainForm()
{
	return _MainForm;
}

inline
WinPen::Ptr WinApplication::GetDefaultPen()
{
	return _Pen;
}

inline
WinBrush::Ptr WinApplication::GetDefaultBrush()
{
	return _Brush;
}

inline
WinFont::Ptr WinApplication::GetDefaultFont()
{
	return _Font;
}

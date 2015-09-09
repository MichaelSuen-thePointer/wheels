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
WinPen::Pointer WinApplication::GetDefaultPen()
{
	return _Pen;
}

inline
WinBrush::Pointer WinApplication::GetDefaultBrush()
{
	return _Brush;
}

inline
WinFont::Pointer WinApplication::GetDefaultFont()
{
	return _Font;
}

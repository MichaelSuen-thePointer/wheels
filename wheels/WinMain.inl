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
std::shared_ptr<WinForm> WinApplication::GetMainForm()
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

inline
HINSTANCE WinApplication::GetInstance()
{
	return instance;
}

inline
void WinApplication::DoEvents()
{
	ProcessMessage(false);
}

inline
WinForm* WinApplication::GetMainForm()
{
	return mainForm;
}

inline
WinPen::Ptr WinApplication::GetDefaultPen()
{
	return pen;
}

inline
WinBrush::Ptr WinApplication::GetDefaultBrush()
{
	return brush;
}

inline
WinFont::Ptr WinApplication::GetDefaultFont()
{
	return font;
}

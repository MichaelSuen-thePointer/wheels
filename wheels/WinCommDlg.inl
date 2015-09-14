inline
bool WinFileDialog::GetMultiSelection()
{
	return _MultiSelection;
}
inline
void WinFileDialog::SetMultiSelection(bool Value)
{
	_MultiSelection = true;
}
inline
bool WinFileDialog::GetAddToRecent()
{
	return _AddToRecent;
}
inline
void WinFileDialog::SetAddToRecent(bool Value)
{
	_AddToRecent = Value;
}
inline
bool WinFileDialog::GetEnableSizing()
{
	return _EnableSizing;
}
inline
void WinFileDialog::SetEnableSizing(bool Value)
{
	_EnableSizing = Value;
}
inline
bool WinFileDialog::GetShowHidden()
{
	return _ShowHidden;
}
inline
void WinFileDialog::SetShowHidden(bool Value)
{
	_ShowHidden = Value;
}
inline
bool WinFileDialog::GetDereferenceLink()
{
	return _DereferenceLink;
}
inline
void WinFileDialog::SetDereferenceLink(bool Value)
{
	_DereferenceLink = Value;
}
inline
bool WinFileDialog::GetNetworkButton()
{
	return _NetworkButton;
}
inline
void WinFileDialog::SetNetworkButton(bool Value)
{
	_NetworkButton = Value;
}
inline
std::wstring WinFileDialog::GetTitle()
{
	return _Title;
}
inline
void WinFileDialog::SetTitle(const std::wstring& Value)
{
	_Title = Value;
}
inline
std::wstring WinFileDialog::GetInitDir()
{
	return _InitDir;
}
inline
void WinFileDialog::SetInitDir(const std::wstring& Value)
{
	_InitDir = Value;
}
inline
std::wstring WinFileDialog::GetFilter()
{
	return _Filter;
}
inline
void WinFileDialog::SetFilter(const std::wstring& Value)
{
	_Filter = Value;
}
inline
std::wstring WinFileDialog::GetDefaultExt()
{
	return _DefaultExt;
}
inline
void WinFileDialog::SetDefaultExt(const std::wstring& Value)
{
	_DefaultExt = Value;
}

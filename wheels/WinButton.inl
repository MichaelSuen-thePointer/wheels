inline
bool WinCustomChecked::GetChecked()
{
	return SendMessageW(handle, BM_GETCHECK, 0, 0) == BST_CHECKED;
}
inline
void WinCustomChecked::SetChecked(bool Value)
{
	SendMessageW(handle, BM_SETCHECK, (Value ? BST_CHECKED : BST_UNCHECKED), 0);
}
inline
bool WinCustomChecked::GetLeftText()
{
	return GetStyle(BS_LEFTTEXT);
}
inline
void WinCustomChecked::SetLeftText(bool Value)
{
	SetStyle(BS_LEFTTEXT, Value);
}
inline
bool WinCustomChecked::GetPushLike()
{
	return GetStyle(BS_PUSHLIKE);
}
inline
void WinCustomChecked::SetPushLike(bool Value)
{
	SetStyle(BS_PUSHLIKE, Value);
}
inline
int WinRadio::GetGroupNumber()
{
	return groupNumber;
}
inline
void WinRadio::SetGroupNumber(int Value)
{
	groupNumber = Value;
}

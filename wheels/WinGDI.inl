inline
bool IsEqual(WinRegion::Pointer Region1, WinRegion::Pointer Region2)
{
	return EqualRgn(Region1->GetHandle(), Region2->GetHandle()) != 0;
}

inline
HRGN WinRegion::GetHandle()
{
	return _Handle;
}

inline
bool WinRegion::ContainPoint(POINT Point)
{
	return PtInRegion(_Handle, Point.x, Point.y) == 1;
}

inline
bool WinRegion::ContainRectangle(RECT Rect)
{
	return RectInRegion(_Handle, &Rect) == 1;
}

inline
RECT WinRegion::GetBoundRectangle()
{
	RECT Rect = {0};
	GetRgnBox(_Handle, &Rect);
	return Rect;	
}

inline
void WinRegion::Move(int OffsetX, int OffsetY)
{
	OffsetRgn(_Handle, OffsetX, OffsetY);
}

inline
WinTransform& WinTransform::operator=(const WinTransform& Transform)
{
	_Transform = Transform._Transform;
	return *this;
}

inline
WinTransform WinTransform::operator*(const WinTransform& Transform)
{
	XFORM Result;
	CombineTransform(&Result, GetHandle(), Transform.GetHandle());
	return Result;
}

inline
const XFORM* WinTransform::GetHandle() const
{
	return &_Transform;
}

inline
WinTransform WinTransform::Translate(float OffsetX, float OffsetY)
{
	return WinTransform(XFORM{1.0F,0.0f,0.0F,1.0F, OffsetX, OffsetY});
}

inline
WinTransform WinTransform::Scale(float ScaleX, float ScaleY)
{
	return WinTransform(XFORM{ScaleX, 0.0F, 0.0F, ScaleY, 0.0F, 0.0F});
}

inline
WinTransform WinTransform::Rotate(float Angle)
{
	return Rotate(sin(Angle), cos(Angle));
}

inline
WinTransform WinTransform::Rotate(float cos, float sin)
{
	return WinTransform(XFORM{cos, sin, -sin, cos, 0.0F, 0.0F});
}

inline
WinTransform WinTransform::ReflectX()
{
	return WinTransform(XFORM{1.0F, 0.0F,0.0F,-1.0F, 0.0F, 0.0F});
}

inline
WinTransform WinTransform::ReflectY()
{
	return WinTransform(XFORM{-1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F});
}

inline
WinTransform WinTransform::Reflect(float VectorX, float VectorY)
{
	float len = sqrt(VectorX * VectorX + VectorY * VectorY);
	float cos = VectorX / len;
	float sin = VectorY / len;
	return Rotate(cos, -sin) * ReflectX() * Rotate(cos, sin);
}

inline
WinTransform WinTransform::Reflect(float OriginX, float OriginY, float VectorX, float VectorY)
{
	float len = sqrt(VectorX * VectorX + VectorY * VectorY);
	float cos = VectorX / len;
	float sin = VectorY / len;
	return Translate(-OriginX, -OriginY) * Rotate(cos, -sin) * ReflectX() * Rotate(cos, sin) * Translate(OriginX, OriginY);
}

inline
WinTransform WinTransform::AxisV(float Xx, float Xy, float Yx, float Yy)
{
	return WinTransform(XFORM{Xx, Xy, Yx, Yy, 0.0F, 0.0F});
}

inline
WinTransform WinTransform::AxisA(float AngleX, float LenX, float AngleY, float LenY)
{
	return WinTransform(XFORM{cos(AngleX)*LenX, cos(AngleX)*LenX, cos(AngleY)*LenY, cos(AngleY)*LenY, 0.0F, 0.0F});
}

inline
void WinMetaFileBuilder::Draw(HENHMETAFILE Handle)
{
	RECT Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = _Width;
	Rect.bottom = _Height;
	PlayEnhMetaFile(_DC->GetHandle(), Handle, &Rect);
}

inline
void WinMetaFileBuilder::Destroy()
{
	DeleteEnhMetaFile(CloseEnhMetaFile(_DC->GetHandle));
}

inline
WinDC* WinMetaFileBuilder::GetWinDC()
{
	return _DC;
}

inline
int WinMetaFileBuilder::GetWidth()
{
	return _Width;
}

inline
int WinMetaFileBuilder::GetHeight()
{
	return _Height;
}

inline
HENHMETAFILE WinMetaFile::GetHandle()
{
	return _Handle;
}

inline
int WinMetaFile::GetWidth()
{
	return _Width;
}

inline
int WinMetaFile::GetHeight()
{
	return _Height;
}

inline
int WinBitmap::GetBitsFromBB(BitmapBits BB)
{
	switch (BB)
	{
	case BitmapBits::Bit32:
		return 32;
	case BitmapBits::Bit24:
		return 24;
	default:
		return 1;
	}
}

inline
WinDC* WinBitmap::GetWinDC()
{
	return _DC;
}

inline
int WinBitmap::GetWidth()
{
	return _Width;
}

inline
int WinBitmap::GetHeight()
{
	return _Height;
}

inline
int WinBitmap::GetLineBytes()
{
	return GetLineBytes(_Width, _Bits);
}

inline
BYTE** WinBitmap::GetScanLines()
{
	return _ScanLines;
}

inline
HBITMAP WinBitmap::GetBitmap()
{
	return _Handle;
}

inline
WinBitmap::BitmapBits WinBitmap::GetBitmapBits()
{
	return _Bits;
}

inline
void WinBitmap::FillCompatibleHeader(BITMAPINFOHEADER* Header)
{
	FillBitmapInfoHeader(_Width, _Height, _Bits, Header);
}

inline
bool WinBitmap::CanBuildAlphaChannel()
{
	return _ScanLines != 0 && _Bits == BitmapBits::Bit32;
}

inline
bool WinBitmap::IsAlphaChannelBuilt()
{
	return _AlphaChannelBuilt;
}

inline
HBRUSH WinBrush::GetHandle()
{
	return _Handle;
}

inline
HPEN WinPen::GetHandle()
{
	return _Handle;
}

inline
HFONT WinFont::GetHandle()
{
	return _Handle;
}

inline
LOGFONT* WinFont::GetInfo()
{
	return &_FontInfo;
}

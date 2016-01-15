inline
bool IsEqual(WinRegion::Ptr Region1, WinRegion::Ptr Region2)
{
	return EqualRgn(Region1->GetHandle(), Region2->GetHandle()) != 0;
}

inline
HRGN WinRegion::GetHandle()
{
	return handle;
}

inline
bool WinRegion::ContainPoint(POINT Point)
{
	return PtInRegion(handle, Point.x, Point.y) == 1;
}

inline
bool WinRegion::ContainRectangle(RECT Rect)
{
	return RectInRegion(handle, &Rect) == 1;
}

inline
RECT WinRegion::GetBoundRectangle()
{
	RECT Rect = {0};
	GetRgnBox(handle, &Rect);
	return Rect;	
}

inline
void WinRegion::Move(int OffsetX, int OffsetY)
{
	OffsetRgn(handle, OffsetX, OffsetY);
}

inline
WinTransform& WinTransform::operator=(const WinTransform& Transform)
{
	transform = Transform.transform;
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
	return &transform;
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
	Rect.right = width;
	Rect.bottom = height;
	PlayEnhMetaFile(proxyDC->GetHandle(), Handle, &Rect);
}

inline
void WinMetaFileBuilder::Destroy()
{
	DeleteEnhMetaFile(CloseEnhMetaFile(proxyDC->GetHandle()));
}

inline
WinDC* WinMetaFileBuilder::GetWinDC()
{
	return proxyDC;
}

inline
int WinMetaFileBuilder::GetWidth()
{
	return width;
}

inline
int WinMetaFileBuilder::GetHeight()
{
	return height;
}

inline
HENHMETAFILE WinMetaFile::GetHandle()
{
	return handle;
}

inline
int WinMetaFile::GetWidth()
{
	return width;
}

inline
int WinMetaFile::GetHeight()
{
	return height;
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
	return imageDC;
}

inline
int WinBitmap::GetWidth()
{
	return width;
}

inline
int WinBitmap::GetHeight()
{
	return height;
}

inline
int WinBitmap::GetLineBytes()
{
	return GetLineBytes(width, bits);
}

inline
BYTE** WinBitmap::GetScanLines()
{
	return scanLines;
}

inline
HBITMAP WinBitmap::GetBitmap()
{
	return handle;
}

inline
WinBitmap::BitmapBits WinBitmap::GetBitmapBits()
{
	return bits;
}

inline
void WinBitmap::FillCompatibleHeader(BITMAPINFOHEADER* Header)
{
	FillBitmapInfoHeader(width, height, bits, Header);
}

inline
bool WinBitmap::CanBuildAlphaChannel()
{
	return scanLines != 0 && bits == BitmapBits::Bit32;
}

inline
bool WinBitmap::IsAlphaChannelBuilt()
{
	return hasAlphaChannelBuilt;
}

inline
HBRUSH WinBrush::GetHandle()
{
	return handle;
}

inline
HPEN WinPen::GetHandle()
{
	return handle;
}

inline
HFONT WinFont::GetHandle()
{
	return handle;
}

inline
LOGFONT* WinFont::GetInfo()
{
	return &fontInfo;
}

inline
HDC WinDC::GetHandle()
{
	return handle;
}
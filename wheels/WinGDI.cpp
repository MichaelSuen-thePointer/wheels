#include "WinMain.h"
#include <fstream>
#pragma comment(lib, "Msimg32.lib")

namespace pl
{
namespace windows
{

WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, bool IsRectangle)
	: _Handle(0)
{
	if (IsRectangle)
	{
		_Handle = CreateRectRgn(Left, Top, Right, Bottom);
	}
	else
	{
		_Handle = CreateEllipticRgn(Left, Top, Right, Bottom);
	}
}

WinRegion::WinRegion(RECT Rect, bool IsRectangle)
	: _Handle(0)
{
	if (IsRectangle)
	{
		_Handle = CreateRectRgnIndirect(&Rect);
	}
	else
	{
		_Handle = CreateEllipticRgnIndirect(&Rect);
	}
}

WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
	: _Handle(CreateRoundRectRgn(Left, Top, Right, Bottom, EllipseWidth, EllipseHeight))
{
}

WinRegion::WinRegion(POINT Points[], int Count, bool Alternate)
	: _Handle(CreatePolygonRgn(Points, Count, Alternate ? ALTERNATE : WINDING))
{
}

WinRegion::WinRegion(WinRegion::Pointer Region)
	: _Handle(CreateRectRgn(0, 0, 1, 1))
{
	CombineRgn(_Handle, Region->GetHandle(), Region->GetHandle(), RGN_COPY);
}

WinRegion::WinRegion(WinRegion::Pointer Region1, WinRegion::Pointer Region2, int CombineMode)
	: _Handle(CreateRectRgn(0, 0, 1, 1))
{
	CombineRgn(_Handle, Region1->GetHandle(), Region2->GetHandle(), CombineMode);
}

WinRegion::WinRegion(HRGN RegionHandle)
	: _Handle(RegionHandle)
{
}

WinRegion::~WinRegion()
{
	DeleteObject(_Handle);
}

WinTransform::WinTransform(XFORM Transform)
	: _Transform(Transform)
{
}

WinTransform::WinTransform(const WinTransform& Transform)
	: _Transform(Transform._Transform)
{
}

void WinMetaFileBuilder::Create(int Width, int Height)
{
	HDC hdcRef = GetDC(NULL);
	int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE);
	int iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE);
	int iWidthPels = GetDeviceCaps(hdcRef, HORZRES);
	int iHeightPels = GetDeviceCaps(hdcRef, VERTRES);
	ReleaseDC(NULL, hdcRef);

	RECT Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = (Width * iWidthMM * 100) / iWidthPels;
	Rect.bottom = (Height * iHeightMM * 100) / iHeightPels;

	HDC Handle = CreateEnhMetaFileW(NULL, NULL, &Rect, L"PointerLibrary++GDI\0Enhanced Metafile\0");
	_DC->Initialize(Handle);
}

WinMetaFileBuilder::WinMetaFileBuilder(int Width, int Height)
	: _Width(Width)
	, _Height(Height)
	, _DC(new WinProxyDC())
{
	Create(Width, Height);
}

WinMetaFileBuilder::~WinMetaFileBuilder()
{
	Destroy();
	delete _DC;
}

void WinMetaFileBuilder::LoadFrom(WinMetaFile* File)
{
	Destroy();
	Create(File->GetWidth(), File->GetHeight());
	Draw(File->GetHandle());
}

void WinMetaFileBuilder::SaveTo(WinMetaFile* File)
{
	HENHMETAFILE Handle = CloseEnhMetaFile(_DC->GetHandle());
	if (File->_Handle)
	{
		DeleteEnhMetaFile(File->_Handle);
	}
	File->_Handle = Handle;
	File->_Width = _Width;
	File->_Height = _Height;
	Create(_Width, _Height);
	Draw(Handle);
}

void WinMetaFileBuilder::LoadFrom(const std::wstring& FileName)
{
	WinMetaFile File(FileName);
	Destroy();
	Create(File.GetWidth(), File.GetHeight());
	Draw(File.GetHandle());
}

void WinMetaFileBuilder::SaveTo(const std::wstring& FileName)
{
	HENHMETAFILE Handle = CloseEnhMetaFile(_DC->GetHandle());
	HENHMETAFILE NewHandle = CopyEnhMetaFileW(Handle, FileName.c_str());
	DeleteEnhMetaFile(NewHandle);
	Create(_Width, _Height);
	Draw(Handle);
	DeleteEnhMetaFile(Handle);
}

WinMetaFile::WinMetaFile(const std::wstring& FileName)
	: _Handle(GetEnhMetaFileW(FileName.c_str()))
	, _Width()
	, _Height()
{
	ENHMETAHEADER Header;
	GetEnhMetaFileHeader(_Handle, sizeof(Header), &Header);
	_Width = (Header.rclFrame.right - Header.rclFrame.left)*Header.szlDevice.cx / (Header.szlMillimeters.cx * 100);
	_Height = (Header.rclFrame.bottom - Header.rclFrame.top)*Header.szlDevice.cy / (Header.szlMillimeters.cy * 100);
}

WinMetaFile::WinMetaFile(WinMetaFileBuilder* Builder)
	: _Handle(NULL)
	, _Width()
	, _Height()
{
	Builder->SaveTo(this);
}

WinMetaFile::~WinMetaFile()
{
	DeleteEnhMetaFile(_Handle);
}

int WinBitmap::GetLineBytes(int Width, BitmapBits BB)
{
	int Bits = GetBitsFromBB(BB);
	int LineBits = Width*Bits;
	int AlignBits = sizeof(WORD) * 8;
	LineBits += (AlignBits - LineBits%AlignBits) % AlignBits;
	return LineBits / 8;
}

void WinBitmap::FillBitmapInfoHeader(int Width, int Height, BitmapBits Bits, BITMAPINFOHEADER* Header)
{
	Header->biSize = sizeof(*Header);
	Header->biWidth = Width;
	Header->biHeight = Height;
	Header->biPlanes = 1;
	Header->biBitCount = GetBitsFromBB(Bits);
	Header->biCompression = BI_RGB;
	Header->biSizeImage = 0;
	Header->biXPelsPerMeter = 0;
	Header->biYPelsPerMeter = 0;
	Header->biClrUsed = (Bits == BitmapBits::Bit2 ? 2 : 0);
	Header->biClrImportant = 0;
}

HBITMAP WinBitmap::CreateDDB(int Width, int Height, BitmapBits Bits)
{
	if (Bits == BitmapBits::Bit2)
	{
		return CreateBitmap(Width, Height, 2, GetBitsFromBB(Bits), NULL);
	}
	else
	{
		WinBitmap Bitmap(1, 1, Bits, true);
		return CreateCompatibleBitmap(Bitmap.GetWinDC()->GetHandle(), Width, Height);
	}
}

HBITMAP WinBitmap::CreateDIB(int Width, int Height, BitmapBits Bits, BYTE**& ScanLines)
{
	BITMAPINFO* Info = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD));
	FillBitmapInfoHeader(Width, Height, Bits, &Info->bmiHeader);

	Info->bmiColors[0].rgbBlue = 0;
	Info->bmiColors[0].rgbGreen = 0;
	Info->bmiColors[0].rgbRed = 0;
	Info->bmiColors[0].rgbReserved = 0;
	Info->bmiColors[1].rgbBlue = 255;
	Info->bmiColors[1].rgbGreen = 255;
	Info->bmiColors[1].rgbRed = 255;
	Info->bmiColors[1].rgbReserved = 255;

	BYTE* FirstLine = 0;
	HBITMAP Handle = CreateDIBSection(_DC->GetHandle(), Info, DIB_RGB_COLORS, reinterpret_cast<void**>(&FirstLine), NULL, 0);
	ScanLines = new BYTE*[Height];
	int LineBytes = GetLineBytes(Width, Bits);
	for (int i = 0; i < Height; i++)
	{
		ScanLines[i] = FirstLine + LineBytes * i;
	}
	free(Info);
	return Handle;
}

void WinBitmap::Constuctor(int Width, int Height, BitmapBits Bits, bool DIBSection)
{
	_DC = new WinImageDC();
	if (DIBSection)
	{
		_Handle = CreateDIB(Width, Height, Bits, _ScanLines);
	}
	else
	{
		_Handle = CreateDDB(Width, Height, Bits);
		_ScanLines = 0;
	}
	_Width = Width;
	_Height = Height;
	_Bits = Bits;
	_AlphaChannelBuilt = false;
	HGDIOBJ Object = SelectObject(_DC->GetHandle(), _Handle);
	if (Object)
	{
		DeleteObject(Object);
	}
}

WinBitmap::WinBitmap(int Width, int Height, BitmapBits Bits, bool DIBSections)
	: _Bits(Bits)
	, _Width()
	, _Height()
	, _DC(nullptr)
	, _Handle(0)
	, _ScanLines(nullptr)
	, _AlphaChannelBuilt(false)
{
	Constuctor(Width, Height, Bits, DIBSections);
}

WinBitmap::WinBitmap(const std::wstring& FileName, bool Use32Bits, bool DIBSections)
	: _Bits(Use32Bits ? BitmapBits::Bit32 : BitmapBits::Bit24)
	, _Width()
	, _Height()
	, _DC(nullptr)
	, _Handle(0)
	, _ScanLines(nullptr)
	, _AlphaChannelBuilt(false)
{
	HBITMAP TempBmp = static_cast<HBITMAP>(LoadImageW(NULL, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	HDC TempDC = CreateCompatibleDC(NULL);
	BITMAP TempRec;

	GetObjectW(TempBmp, sizeof(TempRec), &TempRec);
	DeleteObject(SelectObject(TempDC, TempBmp));

	Constuctor(TempRec.bmWidth, TempRec.bmHeight, _Bits, DIBSections);
	BitBlt(_DC->GetHandle(), 0, 0, TempRec.bmWidth, TempRec.bmHeight, TempDC, 0, 0, SRCCOPY);

	DeleteObject(TempDC);
	DeleteObject(TempBmp);
}

WinBitmap::~WinBitmap()
{
	if (_ScanLines)
	{
		delete[] _ScanLines;
	}
	if (_Handle)
	{
		DeleteObject(_Handle);
	}
	delete _DC;
}

void WinBitmap::SaveToFile(const std::wstring& FileName)
{
	if (_ScanLines)
	{
		BITMAPFILEHEADER Header1;
		BITMAPV5HEADER Header2;
		Header1.bfType = 'M' * 256 + 'B';
		Header1.bfSize = sizeof(Header1) + sizeof(Header2) + GetLineBytes()*_Height;
		Header1.bfReserved1 = 0;
		Header1.bfReserved2 = 0;
		Header1.bfOffBits = sizeof(Header2) + sizeof(Header1);

		memset(&Header2, 0, sizeof(Header2));
		Header2.bV5Size = sizeof(Header2);
		Header2.bV5Width = _Width;
		Header2.bV5Height = -_Height; //top-bottom
		Header2.bV5Planes = 1;
		Header2.bV5BitCount = GetBitsFromBB(_Bits);
		Header2.bV5Compression = BI_RGB;
		Header2.bV5CSType = LCS_sRGB;
		Header2.bV5Intent = LCS_GM_GRAPHICS;

		std::ofstream Output(FileName, std::ofstream::binary);
		Output.write(reinterpret_cast<char*>(&Header1), sizeof(Header1));
		Output.write(reinterpret_cast<char*>(&Header2), sizeof(Header2));
		for (int i = 0; i < _Height; i++)
		{
			Output.write(reinterpret_cast<char*>(_ScanLines[i]), GetLineBytes());
		}
	}
	else
	{
		WinBitmap Temp(_Width, _Height, _Bits, true);
		Temp.GetWinDC()->Copy(0, 0, _Width, _Height, _DC, 0, 0);
		Temp.SaveToFile(FileName);
	}
}

void WinBitmap::BuildAlphaChannel()
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	_AlphaChannelBuilt = true;
	for (int i = 0; i < _Height; i++)
	{
		BYTE* Colors = _ScanLines[i];
		int j = _Width;
		while (j--)
		{
			BYTE Alpha = Colors[3];
			Colors[0] = Colors[0] * Alpha / 255;
			Colors[1] = Colors[1] * Alpha / 255;
			Colors[2] = Colors[2] * Alpha / 255;
			Colors += 4;
		}
	}

}

void WinBitmap::GenerateTrans(COLORREF Color)
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < _Height; i++)
	{
		for (int i = 0; i < _Height; i++)
		{
			COLORREF* Colors = reinterpret_cast<COLORREF*>(_ScanLines[i]);
			int j = _Width;
			while (j--)
			{
				COLORREF Dest = *Colors & 0x00FFFFFF;
				*Colors = Dest | (0xFF000000 * (Dest != Color));
				Colors++;
			}
		}
	}

}

void WinBitmap::GenerateAlpha(BYTE Alpha)
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < _Height; i++)
	{
		BYTE* Colors = _ScanLines[i];
		int j = _Width;
		while (j--)
		{
			Colors[3] = Alpha;
			Colors += 4;
		}
	}

}

void WinBitmap::GenerateTransAlpha(COLORREF Color, BYTE Alpha)
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	COLORREF A = Alpha << 24;
	for (int i = 0; i < _Height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(_ScanLines[i]);
		int j = _Width;
		while (j--)
		{
			COLORREF Dest = *Colors & 0x00FFFFFF;
			*Colors = Dest | (A * (Dest != Color));
			Colors++;
		}
	}
}


void WinBitmap::GenerateLuminance()
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < _Height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(_ScanLines[i]);
		int j = _Width;
		while (j--)
		{
			COLORREF Dest = *Colors & 0x00FFFFFF;
			*Colors = Dest | ((GetRValue(Dest) * 77 + GetGValue(Dest) * 151 + GetBValue(Dest) * 28) & 0x0000FF00) << 16;
			Colors++;
		}
	}
}

void WinBitmap::GenerateGrayLevel()
{
	auto Function = [](COLORREF Dest) {
		return (GetRValue(Dest) + GetGValue(Dest) + GetBValue(Dest)) / 3;
	};
	Generate(Function);
}

void WinBitmap::Generate(std::function<BYTE(COLORREF)> Function)
{
	if (!(CanBuildAlphaChannel() && !_AlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < _Height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(_ScanLines[i]);
		int j = _Width;
		while (j--)
		{
			COLORREF Dest = *Colors & 0x00FFFFFF;
			*Colors = Dest | Function(Dest) << 24;
			Colors++;
		}
	}
}

/*WinBrush*/

WinBrush::WinBrush(COLORREF Color)
	: _DIBMemory(nullptr)
	, _Handle(CreateSolidBrush(Color))
{
}

WinBrush::WinBrush(int Hatch, COLORREF Color)
	: _DIBMemory(nullptr)
	, _Handle(CreateHatchBrush(Hatch, Color))
{
}

WinBrush::WinBrush(WinBitmap::Pointer DIB)
{
	WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::BitmapBits::Bit24, true);
	Temp.GetWinDC()->Draw(0, 0, DIB);
	int HeaderSize = sizeof(BITMAPINFOHEADER);
	_DIBMemory = new BYTE[HeaderSize + DIB->GetHeight() * DIB->GetLineBytes()];
	Temp.FillCompatibleHeader(reinterpret_cast<BITMAPINFOHEADER*>(_DIBMemory));
	memcpy(_DIBMemory + HeaderSize, DIB->GetScanLines()[0], DIB->GetHeight()*DIB->GetLineBytes());

	_Handle = CreateDIBPatternBrushPt(_DIBMemory, DIB_RGB_COLORS);
}

WinBrush::~WinBrush()
{
	DeleteObject(_Handle);
	if (_DIBMemory)
	{
		delete[] _DIBMemory;
	}
}

WinPen::WinPen(int Style, int Width, COLORREF Color)
	: _Handle(CreatePen(Style, Width, Color))
	, _DIBMemory(nullptr)
{
}

WinPen::WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color)
	: _Handle(0)
	, _DIBMemory(nullptr)
{
	LOGBRUSH Brush;
	Brush.lbColor = Color;
	Brush.lbStyle = BS_SOLID;
	Brush.lbHatch = 0;
	_Handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::WinPen(int Style, int EndCap, int Join, int Hatch, int Width, COLORREF Color)
	: _Handle(0)
	, _DIBMemory(nullptr)
{
	_DIBMemory = 0;
	LOGBRUSH Brush;
	Brush.lbColor = Color;
	Brush.lbStyle = BS_HATCHED;
	Brush.lbHatch = Hatch;
	_Handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::WinPen(WinBitmap::Pointer DIB, int Style, int EndCap, int Join, int Width)
	: _Handle(0)
	, _DIBMemory(nullptr)
{
	WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::BitmapBits::Bit24, true);
	Temp.GetWinDC()->Draw(0, 0, DIB);
	int HeaderSize = sizeof(BITMAPINFOHEADER);
	_DIBMemory = new BYTE[HeaderSize + DIB->GetHeight() * DIB->GetLineBytes()];
	Temp.FillCompatibleHeader(reinterpret_cast<BITMAPINFOHEADER*>(_DIBMemory));
	memcpy(_DIBMemory + HeaderSize, DIB->GetScanLines()[0], DIB->GetHeight() * DIB->GetLineBytes());

	LOGBRUSH Brush;
	Brush.lbColor = RGB(0, 0, 0);
	Brush.lbStyle = BS_DIBPATTERNPT;
	Brush.lbHatch = reinterpret_cast<LONG>(_DIBMemory);
	_Handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::~WinPen()
{
	DeleteObject(_Handle);
	if (_DIBMemory)
	{
		delete[] _DIBMemory;
	}
}

WinFont::WinFont(const std::wstring& Name, int Height, int Width, int Escapement, int Orientation, int Weight,
				 bool Italic, bool Underline, bool StrikeOut, bool Antianalise)
	: _FontInfo()
	, _Handle(0)
{
	_FontInfo.lfHeight = Height;
	_FontInfo.lfWidth = Width;
	_FontInfo.lfEscapement = Escapement;
	_FontInfo.lfOrientation = Orientation;
	_FontInfo.lfWeight = Weight;
	_FontInfo.lfItalic = Italic ? TRUE : FALSE;
	_FontInfo.lfUnderline = Underline ? TRUE : FALSE;
	_FontInfo.lfStrikeOut = StrikeOut ? TRUE : FALSE;
	_FontInfo.lfCharSet = DEFAULT_CHARSET;
	_FontInfo.lfOutPrecision = OUT_DEFAULT_PRECIS;
	_FontInfo.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	_FontInfo.lfQuality = Antianalise ? CLEARTYPE_QUALITY : NONANTIALIASED_QUALITY;
	_FontInfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcsncpy_s(_FontInfo.lfFaceName, Name.c_str(), LF_FACESIZE - 1);

	_Handle = CreateFontIndirectW(&_FontInfo);
}

std::shared_ptr<WinFont> WinFont::FromWindow(HWND Handle, const wchar_t* Name, int Point)
{
    HDC WindowDC = GetDC(Handle);
    int Height = MulDiv(Point, GetDeviceCaps(WindowDC, LOGPIXELSY), 72);
    ReleaseDC(Handle, WindowDC);
    return std::make_shared<WinFont>(Name, -Height, 0, 0, 0, FW_NORMAL, false, false, false, true);
}

WinFont::WinFont(LOGFONT* FontInfo)
	: _FontInfo(*FontInfo)
	, _Handle(CreateFontIndirect(&_FontInfo))
{
}

WinFont::~WinFont()
{
	DeleteObject(_Handle);
}

/*WinDC*/

void WinDC::Initialize()
{
	_Pen = GetApplication()->GetDefaultPen();
	_OldPen = reinterpret_cast<HPEN>(SelectObject(_Handle, _Pen->GetHandle()));

	_Brush = GetApplication()->GetDefaultBrush();
	_OldBrush = reinterpret_cast<HBRUSH>(SelectObject(_Handle, _Brush->GetHandle()));

	_Font = GetApplication()->GetDefaultFont();
	_OldFont = reinterpret_cast<HFONT>(SelectObject(_Handle, _Font->GetHandle()));

	SetGraphicsMode(_Handle, GM_ADVANCED);
}

WinDC::WinDC()
	: _Handle(0)
	, _OldPen(nullptr)
	, _OldBrush(nullptr)
	, _OldFont(nullptr)
{
}

WinDC::~WinDC()
{
	SelectObject(_Handle, _OldFont);
	SelectObject(_Handle, _OldBrush);
	SelectObject(_Handle, _OldPen);
}

void WinDC::SetPen(WinPen::Pointer Pen)
{
	SelectObject(_Handle, Pen->GetHandle());
	_Pen = Pen;
}

void WinDC::SetBrush(WinBrush::Pointer Brush)
{
	SelectObject(_Handle, Brush->GetHandle());
	_Brush = Brush;
}

void WinDC::SetFont(WinFont::Pointer Font)
{
	SelectObject(_Handle, Font->GetHandle());
	_Font = Font;
}

COLORREF WinDC::GetBackgroundColor()
{
	return GetBkColor(_Handle);
}

void WinDC::SetBackgroundColor(COLORREF Color)
{
	SetBkColor(_Handle, Color);
}

COLORREF WinDC::GetTextColor()
{
	return ::GetTextColor(_Handle);
}

void WinDC::SetTextColor(COLORREF Color)
{
	::SetTextColor(_Handle, Color);
}

bool WinDC::GetBackgroundTransparent()
{
	return GetBkMode(_Handle) == TRANSPARENT;
}

void WinDC::SetBackgroundTransparent(bool Value)
{
	SetBkMode(_Handle, Value ? TRANSPARENT : OPAQUE);
}

POINT WinDC::GetBrushOrigin()
{
	POINT Point;
	GetBrushOrgEx(_Handle, &Point);
	return Point;
}

void WinDC::SetBrushOrigin(POINT Value)
{
	SetBrushOrgEx(_Handle, Value.x, Value.y, NULL);
}

void WinDC::DrawString(int X, int Y, const std::wstring& Text)
{
	TextOutW(_Handle, X, Y, Text.c_str(), Text.length());
}

void WinDC::DrawString(int X, int Y, const std::wstring& Text, int TabWidth, int TabOriginX)
{
	TabbedTextOutW(_Handle, X, Y, Text.c_str(), Text.length(), 1, &TabWidth, TabOriginX);
}

void WinDC::FillRegion(WinRegion::Pointer Region)
{
	FillRgn(_Handle, Region->GetHandle(), _Brush->GetHandle());
}

void WinDC::FrameRegion(WinRegion::Pointer Region, int BlockWidth, int BlockHeight)
{
	FrameRgn(_Handle, Region->GetHandle(), _Brush->GetHandle(), BlockWidth, BlockHeight);
}

void WinDC::MoveTo(int X, int Y)
{
	::MoveToEx(_Handle, X, Y, NULL);
}

void WinDC::LineTo(int X, int Y)
{
	::LineTo(_Handle, X, Y);
}

void WinDC::Rectangle(int Left, int Top, int  Right, int Bottom)
{
	::Rectangle(_Handle, Left, Top, Right, Bottom);
}

void WinDC::Rectangle(RECT Rect)
{
	::Rectangle(_Handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::FillRect(int Left, int Top, int Right, int Bottom)
{
	RECT Rect{Left, Top, Right, Bottom};
	::FillRect(_Handle, &Rect, _Brush->GetHandle());
}

void WinDC::FillRect(RECT Rect)
{
	::FillRect(_Handle, &Rect, _Brush->GetHandle());
}

void WinDC::Ellipse(int Left, int Top, int Right, int Bottom)
{
	::Ellipse(_Handle, Left, Top, Right, Bottom);
}

void WinDC::Ellipse(RECT Rect)
{
	::Ellipse(_Handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::RoundRect(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
{
	::RoundRect(_Handle, Left, Top, Right, Bottom, EllipseWidth, EllipseHeight);
}

void WinDC::RoundRect(RECT Rect, int EllipseWidth, int EllipseHeight)
{
	::RoundRect(_Handle, Rect.left, Rect.top, Rect.right, Rect.bottom, EllipseWidth, EllipseHeight);
}

void WinDC::PolyLine(const POINT Points[], int Count)
{
	::Polyline(_Handle, Points, Count);
}

void WinDC::PolyLineTo(const POINT Points[], int Count)
{
	::PolylineTo(_Handle, Points, Count);
}

void WinDC::Polygon(const POINT Points[], int Count)
{
	::Polygon(_Handle, Points, Count);
}

void WinDC::PolyBezier(const POINT Points[], int Count)
{
	::PolyBezier(_Handle, Points, Count);
}

void WinDC::PolyBezierTo(const POINT Points[], int Count)
{
	::PolyBezierTo(_Handle, Points, Count);
}

void WinDC::PolyDraw(const POINT Points[], const BYTE Actions[], int PointCount)
{
	::PolyDraw(_Handle, Points, Actions, PointCount);
}

void WinDC::Arc(RECT Bound, POINT Start, POINT End)
{
	::Arc(_Handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Arc(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Arc(_Handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::ArcTo(RECT Bound, POINT Start, POINT End)
{
	::ArcTo(_Handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::ArcTo(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::ArcTo(_Handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::AngleArc(int X, int Y, int Radius, float StartAngle, float SweepAngle)
{
	::AngleArc(_Handle, X, Y, Radius, StartAngle, SweepAngle);
}

void WinDC::AngleArc(int X, int Y, int Radius, double StartAngle, double SweepAngle)
{
	::AngleArc(_Handle, X, Y, Radius, static_cast<float>(StartAngle), static_cast<float>(SweepAngle));
}

void WinDC::Chord(RECT Bound, POINT Start, POINT End)
{
	::Chord(_Handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Chord(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Chord(_Handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::Pie(RECT Bound, POINT Start, POINT End)
{
	::Pie(_Handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Pie(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Pie(_Handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::GradientTriangle(TRIVERTEX Vertices[], int VerticesCount, GRADIENT_TRIANGLE Triangles[], int TriangleCount)
{
	GradientFill(_Handle, Vertices, VerticesCount, Triangles, TriangleCount, GRADIENT_FILL_TRIANGLE);
}

void WinDC::BeginPath()
{
	::BeginPath(_Handle);
}

void WinDC::EndPath()
{
	::EndPath(_Handle);
}

void WinDC::ClosePath()
{
	::CloseFigure(_Handle);
}

void WinDC::DiscardPath()
{
	::AbortPath(_Handle);
}

void WinDC::DrawPath()
{
	::StrokePath(_Handle);
}

void WinDC::FillPath()
{
	::FillPath(_Handle);
}

void WinDC::DrawAndFillPath()
{
	::StrokeAndFillPath(_Handle);
}

WinRegion::Pointer WinDC::RegionFromPath()
{
	return std::make_shared<WinRegion>(::PathToRegion(_Handle));
}

bool WinDC::PointInClip(POINT Point)
{
	return PtVisible(_Handle, Point.x, Point.y) == TRUE;
}

bool WinDC::RectangleInClip(RECT Rect)
{
	return RectVisible(_Handle, &Rect) == TRUE;
}

void WinDC::ClipPath(int CombineMode)
{
	SelectClipPath(_Handle, CombineMode);
}

void WinDC::ClipRegion(WinRegion::Pointer Region)
{
	SelectClipRgn(_Handle, Region->GetHandle());
}

void WinDC::RemoveClip()
{
	SelectClipRgn(_Handle, NULL);
}

void WinDC::MoveClip(int OffsetX, int OffsetY)
{
	OffsetClipRgn(_Handle, OffsetX, OffsetY);
}

void WinDC::CombineClip(WinRegion::Pointer Region, int CombineMode)
{
	ExtSelectClipRgn(_Handle, Region->GetHandle(), CombineMode);
}

void WinDC::IntersetClipRect(RECT Rect)
{
	::IntersectClipRect(_Handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::ExcludeClipRect(RECT Rect)
{
	::ExcludeClipRect(_Handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

WinRegion::Pointer WinDC::GetClipRegion()
{
	HRGN Handle = CreateRectRgn(0, 0, 1, 1);
	GetClipRgn(_Handle, Handle);
	return std::make_shared<WinRegion>(Handle);
}

RECT WinDC::GetClipBoundRect()
{
	RECT Rect;
	GetClipBox(_Handle, &Rect);
	return Rect;
}

WinTransform WinDC::GetTransform()
{
	XFORM Transform;
	GetWorldTransform(_Handle, &Transform);
	return Transform;
}

void WinDC::SetTransform(const WinTransform& Transform)
{
	SetWorldTransform(_Handle, Transform.GetHandle());
}

void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	BitBlt(_Handle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, DrawROP);
}

void WinDC::Copy(RECT dstRect, WinDC* Source, POINT srcPos, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	BitBlt(_Handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top, SourceHandle, srcPos.x, srcPos.y, DrawROP);
}

void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	StretchBlt(_Handle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, srcW, srcH, DrawROP);
}

void WinDC::Copy(RECT dstRect, WinDC* Source, RECT srcRect, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	StretchBlt(_Handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top,
			   SourceHandle, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top,
			   DrawROP);
}

void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC* Source, int srcX, int srcY, int srcW, int srcH)
{
	POINT Pt[3];
	Pt[0] = UpperLeft;
	Pt[1] = UpperRight;
	Pt[2] = LowerLeft;
	PlgBlt(_Handle, Pt, Source->GetHandle(), srcX, srcY, srcW, srcH, 0, 0, 0);
}

void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC*Source, RECT srcRect)
{
	POINT Pt[3];
	Pt[0] = UpperLeft;
	Pt[1] = UpperRight;
	Pt[2] = LowerLeft;
	PlgBlt(_Handle, Pt, Source->GetHandle(), srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, 0, 0, 0);
}

void WinDC::CopyTrans(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, COLORREF Color)
{
	TransparentBlt(_Handle, dstX, dstY, dstW, dstH, Source->GetHandle(), srcX, srcY, srcW, srcH, Color);
}

void WinDC::CopyTrans(RECT dstRect, WinDC* Source, RECT srcRect, COLORREF Color)
{
	TransparentBlt(_Handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top,
				   Source->GetHandle(), srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top,
				   Color);
}

void WinDC::Draw(int dstX, int dstY, WinMetaFile* MetaFile)
{
	Draw(dstX, dstY, MetaFile->GetWidth(), MetaFile->GetHeight(), MetaFile);
}

void WinDC::Draw(POINT Pos, WinMetaFile* MetaFile)
{
	Draw(Pos.x, Pos.y, MetaFile->GetWidth(), MetaFile->GetHeight(), MetaFile);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinMetaFile* MetaFile)
{
	RECT Rect{dstX,dstY, dstX + dstW, dstY + dstH};
	Draw(Rect, MetaFile);
}

void WinDC::Draw(RECT Rect, WinMetaFile* MetaFile)
{
	PlayEnhMetaFile(_Handle, MetaFile->GetHandle(), &Rect);
}

void WinDC::Draw(int dstX, int dstY, WinBitmap::Pointer Bitmap)
{
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(POINT Pos, WinBitmap::Pointer Bitmap)
{
	int dstX = Pos.x;
	int dstY = Pos.y;
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap)
{
	int srcX = 0;
	int srcY = 0;
	int srcW = Bitmap->GetWidth();
	int srcH = Bitmap->GetHeight();
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT Rect, WinBitmap::Pointer Bitmap)
{
	int dstX = Rect.left;
	int dstY = Rect.top;
	int dstW = Rect.right - Rect.left;
	int dstH = Rect.bottom - Rect.top;
	int srcX = 0;
	int srcY = 0;
	int srcW = Bitmap->GetWidth();
	int srcH = Bitmap->GetWidth();
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY)
{
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT Rect, WinBitmap::Pointer Bitmap, POINT Pos)
{
	int dstX = Rect.left;
	int dstY = Rect.top;
	int dstW = Rect.right - Rect.left;
	int dstH = Rect.bottom - Rect.top;
	int srcX = Pos.x;
	int srcY = Pos.y;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, int srcW, int srcH)
{
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend;
		Blend.BlendOp = AC_SRC_OVER;
		Blend.BlendFlags = 0;
		Blend.SourceConstantAlpha = 255;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT dstRect, WinBitmap::Pointer Bitmap, RECT srcRect)
{
	int dstX = dstRect.left;
	int dstY = dstRect.top;
	int dstW = dstRect.right - dstRect.left;
	int dstH = dstRect.bottom - dstRect.top;
	int srcX = srcRect.left;
	int srcY = srcRect.top;
	int srcW = srcRect.right - srcRect.left;
	int srcH = srcRect.bottom - srcRect.top;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend;
		Blend.BlendOp = AC_SRC_OVER;
		Blend.BlendFlags = 0;
		Blend.SourceConstantAlpha = 255;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

/*------------------------------------------------------------------------------*/

void WinDC::Draw(int dstX, int dstY, WinBitmap::Pointer Bitmap, byte Alpha)
{
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	int srcW = dstW;
	int srcH = dstH;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(POINT Pos, WinBitmap::Pointer Bitmap, byte Alpha)
{
	int dstX = Pos.x;
	int dstY = Pos.y;
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	int srcW = dstW;
	int srcH = dstH;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, byte Alpha)
{
	int srcX = 0;
	int srcY = 0;
	int srcW = Bitmap->GetWidth();
	int srcH = Bitmap->GetHeight();

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT Rect, WinBitmap::Pointer Bitmap, byte Alpha)
{
	int dstX = Rect.left;
	int dstY = Rect.top;
	int dstW = Rect.right - Rect.left;
	int dstH = Rect.bottom - Rect.top;
	int srcX = 0;
	int srcY = 0;
	int srcW = Bitmap->GetWidth();
	int srcH = Bitmap->GetHeight();

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, byte Alpha)
{
	int srcW = dstW;
	int srcH = dstH;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT Rect, WinBitmap::Pointer Bitmap, POINT Pos, byte Alpha)
{
	int dstX = Rect.left;
	int dstY = Rect.top;
	int dstW = Rect.right - Rect.left;
	int dstH = Rect.bottom - Rect.top;
	int srcX = Pos.x;
	int srcY = Pos.y;
	int srcW = dstW;
	int srcH = dstH;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, int srcW, int srcH, byte Alpha)
{
	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT dstRect, WinBitmap::Pointer Bitmap, RECT srcRect, byte Alpha)
{
	int dstX = dstRect.left;
	int dstY = dstRect.top;
	int dstW = dstRect.right - dstRect.left;
	int dstH = dstRect.bottom - dstRect.top;
	int srcX = srcRect.left;
	int srcY = srcRect.top;
	int srcW = srcRect.right - srcRect.left;
	int srcH = srcRect.bottom - srcRect.top;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(_Handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

WinControlDC::WinControlDC(HWND Handle)
	: WinDC()
	, _ControlHandle(Handle)
{
	_Handle = GetDC(_ControlHandle);
	Initialize();
}

WinControlDC::~WinControlDC()
{
	ReleaseDC(_ControlHandle, _Handle);
}

WinProxyDC::WinProxyDC()
	: WinDC()
{
}

WinProxyDC::~WinProxyDC()
{
}

void WinProxyDC::Initialize(HDC Handle)
{
	_Handle = Handle;
	WinDC::Initialize();
}

WinImageDC::WinImageDC()
	: WinDC()
{
	_Handle = CreateCompatibleDC(NULL);
	Initialize();
}

WinImageDC::~WinImageDC()
{
	DeleteDC(_Handle);
}

}
}
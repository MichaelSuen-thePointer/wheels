#include "WinMain.h"
#include <fstream>
#pragma comment(lib, "Msimg32.lib")

namespace pl
{
namespace windows
{

WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, bool IsRectangle)
	: handle(0)
{
	if (IsRectangle)
	{
		handle = CreateRectRgn(Left, Top, Right, Bottom);
	}
	else
	{
		handle = CreateEllipticRgn(Left, Top, Right, Bottom);
	}
}

WinRegion::WinRegion(RECT Rect, bool IsRectangle)
	: handle(0)
{
	if (IsRectangle)
	{
		handle = CreateRectRgnIndirect(&Rect);
	}
	else
	{
		handle = CreateEllipticRgnIndirect(&Rect);
	}
}

WinRegion::WinRegion(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
	: handle(CreateRoundRectRgn(Left, Top, Right, Bottom, EllipseWidth, EllipseHeight))
{
}

WinRegion::WinRegion(POINT Points[], int Count, bool Alternate)
	: handle(CreatePolygonRgn(Points, Count, Alternate ? ALTERNATE : WINDING))
{
}

WinRegion::WinRegion(WinRegion::Ptr Region)
	: handle(CreateRectRgn(0, 0, 1, 1))
{
	CombineRgn(handle, Region->GetHandle(), Region->GetHandle(), RGN_COPY);
}

WinRegion::WinRegion(WinRegion::Ptr Region1, WinRegion::Ptr Region2, int CombineMode)
	: handle(CreateRectRgn(0, 0, 1, 1))
{
	CombineRgn(handle, Region1->GetHandle(), Region2->GetHandle(), CombineMode);
}

WinRegion::WinRegion(HRGN RegionHandle)
	: handle(RegionHandle)
{
}

WinRegion::~WinRegion()
{
	DeleteObject(handle);
}

WinTransform::WinTransform(XFORM Transform)
	: transform(Transform)
{
}

WinTransform::WinTransform(const WinTransform& Transform)
	: transform(Transform.transform)
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
	proxyDC->Initialize(Handle);
}

WinMetaFileBuilder::WinMetaFileBuilder(int Width, int Height)
	: width(Width)
	, height(Height)
	, proxyDC(new WinProxyDC())
{
	Create(Width, Height);
}

WinMetaFileBuilder::~WinMetaFileBuilder()
{
	Destroy();
	delete proxyDC;
}

void WinMetaFileBuilder::LoadFrom(WinMetaFile* File)
{
	Destroy();
	Create(File->GetWidth(), File->GetHeight());
	Draw(File->GetHandle());
}

void WinMetaFileBuilder::SaveTo(WinMetaFile* File)
{
	HENHMETAFILE Handle = CloseEnhMetaFile(proxyDC->GetHandle());
	if (File->handle)
	{
		DeleteEnhMetaFile(File->handle);
	}
	File->handle = Handle;
	File->width = width;
	File->height = height;
	Create(width, height);
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
	HENHMETAFILE Handle = CloseEnhMetaFile(proxyDC->GetHandle());
	HENHMETAFILE NewHandle = CopyEnhMetaFileW(Handle, FileName.c_str());
	DeleteEnhMetaFile(NewHandle);
	Create(width, height);
	Draw(Handle);
	DeleteEnhMetaFile(Handle);
}

WinMetaFile::WinMetaFile(const std::wstring& FileName)
	: handle(GetEnhMetaFileW(FileName.c_str()))
	, width()
	, height()
{
	ENHMETAHEADER Header;
	GetEnhMetaFileHeader(handle, sizeof(Header), &Header);
	width = (Header.rclFrame.right - Header.rclFrame.left)*Header.szlDevice.cx / (Header.szlMillimeters.cx * 100);
	height = (Header.rclFrame.bottom - Header.rclFrame.top)*Header.szlDevice.cy / (Header.szlMillimeters.cy * 100);
}

WinMetaFile::WinMetaFile(WinMetaFileBuilder* Builder)
	: handle(NULL)
	, width()
	, height()
{
	Builder->SaveTo(this);
}

WinMetaFile::~WinMetaFile()
{
	DeleteEnhMetaFile(handle);
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
	HBITMAP Handle = CreateDIBSection(imageDC->GetHandle(), Info, DIB_RGB_COLORS, reinterpret_cast<void**>(&FirstLine), NULL, 0);
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
	imageDC = new WinImageDC();
	if (DIBSection)
	{
		handle = CreateDIB(Width, Height, Bits, scanLines);
	}
	else
	{
		handle = CreateDDB(Width, Height, Bits);
		scanLines = 0;
	}
	width = Width;
	height = Height;
	bits = Bits;
	hasAlphaChannelBuilt = false;
	HGDIOBJ Object = SelectObject(imageDC->GetHandle(), handle);
	if (Object)
	{
		DeleteObject(Object);
	}
}

WinBitmap::WinBitmap(int Width, int Height, BitmapBits Bits, bool DIBSections)
	: bits(Bits)
	, width()
	, height()
	, imageDC(nullptr)
	, handle(0)
	, scanLines(nullptr)
	, hasAlphaChannelBuilt(false)
{
	Constuctor(Width, Height, Bits, DIBSections);
}

WinBitmap::WinBitmap(const std::wstring& FileName, bool Use32Bits, bool DIBSections)
	: bits(Use32Bits ? BitmapBits::Bit32 : BitmapBits::Bit24)
	, width()
	, height()
	, imageDC(nullptr)
	, handle(0)
	, scanLines(nullptr)
	, hasAlphaChannelBuilt(false)
{
	HBITMAP TempBmp = static_cast<HBITMAP>(LoadImageW(NULL, FileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	HDC TempDC = CreateCompatibleDC(NULL);
	BITMAP TempRec;

	GetObjectW(TempBmp, sizeof(TempRec), &TempRec);
	DeleteObject(SelectObject(TempDC, TempBmp));

	Constuctor(TempRec.bmWidth, TempRec.bmHeight, bits, DIBSections);
	BitBlt(imageDC->GetHandle(), 0, 0, TempRec.bmWidth, TempRec.bmHeight, TempDC, 0, 0, SRCCOPY);

	DeleteObject(TempDC);
	DeleteObject(TempBmp);
}

WinBitmap::~WinBitmap()
{
	if (scanLines)
	{
		delete[] scanLines;
	}
	if (handle)
	{
		DeleteObject(handle);
	}
	delete imageDC;
}

void WinBitmap::SaveToFile(const std::wstring& FileName)
{
	if (scanLines)
	{
		BITMAPFILEHEADER Header1;
		BITMAPV5HEADER Header2;
		Header1.bfType = 'M' * 256 + 'B';
		Header1.bfSize = sizeof(Header1) + sizeof(Header2) + GetLineBytes()*height;
		Header1.bfReserved1 = 0;
		Header1.bfReserved2 = 0;
		Header1.bfOffBits = sizeof(Header2) + sizeof(Header1);

		memset(&Header2, 0, sizeof(Header2));
		Header2.bV5Size = sizeof(Header2);
		Header2.bV5Width = width;
		Header2.bV5Height = -height; //top-bottom
		Header2.bV5Planes = 1;
		Header2.bV5BitCount = GetBitsFromBB(bits);
		Header2.bV5Compression = BI_RGB;
		Header2.bV5CSType = LCS_sRGB;
		Header2.bV5Intent = LCS_GM_GRAPHICS;

		std::ofstream Output(FileName, std::ofstream::binary);
		Output.write(reinterpret_cast<char*>(&Header1), sizeof(Header1));
		Output.write(reinterpret_cast<char*>(&Header2), sizeof(Header2));
		for (int i = 0; i < height; i++)
		{
			Output.write(reinterpret_cast<char*>(scanLines[i]), GetLineBytes());
		}
	}
	else
	{
		WinBitmap Temp(width, height, bits, true);
		Temp.GetWinDC()->Copy(0, 0, width, height, imageDC, 0, 0);
		Temp.SaveToFile(FileName);
	}
}

void WinBitmap::BuildAlphaChannel()
{
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	hasAlphaChannelBuilt = true;
	for (int i = 0; i < height; i++)
	{
		BYTE* Colors = scanLines[i];
		int j = width;
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
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < height; i++)
	{
		for (int i = 0; i < height; i++)
		{
			COLORREF* Colors = reinterpret_cast<COLORREF*>(scanLines[i]);
			int j = width;
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
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < height; i++)
	{
		BYTE* Colors = scanLines[i];
		int j = width;
		while (j--)
		{
			Colors[3] = Alpha;
			Colors += 4;
		}
	}

}

void WinBitmap::GenerateTransAlpha(COLORREF Color, BYTE Alpha)
{
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	COLORREF A = Alpha << 24;
	for (int i = 0; i < height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(scanLines[i]);
		int j = width;
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
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(scanLines[i]);
		int j = width;
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
	if (!(CanBuildAlphaChannel() && !hasAlphaChannelBuilt))
	{
		return;
	}
	for (int i = 0; i < height; i++)
	{
		COLORREF* Colors = reinterpret_cast<COLORREF*>(scanLines[i]);
		int j = width;
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
	: dibMemory(nullptr)
	, handle(CreateSolidBrush(Color))
{
}

WinBrush::WinBrush(int Hatch, COLORREF Color)
	: dibMemory(nullptr)
	, handle(CreateHatchBrush(Hatch, Color))
{
}

WinBrush::WinBrush(WinBitmap::Ptr DIB)
{
	WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::BitmapBits::Bit24, true);
	Temp.GetWinDC()->Draw(0, 0, DIB);
	int HeaderSize = sizeof(BITMAPINFOHEADER);
	dibMemory = new BYTE[HeaderSize + DIB->GetHeight() * DIB->GetLineBytes()];
	Temp.FillCompatibleHeader(reinterpret_cast<BITMAPINFOHEADER*>(dibMemory));
	memcpy(dibMemory + HeaderSize, DIB->GetScanLines()[0], DIB->GetHeight()*DIB->GetLineBytes());

	handle = CreateDIBPatternBrushPt(dibMemory, DIB_RGB_COLORS);
}

WinBrush::~WinBrush()
{
	DeleteObject(handle);
	if (dibMemory)
	{
		delete[] dibMemory;
	}
}

WinPen::WinPen(int Style, int Width, COLORREF Color)
	: handle(CreatePen(Style, Width, Color))
	, dibMemory(nullptr)
{
}

WinPen::WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color)
	: handle(0)
	, dibMemory(nullptr)
{
	LOGBRUSH Brush;
	Brush.lbColor = Color;
	Brush.lbStyle = BS_SOLID;
	Brush.lbHatch = 0;
	handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::WinPen(int Style, int EndCap, int Join, int Hatch, int Width, COLORREF Color)
	: handle(0)
	, dibMemory(nullptr)
{
	dibMemory = 0;
	LOGBRUSH Brush;
	Brush.lbColor = Color;
	Brush.lbStyle = BS_HATCHED;
	Brush.lbHatch = Hatch;
	handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::WinPen(WinBitmap::Ptr DIB, int Style, int EndCap, int Join, int Width)
	: handle(0)
	, dibMemory(nullptr)
{
	WinBitmap Temp(DIB->GetWidth(), DIB->GetHeight(), WinBitmap::BitmapBits::Bit24, true);
	Temp.GetWinDC()->Draw(0, 0, DIB);
	int HeaderSize = sizeof(BITMAPINFOHEADER);
	dibMemory = new BYTE[HeaderSize + DIB->GetHeight() * DIB->GetLineBytes()];
	Temp.FillCompatibleHeader(reinterpret_cast<BITMAPINFOHEADER*>(dibMemory));
	memcpy(dibMemory + HeaderSize, DIB->GetScanLines()[0], DIB->GetHeight() * DIB->GetLineBytes());

	LOGBRUSH Brush;
	Brush.lbColor = RGB(0, 0, 0);
	Brush.lbStyle = BS_DIBPATTERNPT;
	Brush.lbHatch = reinterpret_cast<LONG>(dibMemory);
	handle = ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
}

WinPen::~WinPen()
{
	DeleteObject(handle);
	if (dibMemory)
	{
		delete[] dibMemory;
	}
}

WinFont::WinFont(const std::wstring& Name, int Height, int Width, int Escapement, int Orientation, int Weight,
				 bool Italic, bool Underline, bool StrikeOut, bool Antianalise)
	: fontInfo()
	, handle(0)
{
	fontInfo.lfHeight = Height;
	fontInfo.lfWidth = Width;
	fontInfo.lfEscapement = Escapement;
	fontInfo.lfOrientation = Orientation;
	fontInfo.lfWeight = Weight;
	fontInfo.lfItalic = Italic ? TRUE : FALSE;
	fontInfo.lfUnderline = Underline ? TRUE : FALSE;
	fontInfo.lfStrikeOut = StrikeOut ? TRUE : FALSE;
	fontInfo.lfCharSet = DEFAULT_CHARSET;
	fontInfo.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fontInfo.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	fontInfo.lfQuality = Antianalise ? CLEARTYPE_QUALITY : NONANTIALIASED_QUALITY;
	fontInfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcsncpy_s(fontInfo.lfFaceName, Name.c_str(), LF_FACESIZE - 1);

	handle = CreateFontIndirectW(&fontInfo);
}

WinFont::Ptr WinFont::GetFontForWindow(HWND Handle, const wchar_t* Name, int Point)
{
    HDC WindowDC = GetDC(Handle);
    int Height = MulDiv(Point, GetDeviceCaps(WindowDC, LOGPIXELSY), 72);
    ReleaseDC(Handle, WindowDC);
    return MakeShared<WinFont>(Name, -Height, 0, 0, 0, FW_NORMAL, false, false, false, true);
}

WinFont::WinFont(LOGFONT* FontInfo)
	: fontInfo(*FontInfo)
	, handle(CreateFontIndirect(&fontInfo))
{
}

WinFont::~WinFont()
{
	DeleteObject(handle);
}

/*WinDC*/

void WinDC::Initialize()
{
	pen = GetApplication()->GetDefaultPen();
	oldPen = reinterpret_cast<HPEN>(SelectObject(handle, pen->GetHandle()));

	brush = GetApplication()->GetDefaultBrush();
	oldBrush = reinterpret_cast<HBRUSH>(SelectObject(handle, brush->GetHandle()));

	font = GetApplication()->GetDefaultFont();
	oldFont = reinterpret_cast<HFONT>(SelectObject(handle, font->GetHandle()));

	SetGraphicsMode(handle, GM_ADVANCED);
}

WinDC::WinDC()
	: handle(0)
	, oldPen(nullptr)
	, oldBrush(nullptr)
	, oldFont(nullptr)
{
}

WinDC::~WinDC()
{
	SelectObject(handle, oldFont);
	SelectObject(handle, oldBrush);
	SelectObject(handle, oldPen);
}

void WinDC::SetPen(WinPen::Ptr Pen)
{
	SelectObject(handle, Pen->GetHandle());
	pen = Pen;
}

void WinDC::SetBrush(WinBrush::Ptr Brush)
{
	SelectObject(handle, Brush->GetHandle());
	brush = Brush;
}

void WinDC::SetFont(WinFont::Ptr Font)
{
	SelectObject(handle, Font->GetHandle());
	font = Font;
}

COLORREF WinDC::GetBackgroundColor()
{
	return GetBkColor(handle);
}

void WinDC::SetBackgroundColor(COLORREF Color)
{
	SetBkColor(handle, Color);
}

COLORREF WinDC::GetTextColor()
{
	return ::GetTextColor(handle);
}

void WinDC::SetTextColor(COLORREF Color)
{
	::SetTextColor(handle, Color);
}

bool WinDC::GetBackgroundTransparent()
{
	return GetBkMode(handle) == TRANSPARENT;
}

void WinDC::SetBackgroundTransparent(bool Value)
{
	SetBkMode(handle, Value ? TRANSPARENT : OPAQUE);
}

POINT WinDC::GetBrushOrigin()
{
	POINT Point;
	GetBrushOrgEx(handle, &Point);
	return Point;
}

void WinDC::SetBrushOrigin(POINT Value)
{
	SetBrushOrgEx(handle, Value.x, Value.y, NULL);
}

void WinDC::DrawString(int X, int Y, const std::wstring& Text)
{
	TextOutW(handle, X, Y, Text.c_str(), Text.length());
}

void WinDC::DrawString(int X, int Y, const std::wstring& Text, int TabWidth, int TabOriginX)
{
	TabbedTextOutW(handle, X, Y, Text.c_str(), Text.length(), 1, &TabWidth, TabOriginX);
}

void WinDC::FillRegion(WinRegion::Ptr Region)
{
	FillRgn(handle, Region->GetHandle(), brush->GetHandle());
}

void WinDC::FrameRegion(WinRegion::Ptr Region, int BlockWidth, int BlockHeight)
{
	FrameRgn(handle, Region->GetHandle(), brush->GetHandle(), BlockWidth, BlockHeight);
}

void WinDC::MoveTo(int X, int Y)
{
	::MoveToEx(handle, X, Y, NULL);
}

void WinDC::LineTo(int X, int Y)
{
	::LineTo(handle, X, Y);
}

void WinDC::Rectangle(int Left, int Top, int  Right, int Bottom)
{
	::Rectangle(handle, Left, Top, Right, Bottom);
}

void WinDC::Rectangle(RECT Rect)
{
	::Rectangle(handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::FillRect(int Left, int Top, int Right, int Bottom)
{
	RECT Rect{Left, Top, Right, Bottom};
	::FillRect(handle, &Rect, brush->GetHandle());
}

void WinDC::FillRect(RECT Rect)
{
	::FillRect(handle, &Rect, brush->GetHandle());
}

void WinDC::Ellipse(int Left, int Top, int Right, int Bottom)
{
	::Ellipse(handle, Left, Top, Right, Bottom);
}

void WinDC::Ellipse(RECT Rect)
{
	::Ellipse(handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::RoundRect(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight)
{
	::RoundRect(handle, Left, Top, Right, Bottom, EllipseWidth, EllipseHeight);
}

void WinDC::RoundRect(RECT Rect, int EllipseWidth, int EllipseHeight)
{
	::RoundRect(handle, Rect.left, Rect.top, Rect.right, Rect.bottom, EllipseWidth, EllipseHeight);
}

void WinDC::PolyLine(const POINT Points[], int Count)
{
	::Polyline(handle, Points, Count);
}

void WinDC::PolyLineTo(const POINT Points[], int Count)
{
	::PolylineTo(handle, Points, Count);
}

void WinDC::Polygon(const POINT Points[], int Count)
{
	::Polygon(handle, Points, Count);
}

void WinDC::PolyBezier(const POINT Points[], int Count)
{
	::PolyBezier(handle, Points, Count);
}

void WinDC::PolyBezierTo(const POINT Points[], int Count)
{
	::PolyBezierTo(handle, Points, Count);
}

void WinDC::PolyDraw(const POINT Points[], const BYTE Actions[], int PointCount)
{
	::PolyDraw(handle, Points, Actions, PointCount);
}

void WinDC::Arc(RECT Bound, POINT Start, POINT End)
{
	::Arc(handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Arc(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Arc(handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::ArcTo(RECT Bound, POINT Start, POINT End)
{
	::ArcTo(handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::ArcTo(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::ArcTo(handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::AngleArc(int X, int Y, int Radius, float StartAngle, float SweepAngle)
{
	::AngleArc(handle, X, Y, Radius, StartAngle, SweepAngle);
}

void WinDC::AngleArc(int X, int Y, int Radius, double StartAngle, double SweepAngle)
{
	::AngleArc(handle, X, Y, Radius, static_cast<float>(StartAngle), static_cast<float>(SweepAngle));
}

void WinDC::Chord(RECT Bound, POINT Start, POINT End)
{
	::Chord(handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Chord(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Chord(handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::Pie(RECT Bound, POINT Start, POINT End)
{
	::Pie(handle, Bound.left, Bound.top, Bound.right, Bound.bottom, Start.x, Start.y, End.x, End.y);
}

void WinDC::Pie(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY)
{
	::Pie(handle, Left, Top, Right, Bottom, StartX, StartY, EndX, EndY);
}

void WinDC::GradientTriangle(TRIVERTEX Vertices[], int VerticesCount, GRADIENT_TRIANGLE Triangles[], int TriangleCount)
{
	GradientFill(handle, Vertices, VerticesCount, Triangles, TriangleCount, GRADIENT_FILL_TRIANGLE);
}

void WinDC::BeginPath()
{
	::BeginPath(handle);
}

void WinDC::EndPath()
{
	::EndPath(handle);
}

void WinDC::ClosePath()
{
	::CloseFigure(handle);
}

void WinDC::DiscardPath()
{
	::AbortPath(handle);
}

void WinDC::DrawPath()
{
	::StrokePath(handle);
}

void WinDC::FillPath()
{
	::FillPath(handle);
}

void WinDC::DrawAndFillPath()
{
	::StrokeAndFillPath(handle);
}

WinRegion::Ptr WinDC::RegionFromPath()
{
	return MakeShared<WinRegion>(::PathToRegion(handle));
}

bool WinDC::PointInClip(POINT Point)
{
	return PtVisible(handle, Point.x, Point.y) == TRUE;
}

bool WinDC::RectangleInClip(RECT Rect)
{
	return RectVisible(handle, &Rect) == TRUE;
}

void WinDC::ClipPath(int CombineMode)
{
	SelectClipPath(handle, CombineMode);
}

void WinDC::ClipRegion(WinRegion::Ptr Region)
{
	SelectClipRgn(handle, Region->GetHandle());
}

void WinDC::RemoveClip()
{
	SelectClipRgn(handle, NULL);
}

void WinDC::MoveClip(int OffsetX, int OffsetY)
{
	OffsetClipRgn(handle, OffsetX, OffsetY);
}

void WinDC::CombineClip(WinRegion::Ptr Region, int CombineMode)
{
	ExtSelectClipRgn(handle, Region->GetHandle(), CombineMode);
}

void WinDC::IntersetClipRect(RECT Rect)
{
	::IntersectClipRect(handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

void WinDC::ExcludeClipRect(RECT Rect)
{
	::ExcludeClipRect(handle, Rect.left, Rect.top, Rect.right, Rect.bottom);
}

WinRegion::Ptr WinDC::GetClipRegion()
{
	HRGN Handle = CreateRectRgn(0, 0, 1, 1);
	GetClipRgn(handle, Handle);
	return MakeShared<WinRegion>(Handle);
}

RECT WinDC::GetClipBoundRect()
{
	RECT Rect;
	GetClipBox(handle, &Rect);
	return Rect;
}

WinTransform WinDC::GetTransform()
{
	XFORM Transform;
	GetWorldTransform(handle, &Transform);
	return Transform;
}

void WinDC::SetTransform(const WinTransform& Transform)
{
	SetWorldTransform(handle, Transform.GetHandle());
}

void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	BitBlt(handle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, DrawROP);
}

void WinDC::Copy(RECT dstRect, WinDC* Source, POINT srcPos, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	BitBlt(handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top, SourceHandle, srcPos.x, srcPos.y, DrawROP);
}

void WinDC::Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	StretchBlt(handle, dstX, dstY, dstW, dstH, SourceHandle, srcX, srcY, srcW, srcH, DrawROP);
}

void WinDC::Copy(RECT dstRect, WinDC* Source, RECT srcRect, DWORD DrawROP)
{
	HDC SourceHandle = Source ? Source->GetHandle() : 0;
	StretchBlt(handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top,
			   SourceHandle, srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top,
			   DrawROP);
}

void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC* Source, int srcX, int srcY, int srcW, int srcH)
{
	POINT Pt[3];
	Pt[0] = UpperLeft;
	Pt[1] = UpperRight;
	Pt[2] = LowerLeft;
	PlgBlt(handle, Pt, Source->GetHandle(), srcX, srcY, srcW, srcH, 0, 0, 0);
}

void WinDC::Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC*Source, RECT srcRect)
{
	POINT Pt[3];
	Pt[0] = UpperLeft;
	Pt[1] = UpperRight;
	Pt[2] = LowerLeft;
	PlgBlt(handle, Pt, Source->GetHandle(), srcRect.left, srcRect.top, srcRect.right - srcRect.left, srcRect.bottom - srcRect.top, 0, 0, 0);
}

void WinDC::CopyTrans(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, COLORREF Color)
{
	TransparentBlt(handle, dstX, dstY, dstW, dstH, Source->GetHandle(), srcX, srcY, srcW, srcH, Color);
}

void WinDC::CopyTrans(RECT dstRect, WinDC* Source, RECT srcRect, COLORREF Color)
{
	TransparentBlt(handle, dstRect.left, dstRect.top, dstRect.right - dstRect.left, dstRect.bottom - dstRect.top,
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
	PlayEnhMetaFile(handle, MetaFile->GetHandle(), &Rect);
}

void WinDC::Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap)
{
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(POINT Pos, WinBitmap::Ptr Bitmap)
{
	int dstX = Pos.x;
	int dstY = Pos.y;
	int dstW = Bitmap->GetWidth();
	int dstH = Bitmap->GetHeight();
	int srcX = 0;
	int srcY = 0;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap)
{
	int srcX = 0;
	int srcY = 0;
	int srcW = Bitmap->GetWidth();
	int srcH = Bitmap->GetHeight();
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap)
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
		StretchBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY)
{
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos)
{
	int dstX = Rect.left;
	int dstY = Rect.top;
	int dstW = Rect.right - Rect.left;
	int dstH = Rect.bottom - Rect.top;
	int srcX = Pos.x;
	int srcY = Pos.y;
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		BitBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, SRCCOPY);
	}
	else
	{
		int srcW = dstW;
		int srcH = dstH;
		BLENDFUNCTION Blend{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH)
{
	if (!Bitmap->IsAlphaChannelBuilt())
	{
		StretchBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend;
		Blend.BlendOp = AC_SRC_OVER;
		Blend.BlendFlags = 0;
		Blend.SourceConstantAlpha = 255;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

void WinDC::Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect)
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
		StretchBlt(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, SRCCOPY);
	}
	else
	{
		BLENDFUNCTION Blend;
		Blend.BlendOp = AC_SRC_OVER;
		Blend.BlendFlags = 0;
		Blend.SourceConstantAlpha = 255;
		Blend.AlphaFormat = AC_SRC_ALPHA;
		AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
	}
}

/*------------------------------------------------------------------------------*/

void WinDC::Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(POINT Pos, WinBitmap::Ptr Bitmap, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, byte Alpha)
{
	int srcW = dstW;
	int srcH = dstH;

	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH, byte Alpha)
{
	BLENDFUNCTION Blend;
	Blend.BlendOp = AC_SRC_OVER;
	Blend.BlendFlags = 0;
	Blend.SourceConstantAlpha = Alpha;
	Blend.AlphaFormat = Bitmap->IsAlphaChannelBuilt() ? AC_SRC_ALPHA : 0;
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

void WinDC::Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect, byte Alpha)
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
	AlphaBlend(handle, dstX, dstY, dstW, dstH, Bitmap->GetWinDC()->GetHandle(), srcX, srcY, srcW, srcH, Blend);
}

WinControlDC::WinControlDC(HWND Handle)
	: WinDC()
	, controlHandle(Handle)
{
	handle = GetDC(controlHandle);
	Initialize();
}

WinControlDC::~WinControlDC()
{
	ReleaseDC(controlHandle, handle);
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
	handle = Handle;
	WinDC::Initialize();
}

WinImageDC::WinImageDC()
	: WinDC()
{
	handle = CreateCompatibleDC(NULL);
	Initialize();
}

WinImageDC::~WinImageDC()
{
	DeleteDC(handle);
}

}
}
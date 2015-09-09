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
{}

WinRegion::WinRegion(POINT Points[], int Count, bool Alternate)
	: _Handle(CreatePolygonRgn(Points, Count, Alternate ? ALTERNATE : WINDING))
{}

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
{}

WinRegion::~WinRegion()
{
	DeleteObject(_Handle);
}

WinTransform::WinTransform(XFORM Transform)
	: _Transform(Transform)
{}

WinTransform::WinTransform(const WinTransform& Transform)
	: _Transform(Transform._Transform)
{}

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
{}

WinBrush::WinBrush(int Hatch, COLORREF Color)
	: _DIBMemory(nullptr)
	, _Handle(CreateHatchBrush(Hatch, Color))
{}

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
{}

WinPen::WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color)
	: _Handle(0)
	, _DIBMemory(nullptr)
{
	LOGBRUSH Brush;
	Brush.lbColor = Color;
	Brush.lbStyle = BS_SOLID;
	Brush.lbHatch = 0;
	_Handle - ExtCreatePen(PS_GEOMETRIC | Style | EndCap | Join, Width, &Brush, 0, 0);
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
	wcsncpy(_FontInfo.lfFaceName, Name.c_str(), LF_FACESIZE - 1);

	_Handle = CreateFontIndirectW(&_FontInfo);
}

WinFont::WinFont(LOGFONT* FontInfo)
	: _FontInfo(*FontInfo)
	, _Handle (CreateFontIndirect(&_FontInfo))
{}

WinFont::~WinFont()
{
	DeleteObject(_Handle);
}


}
}
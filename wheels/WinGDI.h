#pragma once
#ifndef WINGDI_H
#define WINGDI_H

#include "base.h"
#include <string>
#include <memory>
#include <functional>
#include <Windows.h>

namespace pl
{
namespace windows
{

class WinRegion: public Object
{
public:
	using Ptr = SharedPtr<WinRegion>;
	friend bool IsEqual(WinRegion::Ptr Region1, WinRegion::Ptr Region2);
protected:
	HRGN handle;
public:
	WinRegion(int left, int top, int right, int bottom, bool rectangle);
	WinRegion(RECT Rect, bool IsRectangle);
	WinRegion(int left, int top, int right, int bottom, int EllipseWidth, int EllipseHeight);
	WinRegion(POINT Points[], int Count, bool Alternate);
	WinRegion(WinRegion::Ptr Region);
	WinRegion(WinRegion::Ptr Region1, WinRegion::Ptr Region2, int CombineMode);
	WinRegion(HRGN Rectangle);
	~WinRegion();

	HRGN GetHandle();
	bool ContainPoint(POINT Point);
	bool ContainRectangle(RECT Rect);
	RECT GetBoundRectangle();
	void Move(int OffsetX, int OffsetY);
};

class WinTransform: public Object
{
protected:
	XFORM transform;
public:
	WinTransform(XFORM Transform);
	WinTransform(const WinTransform& Transform);

	WinTransform& operator=(const WinTransform& Transform);
	WinTransform operator*(const WinTransform& Transform);
	const XFORM* GetHandle() const;

	static WinTransform Translate(float OffsetX, float OffsetY);
	static WinTransform Scale(float ScaleX, float ScaleY);
	static WinTransform Rotate(float Angle);
	static WinTransform Rotate(float cos, float sin);
	static WinTransform ReflectX();
	static WinTransform ReflectY();
	static WinTransform Reflect(float VectorX, float VectorY);
	static WinTransform Reflect(float OrigionX, float OrigionY, float VectorX, float VectorY);
	static WinTransform AxisV(float Xx, float Xy, float Yx, float Yy);
	static WinTransform AxisA(float AngleX, float LenX, float AngleY, float LenY);
};

class WinDC;
class WinControlDC;
class WinProxyDC;
class WinImageDC;

class WinMetaFileBuilder: public Object
{
	friend class WinMetaFile;
protected:
	int width;
	int height;
	WinProxyDC* proxyDC;

	void Create(int Width, int Height);
	void Draw(HENHMETAFILE Handle);
	void Destroy();
public:
	WinMetaFileBuilder(int Width, int Height);
	~WinMetaFileBuilder();

	void LoadFrom(WinMetaFile* File);
	void SaveTo(WinMetaFile* File);
	void LoadFrom(const std::wstring& FileName);
	void SaveTo(const std::wstring& FileName);
	WinDC* GetWinDC();
	int GetWidth();
	int GetHeight();
};

class WinMetaFile: public Object
{
	friend class WinMetaFileBuilder;
protected:
	HENHMETAFILE handle;
	int width;
	int height;
public:
	WinMetaFile(const std::wstring& FileName);
	WinMetaFile(WinMetaFileBuilder* Builder);
	~WinMetaFile();

	HENHMETAFILE GetHandle();
	int GetWidth();
	int GetHeight();
};

class WinBitmap: public Object
{
public:
	using Ptr = SharedPtr<WinBitmap>;
	enum class BitmapBits
	{
		Bit2,
		Bit24,
		Bit32
	};
protected:
	BitmapBits bits;
	int width;
	int height;
	WinImageDC* imageDC;
	HBITMAP handle;
	BYTE** scanLines;
	bool hasAlphaChannelBuilt;

	int GetBitsFromBB(BitmapBits BB);
	int GetLineBytes(int Width, BitmapBits BB);
	void FillBitmapInfoHeader(int Width, int Height, BitmapBits Bits, BITMAPINFOHEADER* Header);
	HBITMAP CreateDDB(int Width, int Height, BitmapBits Bits);
	HBITMAP CreateDIB(int Width, int Height, BitmapBits Bits, BYTE**& ScanLines);
	void Constuctor(int Width, int Height, BitmapBits Bits, bool DIBSections);
public:
	WinBitmap(int Width, int Height, BitmapBits Bits, bool DIBSections);
	WinBitmap(const std::wstring& FileName, bool IsBit32, bool DIBSections);
	~WinBitmap();

	void SaveToFile(const std::wstring& FileName);

	WinDC* GetWinDC();
	int GetWidth();
	int GetHeight();
	int GetLineBytes();
	BYTE** GetScanLines();
	HBITMAP GetBitmap();
	BitmapBits GetBitmapBits();
	void FillCompatibleHeader(BITMAPINFOHEADER* Header);

	bool CanBuildAlphaChannel();
	bool IsAlphaChannelBuilt();
	void BuildAlphaChannel();
	void GenerateTrans(COLORREF Color);
	void GenerateAlpha(BYTE Alpha);
	void GenerateTransAlpha(COLORREF Color, BYTE Alpha);
	void GenerateLuminance();
	void GenerateGrayLevel();
	void Generate(std::function<BYTE(COLORREF)> Function);
};

class WinBrush: public Object
{
public:
	using Ptr = SharedPtr<WinBrush>;
protected:
	HBRUSH handle;
	BYTE* dibMemory;
public:
	WinBrush(COLORREF Color);
	WinBrush(int Hatch, COLORREF Color);
	WinBrush(WinBitmap::Ptr Bitmap);
	~WinBrush();

	HBRUSH GetHandle();
};

class WinPen: public Object
{
public:
	using Ptr = SharedPtr<WinPen>;
protected:
	HPEN handle;
	BYTE* dibMemory;
public:
	WinPen(int Style, int Width, COLORREF Color);
	WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color);
	WinPen(int Style, int EndCap, int Join, int Hatch, int Width, COLORREF Color);
	WinPen(WinBitmap::Ptr DIB, int Style, int EndCap, int Join, int Width);
	~WinPen();

	HPEN GetHandle();
};

class WinFont: public Object
{
public:
	using Ptr = SharedPtr<WinFont>;
protected:
	LOGFONT fontInfo;
	HFONT handle;
public:
	WinFont(const std::wstring& Name, int Height, int Width, int Escapement, int Orientation, int Weight, bool Italic, bool UnderLine, bool StrikeOut, bool Antialise);
    
    static WinFont::Ptr GetFontForWindow(HWND Handle, const wchar_t* Name, int Point);
    WinFont(LOGFONT* FontInfo);
	~WinFont();
	HFONT GetHandle();
	LOGFONT* GetInfo();
};

class WinDC: public Object
{
protected:
	HDC handle;
	WinPen::Ptr pen;
	WinBrush::Ptr brush;
	WinFont::Ptr font;

	HPEN oldPen;
	HBRUSH oldBrush;
	HFONT oldFont;

	void Initialize();
public:
	WinDC();
	~WinDC();

	HDC GetHandle();

	void SetPen(WinPen::Ptr Pen);
	void SetBrush(WinBrush::Ptr Brush);
	void SetFont(WinFont::Ptr Font);
	COLORREF GetBackgroundColor();
	void SetBackgroundColor(COLORREF Color);
	COLORREF GetTextColor();
	void SetTextColor(COLORREF Color);
	bool GetBackgroundTransparent();
	void SetBackgroundTransparent(bool Transparent);
	POINT GetBrushOrigin();
	void SetBrushOrigin(POINT Point);

	void DrawString(int X, int Y, const std::wstring& Text);
	void DrawString(int X, int Y, const std::wstring& Text,
					int TabWidth, int TabOriginX);

	void FillRegion(WinRegion::Ptr Region);
	void FrameRegion(WinRegion::Ptr Region, int BlockWidth, int BlockHeight);

	void MoveTo(int X, int Y);
	void LineTo(int X, int Y);
	void Rectangle(int Left, int Top, int Right, int Bottom);
	void Rectangle(RECT Rect);
	void FillRect(int Left, int Top, int Right, int Bottom);
	void FillRect(RECT Rect);
	void Ellipse(int Left, int Top, int Right, int Bottom);
	void Ellipse(RECT Rect);
	void RoundRect(int Left, int Top, int Right, int Bottom, int EllipseWidth, int EllipseHeight);
	void RoundRect(RECT Rect, int EllipseWidth, int EllipseHeight);
	void PolyLine(const POINT Points[], int Count);
	void PolyLineTo(const POINT Points[], int Count);
	void Polygon(const POINT Points[], int Count);
	void PolyBezier(const POINT Points[], int Count);
	void PolyBezierTo(const POINT Points[], int Count);
	void PolyDraw(const POINT Points[], const BYTE Actions[], int PointCount);
	void Arc(RECT Bound, POINT Start, POINT End);
	void Arc(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EnxY);
	void ArcTo(RECT Bound, POINT Start, POINT End);
	void ArcTo(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EnxY);
	void AngleArc(int X, int Y, int Radius, float StartAngle, float SweepAngle);
	void AngleArc(int X, int Y, int Radius, double StartAngle, double SweepAngle);
	void Chord(RECT Bound, POINT Start, POINT End);
	void Chord(int ledt, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY);
	void Pie(RECT Bound, POINT Start, POINT End);
	void Pie(int Left, int Top, int Right, int Bottom, int StartX, int StartY, int EndX, int EndY);
	void GradientTriangle(TRIVERTEX Vertices[], int VerticesCount, GRADIENT_TRIANGLE Triangles[], int TriangleCount);

	void BeginPath();
	void EndPath();
	void ClosePath();
	void DiscardPath();
	void DrawPath();
	void FillPath();
	void DrawAndFillPath();
	WinRegion::Ptr RegionFromPath();

	bool PointInClip(POINT Point);
	bool RectangleInClip(RECT Rect);
	void ClipPath(int CombindMode);
	void ClipRegion(WinRegion::Ptr Region);
	void RemoveClip();
	void MoveClip(int OffsetX, int OffsetY);
	void CombineClip(WinRegion::Ptr Region, int CombineMode);
	void IntersetClipRect(RECT Rect);
	void ExcludeClipRect(RECT Rect);
	WinRegion::Ptr GetClipRegion();
	RECT GetClipBoundRect();

	WinTransform GetTransform();
	void SetTransform(const WinTransform& Transform);

	void Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Souce, int srcX, int srcY, DWORD DrawROP = SRCCOPY);
	void Copy(RECT dstRect, WinDC* Source, POINT srcPos, DWORD DrawROP = SRCCOPY);
	void Copy(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, DWORD DrawROP = SRCCOPY);
	void Copy(RECT dstRect, WinDC* Source, RECT srcRect, DWORD DrawROP = SRCCOPY);
	void Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC* Source, int srcX, int srcY, int srcW, int srcH);
	void Copy(POINT UpperLeft, POINT UpperRight, POINT LowerLeft, WinDC* Source, RECT srcRect);
	void CopyTrans(int dstX, int dstY, int dstW, int dstH, WinDC* Source, int srcX, int srcY, int srcW, int srcH, COLORREF Color);
	void CopyTrans(RECT dstRect, WinDC* Source, RECT srcRect, COLORREF Color);

	void Draw(int dstX, int dstY, WinMetaFile* MetaFile);
	void Draw(POINT Pos, WinMetaFile* MetaFile);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinMetaFile* MetaFile);
	void Draw(RECT Rect, WinMetaFile* MetaFile);

	void Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap);
	void Draw(POINT Pos, WinBitmap::Ptr Bitmap);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap);
	void Draw(RECT Rect, WinBitmap::Ptr Bitmap);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY);
	void Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH);
	void Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect);

	void Draw(int dstX, int dstY, WinBitmap::Ptr Bitmap, byte Alpha);
	void Draw(POINT Pos, WinBitmap::Ptr Bitmap, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, byte Alpha);
	void Draw(RECT Rect, WinBitmap::Ptr Bitmap, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, byte Alpha);
	void Draw(RECT Rect, WinBitmap::Ptr Bitmap, POINT Pos, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Ptr Bitmap, int srcX, int srcY, int srcW, int srcH, byte Alpha);
	void Draw(RECT dstRect, WinBitmap::Ptr Bitmap, RECT srcRect, byte Alpha);
};

class WinControlDC: public WinDC
{
protected:
	HWND controlHandle;
public:
	WinControlDC(HWND Handle);
	~WinControlDC();
};

class WinProxyDC: public WinDC
{
public:
	WinProxyDC();
	~WinProxyDC();

	void Initialize(HDC Handle);
};

class WinImageDC: public WinDC
{
public:
	WinImageDC();
	~WinImageDC();
};

#include "WinGDI.inl"

}
}

#endif // !WINGDI_H

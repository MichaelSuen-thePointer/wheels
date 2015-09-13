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
	using Pointer = std::shared_ptr<WinRegion>;
	friend bool IsEqual(WinRegion::Pointer Region1, WinRegion::Pointer Region2);
protected:
	HRGN _Handle;
public:
	WinRegion(int left, int top, int right, int bottom, bool rectangle);
	WinRegion(RECT Rect, bool IsRectangle);
	WinRegion(int left, int top, int right, int bottom, int EllipseWidth, int EllipseHeight);
	WinRegion(POINT Points[], int Count, bool Alternate);
	WinRegion(WinRegion::Pointer Region);
	WinRegion(WinRegion::Pointer Region1, WinRegion::Pointer Region2, int CombineMode);
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
	XFORM _Transform;
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
	int _Width;
	int _Height;
	WinProxyDC* _DC;

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
	HENHMETAFILE _Handle;
	int _Width;
	int _Height;
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
	using Pointer = std::shared_ptr<WinBitmap>;
	enum class BitmapBits
	{
		Bit2,
		Bit24,
		Bit32
	};
protected:
	BitmapBits _Bits;
	int _Width;
	int _Height;
	WinImageDC* _DC;
	HBITMAP _Handle;
	BYTE** _ScanLines;
	bool _AlphaChannelBuilt;

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
	using Pointer = std::shared_ptr<WinBrush>;
protected:
	HBRUSH _Handle;
	BYTE* _DIBMemory;
public:
	WinBrush(COLORREF Color);
	WinBrush(int Hatch, COLORREF Color);
	WinBrush(WinBitmap::Pointer Bitmap);
	~WinBrush();

	HBRUSH GetHandle();
};

class WinPen: public Object
{
public:
	using Pointer = std::shared_ptr<WinPen>;
protected:
	HPEN _Handle;
	BYTE* _DIBMemory;
public:
	WinPen(int Style, int Width, COLORREF Color);
	WinPen(int Style, int EndCap, int Join, int Width, COLORREF Color);
	WinPen(int Style, int EndCap, int Join, int Hatch, int Width, COLORREF Color);
	WinPen(WinBitmap::Pointer DIB, int Style, int EndCap, int Join, int Width);
	~WinPen();

	HPEN GetHandle();
};

class WinFont: public Object
{
public:
	using Pointer = std::shared_ptr<WinFont>;
protected:
	LOGFONT _FontInfo;
	HFONT _Handle;
public:
	WinFont(const std::wstring& Name, int Height, int Width, int Escapement, int Orientation, int Weight, bool Italic, bool UnderLine, bool StrikeOut, bool Antialise);
	WinFont(LOGFONT* FontInfo);
	~WinFont();
	HFONT GetHandle();
	LOGFONT* GetInfo();
};

class WinDC: public Object
{
protected:
	HDC _Handle;
	WinPen::Pointer _Pen;
	WinBrush::Pointer _Brush;
	WinFont::Pointer _Font;

	HPEN _OldPen;
	HBRUSH _OldBrush;
	HFONT _OldFont;

	void Initialize();
public:
	WinDC();
	~WinDC();

	HDC GetHandle();
	
	void SetPen(WinPen::Pointer Pen);
	void SetBrush(WinBrush::Pointer Brush);
	void SetFont(WinFont::Pointer Font);
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

	void FillRegion(WinRegion::Pointer Region);
	void FrameRegion(WinRegion::Pointer Region, int BlockWidth, int BlockHeight);

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
	WinRegion::Pointer RegionFromPath();

	bool PointInClip(POINT Point);
	bool RectangleInClip(RECT Rect);
	void ClipPath(int CombindMode);
	void ClipRegion(WinRegion::Pointer Region);
	void RemoveClip();
	void MoveClip(int OffsetX, int OffsetY);
	void CombineClip(WinRegion::Pointer Region, int CombineMode);
	void IntersetClipRect(RECT Rect);
	void ExcludeClipRect(RECT Rect);
	WinRegion::Pointer GetClipRegion();
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

	void Draw(int dstX, int dstY, WinBitmap::Pointer Bitmap);
	void Draw(POINT Pos, WinBitmap::Pointer Bitmap);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap);
	void Draw(RECT Rect, WinBitmap::Pointer Bitmap);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY);
	void Draw(RECT Rect, WinBitmap::Pointer Bitmap, POINT Pos);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, int srcW, int srcH);
	void Draw(RECT dstRect, WinBitmap::Pointer Bitmap, RECT srcRect);

	void Draw(int dstX, int dstY, WinBitmap::Pointer Bitmap, byte Alpha);
	void Draw(POINT Pos, WinBitmap::Pointer Bitmap, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, byte Alpha);
	void Draw(RECT Rect, WinBitmap::Pointer Bitmap, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, byte Alpha);
	void Draw(RECT Rect, WinBitmap::Pointer Bitmap, POINT Pos, byte Alpha);
	void Draw(int dstX, int dstY, int dstW, int dstH, WinBitmap::Pointer Bitmap, int srcX, int srcY, int srcW, int srcH, byte Alpha);
	void Draw(RECT dstRect, WinBitmap::Pointer Bitmap, RECT srcRect, byte Alpha);
};

class WinControlDC: public WinDC
{
protected:
	HWND _ControlHandle;
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

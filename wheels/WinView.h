#pragma once
#ifndef WINVIEW_H
#define WINVIEW_H

#include "WinClass.h"
#include <CommCtrl.h>

namespace pl
{
namespace windows
{

class WinListViewItem: public Object
{
    friend class WinListView;
protected:
    HWND _Handle;
    int _Index;

    WinListViewItem(HWND Handle, int Index);
public:
    WinListViewItem();
    WinListViewItem(const WinListViewItem& Item);
    WinListViewItem& operator=(const WinListViewItem& Item);
    operator bool();

    int GetIndex();

    std::wstring GetText();
    void SetText(std::wstring& Value);

    int GetImageIndex();
    void SetImageIndex(int Value);

    std::wstring GetSubItem(int Index);
    void SetSubItem(int Index, std::wstring& Value);

    void GetTiledSubItem(std::vector<int>& Columns);
    void SetTiledSubItem(std::vector<int>& Conumns);

    bool GetCutState();
    void SetCutState(bool Value);

    bool GetDropHilitedState();
    void SetDropHilitedState(bool Value);

    bool GetFocusedState();
    void SetFocusedState(bool Value);

    bool GetSelectedState();
    void SetSelectedState(bool Value);

    int GetIndent();
    void SetIndent(int Value);

    bool GetChecked();
    void SetChecked(bool Value);

    void* GetCustomData();
    void SetCustomData(void* Data);

    void EnterGroup(int GroupID);
    void LeaveGroup();
    int GetOwnerGroupID();
    bool IsInGroup();
};

class WinListViewColumn: public Object
{
    friend class WinListView;
protected:
    HWND _Handle;
    int _Index;

    WinListViewColumn(HWND Handle, int Index);
public:
    WinListViewColumn();
    WinListViewColumn(const WinListViewColumn& Item);

    WinListViewColumn& operator=(const WinListViewColumn& Item);
    operator bool();

    int GetIndex();

    std::wstring GetText();
    void SetText(std::wstring& Value);

    int GetSubItemIndex();
    void SetSubItemIndex(int Value);

    int GetWidth();
    void SetWidth(int Value);

    int GetOrder();
    void SetOrder(int Value);

    int GetImageIndex();
    void SetImageIndex(int Value);

    WinHAlign GetTextAlign();
    void SetTextAlign(WinHAlign Value);

    bool GetImageOnRight();
    void SetImageOnRight(bool Value);

    bool GetFixedWidth();
    void SetFixedWidth(bool Value);

    bool GetShowSplitButton();
    void SetShowSplitButton(bool Value);
};

class WinListViewGroup: public Object
{
    friend class WinListView;
protected:
    HWND _Handle;
    int _ID;
    WinListViewGroup(HWND Handle, int ID);

    std::wstring GetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, UINT LVGROUP::* Count);
    std::wstring GetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, int LVGROUP::* Count);
    void SetListViewGroupText(UINT Mask, LPWSTR LVGROUP::* Item, std::wstring& Text);
public:
    WinListViewGroup();
    WinListViewGroup(const WinListViewGroup& Item);

    WinListViewGroup& operator=(const WinListViewGroup& Item);
    operator bool();

    int GetGroupID();

    std::wstring GetHeader();
    void SetHeader(std::wstring& Value);

    std::wstring GetFooter();
    void SetFooter(std::wstring& Value);

    std::wstring GetSubTitle();
    void SetSubTitle(std::wstring& Value);

    std::wstring GetTask();
    void SetTask(std::wstring& Value);

    std::wstring GetDescriptionTop();
    void SetDescriptionTop(std::wstring& Value);

    std::wstring GetDescriptionBottom();
    void SetDescriptionBottom(std::wstring& Value);

    int GetImageIndex();
    void SetImageIndex(int Value);

    int GetExtendedImageIndex();
    void SetExtendedImageIndex(int Value);

    WinHAlign GetHeaderAlign();
    void SetHeaderAlign(WinHAlign Value);
};

enum class ListViewStyle
{
    Large, Small, List, Report, Tile
};

enum class ListViewSortResult
{
    Larger, Equal, Smaller
};

using ListViewItemEvent = Event<Object*, int, int>;
using ListViewEditEvent = Event<Object*, int, bool&, const std::wstring&>;
using ListViewSortEvent = Event<Object*, int, int, void*, ListViewSortResult&>;

class WinListView: public WinControl
{
protected:
    WinImageList* _SmallImageList;
    WinImageList* _LargeImageList;
    WinImageList* _StateImageList;
    WinImageList* _GroupImageList;

    int _ColumnCount;
    int _UsedGroupIDs;

    DWORD InternalGetExStyle() override;
    void InternalSetExStyle(DWORD Exstyle) override;
public:
    WinListView(WinContainer* Parent);
    ~WinListView();

    LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure);

    ListViewItemEvent OnBeginDrag;
    ListViewItemEvent OnBeginRightDrag;
    ListViewItemEvent OnColumnClick;
    ListViewItemEvent OnColumnDropDown;
    ListViewItemEvent OnItemHotTrack;
    ListViewItemEvent OnItemActive;
    ListViewItemEvent OnItemChanging;
    ListViewItemEvent OnItemChanged;
    ListViewItemEvent OnGroupTaskClick;
    ListViewItemEvent OnItemClick;
    ListViewItemEvent OnItemDblClick;
    ListViewItemEvent OnItemRightClick;
    ListViewItemEvent OnItemRightDblClick;
    ListViewEditEvent OnBeginLabelEdit;
    ListViewEditEvent OnEndLabelEdit;
    ListViewSortEvent OnCompareItems;

    WinImageList* GetSmallImageList();
    void SetSmallImageList(WinImageList* ImageList);

    WinImageList* GetLargeImageList();
    void SetLargeImageList(WinImageList* ImageList);

    WinImageList* GetStateImageList();
    void SetStateImageList(WinImageList* ImageList);

    WinImageList* GetGroupImageList();
    void SetGroupImageList(WinImageList* ImageList);

    ListViewStyle GetViewStyle();
    void SetViewStyle(ListViewStyle Style);

    COLORREF GetOutlineColor();
    void SetOutlineColor(COLORREF Value);

    COLORREF GetTextColor();
    void SetTextColor(COLORREF Value);

    COLORREF GetTextBackgroundColor();
    void SetTextBackgroundColor(COLORREF Value);

    COLORREF GetBackgroundColor();
    void SetBackgroundColor(COLORREF Value);

    /*Styles*/

    bool GetAutoArrange();
    void SetAutoArrange(bool Value);
         
    bool GetEditable();
    void SetEditable(bool Value);
         
    bool GetColumnPushable();
    void SetColumnPushable(bool Value);
         
    bool GetAlwaysShowSelection();
    void SetAlwaysShowSelection(bool Value);
         
    bool GetSingleSelection();
    void SetSingleSelection(bool Value);
         
    bool GetShowCheckBoxes();
    void SetShowCheckBoxes(bool Value);
         
    bool GetAutoCheckSelect();
    void SetAutoCheckSelect(bool Value);
         
    bool GetBorderSelect();
    void SetBorderSelect(bool Value);
         
    bool GetShowGridLines();
    void SetShowGridLines(bool Value);
         
    bool GetHeaderDragable();
    void SetHeaderDragable(bool Value);
         
    bool GetOneClickActive();
    void SetOneClickActive(bool Value);
         
    bool GetTwoClickActive();
    void SetTwoClickActive(bool Value);
         
    bool GetUnderlineHot();
    void SetUnderlineHot(bool Value);
         
    bool GetUnderlineCold();
    void SetUnderlineCold(bool Value);
         
    bool GetAutoAutoArrange();
    void SetAutoAutoArrange(bool Value);
         
    bool GetSnapToGrid();
    void SetSnapToGrid(bool Value);

    /*Actions*/

    void EditLabel(int Index);
    void CancelEditLabel();
    void Arrange();
    void SnapToGrid();
    void EnsureItemVisible(int Index, bool AcceptPartialVisible);
    void Sort(void* Param = nullptr);

    /*Items*/

    int GetItemCount();
    void AddItem(std::wstring& Text, int ImageIndex = -1);
    void InsertItem(int Index, std::wstring& Text, int ImageIndex = -1);
    void DeleteItem(int Index);
    void Clear();
    WinListViewItem GetItem(int Index);
    int GetSelectedItemCount();
    int GetSelectionItemMark();
    void SetSelectionItemMark(int Value);
    int GetHotItemIndex();
    void SetHotItemIndex(int Value);

    /*Columns*/

    int GetColumnCount();
    void AddColumn(std::wstring& Text, int SubItemIndex = -1);
    void InsertColumn(int Index, std::wstring& Text, int SubItemIndex = -1);
    void DeleteColumn(int Index);
    WinListViewColumn GetColumn(int index);
    int GetSelectedColumnIndex();
    void SetSelectedColumnIndex(int Value);

    /*Groups*/

    void EnableGrouping(bool Value);
    int GetGroupCount();
    int AddGroup(std::wstring& Text);
    int InsertGroup(int Index, std::wstring& Text);
    void DeleteGroup(int Index);
    void DeleteGroupByID(int ID);
    WinListViewGroup GetGroup(int Index);
    WinListViewGroup GetGroupByID(int ID);
};

class WinTreeViewItem: public Object
{
    friend class WinTreeView;
protected:
    HWND _Handle;
    HTREEITEM _Item;

    WinTreeViewItem(HWND Handle, HTREEITEM Item);
public:
    WinTreeViewItem(const WinTreeViewItem& Item);
    WinTreeViewItem();
    operator bool();
    WinTreeViewItem& operator=(const WinTreeViewItem& Item);

    WinTreeViewItem InsertAtLast(std::wstring& Text, int ImageIndex = -1, int SelectedImageIndex = -1);
    WinTreeViewItem InsertAtFirst(std::wstring& Text, int ImageIndex = -1, int SelectedImageIndex = -1);
    WinTreeViewItem InsertAfter(std::wstring& Text, int ImageIndex = -1, int SelectedImageIndex = -1);
    void DeleteSelf();

    WinTreeViewItem GetParent();
    WinTreeViewItem GetFirstChild();
    WinTreeViewItem GetPrevious();
    WinTreeViewItem GetNext();
    WinTreeViewItem GetNextVisible();
    WinTreeViewItem GetPrevVisible();
    WinTreeViewItem GetNextSelected();

    HTREEITEM GetHandle();
    void SetDropTarget();
    void SetFirstVisible();

    std::wstring GetText();
    void SetText(std::wstring& Value);

    int GetImageIndex();
    void SetImageIndex(int Value);

    int GetSelectedImageIndex();
    void SetSelectedImageIndex(int Value);

    void* GetCustomData();
    void SetCustomData(void* Value);

    bool GetBoldState();
    void SetBoldState(bool Value);

    bool GetCutState();
    void SetCutState(bool Value);

    bool GetDropHilitedState();
    void SetDropHilitedState(bool Value);

    bool GetExpendedState();
    void SetExpendedState(bool Value);

    bool GetSelectedState();
    void SetSelectedState(bool Value);

    bool GetChecked();
    void SetChecked(bool Value);

    void EnsureVisible();
    void Expand();
    void Collapse();
    void Toggle();
};

using TreeViewItemEvent = Event<Object*, WinTreeViewItem>;
using TreeViewVerifyEvent = Event<Object*, WinTreeViewItem, bool&>;
using TreeViewEditEvent = Event<Object*, WinTreeViewItem, bool&, const std::wstring&>;

class WinTreeView: public WinControl
{
protected:
    WinImageList* _NormalImageList;
    WinImageList* _StateImageList;
public:
    WinTreeView(WinContainer* Parent);
    ~WinTreeView();

    LRESULT ProcessMessage(UINT Message, WPARAM& wParam, LPARAM& lParam, bool& CallDefaultProcedure) override;

    NotifyEvent OnClick;
    NotifyEvent OnDblClick;
    NotifyEvent OnRightClick;
    NotifyEvent OnRightDblClick;
    TreeViewItemEvent OnBeginDrag;
    TreeViewItemEvent OnBeginRightDrag;
    TreeViewItemEvent OnItemExpanding;
    TreeViewItemEvent OnItemExpanded;
    TreeViewItemEvent OnItemSelected;
    TreeViewEditEvent OnBeginLabelEdit;
    TreeViewEditEvent OnEndLabelEdit;
    TreeViewVerifyEvent OnItemSelecting;

    WinImageList* GetNormalImageList();
    void SetNormalImageList(WinImageList* ImageList);
                     
    WinImageList* GetStateImageList();
    void SetStateImageList(WinImageList* ImageList);
                     
    COLORREF GetLineColor();
    void SetLineColor(COLORREF Value);
                     
    COLORREF GetTextColor();
    void SetTextColor(COLORREF Value);
             
    COLORREF GetBackColor();
    void SetBackColor(COLORREF Value);
                     
    /*Styles*/
                     
    bool GetShowCheckBoxes();
    void SetShowCheckBoxes(bool Value);
         
    bool GetEnableDragDrop();
    void SetEnableDragDrop(bool Value);
         
    bool GetEditable();
    void SetEditable(bool Value);
         
    bool GetFullRowSelect();
    void SetFullRowSelect(bool Value);
         
    bool GetHasButtons();
    void SetHasButtons(bool Value);
         
    bool GetHasLines();
    void SetHasLines(bool Value);
         
    bool GetAlwaysShowSelection();
    void SetAlwaysShowSelection(bool Value);
         
    bool GetDimmedCheckBoxes();
    void SetDimmedCheckBoxes(bool Value);
         
    bool GetExclusionCheckBoxes();
    void SetExclusionCheckBoxes(bool Value);
         
    bool GetFadeButton();
    void SetFadeButton(bool Value);

    /*Actions*/

    void EditLabel(WinTreeViewItem Item);
    void FinishEditLabel();
    void CancelEditLabel();

    /*Items*/

    WinTreeViewItem AddRootItem(std::wstring& Text, int ImageIndex = -1, int SelectedImageIndex = -1);
    WinTreeViewItem GetFirstRoot();
    WinTreeViewItem GetCurrentSelectedItem();
    WinTreeViewItem GetFirstVisible();
    WinTreeViewItem GetLastVisible();
    WinTreeViewItem GetDropHighlighted();
    void Clear();
};

#include "WinView.inl"

}
}

#endif // !WINVIEW_H

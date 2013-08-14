//////////////////////////////////////////////////////////////////////////
//   File Name: DuiSkinPool
// Description: DuiWindow Skin Definition
//     Creator: ZhangXiaoxuan
//     Version: 2009.4.22 - 1.0 - Create
//				2012.8.18   1.1   huangjianxiong
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "duiskinbase.h"

namespace DuiEngine
{

// State Define
enum
{
    DuiWndState_Normal       = 0x00000000UL,
    DuiWndState_Hover        = 0x00000001UL,
    DuiWndState_PushDown     = 0x00000002UL,
    DuiWndState_Check        = 0x00000004UL,
    DuiWndState_Invisible    = 0x00000008UL,
    DuiWndState_Disable      = 0x00000010UL,
};

#define IIF_STATE2(the_state, normal_value, hover_value) \
    (((the_state) & DuiWndState_Hover) ? (hover_value) : (normal_value))

#define IIF_STATE3(the_state, normal_value, hover_value, pushdown_value) \
    (((the_state) & DuiWndState_PushDown) ? (pushdown_value) : IIF_STATE2(the_state, normal_value, hover_value))

#define IIF_STATE4(the_state, normal_value, hover_value, pushdown_value, disable_value) \
    (((the_state) & DuiWndState_Disable) ? (disable_value) : IIF_STATE3(the_state, normal_value, hover_value, pushdown_value))

class DUI_EXP CDuiImageList: public CDuiSkinBase
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiImageList, "imglst")

public:
    CDuiImageList();

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState,BYTE byAlpha);

    virtual SIZE GetSkinSize();

    virtual BOOL IgnoreState();
    virtual int GetStates();
	void    SetStates(int nStates){m_nStates=nStates;}
	virtual void SetWidth(LONG width) {
		m_lSubImageWidth=width;
		if(m_nStates==0)
		{
			if(m_pDuiImg)
				m_nStates=m_pDuiImg->GetWidth()/m_lSubImageWidth;
			else
				m_nStates=1;
		}
	}

protected:
	virtual void OnAttributeFinish(pugi::xml_node xmlNode);

    LONG m_lSubImageWidth;
	int  m_nStates;
    BOOL m_bTile;
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_INT_ATTRIBUTE("subwidth", m_lSubImageWidth, TRUE)
    DUIWIN_INT_ATTRIBUTE("tile", m_bTile, TRUE)
	DUIWIN_INT_ATTRIBUTE("states",m_nStates,TRUE)
    DUIWIN_DECLARE_ATTRIBUTES_END()
};

class DUI_EXP CDuiSkinImgFrame : public CDuiImageList
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiSkinImgFrame, "imgframe")

public:
    CDuiSkinImgFrame();

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState,BYTE byAlpha);

	void SetMargin(int nLeft,int nTop,int nRight,int nBottom);
protected:
	virtual void OnAttributeFinish(pugi::xml_node xmlNode);

	CRect m_rcMargin;
    COLORREF m_crBg;
    UINT m_uDrawPart;
public:
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_COLOR_ATTRIBUTE("crbg", m_crBg, TRUE)
    DUIWIN_INT_ATTRIBUTE("left", m_rcMargin.left, TRUE)
    DUIWIN_INT_ATTRIBUTE("top", m_rcMargin.top, TRUE)
    DUIWIN_INT_ATTRIBUTE("right", m_rcMargin.right, TRUE)
    DUIWIN_INT_ATTRIBUTE("bottom", m_rcMargin.bottom, TRUE)
    DUIWIN_ENUM_ATTRIBUTE("part", UINT, TRUE)
    DUIWIN_ENUM_VALUE("all", Frame_Part_All)
    DUIWIN_ENUM_VALUE("top", (Frame_Part_All & ~Frame_Part_Bottom))
    DUIWIN_ENUM_VALUE("middle", (Frame_Part_All & ~(Frame_Part_Bottom | Frame_Part_Top)))
    DUIWIN_ENUM_VALUE("bottom", (Frame_Part_All & ~Frame_Part_Top))
    DUIWIN_ENUM_VALUE("left", (Frame_Part_All & ~Frame_Part_Right))
    DUIWIN_ENUM_VALUE("center", (Frame_Part_All & ~(Frame_Part_Right | Frame_Part_Left)))
    DUIWIN_ENUM_VALUE("right", (Frame_Part_All & ~Frame_Part_Left))
    DUIWIN_ENUM_END(m_uDrawPart)
    DUIWIN_DECLARE_ATTRIBUTES_END()
};


class DUI_EXP CDuiSkinButton : public CDuiSkinBase
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiSkinButton, "button")
public:
    CDuiSkinButton();

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState,BYTE byAlpha);

    virtual BOOL IgnoreState();

    virtual int GetStates();
protected:
    COLORREF m_crBg;
    COLORREF m_crBorder;
    COLORREF m_crBgUpNormal;
    COLORREF m_crBgUpHover;
    COLORREF m_crBgUpPush;
    COLORREF m_crBgDownNormal;
    COLORREF m_crBgDownHover;
    COLORREF m_crBgDownPush;

public:
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_COLOR_ATTRIBUTE("bg", m_crBg, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("border", m_crBorder, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bgup", m_crBgUpNormal, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bguphover", m_crBgUpHover, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bguppush", m_crBgUpPush, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bgdown", m_crBgDownNormal, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bgdownhover", m_crBgDownHover, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("bgdownpush", m_crBgDownPush, TRUE)
    DUIWIN_DECLARE_ATTRIBUTES_END()
};

class DUI_EXP CDuiSkinGradation  : public CDuiSkinBase
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiSkinGradation, "gradation")
public:
    CDuiSkinGradation();

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState,BYTE byAlpha);

protected:
    COLORREF m_crFrom;
    COLORREF m_crTo;
    UINT     m_uDirection;
    int      m_nSize;
public:
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_COLOR_ATTRIBUTE("from", m_crFrom, TRUE)
    DUIWIN_COLOR_ATTRIBUTE("to", m_crTo, TRUE)
    DUIWIN_INT_ATTRIBUTE("size", m_nSize, TRUE)
    DUIWIN_ENUM_ATTRIBUTE("direction", UINT, TRUE)
    DUIWIN_ENUM_VALUE("horz", 0)
    DUIWIN_ENUM_VALUE("vert", 1)
    DUIWIN_ENUM_END(m_uDirection)
    DUIWIN_DECLARE_ATTRIBUTES_END()
};

#define MAKESBSTATE(sbCode,nState,bVertical) MAKELONG((sbCode),MAKEWORD((nState),(bVertical)))
#define SB_CORNOR		10

#define THUMB_MINSIZE	18

class DUI_EXP CDuiScrollbarSkin : public CDuiSkinImgFrame
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiScrollbarSkin, "scrollbar")

public:

    CDuiScrollbarSkin();

    virtual void Draw(CDCHandle dc, CRect rcDraw, DWORD dwState,BYTE byAlpha);

public:
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_INT_ATTRIBUTE("magin",m_nMagin,FALSE)
    DUIWIN_DECLARE_ATTRIBUTES_END()
protected:
    int			m_nMagin;
};

class DUI_EXP CDuiSkinMenuBorder : public CDuiSkinImgFrame
{
    DUIOBJ_DECLARE_CLASS_NAME(CDuiSkinMenuBorder, "border")

public:

    CDuiSkinMenuBorder():m_rcBorder(1,1,1,1)
    {

    }

    CRect GetMarginRect()
    {
        return m_rcBorder;
    }
public:
    DUIWIN_DECLARE_ATTRIBUTES_BEGIN()
    DUIWIN_RECT_ATTRIBUTE("border",m_rcBorder,FALSE)
    DUIWIN_DECLARE_ATTRIBUTES_END()
protected:
    CRect		m_rcBorder;
};

}//namespace DuiEngine
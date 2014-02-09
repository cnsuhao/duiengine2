// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "ScintillaWnd.h"
#include "ImgView.h"

#include "NewFileDlg.h"
#include "NewSkinDlg.h"

using namespace pugi;

CMainDlg::CMainDlg() : CDuiHostWnd(_T("IDR_DUI_MAIN_DIALOG"))
{
} 

CMainDlg::~CMainDlg()
{
}


HWND CMainDlg::OnRealWndCreate( CDuiRealWnd *pRealWnd )
{
	if(pRealWnd->GetRealWndParam().m_strClassName==_T("scintilla"))
	{
		CScintillaWnd *pWnd=new CScintillaWnd;
		BOOL bOK=pWnd->Create(pRealWnd->GetRealWndParam().m_strWindowName,WS_CHILD,CRect(0,0,0,0),pRealWnd->GetContainer()->GetHostHwnd(),pRealWnd->GetCmdID(),DuiSystem::getSingleton().GetInstance());
		if(!bOK)
		{
			DUIASSERT(FALSE);
			delete pWnd;
			return 0;
		}
		pRealWnd->SetUserData((ULONG_PTR)pWnd);
		return pWnd->m_hWnd;
	}else
	{
		return __super::OnRealWndCreate(pRealWnd);
	}
}

void CMainDlg::OnRealWndDestroy( CDuiRealWnd *pRealWnd )
{
	if(pRealWnd->GetRealWndParam().m_strClassName==_T("scintilla"))
	{
		CScintillaWnd *pWnd=(CScintillaWnd *)pRealWnd->GetUserData();
		if(pWnd)
		{
			pWnd->DestroyWindow();
			delete pWnd;
		}
	}else
	{
		__super::OnRealWndDestroy(pRealWnd);
	}
}
void CMainDlg::OnBtnClick_ProjectOpen()
{
	CFileDialogEx fileDlg(TRUE,_T("xml"),_T("index.xml"),6,_T("xml files(*.xml)\0*.xml\0All files (*.*)\0*.*\0\0"));
	if(IDOK==fileDlg.DoModal())
	{
		OpenProject(fileDlg.m_szFileName);
	}
}

LRESULT CMainDlg::OnListCtrl_File_SelChanged( LPDUINMHDR pnmh )
{
	DUINMLBSELCHANGE *pLCNmhdr=(DUINMLBSELCHANGE*)pnmh;
	if(pLCNmhdr->nNewSel!=-1)
	{
		CDuiListCtrl * pListFile=FindChildByName2<CDuiListCtrl *>("prj_list_file");
		CDuiTabCtrl * pTabView=FindChildByName2<CDuiTabCtrl *>("tab_view");
		TCHAR szBuf[1025]={0};
		DXLVSUBITEM item;
		item.mask=DUI_LVIF_TEXT;
		item.cchTextMax=1024;
		item.strText=szBuf;
		pListFile->GetSubItem(pLCNmhdr->nNewSel,0,&item);
		if(_tcscmp(szBuf,_T("XML"))==0)
		{
			pTabView->SetCurSel(1);
			pListFile->GetSubItem(pLCNmhdr->nNewSel,2,&item);
			CDuiStringT strXmlPath=m_strPrjPath+szBuf;
			CDuiRealWnd *pRealWnd=FindChildByName2<CDuiRealWnd *>("xmleditor");
			CScintillaWnd *pXmlEditor=(CScintillaWnd *)pRealWnd->GetUserData();
			if(pXmlEditor) pXmlEditor->OpenFile(strXmlPath);
		}else
		{
			pListFile->GetSubItem(pLCNmhdr->nNewSel,2,&item);
			CDuiStringT strImgPath=m_strPrjPath+szBuf;
			pTabView->SetCurSel(0);
			CImgView *pImgView=FindChildByName2<CImgView*>("img_view");
			if(pImgView) pImgView->SetImageFile(strImgPath);
		}
	}
	return 0;
}

LRESULT CMainDlg::OnListCtrl_Skin_SelChanged( LPDUINMHDR pnmh )
{
	CDuiListCtrl * pListSkin=FindChildByName2<CDuiListCtrl *>("prj_list_skin");
	DUINMLBSELCHANGE *pLCNmhdr=(DUINMLBSELCHANGE*)pnmh;
	if(pLCNmhdr->nNewSel!=-1 && pListSkin)
	{
		SkinInfo *pSkinInfo=(SkinInfo *)pListSkin->GetItemData(pLCNmhdr->nNewSel);
		if(!pSkinInfo->strSrc.IsEmpty())
		{
			CDuiStringT strImgPath=GetImageSrcFile(pSkinInfo->strSrc);
			if(!strImgPath.IsEmpty())
			{
				CImgView *pImgView=FindChildByName2<CImgView*>("img_view");
				if(pImgView)
				{
					pImgView->SetImageFile(strImgPath);
					pImgView->SetStates(pSkinInfo->nState);
					pImgView->SetTile(pSkinInfo->bTile);
					pImgView->SetMargin(pSkinInfo->rcMargin);
				}
			}
		}
	}
	return 0;
}

void CMainDlg::OnDestroy()
{
	ClearSkinList();
	SetMsgHandled(FALSE);
}

void CMainDlg::ClearSkinList()
{
	CDuiListCtrl * pListSkin=FindChildByName2<CDuiListCtrl *>("prj_list_skin");
	if(pListSkin)
	{
		for(int i=0;i<pListSkin->GetItemCount();i++)
		{
			SkinInfo *pSkinInfo=(SkinInfo*)pListSkin->GetItemData(i);
			delete pSkinInfo;
		}
		pListSkin->DeleteAllItems();
	}
}

CDuiStringT CMainDlg::GetImageSrcFile( const CDuiStringT & strSrcName )
{
	CDuiListCtrl * pListFile=FindChildByName2<CDuiListCtrl *>("prj_list_file");
	if(pListFile)
	{
		TCHAR szBuf[1025]={0};
		DXLVSUBITEM item;
		item.mask=DUI_LVIF_TEXT;
		item.cchTextMax=1024;
		item.strText=szBuf;
		for(int i=0;i<pListFile->GetItemCount();i++)
		{
			pListFile->GetSubItem(i,1,&item);
			if(strSrcName==szBuf)
			{
				pListFile->GetSubItem(i,2,&item);
				return szBuf;
			}
		}
	}
	return _T("");
}

void CMainDlg::OnBtnClick_Project_File_Add()
{
	CNewFileDlg dlg(this);
	if(!m_strPrjPath.IsEmpty() && IDOK==dlg.DoModal())
	{
		xml_document xmlDoc;
		if(!xmlDoc.load_file(m_strPrjIndex)) return;
		//���name�ظ�
		CDuiStringA strName=DUI_CT2A(dlg.m_strResName,CP_UTF8);
		xml_node resnode=xmlDoc.child("resid");
		while(resnode)
		{
			if(strName==resnode.attribute("name").value())
			{
				DuiMessageBox(GetActiveWindow(),_T("ָ������Դ���ظ�"),_T("����"),MB_OK|MB_ICONSTOP);
				return;
			}
			resnode=resnode.next_sibling("resid");
		}

		BuildFilePath(m_strPrjPath,dlg.m_strResPath,FALSE);
		CopyFile(dlg.m_strSrcFile,m_strPrjPath+_T("\\")+dlg.m_strResPath,FALSE);
		//��дXML�ļ�
		resnode=xmlDoc.append_child("resid");
		resnode.append_attribute("type").set_value(DUI_CT2A(dlg.m_strResType,CP_UTF8));
		resnode.append_attribute("name").set_value(DUI_CT2A(dlg.m_strResName,CP_UTF8));
		resnode.append_attribute("file").set_value(DUI_CT2A(dlg.m_strResPath,CP_UTF8));
		if(dlg.m_strResType==_T("XML"))
			resnode.append_attribute("layout").set_value(dlg.m_bLayout?"0":"1");

		FILE *f=_tfopen(m_strPrjIndex,_T("wb"));
		if(f)
		{
			xml_writer_file xmlfile(f);
			xmlDoc.print(xmlfile);
			fclose(f);
			OpenProject(m_strPrjIndex);			
		}

	}
}


BOOL CMainDlg::InitIndexList(LPCTSTR pszIndexFile)
{
	xml_document xmlDoc;
	if(!xmlDoc.load_file(pszIndexFile)) return FALSE;

	xml_node xmlNode=xmlDoc.child("resid");

	CDuiListCtrl * pListFile=FindChildByName2<CDuiListCtrl *>("prj_list_file");

	pListFile->DeleteAllItems();

	m_strInitFile=_T("xml\\init.xml");

	int iItem=0;
	while(xmlNode)
	{
		CDuiStringT strType=DUI_CA2T(xmlNode.attribute("type").value(),CP_UTF8);
		pListFile->InsertItem(iItem,strType);
		pListFile->SetSubItemText(iItem,1,DUI_CA2T(xmlNode.attribute("name").value(),CP_UTF8));
		CDuiStringT strFile=DUI_CA2T(xmlNode.attribute("file").value(),CP_UTF8);
		pListFile->SetSubItemText(iItem,2,strFile);
		if(strType==_T("XML") && xmlNode.attribute("init").as_bool())
		{//��ó�ʼ��XML������skin��Ԫ��
			m_strInitFile=strFile;
		}
		pListFile->SetSubItemText(iItem,3,DUI_CA2T(xmlNode.attribute("layer").value(),CP_UTF8));
		iItem++;
		xmlNode=xmlNode.next_sibling("resid");
	}
	return TRUE;
}

void CMainDlg::InitSkinList()
{
	CDuiListCtrl * pListSkin=FindChildByName2<CDuiListCtrl *>("prj_list_skin");

	ClearSkinList();

	//����skins
	xml_document xmlInit;
	if(xmlInit.load_file(m_strPrjPath+m_strInitFile))
	{
		xml_node xmlNode=xmlInit.first_child().child("skins");
		if(xmlNode)
		{
			int iItem=0;
			xmlNode=xmlNode.first_child();
			while(xmlNode)
			{
				SkinInfo *pSkinInfo=new SkinInfo;
				pSkinInfo->strType=DUI_CA2T(xmlNode.name(),CP_UTF8);
				pSkinInfo->strName=DUI_CA2T(xmlNode.attribute("name").value(),CP_UTF8);
				pSkinInfo->strSrc=DUI_CA2T(xmlNode.attribute("src").value(),CP_UTF8);
				pSkinInfo->bTile=xmlNode.attribute("tile").as_bool(false);
				pSkinInfo->nState=xmlNode.attribute("states").as_int(1);
				if(pSkinInfo->strType==_T("imgframe"))
				{
					pSkinInfo->rcMargin.left=xmlNode.attribute("left").as_int(0);
					pSkinInfo->rcMargin.right=xmlNode.attribute("right").as_int(-1);
					pSkinInfo->rcMargin.top=xmlNode.attribute("top").as_int(0);
					pSkinInfo->rcMargin.bottom=xmlNode.attribute("bottom").as_int(-1);
					if(pSkinInfo->rcMargin.right == -1) pSkinInfo->rcMargin.right=pSkinInfo->rcMargin.left;
					if(pSkinInfo->rcMargin.bottom == -1) pSkinInfo->rcMargin.bottom=pSkinInfo->rcMargin.top;
				}

				pListSkin->InsertItem(iItem,pSkinInfo->strType);
				pListSkin->SetSubItemText(iItem,1,pSkinInfo->strName);
				pListSkin->SetSubItemText(iItem,2,pSkinInfo->strSrc);

				pListSkin->SetItemData(iItem,(DWORD)pSkinInfo);

				iItem++;
				xmlNode=xmlNode.next_sibling();
			}
		}
	}
}

BOOL CMainDlg::OpenProject( LPCTSTR pszIndexXml )
{
	if(!InitIndexList(pszIndexXml)) return FALSE;

	m_strPrjIndex=m_strPrjPath=pszIndexXml;
	int index=m_strPrjPath.ReverseFind(_T('\\'));
	m_strPrjPath=m_strPrjPath.Left(index+1);

	InitSkinList();

	return TRUE;
}

void CMainDlg::OnBtnClick_Project_Skin_Add()
{
	CNewSkinDlg dlg(this);
	if(!m_strInitFile.IsEmpty() && IDOK==dlg.DoModal())
	{

	}
}

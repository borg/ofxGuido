// fontviewerView.h : interface de la classe CfontviewerView
//


#pragma once


class CfontviewerView : public CView
{
protected: // cr�ation � partir de la s�rialisation uniquement
	CfontviewerView();
	DECLARE_DYNCREATE(CfontviewerView)

// Attributs
public:
	HFONT mFont, mGuidoFont;
	CfontviewerDoc* GetDocument() const;

// Op�rations
public:

// Substitutions
public:
	virtual void OnDraw(CDC* pDC);  // substitu� pour dessiner cette vue
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Impl�mentation
public:
	virtual ~CfontviewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions g�n�r�es de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // version de d�bogage dans fontviewerView.cpp
inline CfontviewerDoc* CfontviewerView::GetDocument() const
   { return reinterpret_cast<CfontviewerDoc*>(m_pDocument); }
#endif


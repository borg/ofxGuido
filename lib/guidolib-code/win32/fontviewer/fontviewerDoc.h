// fontviewerDoc.h : interface de la classe CfontviewerDoc
//


#pragma once


class CfontviewerDoc : public CDocument
{
protected: // cr�ation � partir de la s�rialisation uniquement
	CfontviewerDoc();
	DECLARE_DYNCREATE(CfontviewerDoc)

// Attributs
public:

// Op�rations
public:

// Substitutions
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Impl�mentation
public:
	virtual ~CfontviewerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Fonctions g�n�r�es de la table des messages
protected:
	DECLARE_MESSAGE_MAP()
};



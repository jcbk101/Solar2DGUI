//---------------------------------------------------------------------------

#ifndef CodeViewH
#define CodeViewH

#include <System.Classes.hpp>

#include "CustomControls\TRectangleEx.h"
#include "CustomControls\TTextEx.h"
#include "TexAtlas\TexAtlas.h"
#include "UIAnimation.h"


class UICodeView
{
private:
	void __fastcall generateBaseCode();
	void __fastcall UICodeView::generateCleanUpCode();

	void __fastcall writeNewRect(TRectangleEx *window, AnsiString groupName, AnsiString indent);
	void __fastcall writeNewImageRect(TRectangleEx *window, AnsiString groupName, AnsiString indent);
	void __fastcall writeNewImageSheetRect(TRectangleEx *window, AnsiString groupName, AnsiString sheetName, int sheetIndex, AnsiString indent);
	void __fastcall writeNewText(TTextEx *text, AnsiString groupName, AnsiString indent);
	void __fastcall writeAnchors(TObject *Control, AnsiString indent);
	void __fastcall writeAnimation(TControl *Control, AnsiString target, AnsiString indent);



public:
	__fastcall UICodeView()
	{
		imgSheet = new TStringList();
		objs = new TStringList();
		groups = new TStringList();
	}


	~UICodeView()
	{
		delete imgSheet;
		delete objs;
		delete groups;
	}

	//The [Quote] compiled code [/Quote]
	bool       changesMade;
	bool       IsDisplayed;
	bool       variableType;
	AnsiString codeString;
	AnsiString baseCodeStart;
	AnsiString baseCodeEnd;
	AnsiString baseCleanCode;
	AnsiString imageSheet;
	AnsiString projName;

	TStringList *imgSheet;
	TStringList *objs;
	TStringList *groups;


	AnsiString __fastcall buildSourceCode(TRectangle *Parent, AnsiString projectName, bool useTableType);
	void __fastcall buildWindowCode(TRectangleEx *window, AnsiString groupName, AnsiString indent);
	void __fastcall buildTextCode(TTextEx *text, AnsiString indent);
};

#endif

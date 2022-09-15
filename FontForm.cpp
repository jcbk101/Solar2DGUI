//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "FontForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfontSelect* fontSelect;


//---------------------------------------------------------------------------
__fastcall TfontSelect::TfontSelect(TComponent* Owner, TFont* font)
	: TForm(Owner)
{
	if (font)
	{
		//memset(font, 0, sizeof(TFont));

		Font = font;
	}
}


void __fastcall TfontSelect::FormCreate(TObject* Sender)
{
	TStringList* list = new TStringList();

	if (list)
	{
		list->Sorted = true;
		list->Duplicates = System::Types::TDuplicates::dupIgnore;

		CollectFonts(list);
		fontFamilyList->Clear();

		//
		for (int i = 0; i < list->Count; i++)
		{
			fontFamilyList->Items->Add(list->Strings[ i ]);
		}

		delete list;
	}

}


void __fastcall TfontSelect::okButtonClick(TObject* Sender)
{
	Font->Family = sample->Font->Family;
	Font->Size = sample->Font->Size;
	Font->Style = sample->Font->Style;
}


void __fastcall TfontSelect::underlineChange(TObject* Sender)
{
	TCheckBox* cb = dynamic_cast< TCheckBox * >(Sender);

	if (cb)
	{
		if (cb->IsChecked)
		{
			if (UpperCase(cb->Name) == "UNDERLINE")
			{
				TFontStyles AStyle(4);
				sample->Font->Style = sample->Font->Style + AStyle;
			}
			else
			{
				TFontStyles AStyle(8);
				sample->Font->Style = sample->Font->Style + AStyle;
			}
		}
		else
		{
			if (UpperCase(cb->Name) == "UNDERLINE")
			{
				TFontStyles AStyle(4);
				sample->Font->Style = sample->Font->Style - AStyle;
			}
			else
			{
				TFontStyles AStyle(8);
				sample->Font->Style = sample->Font->Style - AStyle;
			}
		}
	}
}


void __fastcall TfontSelect::familyNameKeyUp(TObject* Sender, WORD& Key, System::WideChar& KeyChar, TShiftState Shift)
{
	if (KeyChar)
	{
		for (int i = 0;i < fontFamilyList->Items->Count; i++)
		{
			//fontFamilyList->Items->Strings[0]
			if (familyName->Text.Length() <= fontFamilyList->Items->Strings[ i ].Length())
			{
				AnsiString s = AnsiString(fontFamilyList->Items->Strings[ i ]).SubString(0,familyName->Text.Length());
				//if (UpperCase(familyName->Text) == UpperCase(fontFamilyList->Items->Strings[ i ]))
				if (UpperCase(familyName->Text) == UpperCase(s))
				{
					fontFamilyList->ItemIndex = i;
					break;
				}
			}
		}
	}
}


void __fastcall TfontSelect::fontSizeKeyUp(TObject* Sender, WORD& Key, System::WideChar& KeyChar, TShiftState Shift)
{
	if (KeyChar)
	{
		for (int i = 0; i < fontSizeList->Items->Count; i++)
		{
			AnsiString s = fontSizeList->Items->Strings[ i ];
			//if (UpperCase(familyName->Text) == UpperCase(fontFamilyList->Items->Strings[ i ]))
			if (fontSize->Text == s)
			{
				fontSizeList->ItemIndex = i;
				break;
			}
		}
	}
}


#ifdef _Windows


int __stdcall EnumFontsProc(const TLogFont* LogFont, const TTextMetric* TextMetric, unsigned long FontType, LPARAM lparam)
{

	TStringList* S = (TStringList *)lparam;
	AnsiString temp;

	temp = LogFont->lfFaceName;
	char c = temp.c_str()[ 0 ];

	if ((S->Count == 0) || (AnsiCompareText(AnsiString(S->Strings[ S->Count - 1 ]), temp) != 0) && c != '@')
	{
		S->Add(temp);
		//return 1;
	}

	return 1;
}
#endif


void __fastcall TfontSelect::CollectFonts(TStringList* FontList)
{

#ifdef __APPLE__
	NsFontManager fManager;
	NSArray list;
	NSString lItem;
#else
	//#ifdef MSWINDOWS
	HDC DC;
	TLogFont* LFont = new TLogFont();
#endif

#ifdef __APPLE__
	fManager = TNsFontManager.Wrap(TNsFontManager.OCClass.sharedFontManager);
	list = fManager.availableFontFamilies;

	if ((list != NULL) && (list.count > 0))
	{
		begin
		for (int i = 0; i < to list.Count - 1; i++)
		{
			lItem = TNSString.Wrap(list.objectAtIndex(i));
			FontList->Add(String(lItem.UTF8String))
		}
	}
#else
	//#ifdef MSWINDOWS
	DC = GetDC(0);
	memset(LFont, 0, sizeof(TLogFont));

	//LFont->lfCharSet = DEFAULT_CHARSET;
	LFont->lfCharSet = ANSI_CHARSET;
	EnumFontFamiliesEx(DC, LFont, & EnumFontsProc, LPARAM(FontList), 0);
	ReleaseDC(0, DC);
#endif
}

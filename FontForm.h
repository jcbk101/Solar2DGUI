//---------------------------------------------------------------------------

#ifndef FontFormH
#define FontFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Edit.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.NumberBox.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <FMX.SearchBox.hpp>

#ifdef __APPLE__

#include <MacApi.Apikit.hpp>
#include <MacApi.Corefoundation.hpp>
#include <MacApi.Foundation.hpp>

#else

#include <Winapi.Messages.hpp>
#include <Winapi.Windows.hpp>

#endif


//---------------------------------------------------------------------------
class TfontSelect : public TForm
{
__published: //IDE-managed Components
	TBindingsList*              BindingsList1;
	TButton*                    cancelButton;
	TButton*                    okButton;
	TLabel*                     sizeLabel;
	TGroupBox*                  GroupBox2;
	TLabel*                     sample;
	TGroupBox*                  GroupBox1;
	TCheckBox*                  underline;
	TCheckBox*                  strikeout;
	TListBox*                   fontSizeList;
	TNumberBox*                 fontSize;
	TListBox*                   fontFamilyList;
	TEdit*                      familyName;
	TLabel*                     familyLabel;
	TLinkFillControlToProperty* LinkFillControlToPropertyTextPrompt;
	TLinkFillControlToProperty* LinkFillControlToPropertyFontFamily;
	TLinkControlToProperty*     LinkControlToPropertyFontSize;
	TLinkFillControlToProperty* LinkFillControlToPropertyText2;

	void __fastcall underlineChange(TObject* Sender);
	void __fastcall FormCreate(TObject* Sender);
	void __fastcall familyNameKeyUp(TObject* Sender, WORD& Key, System::WideChar& KeyChar, TShiftState Shift);
	void __fastcall fontSizeKeyUp(TObject* Sender, WORD& Key, System::WideChar& KeyChar, TShiftState Shift);
	void __fastcall okButtonClick(TObject* Sender);

private: //User declarations
public:  //User declarations
	__fastcall TfontSelect(TComponent* Owner, TFont* font);

	//int __fastcall EnumFontsProc(TLogFont* LogFont, TTextMetric* TextMetric, long FontType, LPARAM lparam);
	void __fastcall CollectFonts(TStringList* FontList);

	TFont* Font;
};

int __stdcall EnumFontsProc(const TLogFont* LogFont, const TTextMetric* TextMetric, unsigned long FontType, LPARAM lparam);

//---------------------------------------------------------------------------
extern PACKAGE TfontSelect* fontSelect;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef MessageBoxH
#define MessageBoxH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>


//---------------------------------------------------------------------------
class TmsgForm : public TForm
{
__published: //IDE-managed Components
	TCornerButton* Button1;
	TCornerButton* Button2;
	TCornerButton* Button3;
	TLabel*        messageText;
private: //User declarations
public:  //User declarations
	__fastcall TmsgForm(TComponent* Owner);
};


//---------------------------------------------------------------------------
extern PACKAGE TmsgForm* msgForm;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef TransitionSelectH
#define TransitionSelectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.ListBox.hpp>


//---------------------------------------------------------------------------
class TtransForm : public TForm
{
__published: //IDE-managed Components
	TFloatAnimation*  FloatAnimation1;
	TGridPanelLayout* GridPanelLayout1;
	TCornerButton*    CornerButton1;
	TCornerButton*    CornerButton2;
	TLabel*           Label1;
	TComboBox*        easeBox;
	TLabel*           Label2;
	TComboBox*        interpolationBox;

	void __fastcall CornerButton2Click(TObject* Sender);

private: //User declarations
public:  //User declarations
	__fastcall TtransForm(TComponent* Owner, int* easeIndex, int* type);

	int* easeIndex;
	int* type;
};


class UIAnimation
{
public:
	//AnsiString Name;
	int        properties; //Using BITS as flags to index into combo box
	long       time;
	long       delay;
	long       iterations;
	int        easeType;
	int        interpolation;
	AnsiString tag;
	bool       delta;
	bool       looping;
};

//---------------------------------------------------------------------------
extern PACKAGE TtransForm* transForm;
//---------------------------------------------------------------------------
#endif

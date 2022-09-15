//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TransitionSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TtransForm* transForm;


//---------------------------------------------------------------------------
__fastcall TtransForm::TtransForm(TComponent* Owner, int* easeIndex, int* type)
	: TForm(Owner)
{
	this->easeIndex = easeIndex;
	this->type = type;
}


void __fastcall TtransForm::CornerButton2Click(TObject* Sender)
{
	*this->easeIndex = easeBox->ItemIndex;
	*this->type = interpolationBox->ItemIndex;
}

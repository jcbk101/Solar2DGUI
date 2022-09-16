//---------------------------------------------------------------------------

#ifndef ZOrderH
#define ZOrderH

#include <fmx.h>

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <System.Rtti.hpp>
#include <System.UITypes.hpp>


class UIObject
{
public:
	//AnsiString Name;
	TMemo*     animation;
	int        zPosition;
	int        alignTextIndex;
	bool       bottomParentChk;
	bool       horiParentChk;
	bool       leftParentChk;
	bool       rightParentChk;
	bool       topParentChk;
	bool       vertParentChk;
	bool       anchorLeftChk;
	bool       anchorTopChk;
	bool       anchorRightChk;
	bool       anchorBottomChk;
	bool       Visible;
	AnsiString aboveCombo;
	AnsiString bottomWithCombo;
	AnsiString horiWithCombo;
	AnsiString leftWithCombo;
	AnsiString rightWithCombo;
	AnsiString topWithCombo;
	AnsiString vertWithCombo;
	AnsiString belowCombo;
	AnsiString leftOfCombo;
	AnsiString rightOfCombo;
};


void calcControlZOrder(TObject* Parent);
int GetControlZOrder(TObject* aControl);
bool SetControlZOrder(TObject* aControl, int aNewZOrder);

#endif

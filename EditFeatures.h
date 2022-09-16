#ifndef EditFeaturesH
#define EditFeaturesH

//#include <System.Classes.hpp>
//#include <FMX.Controls.hpp>
//#include <FMX.Forms.hpp>
//#include <FMX.Objects.hpp>
//#include <System.UITypes.hpp>

#include <fmx.h>
#include "ZOrder.h"


class TEditFeature: public TList
{
private:
	void __fastcall CutChangeParentInfo(TControl* Parent, TControl* dest, AnsiString type);
	void __fastcall CopyChangeParentInfo(TControl* Parent, TControl* dest, TControl* src, AnsiString type);

public:
	//__fastcall TEditFeature()
	//{
	//};
	void __fastcall editCut(TControl* c);
	void __fastcall editCopy(TControl* c);
	void __fastcall editPaste(TControl* c);

	bool IsCut;
	bool wasPasted;
};


#endif

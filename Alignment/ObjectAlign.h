//---------------------------------------------------------------------------

#ifndef ObjectAlignH
#define ObjectAlignH

#include <fmx.h>

#include "LUADisplay\LUADisplay.h"
#include "AboutForm.h"
#include "MessageBox.h"
#include "FontForm.h"
#include "EditFeatures.h"
#include "CustomControls\TRectangleEx.h"
#include "CustomControls\TTextEx.h"


void __fastcall deleteSelectedObjects(TControl* DeviceParent, TStringList* List);
void __fastcall spaceVertInWindow(TControl* Parent, TList* temp);
void __fastcall spaceHoriInWindow(TControl* Parent, TList* temp);
void __fastcall spaceVertWithObjects(TControl* Parent, TList* temp);
void __fastcall spaceHoriWithObjects(TControl* Parent, TList* temp);
void __fastcall alignXxOfObjects(TControl* Parent, TList* temp, int sideOf);
void __fastcall getAllObjectOnParent(TFmxObject* Parent, TList* list);

bool __fastcall formatSelectedObjects(TControl* DeviceParent, TObject* Sender, TStringList* List);
int __fastcall listCompareX(void* Item1, void* Item2);
int __fastcall listCompareY(void* Item1, void* Item2);
TControl* __fastcall findObjectByName(TObject* parent, AnsiString Name);
TStringList* __fastcall getObjectsList(TFmxObject* Sender);


#endif

#ifndef DataProcessingH
#define DataProcessingH

#include <fmx.h>
#include <System.Classes.hpp>
#include <System.JSON.Writers.hpp>
#include <System.JSON.Readers.hpp>
#include <System.JSON.Types.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <FMX.Colors.hpp>

#include "CustomControls\TRectangleEx.h"
#include "CustomControls\TTextEx.h"
//#include "CustomControls\TControlEx.h"
#include "ZOrder.h"
#include "UIAnimation.h"


void __fastcall processJsonWrite(TControl* DeviceParent, TJsonTextWriter* writer, AnsiString ParentName);
void __fastcall jsonWriteWindowObject(TControl* Control, TJsonTextWriter* writer);
void __fastcall jsonWriteTextObject(TControl* Control, TJsonTextWriter* writer);
void __fastcall jsonWriteUIObjectData(TControl* Control, TJsonTextWriter* writer);
void __fastcall jsonWriteObjectEvent(TControl* Control, TJsonTextWriter* writer);
void __fastcall jsonWriteObjectAnimation(TControl* Control, TJsonTextWriter* writer);

void __fastcall processJsonRead(TControl* DeviceParent, TJsonTextReader* reader);
void __fastcall jsonProcessArray(TControl* control, TJsonTextReader* reader);
void __fastcall jsonProcessObject(TControl* control, TJsonTextReader* reader);
//void __fastcall jsonProcessAnimation(TControl* Control, TJsonTextReader* reader);

void __fastcall jsonReadWindowObject(TControl* Control, TJsonTextReader* reader);
void __fastcall jsonReadTextObject(TControl* Control, TJsonTextReader* reader);
void __fastcall jsonReadUIObjectData(TControl* Control, TJsonTextReader* reader);
void __fastcall jsonReadObjectEvent(TControl* Control, TJsonTextReader* reader);
void __fastcall jsonReadObjectAnimation(TControl* Control, TJsonTextReader* reader);

#endif

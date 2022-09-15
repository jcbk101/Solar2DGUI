//---------------------------------------------------------------------------

//This software is Copyright (c) 2015 Embarcadero Technologies, Inc.
//You may only use this software if you are an authorized licensee
//of an Embarcadero developer tools product.
//This software is considered a Redistributable as defined under
//the software license agreement that comes with the Embarcadero Products
//and is subject to that software license agreement.

//---------------------------------------------------------------------------

#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
//---------------------------------------------------------------------------
USEFORM("FontForm.cpp", Form1);
USEFORM("MessageBox.cpp", msgForm);
USEFORM("ControlsForm.cpp", solar2DForm);
USEFORM("AboutForm.cpp", frmAbout);
USEFORM("TexAtlas\TexAtlas.cpp", texAtlasForm);


//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(Tsolar2DForm), & solar2DForm);
//		Application->CreateForm(__classid(TtexAtlasForm), & texAtlasForm);
		//Application->RegisterFormFamily(L"TForm3", OPENARRAY(TComponentClass,(__classid(Tsolar2DForm))));
		//Application->RegisterFormFamily(L"TForm2", OPENARRAY(TComponentClass,(__classid(Tsolar2DForm), __classid(TmsgForm))));
		//Application->RegisterFormFamily(L"TForm", OPENARRAY(TComponentClass,(__classid(Tsolar2DForm), __classid(TfrmAbout))));
		Application->Run();
	}
	catch (Exception& exception)
	{
		Application->ShowException(& exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception& exception)
		{
			Application->ShowException(& exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

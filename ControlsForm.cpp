//---------------------------------------------------------------------------

//This software is Copyright (c) 2015 Embarcadero Technologies, Inc.
//You may only use this software if you are an authorized licensee
//of an Embarcadero developer tools product.
//This software is considered a Redistributable as defined under
//the software license agreement that comes with the Embarcadero Products
//and is subject to that software license agreement.

//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "ControlsForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource ("*.Windows.fmx", _PLAT_MSWINDOWS)


int gripSize = 8;
int halfGrip = gripSize / 1;
int qtrGrip  = gripSize / 4;

//Variable to hold old data
float LW, LH;

bool inSizeMode = false;
bool onLeft     = false;
bool onRight    = false;
bool onTop      = false;
bool onBottom   = false;

//Helper items
TStringList * selectionsList = new TStringList();
TEditFeature *editList       = new TEditFeature();
UICodeView *  codeView       = new UICodeView();

//UNDO/REDO states
TStringList *saveStateString  = new TStringList();
bool         saveStateChanged = false;

//Current screen display
LUADisplay *disp = new LUADisplay();

//List of all running animations
bool   isAnimating = false;
TList *animations  = new TList();

//Main form
Tsolar2DForm *solar2DForm;


//---------------------------------------------------------------------------
__fastcall Tsolar2DForm::Tsolar2DForm(TComponent *Owner)
	: TForm(Owner)
{
	TMetaClass *mc;

	mc = __classid(TRectangleEx);
	RegisterClasses(&mc, 0);

	mc = __classid(TTextEx);
	RegisterClasses(&mc, 0);

	//mc = __classid(TControlEx);
	//RegisterClasses(&mc, 0);

	//These functions reside in 'LUADisplay\LUADisplay.h &.cpp'
	initFunctionCalls();

	//Screen orientation
	screenOrientation = 90;

	exePath = GetCurrentDir();

	//File modified flag
	isModified = false;

	//Initialobject data updates allowed
	allowObjectUpdates = true;

	//Set a default screen display: iPhone 4s
	//(LUADisplay *)(*luaDisplay[ 38 ])(disp);
}


//--------------------------------------------------------
//
//Main form destruction
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::FormDestroy(TObject *Sender)
{
	delete selectionsList;
	delete editList;
	delete deviceScreen;
	delete codeView;
	delete saveStateString;

	if (disp)
	{
		clearDisplayValues(disp);
		delete disp;
	}

}


//--------------------------------------------------------
//
//Main form creation
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::FormCreate(TObject *Sender)
{
	AnsiString Name;

	//Clear in case of design time placement deviceScreen->ChildrenCount
	controlTree->Clear();

	TRectangle *array[] =
	{
		TL, TC, TR,
		L,R,
		BL,BC, BR
	};

	for (int i = 0; i < 8; i++)
	{
		array[i]->Height = gripSize;
		array[i]->Width = gripSize;
	}

	//Screen display
	initBitmapData(disp);

	//Read in settings setting
	if (FileExists(exePath + "\\settings.ini"))
	{
		reloadSettingsClick(NULL);
	}
	else
	{
		screenResolutionClick(display38);
	}

	//Create new window / Text for designing
	TRectangleEx *ARect = (TRectangleEx *)createNewControl(deviceScreen, "Rect");
	TText *AText = (TText *)createNewControl(deviceScreen, "Text");

	ARect->Position->X = (deviceScreen->Width / 2) - (ARect->Width / 2);
	ARect->Position->Y = (deviceScreen->Height / 2) - (ARect->Height / 2);

	AText->AutoSize = true;
	AText->WordWrap = false;
	AText->Text = "First text object";
	AText->WordWrap = true;
	AText->AutoSize = false;

	AText->Position->X = ARect->Position->X + ((ARect->Width / 2) - (AText->Width / 2));
	AText->Position->Y = (ARect->Position->Y - (AText->Height * 2));

	//Get the initial Z order for all USABLE objects
	calcControlZOrder(deviceScreen);
	//Build sorted by Z Order tree view
	updateTreeView();

	//Hide helper items
	colorArray->Visible = false;

	//Non Visual helper items
	selectionsList->Clear();
	//selectionsList->Duplicates = System::Types::TDuplicates::dupIgnore;
	//selectionsList->Sorted = true;
	//
	editList->Clear();

	//
	adjustHandles(false, NULL);
	updateInspector();
	inSizeMode = false;

	// //Screen display
	//initBitmapData(disp);
	//
	// //Read in settings setting
	//if (FileExists(exePath + "\\settings.ini"))
	//{
	//reloadSettingsClick(NULL);
	//}
	//else
	//{
	//screenResolutionClick(display38);
	//}

	//Code View triggers
	codeView->IsDisplayed = false;
	codeView->changesMade = false;

	//First UNDO update
	//saveStateString->Duplicates = System::Types::TDuplicates::dupIgnore;
	//saveStateString->Sorted = true;
	//updateUndoStack("", false);
}


//--------------------------------------------------------
//
//Save the INI file with App data tobe reloaded
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	//Save current desktop settings
	saveSettingsClick(NULL);

	//Make sure nothing needs saving
	if (isModified)
	{
		TmsgForm *msg = new TmsgForm(Application);
		msg->messageText->Text = "Project not saved. Do you wish to save your changes?";
		msg->Caption = "Save current project...";
		msg->ShowModal();

		//What result did we receive?
		if (msg->ModalResult == mrYes)
		{
			//Attempt to save the project
			saveProjClick(NULL);
		}

		msgForm->DisposeOf();
	}
}


//--------------------------------------------------------
//
//Undo / Redo stack
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::updateUndoStack(String state, bool isAnimation)
{
	if (isAnimating)
		return;

	if (state == "")
	{
		TJsonTextWriter *writer       = NULL;
		TStringWriter *  stringWriter = new TStringWriter();

		writer = new TJsonTextWriter(stringWriter);

		//Attempt to save the project data to a file
		if (writer && deviceScreen)
		{
			//NO Formatting! Easier / Faster to process
			//writer->Formatting = TJsonFormatting::Indented;
			//writer->Indentation = 1;
			//writer->IndentChar = '\t';

			//Open it
			writer->WriteStartObject();
			//Write out all the Objects on the 'deviceScreen'
			processJsonWrite(deviceScreen, writer, "");
			//Close it
			writer->WriteEndObject();

			//Running an animation
			if (isAnimation)
			{
				runAnimation->TagString = stringWriter->ToString();
			}
			else
			{
				if (saveStateString->Count == MAXSAVESTATES)
				{
					//Shift and erase some of the older changes. Sorry, user.
					saveStateString->Delete(0);
				}

				//Save the string. Last string in CANNOT match new string in
				if (!saveStateString->Count || (saveStateString->Count && AnsiString(saveStateString->Strings[saveStateString->Count - 1]).AnsiCompareIC(stringWriter->ToString())))
				{
					saveStateString->Add(stringWriter->ToString());
				}
			}

			editUndo->Text = Format("Undo [ %02d / %02d ]", ARRAYOFCONST((saveStateString->Count, MAXSAVESTATES)));
			//
			delete writer;
			delete stringWriter;
		}
	}
	else
	{
		if ((saveStateString->Count + 1) == MAXSAVESTATES)
		{
			//Shift and erase some of the older changes. Sorry, user.
			saveStateString->Delete(0);
		}

		//Save the string
		saveStateString->Add(state);
	}

	//Update inspector and menu items
	updateInspector();
}


//--------------------------------------------------------
//
//Undo / Redo
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::editUndoClick(TObject *Sender)
{
	//Easiest way to handle this is to save the current state
	//of the project as if it were saving the project itself
	//Do so as a stream instead of a file
	if (isAnimating)
		return;

	TMenuItem *m = dynamic_cast<TMenuItem *>(Sender);

	//Menu item check
	if (m || (Sender == NULL && runAnimation->TagString != ""))
	{
		TStringStream *  string = NULL;
		TStreamReader *  stream = NULL; // = new TStreamReader(FOpen->FileName);
		TJsonTextReader *reader = NULL; // = new TJsonTextReader(stream);

		//Restore after an animation
		if (Sender == NULL && runAnimation->TagString != "")
		{
			//Load the last state
			string = new TStringStream(runAnimation->TagString);
			stream = new TStreamReader(string);
			reader = new TJsonTextReader(stream);

			//
			runAnimation->TagString = "";
		}
		else if (Sender && m->Tag == 0 && saveStateString->Count)
		{
			//Load the last state
			string = new TStringStream(saveStateString->Strings[saveStateString->Count - 1]);
			//string->SaveToFile("dummy.txt");

			//Pull from stack
			saveStateString->Delete(saveStateString->Count - 1);

			stream = new TStreamReader(string);
			reader = new TJsonTextReader(stream);
			//saveStateCount--;
		}

		//else if (m->Tag == 1 && (saveStateCount + 1) < MAXSAVESTATES && saveStateString[saveStateCount + 1] != "")
		//{
		// //Redo using the latest change
		//saveStateCount++;
		//stream = new TStreamReader(saveStateString[saveStateCount]);
		//reader = new TJsonTextReader(stream);
		//}

		//Process the Undo / Redo request
		if (reader && stream && deviceScreen)
		{
			//Prevent Errors from selected items that will be deleted
			selectionRect->TagObject = NULL;
			clearObjectSelections();

			//Kill all children and BG image
			deviceScreen->TagString = "";
			deviceScreen->DeleteChildren();
			deviceScreen->Fill->Bitmap->Bitmap = NULL;
			deviceScreen->Fill->Color = TAlphaColorRec::Black;
			deviceScreen->Fill->Kind = TBrushKind::Solid;

			//Read in all the Objects to the 'deviceScreen'
			processJsonRead(deviceScreen, reader);

			delete reader;
			delete stream;
			delete string;

			//project modified
			isModified = true;
			codeView->changesMade = true;
			codeView->IsDisplayed = false;
			codeMemo->Lines->Clear();
			codeTabItem->Text = "Code View [Changes not saved]";
			codeMemo->FontColor = claRed;
			//Italic
			TFontStyles AStyle(2);
			codeMemo->Font->Style = codeMemo->Font->Style + AStyle;

			//If the tab is visible, show the new data
			if (mainTab->ActiveTab == codeTabItem)
			{
				codeTabItemClick(NULL);
			}

			//Update all data
			updateTreeView();
			updateInspector();
			alignObjects();

			//A little screen clean up
			clearObjectSelections();
			clearAllCursors(deviceScreen);
			//Align all objects on the device screen
			setAllAlignments(deviceScreen);

			//
			selectionRect->TagObject = NULL;
			adjustHandles(false, NULL);

			//Set the background image if it exists
			if (deviceScreen->TagString != "" && FileExists(deviceScreen->TagString))
			{
				deviceScreen->Fill->Bitmap->Bitmap->LoadFromFile(deviceScreen->TagString);
				deviceScreen->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
				deviceScreen->Fill->Kind = TBrushKind::Bitmap;
			}

			//Clear animation list per selection. Crashes App some times
			animListBox->Clear();
			//
			deviceScreen->Repaint();
		}

		//
		editUndo->Text = Format("Undo [ %02d / %02d ]", ARRAYOFCONST((saveStateString->Count, MAXSAVESTATES)));
	}
}


//--------------------------------------------------------
//
//Special case keypresses on the form
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift)
{
	//Delete objects
	if (Key == VK_DELETE)
	{
		if (selectionRect->TagObject || selectionsList->Count)
		{
			TSpeedButton *sb = new TSpeedButton(solar2DForm);
			sb->Visible = false;

			//Simulate clicking 'Delete'
			if (sb)
			{
				sb->Tag = 6;
				objectActionClick(sb);

				delete sb;
				return;
			}
		}
	}

	//Cancel actions
	if (Key == VK_ESCAPE)
	{
		if (isAnimating)
		{
			stopAnimationClick(NULL);
		}
		else if (selectionsList->Count)
		{
			//Release the object
			selectionRect->TagObject = NULL;
			adjustHandles(false, NULL);
			updateInspector();
			updateObjectData();
			clearAllCursors(deviceScreen);
			clearObjectSelections();
			deviceScreen->Repaint();
		}
		else if (selectionRect->TagObject)
		{
			TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);

			if (c)
			{
				if (c->ClassType() == __classid(TRectangleEx))
				{
					((TRectangleEx *)c)->AutoCapture = false;
					((TRectangleEx *)c)->Position->X = ((TRectangleEx *)c)->oldX;
					((TRectangleEx *)c)->Position->Y = ((TRectangleEx *)c)->oldY;
					((TRectangleEx *)c)->Width = ((TRectangleEx *)c)->oldW;
					((TRectangleEx *)c)->Height = ((TRectangleEx *)c)->oldH;
				}
				else
				{
					((TTextEx *)c)->AutoCapture = false;
					((TTextEx *)c)->Position->X = ((TTextEx *)c)->oldX;
					((TTextEx *)c)->Position->Y = ((TTextEx *)c)->oldY;
					((TTextEx *)c)->Width = ((TTextEx *)c)->oldW;
					((TTextEx *)c)->Height = ((TTextEx *)c)->oldH;
				}

				//Release the object
				selectionRect->TagObject = NULL;
				adjustHandles(false, NULL);
				updateInspector();
				updateObjectData();
				clearAllCursors(deviceScreen);
				clearObjectSelections();
				deviceScreen->Repaint();
			}
		}
	}
}


void __fastcall Tsolar2DForm::saveSettingsClick(TObject *Sender)
{
	if (isAnimating)
		return;

	TCustomIniFile *ini = new TIniFile(exePath + "\\settings.ini");

	if (ini)
	{
		ini->WriteInteger("Main Window", "X", Left);
		ini->WriteInteger("Main Window", "Y", Top);
		ini->WriteInteger("Main Window", "Width", Width);
		ini->WriteInteger("Main Window", "Height", Height);
		ini->WriteInteger("Main Window", "WindowState", WindowState);

		//Device Screen
		if (deviceScreen->TagString != "" && FileExists(deviceScreen->TagString))
		{
			ini->WriteString("DeviceScreen", "BG", deviceScreen->TagString);
		}

		//Device Frame
		ini->WriteFloat("DeviceFrame", "X", deviceFrame->Position->X);
		ini->WriteFloat("DeviceFrame", "Y", deviceFrame->Position->Y);
		ini->WriteFloat("DeviceFrame", "Scale", ScaleTrack->Value);
		ini->WriteBool("DeviceFrame", "Borders", borderToggle->IsChecked);
		ini->WriteBool("DeviceFrame", "CrossHair", crossHair->IsChecked);

		ini->WriteInteger("DeviceFrame", "Device", disp->selectedDisplay);
		ini->WriteInteger("DeviceFrame", "Orientation", screenOrientation);

		ini->WriteInteger("TabControl", "MainActiveTab", mainTab->TabIndex);
		ini->WriteInteger("TabControl", "SubActiveTab", inspectorTab->TabIndex);

		//Always clean up after yourself
		delete ini;
	}
}


void __fastcall Tsolar2DForm::reloadSettingsClick(TObject *Sender)
{
	if (isAnimating)
		return;

	TCustomIniFile *ini = new TIniFile(exePath + "\\settings.ini");

	if (ini)
	{
		//Main window
		Left = ini->ReadInteger("Main Window", "X", 0);
		Top = ini->ReadInteger("Main Window", "Y", 0);
		Width = ini->ReadInteger("Main Window", "Width", 800);
		Height = ini->ReadInteger("Main Window", "Height",600);
		WindowState = ini->ReadInteger("Main Window", "WindowState", TWindowState::wsNormal);

		//
		if (WindowState == TWindowState::wsMaximized)
		{
			Height = 740;
			Width = 1080;
			Position = TFormPosition::ScreenCenter;
		}

		//Device Screen
		deviceScreen->TagString = ini->ReadString("DeviceScreen", "BG", "");

		if (deviceScreen->TagString != "" && FileExists(deviceScreen->TagString))
		{
			deviceScreen->Fill->Bitmap->Bitmap->LoadFromFile(deviceScreen->TagString);
			deviceScreen->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
			deviceScreen->Fill->Kind = TBrushKind::Bitmap;

			//Depending on the popup location, some objects may
			//retain a resize cursor by accident. Need to clear that up.
			clearAllCursors(deviceScreen);
		}

		//Device Frame
		deviceFrame->Position->X = ini->ReadFloat("DeviceFrame", "X", 0);
		deviceFrame->Position->Y = ini->ReadFloat("DeviceFrame", "Y", 0);
		ScaleTrack->Value = ini->ReadFloat("DeviceFrame", "Scale", 1);
		borderToggle->IsChecked = ini->ReadBool("DeviceFrame", "Borders", false);
		crossHair->IsChecked = ini->ReadBool("DeviceFrame", "CrossHair", false);

		int device = ini->ReadInteger("DeviceFrame", "Device", 38);
		screenOrientation = ini->ReadInteger("DeviceFrame", "Orientation", 90);

		int activeTab = ini->ReadInteger("TabControl", "MainActiveTab", 0);
		mainTab->TabIndex = activeTab;
		activeTab = ini->ReadInteger("TabControl", "SubActiveTab", 0);
		inspectorTab->TabIndex = activeTab;

		//Manually check the right box
		//There are two separate groups of Radio Menu items
		//Which ever group is active, the other 'complete' group
		//needs to be deactivated
		TObject *chk[2] =
		{
			borderlessSubMenu->Children->Items[0],
			viewMenu->Children->Items[0]
		};

		for (int c = 0; c < 2; c++)
		{
			TControl *parent = dynamic_cast<TControl *>(chk[c]);

			if (parent)
			{
				for (int i = 0; i < parent->ChildrenCount; i++)
				{
					TMenuItem *c = dynamic_cast<TMenuItem*>(parent->Children->Items[i]);

					if (c)
					{
						if (c->Tag == device)
							c->IsChecked = true;
						else
							c->IsChecked = false;
					}
				}
			}
		}

		//Set the display
		setScreenDisplay(device);

		//Always clean up after yourself
		delete ini;
	}
}


void __fastcall Tsolar2DForm::FormResize(TObject *Sender)
{
	//Use the row sizes added together to get final size
	int count = propsGridL->RowCollection->Count;
	int h     = 0;

	for (int row = 0; row < count; row++)
	{
		h += (propsGridL->CellRect[0][row].Bottom - propsGridL->CellRect[0][row].Top);
	}

	//Set the 'real' size of the grid so the panel adjust and scrollbox reacts correctly
	propsSizedPanel->Height = h;


	//--------------------------------------
	//Align controls
	//--------------------------------------
	count = alignGridL->RowCollection->Count;
	h = 0;

	for (int row = 0; row < count; row++)
	{
		h += (alignGridL->CellRect[0][row].Bottom - alignGridL->CellRect[0][row].Top);
	}
	//
	gridScrollBox->Align = TAlignLayout::None;
	alignSizedPanel->Height = h;

	//Set the grid to bottom of the scroll box
	controlTree->Height = ((Panel2->Height - (alignSplitter->Height*2)) - alignSizedPanel->Height);

	gridScrollBox->Align = TAlignLayout::Client;


	//--------------------------------------
	//Align Items for the Animation grids
	//--------------------------------------
	count = animGridL->RowCollection->Count;
	h = 0;

	for (int row = 0; row < count; row++)
	{
		h += (animGridL->CellRect[0][row].Bottom - animGridL->CellRect[0][row].Top);
	}

	//
	animListBox->Align = TAlignLayout::None;
	animSizedPanel->Height = h;
	animScrollBox->Height = h + animHSplitter->Height;
	//Set the grid to bottom of the scroll box
	animListBox->Height = (Panel3->Height - animScrollBox->Height);
	animListBox->Align = TAlignLayout::Client;
}


//--------------------------------------------------------
//
//Device screen
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::screenResolutionClick(TObject *Sender)
{
	if (isAnimating)
		return;

	//Screen Resolution selector
	TMenuItem *m = dynamic_cast<TMenuItem*>(Sender);
	TControl *parent;

	if (m)
	{
		//
		if (m->IsChecked == false)
			m->IsChecked = true;

		//There are two separate groups of Radio Menu items
		//Which ever group is active, the other 'complete' group
		//needs to be deactivated
		if (m->GroupIndex == 1)
		{
			//Clear all of GroupIndex 2
			parent = dynamic_cast<TControl *>(borderlessSubMenu->Children->Items[0]);
		}
		else
		{
			parent = dynamic_cast<TControl *>(viewMenu->Children->Items[0]);
		}

		if (parent)
		{
			for (int i = 0; i < parent->ChildrenCount; i++)
			{
				TMenuItem *c = dynamic_cast<TMenuItem*>(parent->Children->Items[i]);

				if (c)
					c->IsChecked = false;
			}
		}

		//Set the new display
		setScreenDisplay(m->Tag);

		//Only show if there is an active SINGLE selection
		if (selectionRect->TagObject)
			adjustHandles(true, NULL);
		else
			adjustHandles(false, NULL);
	}
}


void __fastcall Tsolar2DForm::ScaleTrackChange(TObject *Sender)
{
	//Change scale
	deviceFrame->Scale->X = ScaleTrack->Value;
	deviceFrame->Scale->Y = ScaleTrack->Value;

	//Poistion in top left corner if needed
	if (deviceFrame->Width > deviceScrollBox->Width)
		deviceFrame->Position->X = 0;

	if (deviceFrame->Height > deviceScrollBox->Height)
		deviceFrame->Position->Y = 0;

	//
	TextScale->Text = IntToStr((int)(ScaleTrack->Value*100))+"%";

	//Adjust the selection
	if (selectionRect->IsVisible)
		adjustHandles(true, NULL);
}


void __fastcall Tsolar2DForm::setScreenDisplay(int displayIndex)
{
	//Set the deviceScreen to the appropriate resolution
	(*luaDisplay[displayIndex])(disp);

	//Change the device view
	if (!disp->deviceImage->IsEmpty())
	{
		//
		deviceFrame->Fill->Kind = TBrushKind::Bitmap;
		deviceFrame->Fill->Bitmap->Bitmap = disp->deviceImage;

		int rotate = 0;
		while(rotate < screenOrientation)
		{
			deviceFrame->Fill->Bitmap->Bitmap->Rotate(90);
			rotate += 90;
		}
	}
	else
	{
		deviceFrame->Fill->Kind = TBrushKind::None;
		deviceFrame->Fill->Bitmap->Bitmap->Width = 0;
		deviceFrame->Fill->Bitmap->Bitmap->Height = 0;
	}

	//Change the device dressing if exist
	if (!disp->screenDressing->IsEmpty())
	{
		int rotate = 0;
		while(rotate < screenOrientation)
		{
			disp->screenDressing->Rotate(90);
			rotate += 90;
		}
	}
	else
	{
		//deviceFrame->Fill->Kind = TBrushKind::None;
		// //deviceFrame->Fill->Bitmap->Bitmap = NULL;
		//deviceFrame->Fill->Bitmap->Bitmap->Width = 0;
		//deviceFrame->Fill->Bitmap->Bitmap->Height = 0;
	}

	disp->selectedDisplay = displayIndex;
	//Apply display metrics
	applyDisplayMetrics();

	//Load the DPI, etc data from the table using m->Tag
}


//--------------------------------------------------------
//
//Display Metrics are applied to the Screen and Frame
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::applyDisplayMetrics()
{
	int scW, scH;
	int frW, frH;
	int scX, scY;
	int adjustedX = disp->screenOriginX;
	int adjustedY = disp->screenOriginY;

	//Change the device view
	if (!disp->deviceImage->IsEmpty())
	{
		//Set orientation
		if (screenOrientation == 0 || screenOrientation == 180)//Is it in Landscape more?
		{
			//X is good, Y needs to adjust
			if (screenOrientation == 0)
				adjustedY = disp->deviceImage->Height - (disp->screenHeight + adjustedY);

			//Y is good, X needs to adjust
			if (screenOrientation == 180)
			{
				adjustedX = disp->deviceImage->Width - (disp->screenWidth + adjustedX);
			}

			scW = disp->screenWidth;
			scH = disp->screenHeight;
			scX = adjustedX;
			scY = adjustedY;
			frH = disp->deviceImage->Height;
			frW = disp->deviceImage->Width;
		}
		else
		{
			//Defualt fit is 90 degrees. Do nothing
			//X & Y needs to adjust
			if (screenOrientation == 270)
			{
				adjustedX = disp->deviceImage->Width - (disp->screenWidth + adjustedX);
				adjustedY = disp->deviceImage->Height - (disp->screenHeight + adjustedY);
			}

			//Set to Landscape mode
			scH = disp->screenWidth;
			scW = disp->screenHeight;
			scY = adjustedX;
			scX = adjustedY;
			frW = disp->deviceImage->Height;
			frH = disp->deviceImage->Width;
		}
	}
	else
	{
		//Set orientation
		if (screenOrientation == 0 || screenOrientation == 180)//Is it in Landscape more?
		{
			scW = disp->screenWidth;
			scH = disp->screenHeight;
			scX = adjustedX;
			scY = adjustedY;
			frH = disp->screenHeight;
			frW = disp->screenWidth;
		}
		else
		{
			//Set to Landscape mode
			scH = disp->screenWidth;
			scW = disp->screenHeight;
			scY = adjustedX;
			scX = adjustedY;
			frH = disp->screenWidth;
			frW = disp->screenHeight;
		}
	}

	//Screen and positioing
	deviceScreen->Position->X = scX;
	deviceScreen->Position->Y = scY;
	deviceScreen->Width = scW;
	deviceScreen->Height = scH;

	//Device frame
	deviceFrame->Height = frH;
	deviceFrame->Width = frW;
}


void __fastcall Tsolar2DForm::deviceScreenPainting(TObject *Sender, TCanvas *Canvas, const TRectF &ARect)
{
	//If there is a border Overlay (Dressing). add over all objects
	if (!disp->screenDressing->IsEmpty())
	{
		//Display the overlaydressing
		Canvas->DrawBitmap(disp->screenDressing, disp->screenDressing->Bounds, ARect, 1.0, false);
	}

	if (!disp->screenDressing->IsEmpty())
	{
		//Display the overlaydressing
		Canvas->DrawBitmap(disp->screenDressing, disp->screenDressing->Bounds, ARect, 1.0, false);
	}
}


void __fastcall Tsolar2DForm::deviceScreenResize(TObject *Sender)
{
	//change scale
	deviceFrame->Scale->X = ScaleTrack->Value;
	deviceFrame->Scale->Y = ScaleTrack->Value;

	TextScale->Text = IntToStr((int)(ScaleTrack->Value*100))+"%";
}


void __fastcall Tsolar2DForm::deviceScreenMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y)
{
	if (isAnimating)
		return;

	adjustHandles(false, NULL);

	//Make sure the current object's cursor gets a reset aslo
	if (selectionRect->TagObject)
	{
		((TControl *)selectionRect->TagObject)->Cursor = crDefault;
	}

	//Clear animation list
	animListBox->Clear();
	//Clear allthe object inspector data
	clearAnimationInspector();

	//
	selectionRect->TagObject = NULL;
	setHandleColors(NULL);
	updateInspector();

	//Clear all selections in 'selectionsList'
	clearObjectSelections();
	clearAllCursors(deviceScreen);

	//Pass the click on to the parent
	deviceFrameMouseDown(NULL, Button, Shift, X + deviceScreen->Position->X, Y + deviceScreen->Position->Y);
}


void __fastcall Tsolar2DForm::deviceScreenMouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
	//Pass the click on to the parent
	deviceFrameMouseMove(NULL, Shift, (deviceScreen->Position->X + X)*ScaleTrack->Value, (deviceScreen->Position->Y + Y)*ScaleTrack->Value);
}


void __fastcall Tsolar2DForm::deviceFrameMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y)
{
	//Record PressedPositio.x/y
	if (Sender == NULL)
	{
		deviceFrame->PressedPosition.x = X * ScaleTrack->Value;
		deviceFrame->PressedPosition.y = Y * ScaleTrack->Value;

	}
}


void __fastcall Tsolar2DForm::deviceFrameMouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
	//Text2->Text = "X: " + FloatToStr(X) + ", Y: " + FloatToStr(Y);
	if (Shift.Contains(ssLeft) && !deviceFrame->Tag)
	{
		//Hide the selection while moving
		adjustHandles(false, NULL);

		deviceFrame->Position->X += (X - deviceFrame->PressedPosition.x)* ScaleTrack->Value;
		deviceFrame->Position->Y += (Y - deviceFrame->PressedPosition.y)* ScaleTrack->Value;
		//
		if (deviceFrame->Position->X < 0)
			deviceFrame->Position->X = 0;
		if (deviceFrame->Position->Y < 0)
			deviceFrame->Position->Y = 0;
	}

	//Reset after Open / Save dialoga access
	deviceFrame->Tag = 0;
}


void __fastcall Tsolar2DForm::deviceFrameMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y)
{
	//Hide the selection while moving
	if (selectionRect->TagObject)
		adjustHandles(true, NULL);
	else
		adjustHandles(false, NULL);
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::adjustHandles(bool isVisible, TRectangle *handle)
{
	//TODO: These handles and frame need to be parented by the selected
	//object to operate visually and mechanically correct
	if (selectionRect->TagObject)
	{
		TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);

		clipToParent(c);
	}

	TRectF s = selectionRect->BoundsRect;
	int Grip = halfGrip;

	//Adjust the handles
	//Top
	TL->Position->X = s.left;
	TL->Position->Y = s.top;
	TC->Position->X = s.left + (s.Width() / 2) - Grip;
	TC->Position->Y = s.top;
	TR->Position->X = s.right - gripSize;
	TR->Position->Y = s.top;
	//
	//Center
	L->Position->X = s.left;
	L->Position->Y = s.top + (s.Height()/2) - Grip;
	R->Position->X = s.right - gripSize;
	R->Position->Y = s.top + (s.Height()/2) - Grip;

	//Bottom
	BL->Position->X = s.left;
	BL->Position->Y = s.Bottom - gripSize;
	BC->Position->X = s.left + (s.Width() / 2) - Grip;
	BC->Position->Y = s.bottom - gripSize;
	BR->Position->X = s.right - gripSize;
	BR->Position->Y = s.Bottom - gripSize;

	TRectangle *array[8] =
{
		TL, TC, TR,
		L,R,
		BL,BC, BR
	};

	//
	selectionRect->Visible = isVisible;
	selectionRect->BringToFront();
	setHandleColors(NULL);

	for (int i = 0; i < 8; i++)
	{
		if (handle && array[i] == handle)
			continue;
		//
		array[i]->Visible = isVisible;
		array[i]->BringToFront();
	}
}


void __fastcall Tsolar2DForm::clearAllCursors(TObject *Sender)
{
	TControl *parent = dynamic_cast<TControl *>(Sender);

	if (parent && parent->ClassType() != __classid(TTextEx))
	{
		for (int i = 0; i < parent->ChildrenCount; i++)
		{
			//loop through all children
			if (parent->Children->Items[i] && parent->Children->Items[i] != Parent)
			{
				TControl *c = dynamic_cast<TControl *>(parent->Children->Items[i]);
				if (c && c->HitTest)
				{
					c->Cursor = crDefault;

					//Is a recursive method
					if (c->ChildrenCount)
					{
						clearAllCursors(c);
					}
				}
			}
		}
	}
}


void __fastcall Tsolar2DForm::clearObjectSelections()
{
	if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
			{
				TGlowEffect *e = dynamic_cast<TGlowEffect *>(c->TagObject);

				if (e)
				{
					delete e;
					e = NULL;
					c->TagObject = NULL;
				}
			}
		}

		selectionsList->Clear();
		updateInspector();
		deviceScreen->Repaint();
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y)
{
	Focused = NULL;

	if (isAnimating)
		return;

	//Multiselect objects to do alignments to
	if (Shift.Contains(ssLeft) && Shift.Contains(ssShift))
	{
		//Handle selecting object  . All objects must be in the same parent level
		TControl *c = dynamic_cast<TControl *>(Sender);

		if (c)
		{
			//if (c->ClassType() == __classid(TTextEx))
			//((TTextEx*)c)->AutoSize = false;
			//

			//Don't want any active selections for multi-select
			selectionRect->TagObject = NULL;
			adjustHandles(false,NULL);

			if (selectionsList->Count)
			{
				TControl *chk = findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[0]));
				//This object is required due to some alignment features needed the first object
				//In the list to remain in the first position
				TStringList *test = new TStringList();
				//
				test->Duplicates = System::Types::TDuplicates::dupIgnore;
				test->Sorted = true;

				//
				if (c && chk && (c->Parent == chk->Parent))
				{
					int start = selectionsList->Count;

					//Testing purposes only
					test->AddStrings(selectionsList);
					test->Add(c->Name);
					//selectionsList->Add(c->Name);

					//Do not double glow the object
					//if (start != selectionsList->Count)
					if (start != test->Count)
					{
						TGlowEffect *effect = new TGlowEffect(c);

						effect->Parent = c;
						effect->GlowColor = TAlphaColorRec::Gold;
						effect->Opacity = 1;
						effect->Softness = 1;
						c->TagObject = effect;

						//Add the object to the list
						selectionsList->Add(c->Name);
					}
					else
					{
						//Remove selection status from the object
						TGlowEffect *effect = (TGlowEffect *)c->TagObject;

						if (effect)
						{
							delete effect;
							c->TagObject = NULL;
							selectionsList->Delete(selectionsList->IndexOf(AnsiString(c->Name)));
							deviceScreen->Repaint();
						}
					}
				}

				//No longer required for testing
				delete test;
			}
			else
			{
				selectionsList->Add(c->Name);

				TGlowEffect *effect = new TGlowEffect(c);

				effect->Parent = c;
				effect->GlowColor = TAlphaColorRec::Gold;
				effect->Opacity = 1;
				effect->Softness = 1;
				c->TagObject = effect;
			}

			//Fix cursors
			clearAllCursors(deviceScreen);

			//Disable some controls that do not support multi-select
			updateInspector();
		}

		return;
	}


	//-------------------------------
	//Must clear all selection info
	//-------------------------------
	clearObjectSelections();
	//clearAllCursors(deviceScreen);


	//Standard operations
	if (Shift.Contains(ssLeft))
	{
		//Show selection
		TControl *c = dynamic_cast<TControl*>(Sender);

		if (c)
		{
			//Class declared in: ZOrder.h
			UIObject *o = (UIObject *)c->Tag;

			if (!o)
			{
				o = new UIObject();
				memset(o, 0, sizeof(UIObject));
				o->Visible = true;
				c->Tag = (long)o;
			}

			//Get current ZOrder index
			c->TagFloat = GetControlZOrder(c);
			c->BringToFront();
			deviceScreen->Repaint();

			//Select the item in the Tree viewer
			controlTree->Selected = controlTree->ItemByText(c->Name);

			//controlTree->Selected->Expand();
			//Sub items are not selected unless visible. Needs work

			//Selection adjsutments
			selectionRect->TagObject = Sender;
			clipToParent(c);

			//Build animation list
			buildAnimationList(c);
			//Select a default item
			if (animListBox->Count)
			{
				animListBox->ItemIndex = 0;
				animListBoxClick(NULL);
			}


			//Do show Object inspector data changes immediately after click
			//Alignment data updated after click is released
			updateInspector();
			updateObjectData();


			//
			adjustHandles(false, NULL);
			setHandleColors(c);
			//Enable control

			LW = c->Width;
			LH = c->Height;

			if (c->ClassType()
				== __classid(TRectangleEx))
			{
				((TRectangleEx *)c)->oldX = c->Position->X;
				((TRectangleEx *)c)->oldY = c->Position->Y;
				((TRectangleEx *)c)->oldW = c->Width;
				((TRectangleEx *)c)->oldH = c->Height;
			}
			else
			{
				((TTextEx *)c)->oldX = c->Position->X;
				((TTextEx*)c)->oldY = c->Position->Y;
				((TTextEx *)c)->oldW = c->Width;
				((TTextEx *)c)->oldH = c->Height;
			}

			//OMG! I love you :)
			c->AutoCapture = true;
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectMouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
	if (isAnimating)
		return;

	int gridMove = 1;

	//Move on a grid
	if (Shift.Contains(ssAlt))
	{
		gridMove = 8;
	}

	TControl *c = dynamic_cast<TControl *>(Sender);

	//No mouse key depressed, not in resize mode
	if (!Shift.Contains(ssLeft))
		inSizeMode = false;


	//Save changes only ONCE!!!
	if (Shift.Contains(ssLeft) && !saveStateChanged)
	{
		updateUndoStack("", false);
		saveStateChanged = true;
	}

	//---------------------------------------------
	//
	//Set up resizing
	//
	//---------------------------------------------
	if (!inSizeMode && (selectionRect->TagObject && selectionRect->TagObject == Sender) && !Shift.Contains(ssLeft))
	{
		onLeft = onRight = onTop = onBottom = false;

		//Edges are set at 4 pixels max for resizing
		//Left side
		if (X <= halfGrip)
			onLeft = true;
		if (X >= (c->Width - halfGrip))
			onRight = true;
		if (Y <= halfGrip)
			onTop = true;
		if (Y >= (c->Height - halfGrip))
			onBottom = true;

		//Update the cursor icon
		c->Cursor = getMouseCursor();
		//Change the colors of the handles
		setHandleColors(c);

		//Trigger resize
		if (Shift.Contains(ssLeft) && c->Cursor != crDefault)
			inSizeMode = true;
	}
	else if (selectionsList->Count || (selectionRect->TagObject && selectionRect->TagObject != Sender))
	{
		c->Cursor = crDefault;
	}


	//
	if (Shift.Contains(ssLeft) && selectionRect->TagObject == Sender)
	{
		//Are we moving or resizing?
		if (c)
		{
			float LX = c->PressedPosition.x;
			float LY = c->PressedPosition.y;

			if (c->Cursor != crDefault)
			{
				if (onLeft)
				{
					if (onTop)
					{
						c->Width -= (X - LX);
						c->Height -= (Y - LY);
						c->Position->X += (X - LX);
						c->Position->Y += (Y - LY);
					}
					else if (onBottom)
					{
						c->Width -= (X - LX);
						c->Position->X += (X - LX);
						c->Height = (Y - LY) + LH;
					}
					else
					{
						c->Width -= (X - LX);
						c->Position->X += (X - LX);
					}
				}
				else if (onRight)
				{
					if (onTop)
					{
						c->Width = (X - LX) + LW;
						c->Height -= (Y - LY);
						c->Position->Y += (Y - LY);
					}
					else if (onBottom)
					{
						c->Width = (X - LX) + LW;
						c->Height = (Y - LY) + LH;
					}
					else
					{
						c->Width = (X - LX) + LW;
					}
				}
				else if (onTop)
				{
					c->Height -= (Y - LY);
					c->Position->Y += (Y - LY);
				}
				else if (onBottom)
				{
					c->Height = (Y - LY) + LH;
				}
				else
				{
					//Done resizing
					inSizeMode = false;
				}

				//Final values get adjusted but grid value
				c->Height = abs(c->Height);
				c->Width = abs(c->Width);
			}
			else
			{
				//Final values
				c->Position->X += X - LX;
				c->Position->Y += Y - LY;
				c->Position->X = ((int)c->Position->X / gridMove) * gridMove;
				c->Position->Y = ((int)c->Position->Y / gridMove) * gridMove;
			}

			//
			clipToParent(c);
		}
	}

	//Clipping calculations
	//if (c->Position->X < 0)
	//c->Position->X = 0;
	//if (c->Position->Y < 0)
	//c->Position->Y = 0;
	//
	//if ((c->Position->X + c->Width) > ((TControl *)c->Parent)->Width)
	//c->Position->X = (((TControl *)c->Parent)->Width - c->Width);
	//
	//if ((c->Position->Y + c->Height) > ((TControl *)c->Parent)->Height)
	//c->Position->Y = (((TControl *)c->Parent)->Height - c->Height);
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y)
{
	if (isAnimating)
		return;

	TControl *c = dynamic_cast<TControl *>(Sender);

	if (c)
	{
		if (!selectionsList->Count && selectionRect->TagObject)
		{
			SetControlZOrder(c, c->TagFloat);
			setHandleColors(dynamic_cast<TControl *>(Sender));
		}

		//Get the list of allobjects
		TObject *temp = selectionRect->TagObject;

		//Align all objects on the device screen
		setAllAlignments(deviceScreen);

		//Update selection info
		selectionRect->TagObject = temp;

		//Show updates
		loadAlignmentComboBoxes(false);
		showAlignmentSelections(c);
		updateObjectData();

		//Only show if there is an active SINGLE selection
		if (selectionRect->TagObject)
			adjustHandles(true, NULL);
		else
			adjustHandles(false, NULL);

		//Enable updates again for savestates
		saveStateChanged = false;
	}

	//Background ALWAYS last
	deviceScreen->Repaint();
	inSizeMode = false;
}


//--------------------------------------------------------
//
//Draw a border around TTExt for visibility
//Draw Anchor center upon request
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::textPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect)
{
	TTextEx *c = dynamic_cast<TTextEx *>(Sender);

	if (c)
	{
		if (borderToggle->IsChecked || c->Opacity == 0)
		{
			TRectF r = TRectF(ARect.left + 1, ARect.top + 1, ARect.right - 1, ARect.bottom - 1);
			//
			Canvas->Stroke->Kind = TBrushKind::Solid;
			Canvas->Stroke->Color = claBlack;
			Canvas->Stroke->Thickness = 2;
			//Canvas->DrawRect(r, 0, 0, AllCorners, c->Opacity);
			Canvas->DrawRect(r, 0, 0, AllCorners, 1);
		}
		else
		{
			TRectF r = TRectF(ARect.left + 1, ARect.top + 1, ARect.right - 1, ARect.bottom - 1);
			//
			Canvas->Stroke->Kind = TBrushKind::Solid;
			Canvas->Stroke->Color = claBlack;
			Canvas->Stroke->Thickness = 2;
			Canvas->DrawRect(r, 0, 0, AllCorners, c->Opacity);
		}


		if (crossHair->IsChecked)
		{
			int min, max;

			min = Max(0, (int)(c->Height * c->RotationCenter->Y) + 1);
			max = Min((int)c->Height - 5, (int)(c->Height * c->RotationCenter->Y) + 5);
			//Align this line according to the AnchorY
			TRectF hori = TRectF(
				ARect.left + 1,
				min,
				ARect.right - 1,
				max
				);

			//Align this line according to the AnchorX
			min = Max(0, (int)(c->Width * c->RotationCenter->X) + 1);
			max = Min((int)c->Width - 5, (int)(c->Width * c->RotationCenter->X) + 5);

			TRectF vert = TRectF(
				min,
				ARect.top + 1,
				max,
				ARect.bottom - 1
				);

			//
			Canvas->Fill->Color = claWhite;
			Canvas->FillRect(hori, 0, 0, AllCorners, c->Opacity);
			Canvas->FillRect(vert, 0, 0, AllCorners, c->Opacity);

			Canvas->Stroke->Kind = TBrushKind::Solid;
			Canvas->Stroke->Color = claBlack;
			Canvas->Stroke->Thickness = 2;
			Canvas->DrawRect(hori, 0, 0, AllCorners, c->Opacity);
			Canvas->DrawRect(vert, 0, 0, AllCorners, c->Opacity);
		}
	}
}


//--------------------------------------------------------
//
//Draw Anchor center upon request
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect)
{
	TRectangleEx *c = dynamic_cast<TRectangleEx *>(Sender);

	if (c)
	{
		if (borderToggle->IsChecked || c->Opacity == 0 || (c->Fill->Color == claNull && c->TagString == ""))
		{
			TRectF r = TRectF(ARect.left + 1, ARect.top + 1, ARect.right - 1, ARect.bottom - 1);
			//
			Canvas->Stroke->Kind = TBrushKind::Solid;
			Canvas->Stroke->Color = claBlack;
			Canvas->Stroke->Thickness = 2;
			//Canvas->DrawRect(r, 0, 0, AllCorners, c->Opacity);
			Canvas->DrawRect(r, 0, 0, AllCorners, 1);
		}

		if (crossHair->IsChecked)
		{
			int min, max;

			min = Max(0, (int)(c->Height * c->RotationCenter->Y) + 1);
			max = Min((int)c->Height - 5, (int)(c->Height * c->RotationCenter->Y) + 5);
			//Align this line according to the AnchorY
			TRectF hori = TRectF(
				ARect.left + 1,
				min,
				ARect.right - 1,
				max
				);

			//Align this line according to the AnchorX
			min = Max(0, (int)(c->Width * c->RotationCenter->X) + 1);
			max = Min((int)c->Width - 5, (int)(c->Width * c->RotationCenter->X) + 5);

			TRectF vert = TRectF(
				min,
				ARect.top + 1,
				max,
				ARect.bottom - 1
				);

			//
			Canvas->Fill->Color = claWhite;
			Canvas->FillRect(hori, 0, 0, AllCorners, c->Opacity);
			Canvas->FillRect(vert, 0, 0, AllCorners, c->Opacity);

			Canvas->Stroke->Kind = TBrushKind::Solid;
			Canvas->Stroke->Color = claBlack;
			Canvas->Stroke->Thickness = 2;
			Canvas->DrawRect(hori, 0, 0, AllCorners, c->Opacity);
			Canvas->DrawRect(vert, 0, 0, AllCorners, c->Opacity);
		}

	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::clipToParent(TControl *c)
{
	TControl *parent = dynamic_cast<TControl *>(c->Parent);

	if (parent)
	{
		TPointF p = getNestedPosition(c);
		selectionRect->Position->X = (c->Position->X + p.X);
		selectionRect->Position->Y = (c->Position->Y + p.Y);
		selectionRect->Width = c->Width;
		selectionRect->Height = c->Height;

		// //Automatic
		//p.X = deviceScreen->Position->X;
		//p.Y = deviceScreen->Position->Y;


		selectionRect->Position->X *= ScaleTrack->Value;
		selectionRect->Position->Y *= ScaleTrack->Value;
		selectionRect->Width *= ScaleTrack->Value;
		selectionRect->Height *= ScaleTrack->Value;

		//Do not scale deviceScreen's position
		selectionRect->Position->X += (deviceFrame->Position->X);
		selectionRect->Position->Y += (deviceFrame->Position->Y);
	}
}


//--------------------------------------------------------
//
//Because some objects cancontain nested objects, offsets
//need to be adjusted accordingly
//
//--------------------------------------------------------
TPointF __fastcall Tsolar2DForm::getNestedPosition(TControl *Child)
{
	//Everything ALWAYS relative to deviceScreen Form
	TPointF p;
	TFmxObject *Parent = Child->Parent;

	if (Parent)
	{
		//Loop thru all the parents upto deviceScreen
		//and get the X/Y positions
		//while(Parent != deviceScreen)
		while(Parent != deviceFrame)
		{
			p.X += ((TControl *)Parent)->Position->X;
			p.Y += ((TControl *)Parent)->Position->Y;

			Parent = Parent->Parent;
		}
	}

	return p;
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectMouseLeave(TObject *Sender)
{
	setHandleColors(NULL);
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
TCursor __fastcall Tsolar2DForm::getMouseCursor()
{
	if (onLeft)
	{//Left side cursors
		if (onTop)
			return crSizeNWSE;
		else if (onBottom)
			return crSizeNESW;
		else
			return crSizeWE;
	}
	else if (onRight)
	{
		if (onBottom)
			return crSizeNWSE;
		else if (onTop)
			return crSizeNESW;
		else
			return crSizeWE;
	}
	else if (onTop || onBottom)
		return crSizeNS;

	return crDefault;
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::setHandleColors(TControl *Sender)
{
	TRectangle *array[8] =
{
		TL, TC, TR,
		L, R,
		BL, BC, BR
	};

	int index = -1;

	if (onLeft)
	{
		if (onTop)
			index = 0;
		else if (onBottom)
			index = 5;
		else
			index = 3;
	}
	else if (onRight)
	{
		if (onBottom)
			index = 7;
		else if (onTop)
			index = 2;
		else
			index = 4;
	}
	else if (onTop)
		index = 1;
	else if (onBottom)
		index = 6;


	//
	for (int i = 0;
	i< 8;
	i++)
	{
		array[i]->Fill->Color = System::Uitypes::TAlphaColorRec::Dodgerblue;
	}

	if (Sender && Sender == selectionRect->TagObject)
	{
		if (index != -1)
		{
			array[index]->Fill->Color = System::Uitypes::TAlphaColorRec::Red;
		}
	}
}


//--------------------------------------------------------
//
//Menu Controls
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::exitAppClick(TObject *Sender)
{
	solar2DForm->Close();
}


//--------------------------------------------------------
//
//Simple about dialog
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::aboutItemClick(TObject *Sender)
{
	TfrmAbout *frmAbout = new TfrmAbout(Application);
	frmAbout->ShowModal();
	frmAbout->DisposeOf();
}


//--------------------------------------------------------
//
//Object creation
//
//--------------------------------------------------------
TObject *__fastcall Tsolar2DForm::createNewControl(TObject *Parent,AnsiString type)
{
	TObject *  ARect        = NULL;
	TControl * parent       = dynamic_cast<TControl *>(Parent);
	UIObject * o            = new UIObject();
	int        controlCount = -1;
	AnsiString s            = UpperCase(type);
	AnsiString nameSpace    = "";

	//Get the object count for the parent form
	if (parent)
	{
		controlCount = parent->ChildrenCount - 1;

		//Create a "prefix" for each object per parent
		if (parent->Name != "deviceScreen")
			nameSpace = parent->Name + "_";
	}

	//
	controlCount++;

	//-----------------------------------
	//
	//Create a TTextEx object
	//
	//-----------------------------------
	if (s == "TEXT")
	{
		//TTextEx* c = new TTextEx(parent);
		TTextEx *c = new TTextEx(parent);
		ARect = (TObject *)c;

		c->Parent = parent;
		c->Tag = (long)o;
		c->ClipParent = false;
		c->Anchors = (TAnchors() >> TAnchorKind::akLeft >> TAnchorKind::akRight >> TAnchorKind::akTop >> TAnchorKind::akBottom);

		//AutoSize set to False.
		//Word wrap required for multi-line controls [ Solar2D requires Width/Height be set ]
		c->WordWrap = true;
		c->AutoSize = false;

		//Basic settings
		c->Font->Size = 16;
		c->Text = nameSpace + "T" + IntToStr(controlCount);
		c->TextSettings->HorzAlign = TTextAlign::Leading;
		c->TextSettings->VertAlign = TTextAlign::Leading;
		c->TextSettings->Trimming = TTextTrimming::None;

		//Set color index
		colorArray->ItemIndex = controlCount % (colorArray->Items->Count - 1);

		//Exclude certain colors and child colors matching parent color
		if (colorArray->Color == TAlphaColorRec::Gold || colorArray->Color == TAlphaColorRec::Yellow ||
			colorArray->Color == TAlphaColorRec::Black || colorArray->Color == ((TTextEx*)parent)->Color)
		{
			colorArray->ItemIndex++;
		}

		c->TextSettings->FontColor = colorArray->Color;
		c->Opacity = 1;

		//Popup window for Text objects
		c->PopupMenu = devicePopup;

		//Image encoding
		c->imageType = -1;
		c->imageIndex = -1;

		//Might have created a duplicate name. Need to test for this
		try
		{
			c->Name = c->Text;
		}
		catch(...)
		{
			c->Name = "_" + c->Text;
		}

		//
		ARect = (TObject *)c;
	}
	//-----------------------------------
	//
	//Create a TRectangle(Ex) object
	//
	//-----------------------------------
	else
	{
		TRectangleEx *c = new TRectangleEx(parent);

		c->Parent = parent;
		c->Tag = (long)o;
		//Groups: Need to clip their children
		c->ClipChildren = true;
		c->ClipParent = false;
		c->Anchors = (TAnchors() >> TAnchorKind::akLeft >> TAnchorKind::akRight >> TAnchorKind::akTop >> TAnchorKind::akBottom);

		//Set color index
		colorArray->ItemIndex = controlCount % (colorArray->Items->Count - 1);

		//Exclude certain colors and child colors matching parent color
		if (colorArray->Color == TAlphaColorRec::Gold || colorArray->Color == TAlphaColorRec::Yellow ||
			colorArray->Color == TAlphaColorRec::Black || colorArray->Color == ((TRectangleEx*)parent)->Fill->Color)
		{
			colorArray->ItemIndex++;
		}

		//Colors and restore color
		c->Fill->Color = colorArray->Color;
		c->oldColor = colorArray->Color;
		c->Stroke->Kind = TBrushKind::None;
		c->Opacity = 1;

		//Image encoding
		c->imageType = -1;
		c->imageIndex = -1;

		//Popup window for Windows
		c->PopupMenu = devicePopup;

		//Might have created a duplicate name. Need to test for this
		try
		{
			c->Name = nameSpace + "R" + IntToStr(controlCount);
		}
		catch(...)
		{
			c->Name = nameSpace + "_R" + IntToStr(controlCount);
		}

		//
		ARect = (TObject *)c;
	}

	//Alignment controls for 'relative layout' style parenting
	memset(o, 0, sizeof(UIObject));
	o->Visible = true;
	o->zPosition = controlCount;

	//
	((TControl *)ARect)->Width = (parent->Width/4) + (rand() % (int)(parent->Width/8));
	((TControl *)ARect)->Height = (parent->Height/4) + (rand() % (int)(parent->Height/8));

	((TControl *)ARect)->Enabled = true;
	((TControl *)ARect)->Position->X = (rand() % (int)(parent->Width/2));
	((TControl *)ARect)->Position->Y = (rand() % (int)(parent->Height/2));

	if (ARect->ClassType() == __classid(TRectangleEx))
	{
		((TRectangleEx *)ARect)->Enabled = true;
		((TRectangleEx *)ARect)->OnMouseDown = objectMouseDown;
		((TRectangleEx *)ARect)->OnMouseUp = objectMouseUp;
		((TRectangleEx *)ARect)->OnMouseMove = objectMouseMove;
		((TRectangleEx *)ARect)->OnMouseLeave = objectMouseLeave;
		((TRectangleEx *)ARect)->OnPainting = object9PatchPaint;
		((TRectangleEx *)ARect)->OnPaint = objectPaint;
		((TTextEx *)ARect)->OnResize = objectResize;
	}
	else
	{
		((TTextEx *)ARect)->Enabled = true;
		((TTextEx *)ARect)->OnMouseDown = objectMouseDown;
		((TTextEx *)ARect)->OnMouseUp = objectMouseUp;
		((TTextEx *)ARect)->OnMouseMove = objectMouseMove;
		((TTextEx *)ARect)->OnMouseLeave = objectMouseLeave;
		((TTextEx *)ARect)->OnPaint = textPaint;
	}

	return ARect;
}


//--------------------------------------------------------
//
//Object Inspector Controls
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::updateInspector()
{
	if (isAnimating)
	{
		//Inspector controls: Can be used with multi-selctions
		widthNum->Enabled = false;
		heightNum->Enabled = false;
		posXNum->Enabled = false;
		posYNum->Enabled = false;
		scaleXNum->Enabled = false;
		anchorXNum->Enabled = false;
		scaleYNum->Enabled = false;
		anchorYNum->Enabled = false;
		rotateNum->Enabled = false;
		visibleCheck->Enabled = false;
		alphaNum->Enabled = false;

		//Do not support TTextEx
		shapeColorSelect->Enabled = false;

		//Alignments: Can be used with multi-selections
		bottomParentChk->Enabled = false;
		horiParentChk->Enabled = false;
		leftParentChk->Enabled = false;
		rightParentChk->Enabled = false;
		topParentChk->Enabled = false;
		vertParentChk->Enabled = false;
		anchorLeftChk->Enabled = false;
		anchorTopChk->Enabled = false;
		anchorRightChk->Enabled = false;
		anchorBottomChk->Enabled = false;

		//Toolbar
		deleteSelected->Enabled = false;
		newWindow->Enabled = false;
		newText->Enabled = false;
		imageButton->Enabled = false;


		//Text specific controls
		fontColorSelect->Enabled = false;
		enterText->Enabled = false;
		alignTextSelector->Enabled = false;
		fontSizeNum->Enabled = false;
		fontDialogButton->Enabled = false;

		//Toolbar: Multiselct Only, relies on 'selectionsList'
		spaceVert->Enabled = false;
		spaceHori->Enabled = false;
		spaceVEqually->Enabled = false;
		spaceHEqually->Enabled = false;

		spaceVertEffect->Enabled = true;
		spaceHoriEffect->Enabled = true;
		spaceVEquallyEffect->Enabled = true;
		spaceHEquallyEffect->Enabled = true;


		//Alignments: Toolbar controls
		alignLSideWin->Enabled = false;
		alignTSideWin->Enabled = false;
		alignRSideWin->Enabled = false;
		alignBSideWin->Enabled = false;
		alignVCenterWin->Enabled = false;
		alignHCenterWin->Enabled = false;
		//
		alignLSide->Enabled = false;
		alignTSide->Enabled = false;
		alignRSide->Enabled = false;
		alignBSide->Enabled = false;
		alignVCenter->Enabled = false;
		alignHCenter->Enabled = false;
		setLeftOf->Enabled = false;
		setTopOf->Enabled = false;
		setRightOf->Enabled = false;
		setBottomOf->Enabled = false;


		expandWin->Enabled = false;
		collapseWin->Enabled = false;
		expandV->Enabled = false;
		expandH->Enabled = false;


		alignLSideWinEffect->Enabled = !alignLSideWin->Enabled;
		alignTSideWinEffect->Enabled = !alignTSideWin->Enabled;;
		alignRSideWinEffect->Enabled = !alignRSideWin->Enabled;
		alignBSideWinEffect->Enabled = !alignBSideWin->Enabled;
		alignVCenterWinEffect->Enabled = !alignVCenterWin->Enabled;
		alignHCenterWinEffect->Enabled = !alignHCenterWin->Enabled;

		alignLSideEffect->Enabled = !alignLSide->Enabled;
		alignTSideEffect->Enabled = !alignTSide->Enabled;;
		alignRSideEffect->Enabled = !alignRSide->Enabled;
		alignBSideEffect->Enabled = !alignBSide->Enabled;
		alignVCenterEffect->Enabled = !alignVCenter->Enabled;
		alignHCenterEffect->Enabled = !alignHCenter->Enabled;
		setLeftOfEffect->Enabled = !setLeftOf->Enabled;
		setTopOfEffect->Enabled = !setTopOf->Enabled;
		setRightOfEffect->Enabled = !setRightOf->Enabled;;
		setBottomOfEffect->Enabled = !setBottomOf->Enabled;

		//
		expandWinEffect->Enabled = !expandWin->Enabled;
		collapseWinEffect->Enabled = !collapseWin->Enabled;
		expandVEffect->Enabled = !expandV->Enabled;
		expandHEffect->Enabled = !expandH->Enabled;

		//
		bool active = false;

		//Affected by 'selectionsList'
		nameEdit->Enabled = active;
		imageButton->Enabled = active;

		//Affected by 'selectionsList'
		aboveCombo->Enabled = active;
		bottomWithCombo->Enabled = active;
		horiWithCombo->Enabled = active;
		leftWithCombo->Enabled = active;
		rightWithCombo->Enabled = active;
		topWithCombo->Enabled = active;
		vertWithCombo->Enabled = active;
		belowCombo->Enabled = active;
		leftOfCombo->Enabled = active;
		rightOfCombo->Enabled = active;

		//Toolbar buttons, affected by 'selectionsList'
		raiseToTop->Enabled = active;
		raiseOneLevel->Enabled = active;
		lowerToBottom->Enabled = active;
		lowerOneLevel->Enabled = active;


		//Monochrome Effects for speed buttons
		raiseToTopEffect->Enabled = !raiseToTop->Enabled;
		raiseOneLevelEffect->Enabled = !raiseOneLevel->Enabled;
		lowerToBottomEffect->Enabled = !lowerToBottom->Enabled;
		lowerOneLevelEffect->Enabled = !lowerOneLevel->Enabled;
		newWindowEffect->Enabled = !newWindow->Enabled;
		newTextEffect->Enabled = !newText->Enabled;
		deleteSelectedEffect->Enabled = !deleteSelected->Enabled;

		//Data for animation inspector
		animRefName->Enabled = false;
		timeNum->Enabled = false;
		delayNum->Enabled = false;
		iterationNum->Enabled = false;
		transBox->Enabled = false;
		easeBox->Enabled = false;
		interpolationBox->Enabled = false;
		tagEdit->Enabled = false;
		deltaChk->Enabled = false;
		loopChk->Enabled = false;

		xChk->Enabled = false;
		yChk->Enabled = false;
		widthChk->Enabled = false;
		heightChk->Enabled = false;
		rotationChk->Enabled = false;
		xScaleChk->Enabled = false;
		yScaleChk->Enabled = false;
		alphaChk->Enabled = false;
		fontSizeChk->Enabled = false;

		//Are the values editable?
		animXNum->Enabled = false;
		animYNum->Enabled = false;
		animWidthNum->Enabled = false;
		animHeightNum->Enabled = false;
		animRotationNum->Enabled = false;
		animXScaleNum->Enabled = false;
		animYScaleNum->Enabled = false;
		animAlphaNum->Enabled = false;
		animFSNum->Enabled = false;
	}
	else
	{
		bool active = selectionRect->TagObject != NULL;

		//Inspector controls: Can be used with multi-selctions
		widthNum->Enabled = (active || selectionsList->Count);
		heightNum->Enabled = (active || selectionsList->Count);
		posXNum->Enabled = (active || selectionsList->Count);
		posYNum->Enabled = (active || selectionsList->Count);
		scaleXNum->Enabled = (active || selectionsList->Count);
		anchorXNum->Enabled = (active || selectionsList->Count);
		scaleYNum->Enabled = (active || selectionsList->Count);
		anchorYNum->Enabled = (active || selectionsList->Count);
		rotateNum->Enabled = (active || selectionsList->Count);
		visibleCheck->Enabled = (active || selectionsList->Count);
		alphaNum->Enabled = (active || selectionsList->Count);

		//Do not support TTextEx
		shapeColorSelect->Enabled = (active || selectionsList->Count);

		//Alignments: Can be used with multi-selections
		bottomParentChk->Enabled = (active || selectionsList->Count);
		horiParentChk->Enabled = (active || selectionsList->Count);
		leftParentChk->Enabled = (active || selectionsList->Count);
		rightParentChk->Enabled = (active || selectionsList->Count);
		topParentChk->Enabled = (active || selectionsList->Count);
		vertParentChk->Enabled = (active || selectionsList->Count);
		anchorLeftChk->Enabled = (active || selectionsList->Count);
		anchorTopChk->Enabled = (active || selectionsList->Count);
		anchorRightChk->Enabled = (active || selectionsList->Count);
		anchorBottomChk->Enabled = (active || selectionsList->Count);

		//Toolbar
		deleteSelected->Enabled = (active || selectionsList->Count);
		newWindow->Enabled = !selectionsList->Count;
		newText->Enabled = !selectionsList->Count;
		imageButton->Enabled = (active || selectionsList->Count);

		//Special case: Text cannot behave as a group
		//Special case: Text cannot have 9-Patch
		if (selectionRect->TagObject && selectionRect->TagObject->ClassType() == __classid(TTextEx))
		{
			newText->Enabled = false;
			newWindow->Enabled = false;
			imageButton->Enabled = false;
		}


		//Text specific controls
		fontColorSelect->Enabled = active;
		enterText->Enabled = active;
		alignTextSelector->Enabled = active;
		fontSizeNum->Enabled = active;
		fontDialogButton->Enabled = active;

		if (active)
		{
			TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);
			if (c)
			{
				bool visible = (c->ClassType() == __classid(TTextEx));

				//Use the row sizes added together to get final size
				int count = propsGridL->RowCollection->Count - (visible ? 0 : 5);
				int h     = 0;

				for (int row = 0; row< count; row++)
				{
					h += (propsGridL->CellRect[0][row].Bottom - propsGridL->CellRect[0][row].Top);
				}

				//Set the 'real' size of the grid so the panel adjust and scrollbox reacts correctly
				propsSizedPanel->Height = h;
			}
		}

		//Toolbar: Multiselct Only, relies on 'selectionsList'
		spaceVert->Enabled = (selectionsList->Count >1);
		spaceHori->Enabled = (selectionsList->Count >1);
		spaceVEqually->Enabled = (selectionsList->Count >2);
		spaceHEqually->Enabled = (selectionsList->Count >2);

		spaceVertEffect->Enabled = !spaceVert->Enabled;
		spaceHoriEffect->Enabled = !spaceHori->Enabled;
		spaceVEquallyEffect->Enabled = !spaceVEqually->Enabled;
		spaceHEquallyEffect->Enabled = !spaceHEqually->Enabled;


		//Alignments: Toolbar controls
		alignLSideWin->Enabled = (selectionsList->Count || active);
		alignTSideWin->Enabled = (selectionsList->Count || active);
		alignRSideWin->Enabled = (selectionsList->Count || active);
		alignBSideWin->Enabled = (selectionsList->Count || active);
		alignVCenterWin->Enabled = (selectionsList->Count || active);
		alignHCenterWin->Enabled = (selectionsList->Count || active);
		//
		alignLSide->Enabled = (selectionsList->Count > 1);
		alignTSide->Enabled = (selectionsList->Count > 1);
		alignRSide->Enabled = (selectionsList->Count > 1);
		alignBSide->Enabled = (selectionsList->Count > 1);
		alignVCenter->Enabled = (selectionsList->Count > 1);
		alignHCenter->Enabled = (selectionsList->Count > 1);
		setLeftOf->Enabled = (selectionsList->Count > 1);
		setTopOf->Enabled = (selectionsList->Count > 1);
		setRightOf->Enabled = (selectionsList->Count > 1);
		setBottomOf->Enabled = (selectionsList->Count > 1);


		expandWin->Enabled = (selectionsList->Count || active);
		collapseWin->Enabled = (selectionsList->Count || active);
		expandV->Enabled = (selectionsList->Count || active);
		expandH->Enabled = (selectionsList->Count || active);


		alignLSideWinEffect->Enabled = !alignLSideWin->Enabled;
		alignTSideWinEffect->Enabled = !alignTSideWin->Enabled;;
		alignRSideWinEffect->Enabled = !alignRSideWin->Enabled;
		alignBSideWinEffect->Enabled = !alignBSideWin->Enabled;
		alignVCenterWinEffect->Enabled = !alignVCenterWin->Enabled;
		alignHCenterWinEffect->Enabled = !alignHCenterWin->Enabled;

		alignLSideEffect->Enabled = !alignLSide->Enabled;
		alignTSideEffect->Enabled = !alignTSide->Enabled;;
		alignRSideEffect->Enabled = !alignRSide->Enabled;
		alignBSideEffect->Enabled = !alignBSide->Enabled;
		alignVCenterEffect->Enabled = !alignVCenter->Enabled;
		alignHCenterEffect->Enabled = !alignHCenter->Enabled;
		setLeftOfEffect->Enabled = !setLeftOf->Enabled;
		setTopOfEffect->Enabled = !setTopOf->Enabled;
		setRightOfEffect->Enabled = !setRightOf->Enabled;;
		setBottomOfEffect->Enabled = !setBottomOf->Enabled;

		//
		expandWinEffect->Enabled = !expandWin->Enabled;
		collapseWinEffect->Enabled = !collapseWin->Enabled;
		expandVEffect->Enabled = !expandV->Enabled;
		expandHEffect->Enabled = !expandH->Enabled;

		//Alignment controls
		if (selectionsList->Count)
			active = false;

		//Affected by 'selectionsList'
		nameEdit->Enabled = active;
		imageButton->Enabled = active;

		//Affected by 'selectionsList'
		aboveCombo->Enabled = active;
		bottomWithCombo->Enabled = active;
		horiWithCombo->Enabled = active;
		leftWithCombo->Enabled = active;
		rightWithCombo->Enabled = active;
		topWithCombo->Enabled = active;
		vertWithCombo->Enabled = active;
		belowCombo->Enabled = active;
		leftOfCombo->Enabled = active;
		rightOfCombo->Enabled = active;

		//Toolbar buttons, affected by 'selectionsList'
		raiseToTop->Enabled = active;
		raiseOneLevel->Enabled = active;
		lowerToBottom->Enabled = active;
		lowerOneLevel->Enabled = active;


		//Monochrome Effects for speed buttons
		raiseToTopEffect->Enabled = !raiseToTop->Enabled;
		raiseOneLevelEffect->Enabled = !raiseOneLevel->Enabled;
		lowerToBottomEffect->Enabled = !lowerToBottom->Enabled;
		lowerOneLevelEffect->Enabled = !lowerOneLevel->Enabled;
		newWindowEffect->Enabled = !newWindow->Enabled;
		newTextEffect->Enabled = !newText->Enabled;
		deleteSelectedEffect->Enabled = !deleteSelected->Enabled;

		//Data for animation inspector
		animRefName->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		timeNum->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		delayNum->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		iterationNum->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		transBox->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		easeBox->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		interpolationBox->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		tagEdit->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		deltaChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		loopChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);

		xChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		yChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		widthChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		heightChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		rotationChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		xScaleChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		yScaleChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		alphaChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);
		fontSizeChk->Enabled = (animListBox->Count && selectionRect->TagObject && animListBox->Selected);

		//Are the values editable?
		animXNum->Enabled = xChk->IsChecked;
		animYNum->Enabled = yChk->IsChecked;
		animWidthNum->Enabled = widthChk->IsChecked;
		animHeightNum->Enabled = heightChk->IsChecked;
		animRotationNum->Enabled = rotationChk->IsChecked;
		animXScaleNum->Enabled = xScaleChk->IsChecked;
		animYScaleNum->Enabled = yScaleChk->IsChecked;
		animAlphaNum->Enabled = alphaChk->IsChecked;
		animFSNum->Enabled = fontSizeChk->IsChecked;

		//Undo Menu
		editUndo->Enabled = saveStateString->Count;
	}
}


//--------------------------------------------------------
//
//Object Inspector: Data is updated using this method
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::updateObjectData()
{
	bool active = selectionRect->TagObject != NULL;

	//ABSOLUTELY DO NOT ALLOW USER INTERFACE CHANGES
	//FROM HERE!
	allowObjectUpdates = false;

	if (active)
	{
		TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);
		if (c)
		{
			UIObject *o = (UIObject *)c->Tag;

			if (!o)
			{
				o = new UIObject();
				memset(o, 0, sizeof(UIObject));
				o->Visible = true;
				c->Tag = (long)o;
			}

			//Inspector controls
			nameEdit->Text = c->Name;
			//
			widthNum->Value = c->Width;
			heightNum->Value = c->Height;
			posXNum->Value = c->Position->X;
			posYNum->Value = c->Position->Y;
			alphaNum->Value = c->Opacity;
			visibleCheck->IsChecked = o->Visible;

			//Alignment controls
			bottomParentChk->IsChecked = o->bottomParentChk;
			horiParentChk->IsChecked = o->horiParentChk;
			leftParentChk->IsChecked = o->leftParentChk;
			rightParentChk->IsChecked = o->rightParentChk;
			topParentChk->IsChecked = o->topParentChk;
			vertParentChk->IsChecked = o->vertParentChk;
			anchorLeftChk->IsChecked = o->anchorLeftChk;
			anchorTopChk->IsChecked = o->anchorTopChk;
			anchorRightChk->IsChecked = o->anchorRightChk;
			anchorBottomChk->IsChecked = o->anchorBottomChk;

			//
			if (c->ClassType() == __classid(TRectangleEx))
			{
				scaleXNum->Value = ((TRectangleEx *)c)->Scale->X;
				scaleYNum->Value = ((TRectangleEx *)c)->Scale->Y;
				anchorXNum->Value = ((TRectangleEx *)c)->RotationCenter->X;
				anchorYNum->Value = ((TRectangleEx *)c)->RotationCenter->Y;
				rotateNum->Value = ((TRectangleEx *)c)->RotationAngle;

				//if 9-Patch file loaded to this Rect, show the name
				UnicodeString name = ((TRectangleEx *)c)->TagString;
				name = ExtractFileName(name);

				if (name != "")
					imageButton->Text = name;
				else
					imageButton->Text = "(Empty)...";

				shapeColorSelect->Color = ((TRectangleEx *)c)->Fill->Color;
			}
			else if (c->ClassType() == __classid(TTextEx))
			{
				scaleXNum->Value = ((TTextEx *)c)->Scale->X;
				scaleYNum->Value = ((TTextEx *)c)->Scale->Y;
				anchorXNum->Value = ((TTextEx *)c)->RotationCenter->X;
				anchorYNum->Value = ((TTextEx *)c)->RotationCenter->Y;
				rotateNum->Value = ((TTextEx *)c)->RotationAngle;

				//Text attributes only
				enterText->Text = ((TTextEx *)c)->Text;
				fontColorSelect->Color = ((TTextEx *)c)->TextSettings->FontColor;
				alignTextSelector->ItemIndex = ((TTextEx*)c)->TextSettings->HorzAlign;
				fontSizeNum->Value = ((TTextEx *)c)->Font->Size;
			}

		}
	}

	//
	if (isModified)
		statusLabel->Text = "Modified...";
	else
		statusLabel->Text = "";

	//Updates re-enabled
	allowObjectUpdates = true;
}


void __fastcall Tsolar2DForm::loadAlignmentComboBoxes(bool clearOnly)
{
	//Clear all old data
	aboveCombo->Items->Clear();
	bottomWithCombo->Items->Clear();
	horiWithCombo->Items->Clear();
	leftWithCombo->Items->Clear();
	rightWithCombo->Items->Clear();
	topWithCombo->Items->Clear();
	vertWithCombo->Items->Clear();
	belowCombo->Items->Clear();
	leftOfCombo->Items->Clear();
	rightOfCombo->Items->Clear();

	//Get a list of the objects that can be aligned to
	if (!clearOnly && selectionRect->TagObject)
	{
		TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);

		if (c)
		{
			TStringList *list = getObjectsList(c->Parent);

			//Remove the self ref from the list
			int i = list->IndexOf(c->Name);

			if (i > -1)
				list->Delete(i);

			list->Insert(0, "(None)");

			aboveCombo->Items->Assign(list);
			bottomWithCombo->Items->Assign(list);
			horiWithCombo->Items->Assign(list);
			leftWithCombo->Items->Assign(list);
			rightWithCombo->Items->Assign(list);
			topWithCombo->Items->Assign(list);
			vertWithCombo->Items->Assign(list);
			belowCombo->Items->Assign(list);
			leftOfCombo->Items->Assign(list);
			rightOfCombo->Items->Assign(list);

			delete list;
		}
	}
}


void __fastcall Tsolar2DForm::fontDialogButtonClick(TObject *Sender)
{
	if (isAnimating)
		return;

	TList *temp = new TList();
	TFont *font = new TFont();

	if (temp && font)
	{
		//Choose what gets the requested changes
		if (selectionRect->TagObject)
		{
			temp->Add(selectionRect->TagObject);
		}
		else if (selectionsList->Count)
		{
			for (int i = 0; i < selectionsList->Count; i++)
			{
				TTextEx *c = dynamic_cast<TTextEx *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

				if (c)
					temp->Add(c);
			}
		}

		//
		TfontSelect *fonter = new TfontSelect(Application, font);
		fonter->ShowModal();
		fonter->DisposeOf();

		if (temp->Count)
		{
			if (font->Family != "")
			{
				for (int i = 0; i < temp->Count; i++)
				{
					TTextEx *c = (TTextEx *)temp->Items[i];

					//Editing a control's values
					if (c)
					{
						c->Font->Family = font->Family;
						c->Font->Size = font->Size;
						c->Font->Style = font->Style;
					}
				}

				//Project has changed
				isModified = true;
				codeView->changesMade = true;

				//Updates
				fontDialogButton->Text = font->Family;
				updateInspector();
				updateObjectData();
				deviceScreen->Repaint();
			}
		}
	}

	//Clean up
	delete temp;
	delete font;
}


void __fastcall Tsolar2DForm::objectNumKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift)
{
	if (Key == 13)
	{
		TNumberBox *nb = dynamic_cast<TNumberBox *>(Sender);

		if (nb)
			objectNumChange(nb);
	}
}


//--------------------------------------------------------
//
//Supports multi-selections for some edits
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectNumChange(TObject *Sender)
{
	if (!allowObjectUpdates || isAnimating)
		return;

	//Mormal process allowed
	TNumberBox *nb   = dynamic_cast<TNumberBox *>(Sender);
	TList *     temp = new TList();

	//Choose who gets the requested changes
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
				temp->Add(c);
		}
	}

	//Process request for all objects
	if (nb && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TTextEx *c = static_cast<TTextEx *>(temp->Items[i]);

			//Editing a control's values
			if (c)
			{
				//TRectangleEx *c = (TRectangleEx *)d;
				switch(nb->Tag)
				{
				case 0:
					c->Width = nb->Value;
					break;
				case 1:
					c->Height = nb->Value;
					break;
				case 2:
					c->Position->X = nb->Value;
					break;
				case 3:
					c->Position->Y = nb->Value;
					break;
				case 4:
					c->Scale->X = nb->Value;
					break;
				case 5:
					c->Scale->Y = nb->Value;
					break;
				case 6:
					c->RotationCenter->X =nb->Value;
					break;
				case 7:
					c->RotationCenter->Y = nb->Value;
					break;
				case 8:
					c->RotationAngle = nb->Value;
					break;
				case 9:
					c->Opacity = nb->Value;
					break;

					//Text settings
				case 15:
					if (c->ClassType() == __classid(TTextEx))
						((TTextEx *)c)->Font->Size = nb->Value;
					//
					break;


				}

				//Update selector and handles: Dirty :(
				if (!selectionsList->Count && selectionRect->TagObject)
					adjustHandles(true, NULL);
				else
					adjustHandles(false, NULL);

				//adjustHandles(true, NULL);
			}
		}

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}

	//Clean up the dirt
	deviceScreen->Repaint();

	//Free memory
	delete temp;
}


//--------------------------------------------------------
//
//Text alignment: Supports multi-selections for some edits
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectTextSelectorChange(TObject *Sender)
{
	if (isAnimating)
		return;

	TComboBox *cb   = dynamic_cast<TComboBox *>(Sender);
	TList *    temp = new TList();

	//Choose who gets the requested changes: Supports multi-selections
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
				temp->Add(c);
		}
	}


	//Process request for all objects
	if (cb && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TTextEx *c = (TTextEx *)temp->Items[i];

			//Editing a control's values: Only TTextEx values allowed
			if (c)
			{
				//Text Alignment
				if (UpperCase(cb->Selected->Text) == "RIGHT")
					c->TextSettings->HorzAlign = TTextAlign::Trailing;
				else if (UpperCase(cb->Selected->Text) == "CENTER")
					c->TextSettings->HorzAlign = TTextAlign::Center;
				else
					c->TextSettings->HorzAlign = TTextAlign::Leading;
			}
		}
		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}

	//Free memory
	delete temp;
}


//--------------------------------------------------------
//
//Image / 9-Patch support for image loading on Object Windows
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::imageButtonClick(TObject *Sender)
{
	//Enable image assignment to a Rectangle object
	FOpen->Filter = TBitmapCodecManager::GetFilterString();
	if (FOpen->InitialDir == "")
		FOpen->InitialDir = GetCurrentDir();

	//Single Text objects do not get images for now.
	//Future: May support and convert on export to "newButton" using 'CheckBox'
	if (selectionRect->TagObject && selectionRect->TagObject->ClassType() == __classid(TTextEx))
		return;

	//Try to open dialog
	if (FOpen->Execute())
	{
		int type = 0;
		char buffer[1024];

		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		strcpy(buffer, AnsiString(FOpen->FileName).c_str());
		if (strstr(buffer, ".9."))
			type = 1;
		//
		imageLoader(type, FOpen->FileName);

		//Open dialog clicks will move the frame. Disable
		deviceFrame->Tag = 1;
	}
}


//--------------------------------------------------------
//
//Process image setting on Window
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::imageLoader(int type, AnsiString fileName)
{
	if (isAnimating)
		return;

	TList *temp = new TList();

	//Choose who gets the requested changes: Supports multi-selections
	if (selectionRect->TagObject)
	{
		//Absolutely NO TTextEx types
		if (selectionRect->ClassType()!= __classid(TTextEx))
			temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			//Absolutely NO TTextEx types
			if (c)
				temp->Add(c);
		}
	}

	//Depending on the popup location, some objects may
	//retain a resize cursor by accident. Need to clear that up.
	clearAllCursors(deviceScreen);

	//
	//We have objects to use: NO TTextEx OBJECTS PERIOD!!!
	//
	if (temp->Count)
	{
		// //Enable image assignment to a Rectangle object
		for (int i = 0; i < temp->Count; i++)
		{
			TRectangleEx *c = (TRectangleEx *)temp->Items[i];

			//Editing a control's values: Only TRectangles allowed
			if (c)
			{
				if (type == 1)
				{
					load9PatchImage(c, fileName);
				}
				else
				{
					try
					{
						//Standard image set
						c->Fill->Bitmap->Bitmap->LoadFromFile(fileName);
						c->Fill->Bitmap->WrapMode = TWrapMode::TileOriginal;

						//Hide all color
						c->oldColor = c->Fill->Color;
						c->Fill->Color = TAlphaColorRec::Null;
						c->Fill->Kind = TBrushKind::Solid;
						c->TagString = fileName;
						c->imageType = TRectangleEx::Standard;
						c->imageIndex = -1;
					}
					catch(...)
					{
						ShowMessage("Error: '" + fileName + "' was not found.");
						//Error loading an image
						//c->Fill->Bitmap->Bitmap->LoadFromFile(fileName);
						//c->Fill->Bitmap->WrapMode = TWrapMode::TileOriginal;

						//Hide all color
						c->oldColor = c->Fill->Color;
						c->Fill->Kind = TBrushKind::Solid;
						c->TagString = "";
						c->imageType = -1;
						c->imageIndex = -1;
					}
				}
			}
		}

		//Set the text of imageButton to image's name
		imageButton->Text = ExtractFileName(fileName);
	}

	//Project has changed
	isModified = true;
	codeView->changesMade = true;

	//Free memory
	delete temp;
}


//--------------------------------------------------------
//
//Process image setting from Image Sheet on Window
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::loadImageFromSheet()
{
	if (isAnimating)
		return;

	TList *temp = new TList();

	//Choose who gets the requested changes: Supports multi-selections
	if (selectionRect->TagObject)
	{
		//Absolutely NO TTextEx types
		if (selectionRect->ClassType()!= __classid(TTextEx))
			temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			//Absolutely NO TTextEx types
			if (c)
				temp->Add(c);
		}
	}

	//Depending on the popup location, some objects may
	//retain a resize cursor by accident. Need to clear that up.
	clearAllCursors(deviceScreen);


	//
	//We have objects to use: NO TTextEx OBJECTS PERIOD!!!
	//
	//if (temp->Count && texAtlasList->Count)
	if (temp->Count)
	{
		//Enable image assignment to a Rectangle object
		TBitmap *bmp        = new TBitmap();
		int      imageIndex = -1;

		TtexAtlasForm *tex = new TtexAtlasForm(solar2DForm, bmp, &atlasFile, &imageIndex);
		tex->ShowModal();
		tex->DisposeOf();

		//A selection was made?
		if (!bmp->IsEmpty())
		{
			for (int i = 0; i < temp->Count; i++)
			{
				TRectangleEx *c = (TRectangleEx *)temp->Items[i];

				//Editing a control's values: Only TRectangles allowed
				if (c)
				{
					//image list set
					c->Fill->Bitmap->Bitmap->Assign(bmp);
					c->Fill->Bitmap->WrapMode = TWrapMode::TileOriginal;

					//Hide all color
					c->oldColor = c->Fill->Color;
					c->Fill->Color = TAlphaColorRec::Null;
					c->Fill->Kind = TBrushKind::Solid;
					c->TagString = "";
					//Image sheet type
					c->imageType = TRectangleEx::ImageSheet;
					c->TagString = atlasFile;
					c->imageIndex = imageIndex;
				}
			}

			//Set the text of imageButton to blank
			imageButton->Text = "";
			delete bmp;
		}
	}

	//Project has changed
	isModified = true;
	codeView->changesMade = true;

	//Free memory
	delete temp;
}


void __fastcall Tsolar2DForm::reloadImageFromSheet(TRectangleEx *c, AnsiString fileName, int index)
{
	if (isAnimating)
		return;

	//Enable image assignment to a Rectangle object
	TBitmap *      bmp        = new TBitmap();
	int            imageIndex = index;
	TtexAtlasForm *tex        = new TtexAtlasForm(solar2DForm, NULL, NULL, NULL);
	tex->getTexture(fileName, bmp, index);

	//A selection was made?
	if (!bmp->IsEmpty())
	{
		//Editing a control's values: Only TRectangles allowed
		if (c)
		{
			//image list set
			c->Fill->Bitmap->Bitmap->Assign(bmp);
			c->Fill->Bitmap->WrapMode = TWrapMode::TileOriginal;

			//Hide all color
			c->oldColor = c->Fill->Color;
			c->Fill->Color = TAlphaColorRec::Null;
			c->Fill->Kind = TBrushKind::Solid;
			c->TagString = "";
			//Image sheet type
			c->imageType = TRectangleEx::ImageSheet;
			c->TagString = fileName;
			c->imageIndex = imageIndex;
		}
	}

	//Destroy the instance as it is no longer needed
	tex->DisposeOf();
}


//--------------------------------------------------------
//
//9-Patch: Calculate the offsets
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::calculate9PatchOffsets(TRectangleEx *c, int topX, int sideY, int topW, int sideH)
{
	//Build the mapping data using these 4 values
	TBitmap *bm  = c->Fill->Bitmap->Bitmap;
	TRectF   img = bm->BoundsF;

	//Offset the Rects to actual image data and not 9-Patch lines
	//Adjust img Rect values to skip the 9-Patch pixels
	img.left++;
	img.right--;
	img.top++;
	img.bottom--;


	if (bm)
	{
		//----------------------------
		//Top left tile
		//----------------------------
		//Rect( Left, Top, Width, Height )
		c->TL = Rect(img.left, img.top, topX, sideY);
		//Top center tile
		c->TC = Rect((img.left + topX), img.top, (img.left + topX + topW), sideY);
		//Top right tile
		c->TR = Rect((img.left + topX + topW), img.top, img.right, sideY);

		//----------------------------
		//center left tile
		//----------------------------
		c->L = Rect(img.left, (img.top + sideY), topX, (img.top + sideY + sideH));
		//center tile
		c->C = Rect((img.left + topX), (img.top + sideY), (img.left + topX + topW), (img.top + sideY + sideH));
		//center right tile
		c->R = Rect((img.left + topX + topW), (img.top + sideY), img.right, (img.top + sideY + sideH));

		//----------------------------
		//bottom left tile
		//----------------------------
		c->BL = Rect(img.left, (img.top + sideY + sideH), topX, img.bottom);
		//c->BL = RectF(0+ x, sideY + sideH+ y, topX, sideY);
		// //bottom center tile
		c->BC = Rect((img.left + topX), (img.top + sideY + sideH), (img.left + topX + topW), img.bottom);
		//c->BC = RectF(topX+ x, sideY + sideH+ y, topW, sideY);
		// //bottom right tile
		c->BR = Rect((img.left + topX + topW), (img.top + sideY + sideH), img.right, img.bottom);
		//c->BR = RectF(topX + topW+ x, sideY + sideH+ y, topX, sideY);

		//If it makes it this far, it is good to go!
		//9-Patch type
		c->imageType = TRectangleEx::NinePatch;
		c->imageIndex = -1;
	}
}


//--------------------------------------------------------
//
//9-Patch: Load image data
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::load9PatchImage(TRectangleEx *c, AnsiString fileName)
{
	if (fileName != "" && !FileExists(fileName))
	{
		ShowMessage("Image Load Error: '" + fileName + "' was not found.");
		return;
	}

	if (c && fileName != "")
	{
		c->Fill->Bitmap->Bitmap->LoadFromFile(fileName);
		c->Fill->Bitmap->WrapMode = TWrapMode::TileOriginal;

		//Hide all color
		c->oldColor = c->Fill->Color;
		c->Fill->Color = TAlphaColorRec::Null;
		c->Fill->Kind = TBrushKind::Solid;
		//

		//Try and get the pixel data that indicates the offsets
		//for the Top and side 'stretchable' areas
		TBitmapData bm;
		TBitmap *src = c->Fill->Bitmap->Bitmap;

		//Change the dimensions to the preferred size
		//src->Resize(src->Width >> 1, src->Height>> 1);

		TAlphaColor color;
		int topX       = -1;
		int topWidth   = -1;
		int sideY      = -1;
		int sideHeight = -1;

		//Get bitmap data access !
		if (src && src->Map(TMapAccess::Read, bm))
		{
			unsigned int *data = (unsigned int *)bm.Data;

			//Test direct pixel access here
			//Get the top row of pixels first
			for (int x = 0; x < src->Width; x++)
			{
				//Get a color to test
				color = bm.GetPixel(x, 0);

				if (color)
				{
					//If positive value, increment to get final width
					if (topWidth > -1)
						topWidth++;

					//Set the initialsfor these variables
					if (topX == -1)
					{
						topX = x;
						topWidth = 1;
					}
				}
			}

			//Get the side column of pixels last
			for (int y = 0; y < src->Height; y++)
			{
				//Get a color to test
				color = bm.GetPixel(0, y);

				if (color)
				{
					//If positive value, increment to get final width
					if (sideHeight > -1)
						sideHeight++;

					//Set the initials for these variables
					if (sideY == -1)
					{
						sideY = y;
						sideHeight = 1;
					}
				}
			}

			//Now write back the result !
			src->Unmap(bm);

			//if this is a good 9-Patch file, all the info needed is in
			//topX/W  and sideY/H
			if (topX > -1 && sideY > -1 && topWidth > -1 && sideHeight > -1)
				calculate9PatchOffsets(c, topX, sideY, topWidth, sideHeight);
			else
			{
				c->Fill->Kind = TBrushKind::Bitmap;
				fileName == "";
			}

			//Save the name for access
			c->TagString = fileName;
		}
		else
		{
			ShowMessage("Could not map the image data for direct access.");
		}
	}
}


//--------------------------------------------------------
//
//9-Patch drawing support for Windows loaded with image
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::object9PatchPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect)
{
	TRectangleEx *c = dynamic_cast<TRectangleEx *>(Sender);

	if (c)
	{
		//Type 1 = 9Patch
		if ((c->imageType == 0 || c->imageType == 2) && !c->Fill->Bitmap->Bitmap->IsEmpty() && c->Fill->Kind != TBrushKind::Bitmap)
		{
			Canvas->DrawBitmap(
				c->Fill->Bitmap->Bitmap,
				c->Fill->Bitmap->Bitmap->Bounds,
				ARect,
				c->Opacity, false);
		}
		else if (c->imageType == 1 && !c->Fill->Bitmap->Bitmap->IsEmpty() && c->Fill->Kind != TBrushKind::Bitmap)
		{
			//Override the Bitmap drawing to support 9-Patch
			int boxWidth  = c->Width - (c->TL.Width() + c->TR.Width());
			int boxHeight = c->Height - (c->TL.Height() + c->BL.Height());
			int X         = 0;
			int Y         = 0;

			TRectF dest;
			TBitmap *src = (c->Fill->Bitmap->Bitmap);


			//################################
			//
			//Top left corner
			//
			//################################
			dest = Rect(X, Y, c->TL.Width(), c->TL.Height());
			Canvas->DrawBitmap(src, c->TL, dest, c->Opacity, false);
			X += c->TL.Width();

			//--------------------------------
			//Top center
			//--------------------------------
			//copy stretched image to Window
			dest = Rect(X, Y, (X + boxWidth), c->TC.Height());
			Canvas->DrawBitmap(src, c->TC, dest, c->Opacity, false);
			X += boxWidth;

			//--------------------------------
			//Top right corner
			//--------------------------------
			dest = Rect(X, Y, (X + c->TR.Width()), c->TR.Height());
			Canvas->DrawBitmap(src, c->TR, dest, c->Opacity, false);
			X = 0;
			Y += c->TL.Height();


			//################################
			//
			//left
			//
			//################################
			dest = Rect(X, Y, c->L.Width(), (Y + boxHeight));
			Canvas->DrawBitmap(src, c->L, dest, c->Opacity, false);
			X += c->L.Width();

			//--------------------------------
			//center
			//--------------------------------
			//copy stretched image to Window
			dest = Rect(X, Y, (X + boxWidth), (Y + boxHeight));
			Canvas->DrawBitmap(src, c->C, dest, c->Opacity, false);
			X += boxWidth;


			//--------------------------------
			//right
			//--------------------------------
			dest = Rect(X, Y, (X + c->R.Width()), (Y+ boxHeight));
			Canvas->DrawBitmap(src, c->R, dest, c->Opacity, false);
			X = 0;
			Y += boxHeight;


			//################################
			//
			//Bottom left corner
			//
			//################################
			dest = Rect(X, Y, c->BL.Width(), (Y + c->BL.Height()));
			Canvas->DrawBitmap(src, c->BL, dest, c->Opacity, false);
			X += c->BL.Width();

			//--------------------------------
			//Top center
			//--------------------------------
			//copy stretched image to Window
			dest = Rect(X, Y, (X + boxWidth), (Y + c->BC.Height()));
			Canvas->DrawBitmap(src, c->BC, dest, c->Opacity, false);
			X += boxWidth;

			//--------------------------------
			//Top right corner
			//--------------------------------
			dest = Rect(X, Y, (X + c->BR.Width()), (Y + c->BR.Height()));
			Canvas->DrawBitmap(src, c->BR, dest, c->Opacity, false);
		}
	}

}


//--------------------------------------------------------
//
//Make 9-Patch stretchable part adjustmens
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectResize(TObject *Sender)
{
	TRectangleEx *c = dynamic_cast<TRectangleEx *>(Sender);

	//Active ONLY when 9-Patch files are loaded in the image
	if (c && !c->Fill->Bitmap->Bitmap->IsEmpty())
	{
		if (c->imageType == 1)
		{
			if (c->Width < c->Fill->Bitmap->Bitmap->Width)
				c->Width = c->Fill->Bitmap->Bitmap->Width;

			if (c->Height < c->Fill->Bitmap->Bitmap->Height)
				c->Height = c->Fill->Bitmap->Bitmap->Height;
		}
	}
}


//--------------------------------------------------------
//
//Change Window colors: Supports multi-selections
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectColorSelectChange(TObject *Sender)
{
	if (isAnimating || !allowObjectUpdates)
		return;

	TColorComboBox *cb   = dynamic_cast<TColorComboBox *>(Sender);
	TList *         temp = new TList();

	//Choose who gets the requested changes: Supports multi-selections
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
				temp->Add(c);
		}
	}


	//Process request for all objects
	if (cb && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TControl *c = (TControl *)temp->Items[i];

			//Editing a control's values
			if (c)
			{
				switch(cb->Tag)
				{
					//Fill color
				case 10:
					if (c->ClassType() == __classid(TRectangleEx))
					{
						((TRectangleEx *)c)->Fill->Color = cb->Color;
					}
					//
					break;
					//
				case 14:
					//Font color
					if (c->ClassType() == __classid(TTextEx))
					{
						((TTextEx *)c)->TextSettings->FontColor = cb->Color;
					}
					break;
				}
			}
		}

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}
	//Free memory
	delete temp;
}


void __fastcall Tsolar2DForm::nameEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift)
{
	//Prevent unneccesary triggers
	if (Key == 13)
	{
		nameEdit->BeginUpdate();
		AnsiString temp = nameEdit->Text;
		nameEdit->Text = "";
		nameEdit->Tag = 1;
		nameEdit->EndUpdate();
		nameEdit->Text = temp;
	}
}


void __fastcall Tsolar2DForm::nameEditChange(TObject *Sender)
{
	if (nameEdit->Tag != 1 || isAnimating || !allowObjectUpdates)
		return;

	//User hit the ENTER key
	nameEdit->Tag = 0;

	//Editing a text object
	TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);

	if (c)
	{
		//Make sure there are no duplicates
		//Not case sensitive. Any matching case name is duplicate
		//if not found or not applicable, return -1
		TList *      objs = NULL;
		TStringList *list = NULL;

		try
		{
			list = new TStringList();
			objs = new TList();

			for (int i = 0; i < deviceScreen->ChildrenCount - 1; i++)
			{
				//loop through all children   list
				if ((deviceScreen->Children->Items[i] && deviceScreen->Children->Items[i] != c) &&
					(deviceScreen->Children->Items[i]->ClassType() == __classid(TTextEx) || deviceScreen->Children->Items[i]->ClassType() == __classid(TRectangleEx)))
				{
					if (((TControl*)deviceScreen->Children->Items[i])->HitTest)
					{
						objs->Add(deviceScreen->Children->Items[i]);
						list->Add(deviceScreen->Children->Items[i]->Name);
					}
				}
			}
		}
		catch(...)//ignore all errors
		{
		}

		//Check for duplicates
		for (int i = 0; i < list->Count; i++)
		{
			if (UpperCase(nameEdit->Text) == UpperCase(list->Strings[i]))
			{
				ShowMessage("Error: Name already exist. No duplicates allowed");
				delete list;
				return;
			}
		}


		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		//No duplicates were found so continue
		AnsiString oldName = c->Name;
		c->Name = nameEdit->Text;

		//Fix the references in Alignment settings
		for (int i = 0; i < objs->Count; i++)
		{
			TControl *obj = (TControl *)objs->Items[i];
			UIObject *o   = (UIObject *)obj->Tag;

			if (o)
			{
				//Fix all references to Alignment controls per object
				AnsiString *array[10] =
				{
					&o->aboveCombo,
					&o->bottomWithCombo,
					&o->horiWithCombo,
					&o->leftWithCombo,
					&o->rightWithCombo,
					&o->topWithCombo,
					&o->vertWithCombo,
					&o->belowCombo,
					&o->leftOfCombo,
					&o->rightOfCombo
				};

				for (int y = 0; y < 10; y++)
				{
					if (UpperCase(oldName) == UpperCase(*array[y]))
					{
						*array[y] = nameEdit->Text;
					}
				}
			}

			//
			updateObjectData();
		}

		//Project has changed
		isModified = true;
		codeView->changesMade = true;

		//Clean memory
		delete list;
		delete objs;
	}
}


void __fastcall Tsolar2DForm::visibleCheckChange(TObject *Sender)
{
	if (isAnimating || !allowObjectUpdates)
		return;

	//Changing visiblity to objects: Supports multi-selections
	TCheckBox *cb = dynamic_cast<TCheckBox *>(Sender);

	if (cb)
	{
		cb->Text = (cb->IsChecked ? " True " : " False ");
	}
	else
		return;

	//All is well
	TList *temp = new TList();

	//Choose who gets the requested changes
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
				temp->Add(c);
		}
	}


	//Process request for all objects
	if (cb && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TControl *c = (TControl *)temp->Items[i];

			if (c)
			{
				UIObject *o = (UIObject *)c->Tag;

				if (!o)
				{
					o = new UIObject();
					memset(o, 0, sizeof(UIObject));
					o->Visible = true;
					c->Tag = (long)o;
				}

				o->Visible = cb->IsChecked;
				//c->Visible = cb->IsChecked;
			}
		}

		//Clean up the dirt
		deviceScreen->Repaint();

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}

	//
	cb = NULL;

	//Free memory
	delete temp;
}


//--------------------------------------------------------
//
//Change text message: Support multi-selections
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::enterTextChange(TObject *Sender)
{
	if (isAnimating)
		return;

	//Editing a text object
	TList *temp = new TList();

	//Choose who gets the requested changes
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c && c->ClassType() == __classid(TTextEx))
				temp->Add(c);
		}
	}


	//Process request for all objects
	if (temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TTextEx *c = (TTextEx *)temp->Items[i];

			if (c)
			{
				UIObject *o = (UIObject *)c->Tag;
				if (!o)
				{
					o = new UIObject();
					memset(o, 0, sizeof(UIObject));
					o->Visible = true;
					c->Tag = (long)o;
				}

				if (o)
				{
					c->Text = AnsiString(enterText->Text);
				}
			}
		}

		//Clean up the dirt
		deviceScreen->Repaint();

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}

	//Free memory
	delete temp;
}


void __fastcall Tsolar2DForm::alphaNumClick(TObject *Sender)
{
	if (Sender->ClassType() == __classid(TNumberBox))
	{
		TNumberBox *nb = dynamic_cast<TNumberBox *>(Sender);

		if (nb)
		{
			nb->SelStart = 0;
			nb->SelLength = nb->Text.Length();
		}
	}
	else if (Sender->ClassType() == __classid(TEdit))
	{
		TEdit *nb = dynamic_cast<TEdit *>(Sender);

		if (nb)
		{
			nb->SelStart = 0;
			nb->SelLength = nb->Text.Length();
		}
	}
}


//--------------------------------------------------------
//
//Alignment Controls: Checkboxes support multi-selections
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::controlTreeClick(TObject *Sender)
{
	TTreeView *item = dynamic_cast<TTreeView *>(Sender);

	if (item)
	{//item->Selected

		//for ( int i = 0; i < items->Count
		TControl *c = findObjectByName(deviceScreen, item->Selected->Text);

		if (c)
		{
			selectionRect->TagObject = c;
			adjustHandles(true, NULL);
			updateObjectData();
			updateInspector();
			alignObjects();

			//Clear all selections
			clearObjectSelections();

			//Clean up the dirt
			deviceScreen->Repaint();
		}
	}
}


void __fastcall Tsolar2DForm::alignWithChange(TObject *Sender)
{
	if (!allowObjectUpdates)
		return;

	//Mormal process allowed
	TCheckBox *cb     = dynamic_cast<TCheckBox *>(Sender);
	TObject *  srTemp = selectionRect->TagObject;

	if (cb)
	{
		cb->Text = (cb->IsChecked ? " True " : " False ");
	}
	else
		return;

	//All is well
	TList *temp = new TList();

	//Choose who gets the requested changes
	if (selectionRect->TagObject)
	{
		temp->Add(selectionRect->TagObject);
	}
	else if (selectionsList->Count)
	{
		for (int i = 0; i < selectionsList->Count; i++)
		{
			TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

			if (c)
				temp->Add(c);
		}
	}


	//Process request for all objects
	//Only allow updates from user inteeface
	if (cb && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		for (int i = 0; i < temp->Count; i++)
		{
			TControl *c = (TControl *)temp->Items[i];

			if (c)
			{
				UIObject *o = (UIObject *)c->Tag;
				if (!o)
				{
					o = new UIObject();
					memset(o, 0, sizeof(UIObject));
					o->Visible = true;
					c->Tag = (long)o;
				}

				if (o)
				{
					o->bottomParentChk = bottomParentChk->IsChecked;
					o->horiParentChk = horiParentChk->IsChecked;
					o->leftParentChk = leftParentChk->IsChecked;
					o->rightParentChk = rightParentChk->IsChecked;
					o->topParentChk = topParentChk->IsChecked;
					o->vertParentChk = vertParentChk->IsChecked;
					o->anchorLeftChk = anchorLeftChk->IsChecked;
					o->anchorTopChk = anchorTopChk->IsChecked;
					o->anchorRightChk = anchorRightChk->IsChecked;
					o->anchorBottomChk = anchorBottomChk->IsChecked;

					//TEmp assign to reflect changes
					selectionRect->TagObject = c;

					//Show changes
					alignObjects();

					//Remove assignment
					selectionRect->TagObject = NULL;
				}
			}
		}

		//Restore selectionRect'schild
		selectionRect->TagObject = srTemp;

		//Clean up the dirt
		deviceScreen->Repaint();

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}

	//Free memory
	delete temp;
}


void __fastcall Tsolar2DForm::aboveComboChange(TObject *Sender)
{
	//Show selection
	if (selectionRect->TagObject)
	{
		TComboBox *b = dynamic_cast<TComboBox *>(Sender);
		TControl * c = dynamic_cast<TControl*>(selectionRect->TagObject);

		if (c && b)
		{
			//Add this  to the stack BEFORE change is made
			updateUndoStack("", false);

			//
			if (b->ItemIndex > -1)
			{
				UIObject *o = (UIObject *)c->Tag;

				if (!o)
				{
					o = new UIObject();
					memset(o, 0, sizeof(UIObject));
					o->Visible = true;
					c->Tag = (long)o;
				}

				AnsiString *array[10] =
				{
					&o->aboveCombo,
					&o->bottomWithCombo,
					&o->horiWithCombo,
					&o->leftWithCombo,
					&o->rightWithCombo,
					&o->topWithCombo,
					&o->vertWithCombo,
					&o->belowCombo,
					&o->leftOfCombo,
					&o->rightOfCombo
				};

				//No connection wanted
				if (b->ItemIndex == 0)
				{
					*array[b->Tag] = "";
					b->ItemIndex = -1;
					//updateObjectData();
					alignObjects();

					return;
				}

				//Set the objects alignment string to selected combobox string
				*array[b->Tag] = b->Items->Strings[b->ItemIndex];
			}
		}

		//updateObjectData();
		alignObjects();

		//Project has changed
		isModified = true;
		codeView->changesMade = true;
	}
}


void __fastcall Tsolar2DForm::showAlignmentSelections(TObject *Sender)
{
	if (selectionRect->TagObject)
	{
		TControl *parent = dynamic_cast<TControl *>(Sender);
		TControl *c      = dynamic_cast<TControl*>(selectionRect->TagObject);

		if (c && parent)
		{
			UIObject *o = (UIObject *)c->Tag;

			if (!o)
			{
				o = new UIObject();
				memset(o, 0, sizeof(UIObject));
				o->Visible = true;
				c->Tag = (long)o;

				//Nothing to do here
				return;
			}

			AnsiString *array[10] =
			{
				&o->aboveCombo,
				&o->bottomWithCombo,
				&o->horiWithCombo,
				&o->leftWithCombo,
				&o->rightWithCombo,
				&o->topWithCombo,
				&o->vertWithCombo,
				&o->belowCombo,
				&o->leftOfCombo,
				&o->rightOfCombo
			};

			TComboBox *boxes[10] =
			{
				aboveCombo,
				bottomWithCombo,
				horiWithCombo,
				leftWithCombo,
				rightWithCombo,
				topWithCombo,
				vertWithCombo,
				belowCombo,
				leftOfCombo,
				rightOfCombo
			};

			//Set the active selectin per box
			for (int i = 0; i < 10; i++)
			{
				boxes[i]->ItemIndex = boxes[i]->Items->IndexOf(*array[i]);
			}
		}
	}
}


//--------------------------------------------------------
//
//Method to align all children objects
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::setAllAlignments(TControl *DeviceParent)
{
	try
	{
		TObject *temp = selectionRect->TagObject;

		for (int i = 0; i < DeviceParent->ChildrenCount; i++)
		{
			//loop through all children   ((TControl *)selectionRect->TagObject)->Name
			if (DeviceParent->Children->Items[i] &&
				(DeviceParent->Children->Items[i]->ClassType() == __classid(TTextEx) || DeviceParent->Children->Items[i]->ClassType() == __classid(TRectangleEx)))
			{
				TControl *child = dynamic_cast<TControl *>(DeviceParent->Children->Items[i]);

				if (child && child->HitTest)
				{
					//Take crae of sub children first
					if (child->ChildrenCount)
					{
						//Recursive method
						setAllAlignments(child);
					}

					//Meh!
					selectionRect->TagObject = DeviceParent->Children->Items[i];
					//Re-align all objects, slowly... :(
					alignObjects();
				}
			}
		}

		selectionRect->TagObject = temp;

	}
	catch(...)//ignore all errors
	{

	}
}


void __fastcall Tsolar2DForm::alignObjects()
{
	//Still needs work
	//if not found or not applicable, return -1
	if (selectionRect->TagObject)
	{
		TControl *c     = dynamic_cast<TControl *>(selectionRect->TagObject);
		TControl *p     = dynamic_cast<TControl *>(c->Parent);
		UIObject *o     = (UIObject *)c->Tag;
		TControl *other = NULL;

		if (c && p && o)
		{
			//Get a list of the objects that can be aligned to
			TControl *aboveCombo      = findObjectByName(c->Parent, o->aboveCombo);
			TControl *bottomWithCombo = findObjectByName(c->Parent, o->bottomWithCombo);
			TControl *horiWithCombo   = findObjectByName(c->Parent, o->horiWithCombo);
			TControl *leftWithCombo   = findObjectByName(c->Parent, o->leftWithCombo);
			TControl *rightWithCombo  = findObjectByName(c->Parent,o->rightWithCombo);
			TControl *topWithCombo    = findObjectByName(c->Parent,o->topWithCombo);
			TControl *vertWithCombo   = findObjectByName(c->Parent,o->vertWithCombo);
			TControl *belowCombo      = findObjectByName(c->Parent,o->belowCombo);
			TControl *leftOfCombo     = findObjectByName(c->Parent,o->leftOfCombo);
			TControl *rightOfCombo    = findObjectByName(c->Parent,o->rightOfCombo);
			bool      vertParentChk   = o->vertParentChk;
			bool      bottomParentChk = o->bottomParentChk;
			bool      topParentChk    = o->topParentChk;
			bool      horiParentChk   = o->horiParentChk;
			bool      rightParentChk  = o->rightParentChk;
			bool      leftParentChk   = o->leftParentChk;
			bool      anchorLeftChk   = o->anchorLeftChk;
			bool      anchorTopChk    = o->anchorTopChk;
			bool      anchorRightChk  = o->anchorRightChk;
			bool      anchorBottomChk = o->anchorBottomChk;


			//This section is the most important thing that needs to be kept in mind when using relative positioning.
			//The process of positioning objects relatively executes in the following order:
			//1. Align-Object-With-Parent: Any "Align to Parent" properties takes highest precedence
			//
			//-	Centering: Align-Object-Center-With-Parent, has priority over side alignments.
			//-	Left has priority over right, top over bottom.
			//
			//2. Align-Object-With: The rest of the "Align" properties execute before "Parent alignments".
			//-	Centering: Align-Object-Center-With, has priority over side alignments.
			//-	Left has priority over right, top over bottom.
			//
			//3. Side alignments:
			//-	1.	LeftOf
			//-	2.	RightOf
			//-	3.	Above
			//-	4.	Below
			//-	Left has priority over right, above over below.

			//-----------------------------------------
			//3: Aligning to sides: lowest Precedence
			//-----------------------------------------
			if (belowCombo)
			{
				other = dynamic_cast<TControl *>(belowCombo);
				if (other)
					c->Position->Y = other->BoundsRect.Bottom + 1;
			}
			if (aboveCombo)
			{
				other = dynamic_cast<TControl *>(aboveCombo);
				if (other)
					c->Position->Y = (other->BoundsRect.Top - c->Height) - 1;
			}
			if (rightOfCombo)
			{
				other = dynamic_cast<TControl *>(rightOfCombo);
				if (other)
					c->Position->X = other->BoundsRect.Right + 1;
			}
			if (leftOfCombo)
			{
				other = dynamic_cast<TControl *>(leftOfCombo);
				if (other)
					c->Position->X = (other->BoundsRect.Left - c->Width) - 1;
			}

			//-----------------------------------------
			//2: Aligning to others: Middle Precedence
			//-----------------------------------------
			if (bottomWithCombo)
			{
				other = dynamic_cast<TControl *>(bottomWithCombo);
				if (other)
					c->Position->Y = (other->BoundsRect.Bottom - c->Height);
			}
			if (topWithCombo)
			{
				other = dynamic_cast<TControl *>(topWithCombo);
				if (other)
					c->Position->Y = other->Position->Y;
			}
			if (vertWithCombo)
			{
				other = dynamic_cast<TControl *>(vertWithCombo);
				if (other)
					c->Position->Y = other->Position->Y + ((other->Height / 2) - (c->Height / 2));
			}
			//
			if (rightWithCombo)
			{
				other = dynamic_cast<TControl *>(rightWithCombo);
				if (other)
					c->Position->X = (other->BoundsRect.Right - c->Width);
			}
			if (leftWithCombo)
			{
				other = dynamic_cast<TControl *>(leftWithCombo);
				if (other)
					c->Position->X = other->Position->X;
			}
			if (horiWithCombo)
			{
				other = dynamic_cast<TControl *>(horiWithCombo);
				if (other)
					c->Position->X = other->Position->X + ((other->Width / 2) - (c->Width / 2));
			}

			//-----------------------------------------
			//1: Aligning to parents: Highest Precedence
			//-----------------------------------------
			if (vertParentChk)
				c->Position->Y = (p->Height / 2) - (c->Height / 2);
			else
			{
				if (bottomParentChk)
					c->Position->Y = (p->Height - c->Height);
				if (topParentChk)
					c->Position->Y = 0;
			}

			if (horiParentChk)
				c->Position->X = (p->Width / 2) - (c->Width / 2);
			else
			{
				if (rightParentChk)
					c->Position->X = (p->Width - c->Width);
				if (leftParentChk)
					c->Position->X = 0;
			}

			//-----------------------------------------
			//0: Anchoring to parents: Doesn't matter
			//-----------------------------------------
			//Clear all by default
			//c->Anchors = (TAnchors() >> TAnchorKind::akLeft >> TAnchorKind::akTop >> TAnchorKind::akRight >> TAnchorKind::akBottom);

			//Note: Section is inactive due to Anchor Right / Down causing strange affects
			//Add per request
			if (anchorLeftChk)
				c->Anchors += (TAnchors() << TAnchorKind::akLeft);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akLeft);

			if (anchorTopChk)
				c->Anchors += (TAnchors() << TAnchorKind::akTop);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akTop);

			if (anchorRightChk)
				c->Anchors += (TAnchors() << TAnchorKind::akRight);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akRight);

			if (anchorBottomChk)
				c->Anchors += (TAnchors() << TAnchorKind::akBottom);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akBottom);

			//Only if not adjusting multi-selections do we show the handles
			if (!selectionsList->Count && selectionRect->TagObject)
				adjustHandles(true, NULL);
		}
	}
}


//--------------------------------------------------------
//
//Popup Menu: Supports multi-selections
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::devicePopupPopup(TObject *Sender)
{
	TPopupMenu *m = dynamic_cast<TPopupMenu *>(Sender);
	TControl *  c = dynamic_cast<TControl *>(m->PopupComponent);

	//Clean up residue effects from mouse clicking
	inSizeMode = false;

	//Try and get the caller
	if (m && c)
	{
		setBG->Enabled = false;
		clearBG->Enabled = false;
		setBG->Visible = false;
		clearBG->Visible = false;
		//
		clearImage->Enabled = false;
		clearImage->Visible = false;
		setImage->Enabled = false;
		setImage->Visible = false;
		setImageSheet->Enabled = false;
		setImageSheet->Visible = false;
		//
		addAnimation->Enabled = false;

		//Cut & Paste of multiple items [ Copy is an issue with multiple levls of objects ]
		//May re-visit 'Copy' in the future
		cutObj->Visible = true;
		//copyObj->Visible = true;
		cutObj->Enabled = (c->Name != "deviceScreen");
		//copyObj->Enabled = (c->Name != "deviceScreen");
		pasteObj->Enabled = (editList->Count);

		//
		//TTextEx objects will have limited functions
		//
		if (c->Name != "deviceScreen")
		{
			setImage->Enabled = true;
			clearImage->Enabled = true;
			setImage->Visible = true;
			clearImage->Visible = true;
			setImageSheet->Enabled = true;
			setImageSheet->Visible = setImageSheet->Enabled;
			//
			addAnimation->Enabled = true;

			//Multi-selection?
			if (!selectionsList->Count)
			{
				selectionRect->TagObject = c;
				adjustHandles(true, NULL);
			}
			else
			{//
				selectionRect->TagObject = NULL;
				adjustHandles(false, NULL);
			}

			//Build animation list
			buildAnimationList(c);
			//Select a default item
			if (animListBox->Count)
			{
				animListBox->ItemIndex = 0;
				animListBoxClick(NULL);
			}
		}
		else
		{
			//Devicescreen does not support mullti-selection
			clearObjectSelections();
			//Clear animation list
			animListBox->Clear();
			//Clear allthe object inspector data
			clearAnimationInspector();

			//
			setBG->Enabled = true;
			clearBG->Enabled = true;
			setBG->Visible = true;
			clearBG->Visible = true;
			cutObj->Visible = false;
			copyObj->Visible = false;
			//
			selectionRect->TagObject = NULL;
			adjustHandles(false, NULL);
		}
	}

}


void __fastcall Tsolar2DForm::addNewTextClick(TObject *Sender)
{
	//0 = Set Background image for device screen
	//1 = Clear Background image for device screen
	//2 = Create new window
	//3 = Create new text
	//4 = Set 9-Patch image to window
	//5 = Set image from file to window
	//6 = Set image from texture atlas to window
	//7 = Clear image from window
	//8 = Cut object from designer
	//9 = Copy an object in the designer
	//10 = Paste an object to the designer
	TMenuItem *   m     = dynamic_cast<TMenuItem *>(Sender);
	TRectangleEx *ARect = NULL;
	TTextEx *     AText = NULL;
	TList *       temp  = new TList();

	//-------------------------------
	//
	//Must be deviceScreen
	//
	//-------------------------------
	if (!selectionRect->TagObject && !selectionsList->Count)
	{
		//Transfer to temp buffer
		temp->Add(deviceScreen);
	}
	else
	{
		//Single clicked object
		if (selectionRect->TagObject && !selectionsList->Count)
		{
			//TTextEx NOT SUPPORTED in most cases
			if (selectionRect->TagObject->ClassType() == __classid(TTextEx) && m->Tag != 7 && m->Tag != 8)
			{
				//Free up memory
				delete temp;
				return;
			}

			//Transfer to temp buffer
			temp->Add(selectionRect->TagObject);
		}
		//Multiple objects: All of these features only support TRectangle(Ex)s
		else if (selectionsList->Count)
		{
			//Transfer to temp buffer
			for (int i = 0; i < selectionsList->Count; i++)
			{
				TControl *c = dynamic_cast<TControl *>(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[i])));

				//TTextEx NOT SUPPORTED in most cases
				if (c->ClassType() != __classid(TTextEx) || (c->ClassType() == __classid(TTextEx) && (m->Tag == 6 || m->Tag == 7)))
					temp->Add(c);
			}
		}
	}


	//-----------------------------------------------
	//
	//Process the request
	//
	//-----------------------------------------------
	if (m && temp->Count)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);


		//--------------------------------------------
		//
		//Iterate thru all children selected or just
		//the deviceScreen / single item
		//
		//--------------------------------------------
		for (int i = 0; i < temp->Count; i++)
		{
			TControl *c         = (TControl *)temp->Items[i];
			int       winWidth  = ((TControl *)c->Parent)->Width;
			int       winHeight = ((TControl *)c->Parent)->Height;

			if (c)
			{
				switch(m->Tag)
				{
					//Set Background image: deviceScreen ONLY
				case 0:
					FOpen->Filter = TBitmapCodecManager::GetFilterString();
					if (FOpen->InitialDir == "")
						FOpen->InitialDir = GetCurrentDir();

					//Try to open dialog
					if (FOpen->Execute())
					{
						deviceScreen->Fill->Bitmap->Bitmap->LoadFromFile(FOpen->FileName);
						deviceScreen->TagString = FOpen->FileName;
						deviceScreen->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
						deviceScreen->Fill->Kind = TBrushKind::Bitmap;

						//Depending on the popup location, some objects may
						//retain a resize cursor by accident. Need to clear that up.
						clearAllCursors(deviceScreen);

						//Set as active directory
						FOpen->InitialDir = ExtractFilePath(FOpen->FileName);

						//Open dialog clicks will move the frame. Disable
						deviceFrame->Tag = 1;
					}

					//Only deviceScreen can attach a background image
					i = temp->Count;

					break;
				case 1:
					//Clear Background image: deviceScreen ONLY
					deviceScreen->Fill->Bitmap->Bitmap = NULL;
					deviceScreen->Fill->Color = TAlphaColorRec::Black;
					deviceScreen->Fill->Kind = TBrushKind::Solid;
					break;
				case 2:
					//New window: Should not be here if inactive
					ARect = (TRectangleEx *)createNewControl(c, "RECT");
					selectionRect->TagObject = ARect;
					adjustHandles(true,NULL);
					updateInspector();
					break;
				case 3:
					//New text: Should not be here if inactive
					AText = (TTextEx *)createNewControl(c, "TEXT");
					selectionRect->TagObject = AText;
					adjustHandles(true,NULL);
					updateInspector();
					break;
				case 4:
					//Load a image from file: Window objects only
					//Use existing code
					imageButtonClick(NULL);

					//Exit after completion
					i = temp->Count;
					break;
				case 5:
					//Load a image from Image Sheet: Window objects only
					//Using an Image sheet
					loadImageFromSheet();

					//Exit after completion
					i = temp->Count;
					break;
				case 6:
					//Clear image: Window objects only
					((TRectangleEx *)c)->Fill->Bitmap->Bitmap->Width = 0;
					((TRectangleEx *)c)->Fill->Bitmap->Bitmap->Height = 0;
					((TRectangleEx*)c)->Fill->Color = ((TRectangleEx*)c)->oldColor;
					((TRectangleEx*)c)->Fill->Kind = TBrushKind::Solid;

					//Must erase the name for 'Code Export' feature to NOT think an image exist
					c->TagString = "";
					((TRectangleEx*)c)->imageIndex = -1;
					((TRectangleEx*)c)->imageType = -1;
					break;
				case 7:
					//Cut object
					editList->editCut(c);
					adjustHandles(false,NULL);
					selectionRect->TagObject = NULL;
					break;
				case 8:
					//Copy object
					//editList->editCopy(c);
					break;
					//Paste object
				case 9:
					//Paste object(s)
					editList->editPaste(c);
					break;
				default:
					break;
				}

			}
		}

		//If a Paste request was made and successful, delete items in the list!
		if (m->Tag == 9 && editList->wasPasted)
			editList->Clear();

		//Add to the tree and give it ZOrdering
		calcControlZOrder(deviceScreen);
		updateTreeView();
	}

	//Free up memory
	delete temp;
}


//--------------------------------------------------------
//
//Tool bar controls
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::objectZOrderClick(TObject *Sender)
{
	//0 = Raise to top
	//1 = Raise one level
	//2 = Lower to bottom
	//3 = Lower one level
	//4 = Create new window
	//5 = Create new text
	//17 = Rotate the device window clockwise by 90 degrees
	//
	TSpeedButton *sb    = dynamic_cast<TSpeedButton *>(Sender);
	TRectangleEx *ARect = NULL;
	TTextEx *     AText = NULL;

	//Screen rotation does not requirea selection
	if (sb && sb->Tag == 17)
	{
		screenOrientation += 90;
		if (screenOrientation >= 360)
			screenOrientation = 0;

		//Set rotation
		deviceFrame->Fill->Bitmap->Bitmap->Rotate(90);

		//Rotate screen dresing if exist
		if (!disp->screenDressing->IsEmpty())
			disp->screenDressing->Rotate(90);

		//Apply changes
		applyDisplayMetrics();

		//
		if (selectionRect->TagObject)
			adjustHandles(true,NULL);
		else
			adjustHandles(false,NULL);
		//
		deviceScreen->Repaint();
		return;
	}

	//Single object actions
	//If newWindow or newText, selectionRect->Active = window/text in window creation
	if (selectionRect->TagObject)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		TControl *   c       = dynamic_cast<TControl *>(selectionRect->TagObject);
		TStringList *l       = getObjectsList(deviceScreen);
		int          listTop = l->Count;
		//int           listTop = deviceScreen->ControlsCount;
		//Free memory
		delete l;

		if (sb && c)
		{
			UIObject *o = (UIObject *)c->Tag;
			if (o)
			{
				switch(sb->Tag)
				{
				case 0 :
					SetControlZOrder(c, listTop);
					o->zPosition = listTop;
					break;
				case 1:
					if ((o->zPosition + 1) <= listTop)
					{
						SetControlZOrder(c, o->zPosition + 1);
						o->zPosition++;;
					}
					break;
				case 2:
					SetControlZOrder(c, 0);
					o->zPosition = 0;
					break;
				case 3:
					if ((o->zPosition - 1) > -1)
					{
						SetControlZOrder(c, o->zPosition - 1);
						o->zPosition--;
					}
					break;
					//New window: Should not be here if inactive
				case 4:
					ARect = (TRectangleEx *)createNewControl(c, "RECT");
					selectionRect->TagObject = ARect;
					adjustHandles(true,NULL);
					updateInspector();
					break;
					//New text: Should not be here if inactive
				case 5:
					AText = (TTextEx *)createNewControl(c, "TEXT");
					selectionRect->TagObject = AText;
					adjustHandles(true,NULL);
					updateInspector();
					break;
				default:
					break;
				}

				//Re-sort
				calcControlZOrder(c->Parent);
				//Update tree
				updateTreeView();
			}
		}
	}
	else
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		//
		//Only thing that should work here is newWindow and newText
		//
		if (sb)
		{
			switch(sb->Tag)
			{
				//New window: Should not be here if inactive
			case 4:
				ARect = (TRectangleEx *)createNewControl(deviceScreen, "RECT");
				selectionRect->TagObject = ARect;
				adjustHandles(true,NULL);
				updateInspector();
				break;
				//New text: Should not be here if inactive
			case 5:
				AText = (TTextEx *)createNewControl(deviceScreen, "TEXT");
				selectionRect->TagObject = AText;
				adjustHandles(true,NULL);
				updateInspector();
				break;
			default:
				break;
			}

			//Re-sort
			calcControlZOrder(((TControl *)selectionRect->TagObject)->Parent);
			//Update tree
			updateTreeView();
		}
	}
}


void __fastcall Tsolar2DForm::objectActionClick(TObject *Sender)
{
	TSpeedButton *sb   = dynamic_cast<TSpeedButton *>(Sender);
	TStringList * temp = new TStringList();

	if (temp)
	{
		//Add this  to the stack BEFORE change is made
		updateUndoStack("", false);

		if (selectionRect->TagObject)
		{
			//One item handler
			temp->Add(((TControl *)selectionRect->TagObject)->Name);
		}
		else if (selectionsList->Count)
		{
			temp->AddStrings(selectionsList);
		}

		//Save the
		//Process the request
		if (formatSelectedObjects(deviceScreen, Sender, temp))
		{
			//Delete request was made
			selectionRect->TagObject = NULL;

			//project modified
			isModified = true;
			codeView->changesMade = true;
		}

		//Clear animation list
		animListBox->Clear();
		//Clear allthe object inspector data
		clearAnimationInspector();

		//Update all data
		updateTreeView();
		updateInspector();


		//clearObjectSelections();

		//Adjust active handles and selector
		if (selectionRect->TagObject)
			adjustHandles(true,NULL);
		else
			adjustHandles(false, NULL);

		//Clean up the dirt
		deviceScreen->Repaint();
	}

	delete temp;
}


//--------------------------------------------------------
//
//Tree view
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::updateTreeView()
{
	//Clear for update
	controlTree->BeginUpdate();
	controlTree->Clear();

	for (int i = 0; i < deviceScreen->ChildrenCount; i++)
	{
		TTreeViewItem *t = new TTreeViewItem(controlTree);
		TControl *     c = dynamic_cast<TControl *>(deviceScreen->Children->Items[i]);

		if (c)
		{
			t->Align = Fmx::Types::TAlignLayout::Client;
			t->Text = c->Name;
			t->Parent = controlTree;

			//
			if (c->ChildrenCount)
			{
				//Build out branches
				buildTreeBranches(c, t);
			}
		}
	}

	//Done updating
	controlTree->EndUpdate();
}


void __fastcall Tsolar2DForm::buildTreeBranches(TObject *Parent, TTreeViewItem *Child)
{
	//Recursive routine          p->Name
	TControl *p = dynamic_cast<TControl *>(Parent);

	if (p && Child)
	{
		for (int i = 0; i < p->ChildrenCount; i++)
		{
			TTreeViewItem *t = new TTreeViewItem(Child);
			TControl *     c = dynamic_cast<TControl *>(p->Children->Items[i]);

			if (c)
			{
				t->Align = Fmx::Types::TAlignLayout::Client;
				t->Text = c->Name;
				t->Parent = Child;

				//
				if (c->ChildrenCount)
				{
					//Build out branches
					buildTreeBranches(c, t);
				}
			}
		}
	}
}


//--------------------------------------------------------
//
//Start new project
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::newProjClick(TObject *Sender)
{
	if (isAnimating)
		return;

	//Make sure nothing needs saving
	if (isModified)
	{
		TmsgForm *msg = new TmsgForm(Application);
		msg->messageText->Text = "Project not saved. Do you wish to save your changes before loading a different project?";
		msg->Caption = "Save current project...";
		msg->ShowModal();

		//What result did we receive?
		if (msg->ModalResult == mrYes)
		{
			//Attempt to save the project
			saveProjClick(NULL);
		}

		if (msg->ModalResult == mrCancel)
		{
			//Do nothing
			msgForm->DisposeOf();
			return;
		}

		msgForm->DisposeOf();
	}

	solar2DForm->Caption = "Solar2D Generic GUI Designer [ *New Project ]";

	//Clear selection
	selectionRect->TagObject = NULL;
	//Hide size handles
	adjustHandles(false, NULL);

	//Kill all children and BG image
	deviceScreen->TagString = "";
	deviceScreen->DeleteChildren();
	deviceScreen->Fill->Bitmap->Bitmap = NULL;
	deviceScreen->Fill->Color = TAlphaColorRec::Black;
	deviceScreen->Fill->Kind = TBrushKind::Solid;

	//
	animations->Clear();
	clearObjectSelections();
	clearAllCursors(NULL);
	updateTreeView();
	updateInspector();

	//Erase curent file name
	projectName = "";

	//
	isModified = false;
	codeView->changesMade = false;
	codeView->IsDisplayed = false;
	codeMemo->Lines->Clear();
	codeTabItem->Text = "Code View";
	codeMemo->FontColor = claBlack;
	//Italic
	TFontStyles AStyle(2);
	codeMemo->Font->Style = codeMemo->Font->Style - AStyle;

	//Clean UNDO stack and add initial state
	saveStateString->Clear();
	editUndo->Text = Format("Undo [ %02d / %02d ]", ARRAYOFCONST((saveStateString->Count, MAXSAVESTATES)));
	//updateUndoStack("", false);
}


//--------------------------------------------------------
//
//Load the current project
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::openProjClick(TObject *Sender)
{
	if (isAnimating)
		return;

	//Make sure nothing needs saving
	if (isModified)
	{
		TmsgForm *msg = new TmsgForm(Application);
		msg->messageText->Text = "Project not saved. Do you wish to save your changes before loading a different project?";
		msg->Caption = "Save current project...";
		msg->ShowModal();

		//What result did we receive?
		if (msg->ModalResult == mrYes)
		{
			//Attempt to save the project
			saveProjClick(NULL);
		}

		if (msg->ModalResult == mrCancel)
		{
			//Do nothing
			msgForm->DisposeOf();
			return;
		}

		msgForm->DisposeOf();
	}

	//Make sure a name exist.If not exist, call 'Save As'
	//Which will recursively call this method once a name is created
	FOpen->FilterIndex = 0;
	FOpen->Filter = "Solar2D GUI Project Files(*.gua)|*.gua";

	if (FOpen->Execute())
	{
		//Kill everything on deviceScreen currently
		//Ask to save first if not saved alfready;
		deviceScreen->TagString = "";
		deviceScreen->DeleteChildren();
		deviceScreen->Fill->Bitmap->Bitmap = NULL;
		deviceScreen->Fill->Color = TAlphaColorRec::Black;
		deviceScreen->Fill->Kind = TBrushKind::Solid;


		TJsonTextReader *reader = NULL;
		TControl *       Parent = dynamic_cast<TControl *>(deviceScreen);
		TStreamReader *  stream = new TStreamReader(FOpen->FileName);
		reader = new TJsonTextReader(stream);

		projectName = FOpen->FileName;

		//Prevent Errors from selected items that will be deleted
		selectionRect->TagObject = NULL;
		clearObjectSelections();

		//Attempt to load the project data to a file
		if (reader && Parent)
		{
			//Read in all the Objects to the 'deviceScreen'
			processJsonRead(deviceScreen, reader);

			delete reader;
			delete stream;

			//project modified
			isModified = false;
			codeView->changesMade = false;
			codeView->IsDisplayed = false;
			codeMemo->Lines->Clear();
			codeTabItem->Text = "Code View";
			codeMemo->FontColor = claBlack;
			//Italic
			TFontStyles AStyle(2);
			codeMemo->Font->Style = codeMemo->Font->Style - AStyle;

			//If the tab is visible, show the new data
			if (mainTab->ActiveTab == codeTabItem)
			{
				codeTabItemClick(NULL);
			}

			//Update all data
			updateTreeView();
			updateInspector();
			alignObjects();

			//A little screen clean up
			clearObjectSelections();
			clearAllCursors(deviceScreen);
			adjustHandles(false, NULL);

			//Align all objects on the device screen
			setAllAlignments(deviceScreen);

			//Set the background image if it exists
			if (deviceScreen->TagString != "" && FileExists(deviceScreen->TagString))
			{
				deviceScreen->Fill->Bitmap->Bitmap->LoadFromFile(deviceScreen->TagString);
				//deviceScreen->TagString = FOpen->FileName;
				deviceScreen->Fill->Bitmap->WrapMode = TWrapMode::TileStretch;
				deviceScreen->Fill->Kind = TBrushKind::Bitmap;
			}
			//
			deviceScreen->Repaint();

			//
			solar2DForm->Caption = "Solar2D Generic GUI Designer [ " + ExtractFileName(projectName)+ " ]";

			//Open dialog clicks will move the frame. Disable
			deviceFrame->Tag = 1;
		}

		//Clean UNDO stack and add initial state
		saveStateString->Clear();
		//updateUndoStack("", false);
		editUndo->Text = Format("Undo [ %02d / %02d ]", ARRAYOFCONST((saveStateString->Count, MAXSAVESTATES)));
	}
}


//--------------------------------------------------------
//
//Save the current project
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::saveProjClick(TObject *Sender)
{
	if (isAnimating)
		return;

	//Make sure a name exist. If not exist, call 'Save As'
	//Which will recursively call this method once a name is created
	if (projectName != "")
	{
		TJsonTextWriter *writer       = NULL;
		TStringWriter *  stringWriter = new TStringWriter();
		TControl *       Parent       = dynamic_cast<TControl *>(deviceScreen);
		TMemo *          m            = new TMemo(solar2DForm);

		writer = new TJsonTextWriter(stringWriter);

		//Attempt to save the project data to a file
		if (writer && Parent && m)
		{
			m->Visible = false;

			//Formatting
			writer->Formatting = TJsonFormatting::Indented;
			writer->Indentation = 4;
			//writer->IndentChar = '\t';

			writer->WriteStartObject();
			//Write out all the Objects on the 'deviceScreen'
			processJsonWrite(deviceScreen, writer, "");

			writer->WriteEndObject();


			m->Lines->Add(stringWriter->ToString());
			m->Lines->SaveToFile(projectName);

			//No save required
			isModified = false;
			codeView->changesMade = false;
			codeView->IsDisplayed = false;
			codeTabItem->Text = "Code View";
			codeMemo->FontColor = claBlack;
			//Italic
			TFontStyles AStyle(2);
			codeMemo->Font->Style = codeMemo->Font->Style - AStyle;
			//Reload new data
			codeTabItemClick(NULL);
			//
			updateObjectData();

			//Add this change to the stack
			updateUndoStack(stringWriter->ToString(), false);

			//
			delete writer;
			delete stringWriter;
			delete m;
		}
	}
	else
	{
		saveAsClick(Sender);
	}
}


void __fastcall Tsolar2DForm::saveAsClick(TObject *Sender)
{
	if (isAnimating)
		return;

	//Select / Create a file and save to it.
	if (FSave->Execute())
	{
		projectName = FSave->FileName;

		saveProjClick(NULL);

		//
		solar2DForm->Caption = "Solar2D Generic GUI Designer [ " + ExtractFileName(projectName)+ " ]";
	}
}


//--------------------------------------------------------
//
//Enable / Disable window borders for visibility issues
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::borderToggleClick(TObject *Sender)
{
	TMenuItem *m    = dynamic_cast<TMenuItem *>(Sender);
	TList *    list = new TList();
	int        kind = TBrushKind::None;

	if (m)
	{
		//Set checked / Unchecked status
		m->IsChecked = !m->IsChecked;
		deviceScreen->Repaint();
	}
}


//--------------------------------------------------------
//
//Enable / Disable AnchorX/Y point cross hair
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::crossHairClick(TObject *Sender)
{
	TMenuItem *m = dynamic_cast<TMenuItem *>(Sender);

	if (m)
	{
		//Set checked / Unchecked status
		m->IsChecked = !m->IsChecked;
		deviceScreen->Repaint();
	}
}


//--------------------------------------------------------
//
//Delete objects Menu
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::MenuItem4Click(TObject *Sender)
{
	if (selectionRect->TagObject || selectionsList->Count)
	{
		TSpeedButton *b = new TSpeedButton(solar2DForm);

		b->Visible = false;
		b->Tag = 6;

		//List needs to be filled
		if (!selectionsList->Count)
		{
			TControl *c = dynamic_cast<TControl *>(selectionRect->TagObject);

			if (c)
			{
				selectionsList->Add(c->Name);
				selectionRect->TagObject = NULL;
			}
		}

		//Process the request
		if (formatSelectedObjects(deviceScreen, b, selectionsList))
		{
			//Delete request was made
			selectionRect->TagObject = NULL;

			//project modified
			isModified = true;
			codeView->changesMade = true;

			//Clear animation list
			animListBox->Clear();
			//Clear allthe object inspector data
			clearAnimationInspector();

			//Update all data
			updateTreeView();
			updateInspector();

			//Adjust active handles and selector
			adjustHandles(false, NULL);

			//Clean up the dirt
			deviceScreen->Repaint();
		}

		delete b;
	}
}


//--------------------------------------------------------
//
//Handle Animation inspector controls
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::animationPopupPopup(TObject *Sender)
{
	//Choose who gets the requested changes
	addAnim->Enabled = true;
	delAnim->Enabled = false;

	if (animListBox->Count)
	{
		delAnim->Enabled = true;
	}
}


void __fastcall Tsolar2DForm::addAnimClick(TObject *Sender)
{
	if (selectionRect->TagObject)
	{
		TMenuItem *m = dynamic_cast<TMenuItem *>(Sender);

		if (m && m->Tag == 0)
		{
			UIAnimation *o     = new UIAnimation();
			int          count = 0;
			AnsiString name;

			//Modify data in animation class
			if (o)
			{
				memset(o, 0, sizeof(UIAnimation));
			}
			else
				return;

			//Add it to the Object's list
			//Choose who gets the requested changes
			if (selectionRect->TagObject->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *c = dynamic_cast<TRectangleEx *>(selectionRect->TagObject);
				c->animations->Add(o);
				count = c->animations->Count;
				name = c->Name;
			}
			else if (selectionRect->TagObject->ClassType() == __classid(TTextEx))
			{
				TTextEx *c = dynamic_cast<TTextEx *>(selectionRect->TagObject);
				c->animations->Add(o);
				count = c->animations->Count;
				name = c->Name;
			}

			//
			TListBoxItem *item = new TListBoxItem(animListBox);

			item->Text = AnsiString("[" + name + "] Animation #" + IntToStr(count));
			animListBox->AddObject(item);
		}
		else if (m && m->Tag == 1)
		{
			//Delete an entry
			int index = animListBox->ItemIndex;
			if (index > -1)
			{
				TmsgForm *msg = new TmsgForm(Application);
				msg->messageText->Text = "Do you wish to delete the selected animation entry?";
				msg->Caption = "Delete animation";
				msg->ShowModal();

				//What result did we receive?
				if (msg->ModalResult == mrYes)
				{
					//Attempt to save the project
					animListBox->Items->Delete(index);

					if (selectionRect->TagObject->ClassType() == __classid(TRectangleEx))
					{
						TRectangleEx *c = dynamic_cast<TRectangleEx *>(selectionRect->TagObject);
						if (c)
							c->animations->Delete(index);
					}
					else if (selectionRect->TagObject->ClassType() == __classid(TTextEx))
					{
						TTextEx *c = dynamic_cast<TTextEx *>(selectionRect->TagObject);
						if (c)
							c->animations->Delete(index);
					}
				}
			}
		}
	}
}


void __fastcall Tsolar2DForm::animListBoxClick(TObject *Sender)
{
	if (selectionRect->TagObject)
	{
		UIAnimation *o     = NULL;
		int          index = animListBox->ItemIndex;
		AnsiString   name  = "";


		//Add it to the Object's list
		//Choose who gets the requested changes
		//Get the selected item
		if (index > -1)
		{
			if (selectionRect->TagObject->ClassType() == __classid(TTextEx))
			{
				TTextEx *c = dynamic_cast<TTextEx *>(selectionRect->TagObject);
				if (c)
					o = (UIAnimation *)c->animations->Items[index];
			}
			else
			{
				TRectangleEx *c = dynamic_cast<TRectangleEx *>(selectionRect->TagObject);
				if (c)
					o = (UIAnimation *)c->animations->Items[index];
			}


			//Good class data
			if (o)
			{
				animRefName->Text = o->Name;
				timeNum->Value = o->time;
				delayNum->Value = o->delay;
				iterationNum->Value = o->iterations;
				transBox->ItemIndex = o->transition;
				easeBox->ItemIndex = o->easeType;
				interpolationBox->ItemIndex = o->interpolation;
				tagEdit->Text = o->tag;
				deltaChk->IsChecked = o->delta;
				loopChk->IsChecked = o->looping;

				xChk->IsChecked = o->xChk;
				yChk->IsChecked = o->yChk;
				widthChk->IsChecked = o->widthChk;
				heightChk->IsChecked = o->heightChk;
				rotationChk->IsChecked = o->rotationChk;
				xScaleChk->IsChecked = o->xScaleChk;
				yScaleChk->IsChecked = o->yScaleChk;
				alphaChk->IsChecked = o->alphaChk;
				fontSizeChk->IsChecked = o->fontSizeChk;
				//
				animXNum->Value = o->x;
				animYNum->Value = o->y;
				animWidthNum->Value = o->width;
				animHeightNum->Value = o->height;
				animRotationNum->Value = o->rotation;
				animXScaleNum->Value = o->xScale;
				animYScaleNum->Value = o->yScale;
				animAlphaNum->Value = o->alpha;
				animFSNum->Value = o->fontSize;

			}

			updateInspector();
		}
	}
}


void __fastcall Tsolar2DForm::clearAnimationInspector()
{
	animRefName->Text = "";
	timeNum->Value = 500;
	delayNum->Value = 0;
	iterationNum->Value = 1;
	transBox->ItemIndex = 0;
	easeBox->ItemIndex = 0;
	interpolationBox->ItemIndex = 0;
	tagEdit->Text = "";
	deltaChk->IsChecked = false;
	loopChk->IsChecked = false;

	xChk->IsChecked = false;
	yChk->IsChecked = false;
	widthChk->IsChecked = false;
	heightChk->IsChecked = false;
	rotationChk->IsChecked = false;
	xScaleChk->IsChecked = false;
	yScaleChk->IsChecked = false;
	alphaChk->IsChecked = false;
	fontSizeChk->IsChecked = false;
	//
	animXNum->Value = 0;
	animYNum->Value = 0;
	animWidthNum->Value = 0;
	animHeightNum->Value = 0;
	animRotationNum->Value = 0;
	animXScaleNum->Value = 0;
	animYScaleNum->Value = 0;
	animAlphaNum->Value = 0;
	animFSNum->Value = 0;
}


void __fastcall Tsolar2DForm::buildAnimationList(TControl *Control)
{
	//Clear the list
	animListBox->Clear();

	if (Control)
	{
		TList *list;

		//Choose who gets the requested changes
		if (Control->ClassType() == __classid(TRectangleEx))
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(Control);
			list = c->animations;
		}
		else if (Control->ClassType() == __classid(TTextEx))
		{
			TTextEx *c = dynamic_cast<TTextEx *>(Control);
			list = c->animations;
		}

		//Display what we need
		for (int i = 0; i < list->Count; i++)
		{
			//
			TListBoxItem *item = new TListBoxItem(animListBox);

			item->Text = AnsiString("[" + Control->Name + "] Animation #" + IntToStr(i + 1));
			animListBox->AddObject(item);
		}
	}
}


void __fastcall Tsolar2DForm::xChkChange(TObject *Sender)
{
	if (!allowObjectUpdates)
		return;

	//Mormal process allowed
	TCheckBox *  cb   = dynamic_cast<TCheckBox *>(Sender);
	UIAnimation *o    = NULL;
	TList *      list = NULL;

	//Process request for all objects
	if (selectionRect->TagObject && cb)
	{
		if (selectionRect->TagObject->ClassType() == __classid(TRectangleEx))
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(selectionRect->TagObject);
			list = c->animations;
		}
		else if (selectionRect->TagObject->ClassType() == __classid(TTextEx))
		{
			TTextEx *c = dynamic_cast<TTextEx *>(selectionRect->TagObject);
			list = c->animations;
		}

		//Need a good index
		if (animListBox->ItemIndex > -1)
			o = (UIAnimation *)list->Items[animListBox->ItemIndex];

		//Editing a control's values
		if (list && o)
		{
			switch(cb->Tag)
			{
			case 0:
				o->xChk = cb->IsChecked;
				break;
			case 1:
				o->yChk = cb->IsChecked;
				break;
			case 2:
				o->widthChk = cb->IsChecked;
				break;
			case 3:
				o->heightChk = cb->IsChecked;
				break;
			case 4:
				o->rotationChk = cb->IsChecked;
				break;
			case 5:
				o->xScaleChk = cb->IsChecked;
				break;
			case 6:
				o->yScaleChk = cb->IsChecked;
				break;
			case 7:
				o->alphaChk = cb->IsChecked;
				break;
			case 8:
				o->fontSizeChk = cb->IsChecked;
				break;
			}
		}

		//Project has changed
		isModified = true;
		codeView->changesMade = true;

		//Show changes
		updateInspector();
	}

}


//--------------------------------------------------------
//
//Update all the values
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::animRefNameChange(TObject *Sender)
{
	if (!allowObjectUpdates)
		return;

	//Normal process allowed
	TControl *   d    = dynamic_cast<TControl *>(Sender);
	UIAnimation *o    = NULL;
	TList *      list = NULL;


	//Process request for all objects
	if (selectionRect->TagObject && d)
	{
		if (selectionRect->TagObject->ClassType() == __classid(TRectangleEx))
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(selectionRect->TagObject);
			list = c->animations;
		}
		else if (selectionRect->TagObject->ClassType() == __classid(TTextEx))
		{
			TTextEx *c = dynamic_cast<TTextEx *>(selectionRect->TagObject);
			list = c->animations;
		}

		//Need a good index
		if (animListBox->ItemIndex > -1)
			o = (UIAnimation *)list->Items[animListBox->ItemIndex];

		//Editing a control's values
		if (list && o)
		{
			switch(d->Tag)
			{
			case 0:
				o->Name = ((TEdit *)d)->Text;
				break;
			case 1:
				o->time = ((TNumberBox *)d)->Value;
				break;
			case 2:
				o->delay = ((TNumberBox *)d)->Value;
				break;
			case 3:
				o->iterations = ((TNumberBox *)d)->Value;
				break;
			case 4:
				o->easeType = ((TComboBox *)d)->ItemIndex;
				break;
			case 5:
				o->interpolation = ((TComboBox *)d)->ItemIndex;
				break;
			case 6:
				o->tag = ((TEdit *)d)->Text;
				break;
			case 7:
				o->delay = ((TCheckBox *)d)->IsChecked;
				break;
			case 8:
				o->looping = ((TCheckBox *)d)->IsChecked;
				break;
			case 9:
				o->transition = ((TComboBox *)d)->ItemIndex;
				break;
			case 10:
				o->x = ((TNumberBox *)d)->Value;
				break;
			case 11:
				o->y = ((TNumberBox *)d)->Value;
				break;
			case 12:
				o->width = ((TNumberBox *)d)->Value;
				break;
			case 13:
				o->height = ((TNumberBox *)d)->Value;
				break;
			case 14:
				o->rotation = ((TNumberBox *)d)->Value;
				break;
			case 15:
				o->xScale = ((TNumberBox *)d)->Value;
				if (o->xScale == 0)
					o->xScale = 0.01;
				break;
			case 16:
				o->yScale = ((TNumberBox *)d)->Value;
				if (o->yScale == 0)
					o->yScale = 0.01;
				break;
			case 17:
				o->alpha = ((TNumberBox *)d)->Value;
				break;
			case 18:
				o->fontSize = ((TNumberBox *)d)->Value;
				break;
			}
		}
	}

	//Project has changed
	isModified = true;
	codeView->changesMade = true;

	//Clean up the dirt
	deviceScreen->Repaint();
}


//--------------------------------------------------------
//
//Run all animations in the project for testing
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::runAnimationClick(TObject *Sender)
{
	TMenuItem *m = dynamic_cast<TMenuItem *>(Sender);

	//Test to see if we are already running
	if (m->IsChecked)
	{
		if (animations->Count)
		{
			//Stop all the animations now!
			for (int i = 0; i < animations->Count; i++)
			{
				((TFloatAnimation *)animations->Items[i])->Stop();
			}

			//Start all the animations now!
			for (int i = 0; i < animations->Count; i++)
			{
				TFloatAnimation *anim = (TFloatAnimation*)animations->Items[i];
				if (anim)
				{
					//anim->Enabled = true;
					anim->Start();
				}
			}
		}

		return;
	}


	//Save the current state to be restored
	//when animation 'Stop' is processed
	updateUndoStack("", true);


	TList *objects  = new TList();
	int    fontSize = 0;
	bool   isText   = false;

	//Erase all existing animations, reload  them and then run!
	animations->Clear();

	//Get all the objects on screen
	getAllObjectOnParent(deviceScreen, objects);

	//Load the list for as many TFloatAnimations as required
	for (int i = 0; i < objects->Count; i++)
	{
		UIAnimation *o    = NULL;
		TList *      temp = NULL;
		TControl *   c    = (TControl *)objects->Items[i];

		if (c->ClassType() == __classid(TTextEx))
		{
			TTextEx *r = (TTextEx *)c;

			r->oldH = r->Height;
			r->oldW = r->Width;
			r->oldX = r->Position->X;
			r->oldY = r->Position->Y;

			temp = r->animations;
			fontSize = r->Font->Size;
			isText = true;
		}
		else
		{
			TRectangleEx *r = (TRectangleEx *)c;

			r->oldH = r->Height;
			r->oldW = r->Width;
			r->oldX = r->Position->X;
			r->oldY = r->Position->Y;

			temp = r->animations;
			isText = false;
		}


		//Build a list of the objects animations
		for (int a = 0; a < temp->Count; a++)
		{
			o = (UIAnimation *)temp->Items[a];

			if (o)
			{
				TFloatAnimation *anim = NULL;

				//Check each property
				if (o->xChk)
				{
					anim = buildFloatAnimation(c, o, 0, fontSize);
					animations->Add(anim);
				}
				if (o->yChk)
				{
					anim = buildFloatAnimation(c, o, 1, fontSize);
					animations->Add(anim);

				}
				if (o->widthChk)
				{
					anim = buildFloatAnimation(c, o, 2, fontSize);
					if (anim)
						anim->OnProcess = onAnimationProcessWidth;
					//
					animations->Add(anim);
				}
				if (o->heightChk)
				{
					anim = buildFloatAnimation(c, o, 3, fontSize);
					if (anim)
						anim->OnProcess = onAnimationProcessHeight;
					//
					animations->Add(anim);
				}
				if (o->rotationChk)
				{
					anim = buildFloatAnimation(c, o, 4, fontSize);
					animations->Add(anim);
				}
				//Gets a listener
				if (o->xScaleChk)
				{
					anim = buildFloatAnimation(c, o, 5, fontSize);
					if (anim)
						anim->OnProcess = onAnimationProcessScaleX;
					//
					animations->Add(anim);
				}

				//Gets a listener
				if (o->yScaleChk)
				{
					anim = buildFloatAnimation(c, o, 6, fontSize);
					if (anim)
						anim->OnProcess = onAnimationProcessScaleY;
					//
					animations->Add(anim);
				}

				if (o->alphaChk)
				{
					anim = buildFloatAnimation(c, o, 7, fontSize);
					animations->Add(anim);
				}
				if (o->fontSizeChk)
				{
					anim = buildFloatAnimation(c, o, 8, fontSize);
					animations->Add(anim);
				}
			}
		}

		//Start all the animations now!
		for (int i = 0; i < animations->Count; i++)
		{
			TFloatAnimation *anim = (TFloatAnimation*)animations->Items[i];
			if (anim)
			{
				//anim->Enabled = true;
				anim->Start();
			}
		}

		//Set the animations in motion
		if (animations->Count)
		{
			//These changes should not affect real data
			isAnimating = true;
			updateInspector();
			//
			m->IsChecked = true;
			adjustHandles(false, NULL);
		}
	}

	delete objects;
}


TFloatAnimation *__fastcall Tsolar2DForm::buildFloatAnimation(TFmxObject *Sender, UIAnimation *animation, int property, int fontSize)
{
	TFloatAnimation *anim           = NULL;
	bool             transitionType = (animation->transition == 1);
	float            start          = 0;
	float            end            = 0;
	TTextEx *        c              = static_cast<TTextEx *>(Sender);

	if (c)
	{
		AnsiString s[] =
		{
			"Position.X",
			"Position.Y",
			"Width",
			"Height",
			"RotationAngle",
			"Scale.X",
			"Scale.Y",
			"Opacity",
			"Font.Size"
		};

		float animValue[] =
		{
			animation->x,
			animation->y,
			animation->width,
			animation->height,
			animation->rotation,
			animation->xScale,
			animation->yScale,
			animation->alpha,
			animation->fontSize
		};

		float objValue[] =
		{
			c->Position->X,
			c->Position->Y,
			c->Width,
			c->Height,
			c->RotationAngle,
			c->Scale->X,
			c->Scale->Y,
			c->Opacity,
			fontSize
		};

		//This does NOT assign a parent!
		anim = new TFloatAnimation(c);

		if (anim)
		{
			//Will not run without a parent
			anim->Parent = c;

			//All times based on milliseconds
			if (animation->easeType == 3)
			{
				////This is OutIn -- Not supported by Borland
				////Halve the Duration and set an onFinish listener that
				////is universal
				//anim->Duration = (animation->time /1000);
				//anim->Delay = (animation->delay / 1000);
				//anim->AnimationType = 2; //Use 'OUT' first, then 'IN'
				//anim->Interpolation = animation->interpolation;
				//anim->Loop = animation->looping; //must be false
				//anim->PropertyName = s[property];
				//
				// //Determine if transition.To/From
				//if (transitionType)
				//{
				// //Using FROM setting
				//anim->StartValue = objValue[property];
				//anim->StopValue = animValue[property];
				//}
				//else
				//{
				// //Using TO setting
				//anim->StartFromCurrent = true;
				//anim->StartValue = objValue[property];
				//anim->StopValue = objValue[property];
				//}
			}
			else
			{
				anim->Duration = (animation->time / 1000);
				anim->Delay = (animation->delay / 1000);
				anim->AnimationType = animation->easeType;
				anim->Interpolation = animation->interpolation;
				anim->Loop = animation->looping;
				anim->PropertyName = s[property];

				//Determine if transition.To/From
				if (transitionType)
				{
					//Using FROM setting: From value entered to current value
					anim->StartValue = animValue[property];
					anim->StopValue = objValue[property];
				}
				else
				{
					//Using TO setting: From current value to value entered
					anim->StartFromCurrent = true;
					anim->StopValue = animValue[property];
				}
			}
		}
	}

	return anim;
}


//--------------------------------------------------------
//
//Pause all animations in the project for testing
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::pauseAnimationClick(TObject *Sender)
{
	//Only if animations arerunning
	if (!runAnimation->IsChecked)
		return;

	TMenuItem *m = dynamic_cast<TMenuItem *>(Sender);

	m->IsChecked = !m->IsChecked;

	if (m)
	{
		if (!m->IsChecked)
		{
			//Resume all the animations now!
			for (int i = 0; i < animations->Count; i++)
			{
				((TFloatAnimation *)animations->Items[i])->Pause = false;
			}
		}
		else
		{
			//Pause all the animations now!
			for (int i = 0; i < animations->Count; i++)
			{
				((TFloatAnimation *)animations->Items[i])->Pause = true;
			}
		}
	}
}


//--------------------------------------------------------
//
//Stop all animations in the project for testing
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::stopAnimationClick(TObject *Sender)
{
	//Only if animations arerunning
	if (!runAnimation->IsChecked)
		return;

	//Stop all the animations now!
	for (int i = 0; i < animations->Count; i++)
	{
		((TFloatAnimation *)animations->Items[i])->Stop();
	}

	//Clear flags
	runAnimation->IsChecked = false;
	pauseAnimation->IsChecked = false;

	isAnimating = false;

	//Restore the last state before animation
	editUndoClick(NULL);
	updateInspector();

	//Show handles if were originally displayed
	selectionRect->TagObject = NULL;
	//if (selectionRect->TagObject)
	//adjustHandles(true, NULL);
	//else
	adjustHandles(false, NULL);
}


void __fastcall Tsolar2DForm::onAnimationFinish(TObject *Sender)
{
	//
}


//--------------------------------------------------------
//
//Sola2D: Simulate scaling with AnchorX/Y
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::onAnimationProcessScaleX(TObject *Sender)
{
	TFloatAnimation *anim = dynamic_cast<TFloatAnimation *>(Sender);

	if (anim)
	{
		TControl *c = dynamic_cast<TControl *>(anim->Parent);

		if (c)
		{
			if (c->ClassType() == __classid(TTextEx))
			{
				TTextEx *r = dynamic_cast<TTextEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newX = r->oldX + ((r->oldW - (r->Width * r->Scale->X)) * r->RotationCenter->X);
					r->Position->X = newX;
				}
			}
			else if (c->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *r = dynamic_cast<TRectangleEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newX = r->oldX + ((r->oldW - (r->Width * r->Scale->X)) * r->RotationCenter->X);
					r->Position->X = newX;
				}
			}
		}
	}
}


void __fastcall Tsolar2DForm::onAnimationProcessScaleY(TObject *Sender)
{
	TFloatAnimation *anim = dynamic_cast<TFloatAnimation *>(Sender);

	if (anim)
	{
		TControl *c = dynamic_cast<TControl *>(anim->Parent);

		if (c)
		{
			if (c->ClassType() == __classid(TTextEx))
			{
				TTextEx *r = dynamic_cast<TTextEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newY = r->oldY + ((r->oldH - (r->Height * r->Scale->Y)) * r->RotationCenter->Y);
					r->Position->Y = newY;
				}
			}
			else if (c->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *r = dynamic_cast<TRectangleEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newY = r->oldY + ((r->oldH - (r->Height * r->Scale->Y)) * r->RotationCenter->Y);
					r->Position->Y = newY;
				}
			}
		}

	}

}


void __fastcall Tsolar2DForm::onAnimationProcessWidth(TObject *Sender)
{
	TFloatAnimation *anim = dynamic_cast<TFloatAnimation *>(Sender);

	if (anim)
	{
		TControl *c = dynamic_cast<TControl *>(anim->Parent);

		if (c)
		{
			if (c->ClassType() == __classid(TTextEx))
			{
				TTextEx *r = dynamic_cast<TTextEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newX = r->oldX + ((r->oldW - r->Width) * r->RotationCenter->X);
					r->Position->X = newX;
				}
			}
			else if (c->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *r = dynamic_cast<TRectangleEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newX = r->oldX + ((r->oldW - r->Width) * r->RotationCenter->X);
					r->Position->X = newX;
				}
			}
		}
	}

}


void __fastcall Tsolar2DForm::onAnimationProcessHeight(TObject *Sender)
{
	TFloatAnimation *anim = dynamic_cast<TFloatAnimation *>(Sender);

	if (anim)
	{
		TControl *c = dynamic_cast<TControl *>(anim->Parent);

		if (c)
		{
			if (c->ClassType() == __classid(TTextEx))
			{
				TTextEx *r = dynamic_cast<TTextEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newY = r->oldY + ((r->oldH - r->Height) * r->RotationCenter->Y);
					r->Position->Y = newY;
				}
			}
			else if (c->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *r = dynamic_cast<TRectangleEx *>(c);

				if (r)
				{
					//This is strictly for X/Y adjustments when scalling is being done
					float newY = r->oldY + ((r->oldH - r->Height) * r->RotationCenter->Y);
					r->Position->Y = newY;
				}
			}
		}
	}
}


//--------------------------------------------------------
//
//Display the code generated thus far
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::codeTabItemClick(TObject *Sender)
{
	//Has the code been displayed yet?
	//OR...
	//Code displayed, Changes saved but code NOT yet updated
	if (!codeView->IsDisplayed || (codeView->IsDisplayed && !isModified))
		//if (!codeView->IsDisplayed || (codeView->IsDisplayed && codeMemo->FontColor == claRed && !isModified))
	{
		//Time to update code to reflect what has changed
		AnsiString text = codeView->buildSourceCode(deviceScreen, projectName);

		if (text != "")
		{
			codeMemo->BeginUpdate();
			codeMemo->Lines->Text = text;
			codeMemo->EndUpdate();
		}

		//
		codeMemo->FontColor = claBlack;
		//Italic
		TFontStyles AStyle(2);
		codeMemo->Font->Style = codeMemo->Font->Style - AStyle;
		codeTabItem->Text = "Code View";
		codeView->IsDisplayed = true;
	}
	else
	{
		codeTabItem->Text = "Code View [Not displaying any updates]";
		codeMemo->FontColor = TAlphaColorRec::Red;
		//Italic
		TFontStyles AStyle(2);
		codeMemo->Font->Style = codeMemo->Font->Style + AStyle;
	}
}


//--------------------------------------------------------
//
//Create a folder, create 'assets' folder inside
//Copy all required assests ref'd by objectsinto 'assets'
//Create 'ProjectName.lua' for with source code
//Maybe give an option to test the code
//
//--------------------------------------------------------
void __fastcall Tsolar2DForm::exportLuaClick(TObject *Sender)
{
	String Dir;
	TList *temp = new TList();

	if (isModified || projectName == "")
	{
		TmsgForm *msg = new TmsgForm(Application);
		msg->messageText->Text = "Project not saved. Do you wish to save your changes before exporting?";
		msg->Caption = "Save current project...";
		msg->ShowModal();

		//What result did we receive?
		if (msg->ModalResult == mrYes)
		{
			//Attempt to save the project
			saveProjClick(NULL);
		}

		msgForm->DisposeOf();
	}

	//
	if (exportDir != "")
		Dir = exportDir;

	//
	if (projectName != "" && (exportDir != "" || SelectDirectory("Select or Create a project folder...", exePath, Dir)))
	{
		exportDir = AnsiString(Dir);

		//Create the assets folder
		if (!DirectoryExists(exportDir + "\\assets"))
			CreateDir(exportDir + "\\assets");
		//

		//Get the list of objects on the screen
		getAllObjectOnParent(deviceScreen, temp);

		//Background image
		if (deviceScreen->TagString != "")
		{
			AnsiString dest = exportDir + "\\assets\\" + ExtractFileName(deviceScreen->TagString);

			//Don't try and copy the file already in use
			if (dest.AnsiCompareIC(deviceScreen->TagString) != 0)
				TFile::Copy(deviceScreen->TagString, dest, true);
		}

		//
		//Copy the files referrenced to the assets folder
		//
		for (int i = 0; i < temp->Count; i++)
		{
			if (((TControl *)temp->Items[i])->ClassType() == __classid(TRectangleEx))
			{
				TRectangleEx *c = (TRectangleEx *)temp->Items[i];

				//Copy the image to the assets folder
				if (c->TagString != "")
				{
					AnsiString dest = exportDir + "\\assets\\" + ExtractFileName(c->TagString);

					//Don't try and copy the file already in use
					if (dest.AnsiCompareIC(c->TagString) != 0)
					{
						TFile::Copy(c->TagString, dest, true);

						//Check to see if this file ref is a sprite sheet / atlas
						if (UpperCase(ExtractFileExt(c->TagString)) == ".LUA")
						{
							//Send over the PNG file too
							AnsiString src  = ChangeFileExt(c->TagString, ".png");
							AnsiString dest = exportDir + "\\assets\\" + ChangeFileExt(ExtractFileName(c->TagString), ".png");

							if (FileExists(src))
								TFile::Copy(src, dest, true);
						}
					}
				}
			}
		}

		//Create the source file to use in Solar2D
		codeTabItemClick(NULL);
		Dir = exportDir + "\\" + ChangeFileExt(ExtractFileName(projectName), ".lua");
		codeMemo->Lines->SaveToFile(Dir);

		ShowMessage("'" + Dir + "' was exported successfully.");
	}
}

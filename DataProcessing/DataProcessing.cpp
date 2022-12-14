/*
 Mostly custom JSON code for data manipulation.
 */

#pragma hdrstop

#include "DataProcessing.h"

//Need access to "createNew()'
#include "ControlsForm.h"

#pragma package(smart_init)

extern float *test = NULL;

TControl *MainParent = NULL;


//--------------------------------------------------------
//
//
//Main WRITE process: Recursive
//
//
//--------------------------------------------------------
void __fastcall processJsonWrite(TControl *DeviceParent, TJsonTextWriter *writer, AnsiString ParentName)
{
	//Start the array list of children
	//writer->WriteStartObject();
	writer->WritePropertyName((ParentName == "" ? AnsiString("device") : ParentName));
	writer->WriteStartArray();

	if (DeviceParent->Name == "deviceScreen" && DeviceParent->TagString != "" && FileExists(DeviceParent->TagString))
	{
		MainParent = DeviceParent;

		writer->WriteStartObject();
		writer->WritePropertyName("background");
		writer->WriteValue(DeviceParent->TagString);
		writer->WriteEndObject();
	}

	//Process it all
	for (int i = 0; i < DeviceParent->ChildrenCount; i++)
	{
		TControl *c = dynamic_cast<TControl *>(DeviceParent->Children->Items[i]);

		if (c)
		{
			//Open curly
			writer->WriteStartObject();
			{
				writer->WritePropertyName((c->ClassType() == __classid(TTextEx) ? "TEXT" : "WINDOW"));
				writer->WriteStartObject();

				if (c->ClassType() == __classid(TRectangleEx))
					jsonWriteWindowObject(c, writer);
				else
					jsonWriteTextObject(c, writer);

				//Universal data
				jsonWriteUIObjectData(c, writer);
				//jsonWriteObjectEvent(c , writer);

				if (c->ClassType() == __classid(TRectangleEx))
				{
					if (((TRectangleEx *)c)->animations->Count)
						jsonWriteObjectAnimation(c, writer);
				}
				else
				{
					if (((TTextEx *)c)->animations->Count)
						jsonWriteObjectAnimation(c, writer);

				}

				//Method is recursive
				if (c->ChildrenCount)
				{
					//writer->WritePropertyName("children");
					//writer->WriteStartArray();

					//Make a recursive call
					processJsonWrite(c, writer, "children");

					//writer->WriteEndArray();
				}

				//Close curly
				writer->WriteEndObject();
			}

			//close curly
			writer->WriteEndObject();
		}
	}

	//End the array list of children
	writer->WriteEndArray();
	//writer->WriteEndObject();
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonWriteWindowObject(TControl *control, TJsonTextWriter *writer)
{
	TRectangleEx *Control = dynamic_cast<TRectangleEx *>(control);

	if (Control)
	{
		//Write the info
		writer->WritePropertyName("name");
		writer->WriteValue(Control->Name);

		writer->WritePropertyName("width");
		writer->WriteValue(Control->Width);
		writer->WritePropertyName("height");
		writer->WriteValue(Control->Height);
		writer->WritePropertyName("x");
		writer->WriteValue(Control->Position->X);
		writer->WritePropertyName("y");
		writer->WriteValue(Control->Position->Y);
		writer->WritePropertyName("scaleX");
		writer->WriteValue(Control->Scale->X);
		writer->WritePropertyName("scaleY");
		writer->WriteValue(Control->Scale->Y);
		writer->WritePropertyName("anchorX");
		writer->WriteValue(Control->RotationCenter->X);
		writer->WritePropertyName("anchorY");
		writer->WriteValue(Control->RotationCenter->Y);
		writer->WritePropertyName("rotation");
		writer->WriteValue(Control->RotationAngle);
		writer->WritePropertyName("alpha");
		writer->WriteValue(Control->Opacity);

		writer->WritePropertyName("color");
		writer->WriteValue(Control->Fill->Color);
		//Determine image type
		writer->WritePropertyName("imageType");
		writer->WriteValue(Control->imageType);
		writer->WritePropertyName("imageIndex");
		writer->WriteValue(Control->imageIndex);
		writer->WritePropertyName("imageFile");
		writer->WriteValue(Control->TagString);
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonWriteTextObject(TControl *Control, TJsonTextWriter *writer)
{
	TTextEx *c = dynamic_cast<TTextEx *>(Control);

	if (c)
	{
		//Write the info
		writer->WritePropertyName("name");
		writer->WriteValue(c->Name);

		writer->WritePropertyName("width");
		writer->WriteValue(c->Width);
		writer->WritePropertyName("height");
		writer->WriteValue(c->Height);
		writer->WritePropertyName("x");
		writer->WriteValue(c->Position->X);
		writer->WritePropertyName("y");
		writer->WriteValue(c->Position->Y);
		writer->WritePropertyName("scaleX");
		writer->WriteValue(c->Scale->X);
		writer->WritePropertyName("scaleY");
		writer->WriteValue(c->Scale->Y);
		writer->WritePropertyName("anchorX");
		writer->WriteValue(c->RotationCenter->X);
		writer->WritePropertyName("anchorY");
		writer->WriteValue(c->RotationCenter->Y);
		writer->WritePropertyName("rotation");
		writer->WriteValue(c->RotationAngle);
		writer->WritePropertyName("alpha");
		writer->WriteValue(c->Opacity);

		//Text specific items
		writer->WritePropertyName("text");
		writer->WriteValue(c->Text);
		writer->WritePropertyName("font");
		writer->WriteValue(c->TextSettings->Font->Family);
		writer->WritePropertyName("fontColor");
		writer->WriteValue(c->TextSettings->FontColor);
		writer->WritePropertyName("fontSize");
		writer->WriteValue(c->TextSettings->Font->Size);
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonWriteUIObjectData(TControl *Control, TJsonTextWriter *writer)
{
	//TMemo* animation; <-- Not yet supported

	//UIObjects are sub items
	writer->WritePropertyName("UI");
	writer->WriteStartObject();

	UIObject *o = (UIObject *)Control->Tag;

	//Data is present andaccessible
	if (o)
	{
		writer->WritePropertyName("zPosition");
		writer->WriteValue(o->zPosition);
		writer->WritePropertyName("pAlignLeft");
		writer->WriteValue(o->leftParentChk);
		writer->WritePropertyName("pAlignTop");
		writer->WriteValue(o->topParentChk);
		writer->WritePropertyName("pAlignRight");
		writer->WriteValue(o->rightParentChk);
		writer->WritePropertyName("pAlignBottom");
		writer->WriteValue(o->bottomParentChk);
		writer->WritePropertyName("pAlignVert");
		writer->WriteValue(o->vertParentChk);
		writer->WritePropertyName("pAlignHori");
		writer->WriteValue(o->horiParentChk);
		writer->WritePropertyName("pAnchorLeft");
		writer->WriteValue(o->anchorLeftChk);
		writer->WritePropertyName("pAnchorTop");
		writer->WriteValue(o->anchorTopChk);
		writer->WritePropertyName("pAnchorRight");
		writer->WriteValue(o->anchorRightChk);
		writer->WritePropertyName("pAnchorBottom");
		writer->WriteValue(o->anchorBottomChk);
		writer->WritePropertyName("visible");
		writer->WriteValue(o->Visible);
		//The name it's aligned to
		writer->WritePropertyName("alignAbove");
		writer->WriteValue(o->aboveCombo);
		writer->WritePropertyName("alignBottomWith");
		writer->WriteValue(o->bottomWithCombo);
		writer->WritePropertyName("alignHoriWith");
		writer->WriteValue(o->horiWithCombo);
		writer->WritePropertyName("alignLeftWith");
		writer->WriteValue(o->leftWithCombo);
		writer->WritePropertyName("alignRightWith");
		writer->WriteValue(o->rightWithCombo);
		writer->WritePropertyName("alignTopWith");
		writer->WriteValue(o->topWithCombo);
		writer->WritePropertyName("alignVertWith");
		writer->WriteValue(o->vertWithCombo);
		writer->WritePropertyName("alignBelow");
		writer->WriteValue(o->belowCombo);
		writer->WritePropertyName("alignLeftOf");
		writer->WriteValue(o->leftOfCombo);
		writer->WritePropertyName("alignRightOf");
		writer->WriteValue(o->rightOfCombo);

		//
		TTextEx *c = dynamic_cast<TTextEx *>(Control);

		if (c)
		{
			writer->WritePropertyName("textAlignment");
			writer->WriteValue(o->alignTextIndex);
		}
	}

	//End it
	writer->WriteEndObject();
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonWriteObjectEvent(TControl *Control, TJsonTextWriter *writer)
{
	//Event data: Can have multiple evrnts attached
	//Currently not supported. May re-visit in the future
	writer->WritePropertyName("events");
	writer->WriteStartArray();
	writer->WriteStartObject();

	writer->WriteEndObject();
	writer->WriteEndArray();
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonWriteObjectAnimation(TControl *Control, TJsonTextWriter *writer)
{
	TList *list   = NULL;
	bool   isText = false;

	//UIAnimation(s) are sub items
	writer->WritePropertyName("animations");
	writer->WriteStartArray();

	//Figure out which object we are processing
	if (Control->ClassType() == __classid(TTextEx))
	{
		list = ((TTextEx *)Control)->animations;
		isText = true;
	}
	else
	{
		list = ((TRectangleEx *)Control)->animations;
	}


	//Data is present andaccessible
	if (list && list->Count)
	{
		//Write ONLY if animation sexist
		writer->WriteStartObject();

		for (int i = 0; i < list->Count; i++)
		{
			UIAnimation *o = (UIAnimation *)list->Items[i];

			//Start object writer
			writer->WritePropertyName("ANIM");
			writer->WriteStartObject();

			writer->WritePropertyName("name");
			writer->WriteValue(o->Name);
			writer->WritePropertyName("time");
			writer->WriteValue(o->time);
			writer->WritePropertyName("delay");
			writer->WriteValue(o->delay);
			writer->WritePropertyName("iterations");
			writer->WriteValue(o->iterations);
			writer->WritePropertyName("transition");
			writer->WriteValue(o->transition);
			writer->WritePropertyName("easeType");
			writer->WriteValue(o->easeType);
			writer->WritePropertyName("interpolation");
			writer->WriteValue(o->interpolation);
			writer->WritePropertyName("tag");
			writer->WriteValue(o->tag);
			writer->WritePropertyName("delta");
			writer->WriteValue(o->delta);
			writer->WritePropertyName("loop");
			writer->WriteValue(o->looping);

			//Check boxes
			writer->WritePropertyName("x-Enabled");
			writer->WriteValue(o->xChk);
			writer->WritePropertyName("y-Enabled");
			writer->WriteValue(o->yChk);
			writer->WritePropertyName("width-Enabled");
			writer->WriteValue(o->widthChk);
			writer->WritePropertyName("height-Enabled");
			writer->WriteValue(o->heightChk);
			writer->WritePropertyName("rotation-Enabled");
			writer->WriteValue(o->rotationChk);
			writer->WritePropertyName("xScale-Enabled");
			writer->WriteValue(o->xScaleChk);
			writer->WritePropertyName("yScale-Enabled");
			writer->WriteValue(o->yScaleChk);
			writer->WritePropertyName("alpha-Enabled");
			writer->WriteValue(o->alphaChk);
			writer->WritePropertyName("fontSize-Enabled");
			writer->WriteValue(o->fontSizeChk);
			//Values
			writer->WritePropertyName("x");
			writer->WriteValue(o->x);
			writer->WritePropertyName("y");
			writer->WriteValue(o->y);
			writer->WritePropertyName("width");
			writer->WriteValue(o->width);
			writer->WritePropertyName("height");
			writer->WriteValue(o->height);
			writer->WritePropertyName("rotation");
			writer->WriteValue(o->rotation);
			writer->WritePropertyName("xScale");
			writer->WriteValue(o->xScale);
			writer->WritePropertyName("yScale");
			writer->WriteValue(o->yScale);
			writer->WritePropertyName("alpha");
			writer->WriteValue(o->alpha);
			writer->WritePropertyName("fontSize");
			writer->WriteValue(o->fontSize);

			//End it
			writer->WriteEndObject();
		}

		//Write ONLY if animations exist
		writer->WriteEndObject();
	}

	writer->WriteEndArray();
}


//--------------------------------------------------------
//
//
//Main READ process: Recursive
//
//
//--------------------------------------------------------
void __fastcall processJsonRead(TControl *DeviceParent, TJsonTextReader *reader)
{
	if (DeviceParent->Name == "deviceScreen")
	{
		MainParent = DeviceParent;
	}

	//Start off with 'Object Start Token'
	AnsiString name = "";
	int start = 1;

	//Get the first token: StartObject
	reader->Read();

	while(start > 0 && reader->Read())
	{
		if (reader->TokenType == TJsonToken::StartArray)
		{
			//All all array types used
			jsonProcessArray(DeviceParent, reader);
		}
		else if (reader->TokenType == TJsonToken::StartObject)
		{
			//New object found
			jsonProcessObject(DeviceParent, reader);
		}
		else if (reader->TokenType == TJsonToken::EndObject)
		{
			//End object found
			start--;
		}
		else if (reader->TokenType == TJsonToken::PropertyName)
		{
			//Test to see what we have
			name = reader->Value.AsString();
		}
	}

}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonProcessArray(TControl *control, TJsonTextReader *reader)
{
	int start = 1;

	while(start > 0 && reader->Read())
	{
		if (reader->TokenType == TJsonToken::EndArray)
		{
			//When start = 0, we can leave
			start--;
		}
		else if (reader->TokenType == TJsonToken::StartObject)
		{
			//Handle all objects used
			jsonProcessObject(control, reader);
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonProcessObject(TControl *Control, TJsonTextReader *reader)
{
	int        start    = 1;
	AnsiString propName = "";

	while(start > 0 && reader->Read())
	{
		if (reader->TokenType == TJsonToken::StartObject)
		{
			//Must be a nested array: Recursive call should be made
			jsonProcessObject(Control, reader);
			continue;
		}
		else if (reader->TokenType == TJsonToken::StartArray)
		{
			//Must be a nested array: Recursive call should be made
			jsonProcessArray(Control, reader);
			continue;
		}
		else if (reader->TokenType == TJsonToken::EndObject)
		{
			//When start = 0, we can leave
			start--;
		}
		else if (reader->TokenType == TJsonToken::PropertyName)
		{
			//Token
			propName = UpperCase(reader->Value.AsString());

			if (propName == "BACKGROUND")
			{
				if (MainParent)
				{
					reader->Read();

					MainParent->TagString = reader->Value.AsString();

					if (MainParent->TagString != "" && FileExists(MainParent->TagString))
					{
						((TRectangle *)MainParent)->Fill->Bitmap->Bitmap->LoadFromFile(MainParent->TagString);
						((TRectangle *)MainParent)->Fill->Color = TAlphaColorRec::Null;
						((TRectangle *)MainParent)->Fill->Kind = TBrushKind::Bitmap;
					}
				}
			}
			else if (propName == "WINDOW")
			{
				//Create a new window and fill the data
				jsonReadWindowObject(Control, reader);
				continue;
			}
			else if (propName == "ANIM")
			{
				//Must be a nested array
				jsonReadObjectAnimation(Control, reader);
				continue;
			}
			else
			{
				jsonReadTextObject(Control, reader);
			}
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonReadWindowObject(TControl *Control, TJsonTextReader *reader)
{
	//Create the new object
	TRectangleEx *c = (TRectangleEx*)solar2DForm->createNewControl(Control, "RECT");

	if (c)
	{
		int          start = 1;
		TStringList *list  = new TStringList();
		list->Add("name");
		list->Add("width");
		list->Add("height");
		list->Add("x");
		list->Add("y");
		list->Add("scaleX");
		list->Add("scaleY");
		list->Add("anchorX");
		list->Add("anchorY");
		list->Add("rotation");
		list->Add("alpha");
		list->Add("color");
		list->Add("imageType");
		list->Add("imageIndex");
		list->Add("imageFile");


		//
		//Process request
		//
		while(start > 0 && reader->Read())
		{
			if (reader->TokenType == TJsonToken::EndObject)
			{
				start--;
			}
			else if (reader->TokenType == TJsonToken::PropertyName)
			{
				AnsiString propName = reader->Value.AsString();
				int        index    = -1;


				//Check for sub items
				if (UpperCase(propName) == "UI")
				{
					jsonReadUIObjectData(c, reader);
					continue;
				}
				else if(UpperCase(propName) == "CHILDREN")
				{
					//THIS object Must be a window to get children loaded
					reader->Read();
					jsonProcessArray(c, reader);
					continue;
				}
				else if (UpperCase(propName) == "EVENTS")
				{
					//Does nothing for now
					reader->Read();
					jsonProcessArray(c, reader);
					continue;
				}
				else if (UpperCase(propName) == "ANIMATIONS")
				{
					//Does nothing for now
					reader->Read();
					jsonProcessArray(c, reader);
					continue;
				}
				else
				{
					//Next token: Data stored in 'Value'
					reader->Read();

					index = list->IndexOf(propName);

					if (index != -1)
					{
						switch(index)
						{
						case 0 :
							c->Name = reader->Value.AsString();
							break;
						case 1:
							c->Width = StrToFloat(reader->Value.ToString());
							break;
						case 2:
							c->Height = StrToFloat(reader->Value.ToString());
							break;
						case 3 :
							c->Position->X = StrToFloat(reader->Value.ToString());
							break;
						case 4:
							c->Position->Y = StrToFloat(reader->Value.ToString());
							break;
						case 5:
							c->Scale->X = StrToFloat(reader->Value.ToString());
							break;
						case 6:
							c->Scale->Y = StrToFloat(reader->Value.ToString());
							break;
						case 7:
							c->RotationCenter->X = StrToFloat(reader->Value.ToString());
							break;
						case 8:
							c->RotationCenter->Y = StrToFloat(reader->Value.ToString());
							break;
						case 9:
							c->RotationAngle = StrToFloat(reader->Value.ToString());
							break;
						case 10:
							c->Opacity = StrToFloat(reader->Value.ToString());
							break;
						case 11:
							c->Fill->Color = reader->Value.AsInteger();
							break;
						case 12:
							//imageType
							c->imageType = reader->Value.AsInteger();
							break;
						case 13:
							//imageIndex
							c->imageIndex = reader->Value.AsInteger();
							break;
						case 14:
							//image Filename
							c->TagString = reader->Value.AsString();

							//Must be a good name
							if (c->TagString != "")
							{
								if (c->imageType == 1)
									solar2DForm->load9PatchImage(c, c->TagString);
								else if (c->imageType == 0)
								{
									solar2DForm->selectionRect->TagObject = c;
									//
									solar2DForm->imageLoader(c->imageType, c->TagString);
									solar2DForm->selectionRect->TagObject = NULL;
								}
								else if (c->imageType == 2)
									solar2DForm->reloadImageFromSheet(c, c->TagString, c->imageIndex);
							}
							//
							break;
						}
					}
				}
			}
		}

		//
		list->Free();
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonReadTextObject(TControl *Control, TJsonTextReader *reader)
{
	//Create the new object
	TTextEx *c = (TTextEx*)solar2DForm->createNewControl(Control, "TEXT");

	if (c)
	{
		int          start = 1;
		TStringList *list  = new TStringList();
		list->Add("name");
		list->Add("width");
		list->Add("height");
		list->Add("x");
		list->Add("y");
		list->Add("scaleX");
		list->Add("scaleY");
		list->Add("anchorX");
		list->Add("anchorY");
		list->Add("rotation");
		list->Add("alpha");
		list->Add("color");
		list->Add("9Patch");
		list->Add("text");
		list->Add("font");
		list->Add("fontColor");
		list->Add("fontSize");

		//
		//Process request
		//
		while(start > 0 && reader->Read())
		{
			if (reader->TokenType == TJsonToken::EndObject)
			{
				start--;
			}
			else if (reader->TokenType == TJsonToken::PropertyName)
			{
				AnsiString propName = reader->Value.AsString();
				int        index    = -1;


				//Check for sub items
				if (UpperCase(propName) == "UI")
				{
					jsonReadUIObjectData(c, reader);
					continue;
				}
				else if (UpperCase(propName) == "EVENTS")
				{
					//Does nothing for now
					reader->Read();
					jsonProcessArray(c, reader);
					continue;
				}
				else if (UpperCase(propName) == "ANIMATIONS")
				{
					//Does nothing for now
					reader->Read();
					jsonProcessArray(c, reader);
					continue;
				}
				else
				{
					//Next token: Data stored in 'Value'
					reader->Read();

					index = list->IndexOf(propName);

					float v;

					if (index != -1)
					{
						switch(index)
						{
						case 0 :
							c->Name = reader->Value.AsString();
							break;
						case 1:
							c->Width = StrToFloat(reader->Value.ToString());
							break;
						case 2:
							c->Height = StrToFloat(reader->Value.ToString());
							break;
						case 3 :
							c->Position->X = StrToFloat(reader->Value.ToString());
							break;
						case 4:
							c->Position->Y = StrToFloat(reader->Value.ToString());
							break;
						case 5:
							c->Scale->X = StrToFloat(reader->Value.ToString());
							break;
						case 6:
							c->Scale->Y = StrToFloat(reader->Value.ToString());
							break;
						case 7:
							c->RotationCenter->X = StrToFloat(reader->Value.ToString());
							break;
						case 8:
							c->RotationCenter->Y = StrToFloat(reader->Value.ToString());
							break;
						case 9:
							c->RotationAngle = StrToFloat(reader->Value.ToString());
							break;
						case 10:
							c->Opacity = StrToFloat(reader->Value.ToString());
							break;
						case 11:
							//c->Fill->Color = reader->Value.AsInteger();
							break;
						case 12:
							//image
							c->TagString = reader->Value.AsString();
							break;
						case 13:
							c->Text = reader->Value.AsString();
							break;
						case 14:
							c->TextSettings->Font->Family = reader->Value.AsString();
							break;
						case 15:
							c->TextSettings->FontColor = reader->Value.AsInteger();
							break;
						case 16:
							c->TextSettings->Font->Size = reader->Value.AsInteger();
							break;
						}
					}
				}
			}
		}

		//
		delete list;
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonReadUIObjectData(TControl *Control, TJsonTextReader *reader)
{
	UIObject *o = new UIObject();

	if (o)
	{
		TStringList *list = new TStringList();
		list->Add("zPosition");
		list->Add("pAlignLeft");
		list->Add("pAlignTop");
		list->Add("pAlignRight");
		list->Add("pAlignBottom");
		list->Add("pAlignVert");
		list->Add("pAlignHori");
		list->Add("pAnchorLeft");
		list->Add("pAnchorTop");
		list->Add("pAnchorRight");
		list->Add("pAnchorBottom");
		list->Add("visible");
		//The name it's aligned to
		list->Add("alignAbove");
		list->Add("alignBottomWith");
		list->Add("alignHoriWith");
		list->Add("alignLeftWith");
		list->Add("alignRightWith");
		list->Add("alignTopWith");
		list->Add("alignVertWith");
		list->Add("alignBelow");
		list->Add("alignLeftOf");
		list->Add("alignRightOf");
		list->Add("textAlignment");

		long data[] =
		{
			(long)&o->zPosition,
			(long)&o->leftParentChk,
			(long)&o->topParentChk,
			(long)&o->rightParentChk,
			(long)&o->bottomParentChk,
			(long)&o->vertParentChk,
			(long)&o->horiParentChk,
			(long)&o->anchorLeftChk,
			(long)&o->anchorTopChk,
			(long)&o->anchorRightChk,
			(long)&o->anchorBottomChk,
			(long)&o->Visible,
			//The name it's aligned to: Strings
			(long)&o->aboveCombo,
			(long)&o->bottomWithCombo,
			(long)&o->horiWithCombo,
			(long)&o->leftWithCombo,
			(long)&o->rightWithCombo,
			(long)&o->topWithCombo,
			(long)&o->vertWithCombo,
			(long)&o->belowCombo,
			(long)&o->leftOfCombo,
			(long)&o->rightOfCombo,
			//TTextEx only value
			(long)&o->alignTextIndex
		};


		//Assign to the control
		Control->Tag = (long)o;

		int start = 1;
		while(start > 0 && reader->Read())
		{
			if (reader->TokenType == TJsonToken::EndObject)
			{
				start--;
			}
			else if (reader->TokenType == TJsonToken::PropertyName)
			{
				AnsiString propName = reader->Value.AsString();
				int        index    = -1;

				//Next token
				reader->Read();

				index = list->IndexOf(propName);

				if (index != -1)
				{
					if (!index)
					{
						long *value = (long *)data[index];
						*value = reader->Value.AsInteger();
					}
					else if (index > 0 && index < 12)
					{
						bool *value = (bool *)data[index];
						*value = reader->Value.AsBoolean();
					}
					else if (index > 11 && index < 22)
					{
						AnsiString *str = (AnsiString *)data[index];
						*str = reader->Value.AsString();
					}
					else if(index == 22)
					{
						long *value = (long *)data[index];
						*value = reader->Value.AsInteger();
					}
				}
			}
		}


		//This values obly affect TTextEx objcts
		TTextEx *c = dynamic_cast<TTextEx *>(Control);

		if (c)
		{
			//Text alignment
			c->TextSettings->HorzAlign = o->alignTextIndex;

			//Set anchors for the object now!
			//Add per request
			if (o->anchorLeftChk)
				c->Anchors += (TAnchors() << TAnchorKind::akLeft);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akLeft);

			if (o->anchorTopChk)
				c->Anchors += (TAnchors() << TAnchorKind::akTop);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akTop);

			if (o->anchorRightChk)
				c->Anchors += (TAnchors() << TAnchorKind::akRight);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akRight);

			if (o->anchorBottomChk)
				c->Anchors += (TAnchors() << TAnchorKind::akBottom);
			else
				c->Anchors -= (TAnchors() << TAnchorKind::akBottom);
		}

		//
		delete list;
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonReadObjectEvent(TControl *Control, TJsonTextReader *reader)
{
	//Animation data: Canhave multiple animations attached
	//writer->WritePropertyName("event");
	//writer->WriteStartArray();
	//writer->WriteStartObject();
	//
	//writer->WriteEndObject();
	//writer->WriteEndArray();
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall jsonReadObjectAnimation(TControl *Control, TJsonTextReader *reader)
{
	UIAnimation *o = new UIAnimation();

	if (o)
	{//Clear all garbage data
		memset(o, 0, sizeof(UIAnimation));

		//
		TStringList *list = new TStringList();

		list->Add("name");
		list->Add("time");
		list->Add("delay");
		list->Add("iterations");
		list->Add("transition");
		list->Add("easeType");
		list->Add("interpolation");
		list->Add("tag");
		list->Add("delta");
		list->Add("loop");
		//Check boxes
		list->Add("x-Enabled");
		list->Add("y-Enabled");
		list->Add("width-Enabled");
		list->Add("height-Enabled");
		list->Add("rotation-Enabled");
		list->Add("xScale-Enabled");
		list->Add("yScale-Enabled");
		list->Add("alpha-Enabled");
		list->Add("fontSize-Enabled");
		//Values
		list->Add("x");
		list->Add("y");
		list->Add("width");
		list->Add("height");
		list->Add("rotation");
		list->Add("xScale");
		list->Add("yScale");
		list->Add("alpha");
		list->Add("fontSize");

		long data[] =
		{
			(long)&o->Name,
			(long)&o->time,
			(long)&o->delay,
			(long)&o->iterations,
			(long)&o->transition,
			(long)&o->easeType,
			(long)&o->interpolation,
			(long)&o->tag,
			(long)&o->delta,
			(long)&o->looping,
			//Check boxes
			(long)&o->xChk,
			(long)&o->yChk,
			(long)&o->widthChk,
			(long)&o->heightChk,
			(long)&o->rotationChk,
			(long)&o->xScaleChk,
			(long)&o->yScaleChk,
			(long)&o->alphaChk,
			(long)&o->fontSizeChk,
			//Values
			(long)&o->x,
			(long)&o->y,
			(long)&o->width,
			(long)&o->height,
			(long)&o->rotation,
			(long)&o->xScale,
			(long)&o->yScale,
			(long)&o->alpha,
			(long)&o->fontSize
		};


		if (Control->ClassType() == __classid(TTextEx))
		{
			TTextEx *c = dynamic_cast<TTextEx *>(Control);
			if (c)
			{
				c->animations->Add(o);
			}
		}
		else
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(Control);
			if (c)
			{
				c->animations->Add(o);
			}
		}


		//Assign to the control
		//Figure out which object we are processing

		//Even if the class data does not exist, data still needs to
		//be read and processed
		int start = 1;
		while(start > 0 && reader->Read())
		{
			if (reader->TokenType == TJsonToken::EndObject)
			{
				start--;
			}
			else if (reader->TokenType == TJsonToken::PropertyName)
			{
				AnsiString propName = reader->Value.AsString();
				int        index    = -1;

				//Next token
				reader->Read();

				index = list->IndexOf(propName);

				if (index != -1)
				{
					if (!index || index == 7)
					{
						AnsiString *str = (AnsiString *)data[index];
						*str = reader->Value.AsString();
					}
					else if (index > 7 && index < 19)
					{
						bool *value = (bool *)data[index];
						*value = reader->Value.AsBoolean();
					}
					else if (index > 0 && index < 7)
					{
						int *value = (int *)data[index];
						*value = reader->Value.AsInteger();
					}
					else
					{
						float *value = (float *)data[index];
						*value = StrToFloat(reader->Value.ToString());
					}
				}
			}
		}

		delete list;
	}
}

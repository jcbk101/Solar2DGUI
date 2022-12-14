//---------------------------------------------------------------------------

#pragma hdrstop

#include"CodeView.h"

#pragma package(smart_init)


//--------------------------------------------------------
//
//Main loop: Source builder
//
//--------------------------------------------------------
AnsiString __fastcall UICodeView::buildSourceCode(TRectangle *Parent, AnsiString projectName, bool useTableType)
{
	TControl *object;
	AnsiString groupName = "";
	char buffer[1024];

	//Clear data and start fresh
	imgSheet->Clear();
	objs->Clear();
	groups->Clear();

	//Flag to determine is using 'local var' or 'M.var'
	variableType = useTableType;

	//Generate Base code
	generateBaseCode();


	//Build the code
	if (Parent && Parent->ChildrenCount)
	{
		for (int i = 0; i < Parent->ChildrenCount; i++)
		{
			object = dynamic_cast<TControl *>(Parent->Children->Items[i]);

			//Any children exist?
			if (object)
			{
				if (object->ClassType() == __classid(TRectangleEx))
				{
					//Build for window object
					buildWindowCode((TRectangleEx *)object, "", "\t");
				}
				else if (object->ClassType() == __classid(TTextEx))
				{
					//Build for text object
					writeNewText((TTextEx *)object, groupName,"\t");
					//Anchor code
					writeAnchors(object, "\t");
				}
			}
		}


		//If a background image exist, add to the build
		//Device Screen
		if (Parent->TagString !="" && FileExists(Parent->TagString))
		{
			char buffer[1024];
			AnsiString dir = ChangeFileExt(ExtractFileName(projectName),"");

			if (!variableType)
			{
				sprintf(buffer,
					"\t-- Background image\n"
					//"\tlocal %s_BG = display.newImageRect( \"assets/%s\", display.contentWidth, display.contentHeight )\n"
					"\tlocal %s_BG = display.newImageRect( \"assets/%s\", display.contentWidth, display.contentHeight )\n"
					"\t%s_BG.x, %s_BG.y = display.contentCenterX, display.contentCenterY\n\n",
					dir.c_str(),
					AnsiString(ExtractFileName(Parent->TagString)).c_str(),
					dir.c_str(), dir.c_str()
					);
			}
			else
			{
				sprintf(buffer,
					"\t-- Background image\n"
					//"\tlocal %s_BG = display.newImageRect( \"assets/%s\", display.contentWidth, display.contentHeight )\n"
					"\tM.%s_BG = display.newImageRect( \"assets/%s\", display.contentWidth, display.contentHeight )\n"
					"\tM.%s_BG.x, M.%s_BG.y = display.contentCenterX, display.contentCenterY\n\n",
					dir.c_str(),
					AnsiString(ExtractFileName(Parent->TagString)).c_str(),
					dir.c_str(), dir.c_str()
					);
			}

			this->baseCodeStart = this->baseCodeStart + AnsiString(buffer);
		}

		//Finish the job
		if (imgSheet->Count)
		{
			AnsiString path  = "";
			AnsiString sheet = "";
			AnsiString asset = "";
			AnsiString file  = "";
			AnsiString req   = "";
			char buffer[1024];

			//Move all assets s into this folder within the main project folder
			//Then use this as a reference to the images
			//assets = dir +"assets.";

			for (int i = 0; i < imgSheet->Count; i++)
			{
				//
				file = "assets/"+ ChangeFileExt(ExtractFileName(imgSheet->Strings[i]),".png");
				asset = ChangeFileExt(ExtractFileName(imgSheet->Strings[i]),"");

				if (!variableType)
				{
					req = req + "\tlocal req_" + asset + " = require(\"" + "assets." + asset + "\")\n";
					req = req + "\tlocal sheet_" + asset + " = graphics.newImageSheet(\"" + file + "\", req_" + asset + ":getSheet())\n \n";
				}
				else
				{
					req = req + "\M.req_" + asset + " = require(\"" + "assets." + asset + "\")\n";
					req = req + "\tM.sheet_" + asset + " = graphics.newImageSheet(\"" + file + "\", M.req_" + asset + ":getSheet())\n \n";
				}
			}

			//Write it all out
			this->baseCodeStart = this->baseCodeStart + req;
		}
	}

	//Generate Clean-up code
	generateCleanUpCode();

	//Retun the final string
	return (this->baseCodeStart + this->codeString + this->baseCleanCode + this->baseCodeEnd);
}


//--------------------------------------------------------
//
//Generate the base INIT code
//
//--------------------------------------------------------
void __fastcall UICodeView::generateBaseCode()
{
	//Clear the string buffer and start new
	this->codeString = "";
	this->baseCodeStart =
		(
		"local M = {}\n"
		"\n"
		"function M.new()\n \n"
		"\t-- Code to display GUI \n"
		);


	this->baseCodeEnd =
		(
		"end\n\n"
		"return M\n"
		);

	//Setup the image sheet list
	imgSheet->Duplicates = System::Types::TDuplicates::dupIgnore;
	imgSheet->Sorted = true;
}


//--------------------------------------------------------
//
//Generate the base INIT code
//
//--------------------------------------------------------
void __fastcall UICodeView::generateCleanUpCode()
{
	char buffer[1024];

	//Clear the string buffer and start new
	this->baseCleanCode =
		(
		"\t-- Clean up code\n"
		"\tfunction M:finalize()\n"
		"\n"
		);


	if (imgSheet->Count)
	{
		this->baseCleanCode = this->baseCleanCode + "\t\t-- Release image sheet data\n";

		for (int i = 0; i < imgSheet->Count; i++)
		{
			if (!variableType)
			{
				this->baseCleanCode = this->baseCleanCode + "\t\treq_" + ChangeFileExt(imgSheet->Strings[i], "") + " = nil\n";
				this->baseCleanCode = this->baseCleanCode + "\t\tsheet_" + ChangeFileExt(imgSheet->Strings[i], "") + " = nil\n\n";
			}
			else
			{
				this->baseCleanCode = this->baseCleanCode + "\t\tM.req_" + ChangeFileExt(imgSheet->Strings[i], "") + " = nil\n";
				this->baseCleanCode = this->baseCleanCode + "\t\tM.sheet_" + ChangeFileExt(imgSheet->Strings[i], "") + " = nil\n\n";
			}
		}

		this->baseCleanCode = this->baseCleanCode + "\n";
	}

	if (objs->Count)
	{
		this->baseCleanCode = this->baseCleanCode + "\t\t-- Release display objects\n";
		for (int i = 0; i < objs->Count; i++)
		{
			if (!variableType)
			{
				this->baseCleanCode = this->baseCleanCode + "\t\tdisplay.remove( " + objs->Strings[i] + ")\n";
				this->baseCleanCode = this->baseCleanCode + "\t\t" + objs->Strings[i] + " = nil \n";
			}
			else
			{
				this->baseCleanCode = this->baseCleanCode + "\t\tdisplay.remove( M." + objs->Strings[i] + ")\n";
				this->baseCleanCode = this->baseCleanCode + "\t\tM." + objs->Strings[i] + " = nil \n";
			}
		}
		this->baseCleanCode = this->baseCleanCode + "\n";
	}

	if (groups->Count)
	{
		this->baseCleanCode = this->baseCleanCode + "\t\t-- Release group objects\n";

		for (int i = 0; i < groups->Count; i++)
		{
			if (!variableType)
			{
				this->baseCleanCode = this->baseCleanCode + "\t\tdisplay.remove(" + groups->Strings[i] + ")\n";
				this->baseCleanCode = this->baseCleanCode + "\t\t" + groups->Strings[i] + " = nil\n";
			}
			else
			{
				this->baseCleanCode = this->baseCleanCode + "\t\tdisplay.remove(M." + groups->Strings[i] + ")\n";
				this->baseCleanCode = this->baseCleanCode + "\t\tM." + groups->Strings[i] + " = nil\n";
			}
		}
		this->baseCleanCode = this->baseCleanCode + "\n";
	}

	this->baseCleanCode = this->baseCleanCode +
		(
		"\t\t-- Garbage collection\n"
		"\t\tcollectgarbage()\n"
		"\tend\n"
		);
}


//--------------------------------------------------------
//
//Build the code for windows used in the GUI
//
//--------------------------------------------------------
void __fastcall UICodeView::buildWindowCode(TRectangleEx *window, AnsiString group, AnsiString indent)
{
	AnsiString groupName = group;
	char buffer[1024];

	//Find all data attached to this object and generate LUA code for it
	//Windows can be a typical 'imageRect' or be in a 'Group'
	if (window)
	{
		if (window->ChildrenCount)
		{
			if (group != "")
			{
				//New group is nested within a parent group
				groupName = window->Name + "_group";
				groups->Add(groupName);

				if (!variableType)
				{
					this->codeString = this->codeString + indent +"local " + groupName + " = display.newGroup()\n";
					//Nest this inside of the parent grop
					this->codeString = this->codeString + indent + group + ":insert(" + groupName + ")\n";
				}
				else
				{
					this->codeString = this->codeString + indent + "M." + groupName + " = display.newGroup()\n";
					//Nest this inside of the parent grop
					this->codeString = this->codeString + indent + "M." + group + ":insert(M." + groupName + ")\n";
				}
			}
			else
			{
				//Their are nested children. Create a group
				groupName = window->Name + "_group";
				groups->Add(groupName);

				if (!variableType)
					this->codeString = this->codeString + indent + "local " + groupName + " = display.newGroup()\n";
				else
					this->codeString = this->codeString + indent + "M." + groupName + " = display.newGroup()\n";
			}
		}

		//What type of window is this?
		//1: 'imageRect' with regular image / nine patch / texture atlas pointer
		//2: 'newRect' with a color?
		if (window->TagString !="")
		{
			//Regular 'imageRect' (IE: nine patch / Standard image / Image Sheet)
			if (window->imageType == TRectangleEx::Standard)
			{
				writeNewImageRect(window, groupName, indent);
			}
			else if (window->imageType == TRectangleEx::NinePatch)
			{
				//Currently, NinePatch is not a supported internal format
				writeNewImageRect(window, groupName, indent);
			}
			else if (window->imageType == TRectangleEx::ImageSheet)
			{
				//Use which ever image sheet the object gets it's
				//data from. Add to the list of Image sheets used
				int indexOf = imgSheet->IndexOf(window->TagString);
				AnsiString sheetName;

				//Check if it exist
				if (indexOf > -1)
				{
					//imgSheet->Add(imgSheet->Strings[indexOf]);
					sheetName = ExtractFileName(imgSheet->Strings[indexOf]);
					//Set up for clean up later
					imgSheet->Add(sheetName);
				}
				else
				{
					sheetName = ExtractFileName(window->TagString);
					//Set up for clean up later
					imgSheet->Add(sheetName);
				}

				//Remove the EXT to constuct a good variable call
				sheetName = "sheet_" + ChangeFileExt(sheetName,"");
				writeNewImageSheetRect(window, groupName, AnsiString(sheetName), window->imageIndex, indent);

				//Set up for clean up later
				//imgSheet->Add(sheetName);
			}
		}
		else
		{
			//Regular 'newRect' with a color
			writeNewRect(window, groupName, indent);
		}

		//-----------------------------------------------------------
		//
		//Anchor String: Only print if anchors are NOT ".5 and .5"
		//
		//-----------------------------------------------------------
		writeAnchors(window, indent);

		//Check for recursive build
		if (window->ChildrenCount)
		{
			//Use '{}' to force formating
			this->codeString = this->codeString + indent + "-- [Children of '" + window->Name + "']\n";

			for (int i = 0; i < window->ChildrenCount; i++)
			{
				TControl *c = dynamic_cast<TControl *>(window->Children->Items[i]);
				int x;
				int y;

				if (c)
				{
					if (c->ClassType() == __classid(TRectangleEx))
					{
						//Recursive call
						//Adjust positions tempararily for nested placememnt
						x = c->Position->X;
						y = c->Position->Y;
						c->Position->X += window->Position->X;
						c->Position->Y += window->Position->Y;
						//
						buildWindowCode((TRectangleEx *)c, groupName, indent +"\t");
						//
						c->Position->X = x;
						c->Position->Y = y;

					}
					else if (c->ClassType() == __classid(TTextEx))
					{
						x = c->Position->X;
						y = c->Position->Y;
						c->Position->X += window->Position->X;
						c->Position->Y += window->Position->Y;
						//
						writeNewText((TTextEx *)c, groupName, indent +"\t");
						//Anchor code
						writeAnchors(c, indent + "\t");
						//
						c->Position->X = x;
						c->Position->Y = y;
					}
				}
			}

			this->codeString = this->codeString + indent + "-- [/Children]\n";

			//When a group is animated, positioning is important
			//Make sure that group.anchorChildren = true
			//Make sure that group.x/y = main Window's x/y
			if (!variableType)
			{
				this->codeString = this->codeString + indent + groupName + ".anchorChildren = true\n";
				sprintf(buffer,"%s.x, %s.y = %s.x, %s.y\n\n", groupName.c_str(), groupName.c_str(), AnsiString(window->Name), AnsiString(window->Name));
			}
			else
			{
				this->codeString = this->codeString + indent + "M." + groupName + ".anchorChildren = true\n";
				sprintf(buffer,"M.%s.x, M.%s.y = M.%s.x, M.%s.y\n\n", groupName.c_str(), groupName.c_str(), AnsiString(window->Name), AnsiString(window->Name));
			}


			this->codeString = this->codeString + indent + AnsiString(buffer);

			//Whenever a 'window' has children, the window's animation
			//shall apply to the group it created
			writeAnimation(window, groupName, indent);
			this->codeString = this->codeString + "\n";
		}
		else
		{
			//Write out this object's animation code
			writeAnimation(window, window->Name, indent);
			this->codeString = this->codeString + "\n";
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall UICodeView::writeNewRect(TRectangleEx *window, AnsiString groupName, AnsiString indent)
{
	//This code adjusts the object so it is aligned properly in Solar2D
	//Using the anchorX/Y values
	int x = (int)(window->Position->X + (window->Width * window->RotationCenter->X));
	int y = (int)(window->Position->Y + (window->Height * window->RotationCenter->Y));
	char buffer[1024];

	//Convert the colors to Solar2D format
	float b = (window->Fill->Color & 0x000000FF) / (float)0xFF,
		g = (window->Fill->Color & 0x0000FF00) / (float)0xFF00,
		r = (window->Fill->Color & 0x00FF0000) / (float)0xFF0000,
		a = (window->Fill->Color & 0xFF000000) / (float)0xFF000000;

	AnsiString group = "";
	//
	if (groupName !="")
		group = AnsiString(groupName +", ");

	//Regular 'newRect' with a color
	if (!variableType)
	{
		sprintf(buffer,
			"%slocal %s = display.newRect(%s%d, %d, %d, %d)\n"
			"%s%s:setFillColor(%.2f, %.2f, %.2f, %.2f)\n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),
			x, y,
			(int)window->Width,
			(int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			r, g, b, a
			);
	}
	else
	{
		sprintf(buffer,
			"%sM.%s = display.newRect(M.%s%d, %d, %d, %d)\n"
			"%sM.%s:setFillColor(%.2f, %.2f, %.2f, %.2f)\n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),
			x, y,
			(int)window->Width,
			(int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			r, g, b, a
			);
	}
	//Set up for clean up later
	objs->Add(window->Name);

	//Add to string builder
	this->codeString = this->codeString + AnsiString(buffer);
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall UICodeView::writeNewImageRect(TRectangleEx *window, AnsiString groupName, AnsiString indent)
{
	//This code adjust the object so it is aligned properly in Solar2D
	//Using the anchorX/Y values
	int x = (int)(window->Position->X + (window->Width * window->RotationCenter->X));
	int y = (int)(window->Position->Y + (window->Height * window->RotationCenter->Y));
	char buffer[1024];

	AnsiString group = "";
	//
	if (groupName !="")
		group = AnsiString(groupName +", ");

	//Regular 'newImageRect'
	if (!variableType)
	{
		sprintf(buffer,
			"%slocal %s = display.newImageRect(%s\"%s \", %d, %d)\n"
			"%s%s.x, %s.y = %d, %d \n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),

			AnsiString("assets/"+ ExtractFileName(window->TagString)).c_str(),
			//AnsiString(window->TagString).c_str(),

			(int)window->Width, (int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(), AnsiString(window->Name).c_str(),
			x, y
			);
	}
	else
	{
		sprintf(buffer,
			"%sM.%s = display.newImageRect(M.%s\"%s \", %d, %d)\n"
			"%sM.%s.x, M.%s.y = %d, %d \n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),

			AnsiString("assets/"+ ExtractFileName(window->TagString)).c_str(),
			//AnsiString(window->TagString).c_str(),

			(int)window->Width, (int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(), AnsiString(window->Name).c_str(),
			x, y
			);
	}
	//Set up for clean up later
	objs->Add(window->Name);

	//Add to string builder
	this->codeString = this->codeString + AnsiString(buffer);
}


void __fastcall UICodeView::writeNewImageSheetRect(TRectangleEx *window, AnsiString groupName, AnsiString sheetName, int sheetIndex, AnsiString indent)
{
	//This code adjust the object so it is aligned properly in Solar2D
	//Using the anchorX/Y values
	int x = (int)(window->Position->X + (window->Width * window->RotationCenter->X));
	int y = (int)(window->Position->Y + (window->Height * window->RotationCenter->Y));
	char buffer[1024];

	AnsiString group = "";
	//
	if (groupName !="")
		group = AnsiString(groupName +", ");

	//Regular 'newImageRect' using an image sheet index
	if (!variableType)
	{
		sprintf(buffer,
			"%slocal %s = display.newImageRect(%s%s, %d, %d, %d)\n"
			"%s%s.x, %s.y = %d, %d \n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),
			AnsiString(sheetName).c_str(), sheetIndex,
			(int)window->Width, (int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(), AnsiString(window->Name).c_str(),
			x, y
			);
	}
	else
	{
		sprintf(buffer,
			"%sM.%s = display.newImageRect(M.%s%s, %d, %d, %d)\n"
			"%sM.%s.x, M.%s.y = %d, %d \n",
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(),
			AnsiString(group).c_str(),
			AnsiString(sheetName).c_str(), sheetIndex,
			(int)window->Width, (int)window->Height,
			//
			AnsiString(indent).c_str(),
			AnsiString(window->Name).c_str(), AnsiString(window->Name).c_str(),
			x, y
			);
	}
	//Set up for clean up later
	objs->Add(window->Name);

	//Add to string builder
	this->codeString = this->codeString + AnsiString(buffer);
}


//--------------------------------------------------------
//
//Build the code for TEXT used in the GUI
//
//--------------------------------------------------------
void __fastcall UICodeView::writeNewText(TTextEx *text, AnsiString groupName, AnsiString indent)
{
	//This code adjust the object so it is aligned properly in Solar2D
	//Using the anchorX/Y values
	int        x       = (int)(text->Position->X + (text->Width * text->RotationCenter->X));
	int        y       = (int)(text->Position->Y + (text->Height * text->RotationCenter->Y));
	AnsiString buffer  = "";
	AnsiString options = "";
	AnsiString align[] =
{
		"center",
		"left",
		"right"
	};

	//Convert the colors to Solar2D format
	char numbers[1024];
	float b = (text->TextSettings->FontColor & 0x000000FF) / (float)0xFF,
		g = (text->TextSettings->FontColor & 0x0000FF00) / (float)0xFF00,
		r = (text->TextSettings->FontColor & 0x00FF0000) / (float)0xFF0000,
		a = (text->TextSettings->FontColor & 0xFF000000) / (float)0xFF000000;

	AnsiString group = "";
	//
	if (groupName !="")
		group = AnsiString(groupName + ", ");

	//Regular 'newText'
	//buffer = indent + "local " + text->Name + " = display.newText( " + group + text->Name + "_options )\n";
	if (!variableType)
	{
		buffer = indent + "local " + text->Name + " = display.newText( " + text->Name + "_options )\n";
		options = "\n" + indent + "-- Text data\n";
		options = options + indent + "local " + text->Name + "_options = {\n";
	}
	else
	{
		buffer = indent + "M." + text->Name + " = display.newText( M." + text->Name + "_options )\n";
		options = "\n" + indent + "-- Text data\n";
		options = options + indent + "M." + text->Name + "_options = {\n";
	}

	options = options + indent + "\ttext = \"" + text->Text + "\",\n";
	options = options + indent + "\tx = " + IntToStr(x) + ",\n";
	options = options + indent + "\ty = " + IntToStr(y) + ",\n";
	options = options + indent + "\twidth = " + FloatToStr(text->Width) + ",\n";
	options = options + indent + "\tfont = \"" + text->Font->Family + "\",\n";
	options = options + indent + "\tfontSize = " + FloatToStr(text->Font->Size) + ",\n";
	options = options + indent + "\talign = \"" + align[text->HorzTextAlign] + "\"\n";
	options = options + indent + "}\n";

	if (!variableType)
	{
		sprintf(numbers,
			"%s:setFillColor(%.2f, %.2f, %.2f, %.2f)\n",
			AnsiString(text->Name)
			.c_str(),
			r, g, b, a
			);
	}
	else
	{
		sprintf(numbers,
			"M.%s:setFillColor(%.2f, %.2f, %.2f, %.2f)\n",
			AnsiString(text->Name)
			.c_str(),
			r, g, b, a
			);
	}
	//Set up for clean up later
	objs->Add(text->Name);

	//Add to string builder
	this->codeString = this->codeString + options + buffer + indent + AnsiString(numbers);

	if (group != "")
	{
		if (!variableType)
			this->codeString = this->codeString + indent + groupName + ":insert(" + text->Name + ")\n\n";
		else
			this->codeString = this->codeString + indent + "M." + groupName + ":insert(M." + text->Name + ")\n\n";
	}
	else
		this->codeString = this->codeString + "\n";
}


void __fastcall UICodeView::writeAnchors(TObject *Control, AnsiString indent)
{
	char buffer[1024];
	TTextEx *c = static_cast<TTextEx *>(Control);
	//
	//Anchor String: Only print if anchors are NOT".5 and .5"
	//
	if (c)
	{
		if (c->RotationCenter->X != 0.5 || c->RotationCenter->Y != 0.5)
		{
			if (!variableType)
			{
				sprintf(buffer,
					"%s.anchorX, %s.anchorY = %.2f, %.2f \n \n",
					AnsiString(c->Name).c_str(), AnsiString(c->Name).c_str(),
					c->RotationCenter->X, c->RotationCenter->Y
					);

			}
			else
			{
				sprintf(buffer,
					"M.%s.anchorX, M.%s.anchorY = %.2f, %.2f \n \n",
					AnsiString(c->Name).c_str(), AnsiString(c->Name).c_str(),
					c->RotationCenter->X, c->RotationCenter->Y
					);
			}

			this->codeString = this->codeString + indent + AnsiString(buffer);
		}
		else
		{
			this->codeString = this->codeString +"\n";
		}
	}
}


//--------------------------------------------------------
//
//Build the animation code for TEXT used in the GUI
//
//--------------------------------------------------------
void __fastcall UICodeView::writeAnimation(TControl *Control, AnsiString target, AnsiString indent)
{
	UIAnimation *anim;
	TList *animList;
	AnsiString animationStr = "";

	if (Control)
	{
		AnsiString toFrom[] =
		{
			"transition.to",
			"transition.from"
		};
		AnsiString property[] =
		{
			"x = ",
			"y = ",
			"width = ",
			"height = ",
			"rotation = ",
			"xScale = ",
			"yScale = ",
			"alpha = ",
			"size = "
		};

		AnsiString transitionTypes[] =
		{
			"easing.in",
			"easing.out",
			"easing.inOut",
			"easing.outIn"
		};

		AnsiString interpolation[] =
		{
			"Linear",
			"Quad",
			"Cubic",
			"Quart",
			"Quint",
			"Sine",
			"Expo",
			"Circ",
			"Elastic",
			"Back",
			"Bounce"
		};

		//float *startValue;
		char buffer[2048];

		//
		if (Control->ClassType() == __classid(TRectangleEx))
		{
			TRectangleEx *c = dynamic_cast<TRectangleEx *>(Control);
			animList = ((TRectangleEx *)Control)->animations;
		}
		else if (Control->ClassType() == __classid(TTextEx))
		{
			TTextEx *c = dynamic_cast<TTextEx *>(Control);
			animList = ((TTextEx *)Control)->animations;
		}

		//There are animation that exist
		if (animList)
		{
			//
			if (animList->Count)
				animationStr = indent + "-- Animation for '" + target + "' \n";

			//
			//Loop thru all the animations
			//
			for (int i = 0; i < animList->Count; i++)
			{
				anim = (UIAnimation *)animList->Items[i];
				AnsiString animation = "";

				if (anim)
				{
					bool chkBoxes[] =
					{
						anim->xChk,
						anim->yChk,
						anim->widthChk,
						anim->heightChk,
						anim->rotationChk,
						anim->xScaleChk,
						anim->yScaleChk,
						anim->alphaChk,
						anim->fontSizeChk
					};

					float startValue[] =
					{
						anim->x,
						anim->y,
						anim->width,
						anim->height,
						anim->rotation,
						anim->xScale,
						anim->yScale,
						anim->alpha,
						anim->fontSize
					};

					bool transitionType = (anim->transition == 1);

					if (!variableType)
						animation = toFrom[anim->transition] + "( " + target + ", { ";
					else
						animation = toFrom[anim->transition] + "( M." + target + ", { ";

					//
					animation = animation + "time = " + anim->time + ", ";
					animation = animation + "delay = " + anim->delay + ", ";
					//animation = animation + "transition = " + interpolation[anim->interpolation] + ", ";

					//
					if (anim->iterations > 1)
						animation = animation + "iterations = " + anim->iterations + ", ";
					if (anim->tag != "")
						animation = animation + "tag = \"" + anim->tag + "\", ";

					//Handle interpolations
					if (anim->looping)
					{
						animation = animation + "transition = easing.continousLoop, ";
					}
					else if(anim->interpolation == 0)
					{
						animation = animation + "transition = easing.Linear, ";
					}
					else
					{
						animation = animation + "transition = " + transitionTypes[anim->easeType] + interpolation[anim->interpolation] + ", ";
					}

					//Delta type?
					if (anim->delta)
						animation = animation + "delta = true, ";

					//
					for (int c = 0; c < sizeof(chkBoxes); c++)
					{
						if (chkBoxes[c])
						{
							if (c == 5 || c == 6)
							{
								//X/Y Scale cannot be set to ZERO!
								float value = startValue[c];

								if (value == 0)
									value = 0.01;
								//
								animation = animation + property[c] + Format("%.2f ", ARRAYOFCONST((value))) + ", ";
							}
							else
							{
								//Using FROM setting: From value entered to current value
								animation = animation + property[c] + Format("%.2f ", ARRAYOFCONST((startValue[c]))) + ", ";
							}
						}
					}

					//
					strcpy(buffer, animation.c_str());
					if (buffer[strlen(buffer) - 2] == ',' && buffer[strlen(buffer) - 1] == ' ')
						buffer[strlen(buffer) - 2] = 0;
					//
					animation = AnsiString(buffer);

					//End it
					animation = animation + "} )\n";

					//Add to the main buffer
					animationStr = animationStr + indent + animation;
				}
			}
		}
	}

	this->codeString = this->codeString + animationStr;
}

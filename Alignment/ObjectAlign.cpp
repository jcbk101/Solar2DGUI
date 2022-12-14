//---------------------------------------------------------------------------

#pragma hdrstop

#include "ObjectAlign.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//--------------------------------------------------------
//
//List compare testing X values
//
//--------------------------------------------------------
int __fastcall listCompareX(void* Item1, void* Item2)
{
	if (((TControl *)Item1)->Position->X < ((TControl *)Item2)->Position->X)
		return -1;
else
		if (((TControl *)Item2)->Position->X < ((TControl *)Item1)->Position->X)
		return 1;
	else
		return 0;
}


//--------------------------------------------------------
//
//List compare testing Y values
//
//--------------------------------------------------------
int __fastcall listCompareY(void* Item1, void* Item2)
{
	if (((TControl *)Item1)->Position->Y < ((TControl *)Item2)->Position->Y)
		return -1;
else
		if (((TControl *)Item2)->Position->Y < ((TControl *)Item1)->Position->Y)
		return 1;
	else
		return 0;
}


//--------------------------------------------------------
//
//Main hub for alignment request
//
//--------------------------------------------------------
bool __fastcall formatSelectedObjects(TControl* DeviceParent, TObject* Sender, TStringList* List)
{
	//6  = Delete current / multi-selection
	//7  = Space objects in window vertically with equal space (Only > 1)
	//8  = Space objects in window orizontally with equal space (Only > 1)
	//9  = Aign objects to the left side of the window
	//10 = Aign objects to the top side of the window
	//11 = Aign objects to the right side of the window
	//12 = Aign objects to the bottom side of the window
	//13 = Aign objects to the window's vertical center
	//14 = Aign objects to the window's horizontal center
	//15 = Space selected objects equally vertically
	//16 = Space selected objects equally horizontally
	//18 = Fully expand objects in window
	//19 = Collapse objects to a smaller size in window
	//20 = Expand objects vertically in window
	//21 = Expand objects horizontally in window
	//22  = Aign objects to the left side of the first selected object
	//23 = Aign objects to the top side of the first selected object
	//24 = Aign objects to the right side of the first selected object
	//25 = Aign objects to the bottom side of the first selected object
	//26 = Aign objects to the first selected object vertical center
	//27 = Aign objects to the first selected object horizontal center
	//28 Align right edges of objects to the left egde of the anchor
	//29 Align bottom edges of objects to the top egde of the anchor
	//30 Align left edges of objects to the right egde of the anchor
	//31 Align top edges of objects to the bottom egde of the anchor
	TSpeedButton* sb = dynamic_cast<TSpeedButton *>(Sender);
	TList* temp;
	bool wasDeleted = false;

	//---------------------------------
	//
	//Actions: Support for multi-select
	//
	//---------------------------------
	if (sb && List->Count)
	{
		//Create a list of objects from the string names
		TControl* c = dynamic_cast<TControl *>(findObjectByName(DeviceParent, AnsiString(List->Strings[0])));

		if (c)
		{
			TControl* Parent = dynamic_cast<TControl *>(c->Parent);
			temp = new TList();

			//Transfer
			for (int i = 0; i < List->Count; i++)
			{
				TControl* c = dynamic_cast<TControl *>(findObjectByName(DeviceParent, AnsiString(List->Strings[i])));
				temp->Add(c);
			}


			//Align to either side or V/H center of a parent window
			for (int i = 0; i < temp->Count; i++)
			{
				TControl* c         = (TControl *)temp->Items[i];
				int       winWidth  = ((TControl *)c->Parent)->Width;
				int       winHeight = ((TControl *)c->Parent)->Height;

				if (c)
				{
					switch(sb->Tag)
					{
					case 6:
						deleteSelectedObjects(DeviceParent, List);
						i = temp->Count;
						wasDeleted = true;
						break;
					case 7:
						spaceVertInWindow(Parent, temp);
						i = temp->Count;
						break;
					case 8:
						spaceHoriInWindow(Parent, temp);
						i = temp->Count;
						break;
					case 9:
						c->Position->X = 0;
						break;
					case 10:
						c->Position->Y = 0;
						break;
					case 11:
						c->Position->X = (winWidth - c->Width);
						break;
					case 12:
						c->Position->Y = (winHeight- c->Height);
						break;
					case 13:
						c->Position->X = (winWidth / 2) - (c->Width /2);
						break;
					case 14:
						c->Position->Y = (winHeight / 2) - (c->Height /2);
						break;
					case 15:
						spaceVertWithObjects(Parent, temp);
						i = temp->Count;
						break;
					case 16:
						spaceHoriWithObjects(Parent, temp);
						i = temp->Count;
						break;
					case 18:
						c->Position->X = 0;
						c->Position->Y = 0;
						c->Width = winWidth;
						c->Height = winHeight;
						break;
					case 19:
						//Would be better to support an 'UNDO' feature
						//Testing: Current object MUST be expanded fully already
						if (c->Width == winWidth && c->Height == winHeight)
						{
							c->Position->X = (i * 3);
							c->Position->Y = (i * 3);
							c->Width = (winWidth / 3);
							c->Height = (winHeight / 3);
						}
						break;
					case 20:
						c->Position->Y = 0;
						c->Height = winHeight;
						break;
					case 21:
						c->Position->X = 0;
						c->Width = winWidth;
						break;
					case 22:
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 31:
						alignXxOfObjects(Parent, temp, sb->Tag - 22);
						i = temp->Count;
						break;
					}
				}
			}
		}

		//Free memory
		delete temp;
	}

	return wasDeleted;
}


//---------------------------------
//
//Delete selected objects
//
//---------------------------------
void __fastcall deleteSelectedObjects(TControl* DeviceParent, TStringList* List)
{
	TmsgForm* msgForm = new TmsgForm(Application);

	msgForm->Caption = AnsiString("Delete selected object") + (List->Count > 1 ? "s...": "...");
	msgForm->messageText->Text = AnsiString("Delete the selected object") + (List->Count > 1 ? "s": "") + " from the design form?";
	msgForm->ShowModal();

	//What result did we receive?
	if (msgForm->ModalResult == mrYes)
	{
		try
		{
			//Iterate thru the list
			for (int i = 0; i < List->Count; i++)
			{
				TControl* c = dynamic_cast<TControl *>(findObjectByName(DeviceParent, AnsiString(List->Strings[i])));

				//Delete it if found
				if (c)
				{
					//deviceScreen->Delete()c);
					c->Parent = NULL;
					delete c;
				}
			}
		}
		catch(...)
		{
			//crshing when borders are active for delete
			int a = 1;
		}
	}

	msgForm->DisposeOf();
}


//---------------------------------
//
//Space vertically using window
//
//---------------------------------
void __fastcall spaceVertInWindow(TControl* Parent, TList* temp)
{
	int windowHeight   = Parent->Height;
	int combinedHeight = 0;
	int padding        = 0;
	int advance        = 0;

	//Get the combined height of the objects
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
			combinedHeight += c->Height;
	}

	//Do not space if the objects are larger than the window
	if (combinedHeight > windowHeight)
	{
		ShowMessage("Error: Objects vertical coverage with spacing\nis greater than window's vertical dimension.");
		//delete temp;
		return;
	}

	//Important: To maintain positions on screen, sort the
	//list NOT BY SELECTION, but from top to bottom
	//Only affected objects are the ones between the left and right objects
	temp->Sort(listCompareY);

	//All good? Space away!
	//Calculate spacing formula:
	/*
	 SpaceToSplit = (WindowHeight - CombinedHeight)
	 SpaceBetweenObjects = SpaceToSplit / (ObjectCount + 1);
	 SpaceBetweenObjects is the amount of 'Padding' around each side of the objects
	 */
	padding = (windowHeight - combinedHeight) / (temp->Count + 1);
	advance = padding;

	//Space the objects
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			c->Position->Y = advance;
			advance += (c->Height + padding);
		}
	}
}


//---------------------------------
//
//Space horizontally using window
//
//---------------------------------
void __fastcall spaceHoriInWindow(TControl* Parent, TList* temp)
{
	int windowWidth   = Parent->Width;
	int combinedWidth = 0;
	int padding       = 0;
	int advance       = 0;

	//Get the combined width of the objects
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
			combinedWidth += c->Width;
	}

	//Do not space if the objects are larger than the window
	if (combinedWidth > windowWidth)
	{
		ShowMessage("Error: Objects horizontal coverage with spacing\nis greater than window's horizontal dimension.");
		//delete temp;
		return;
	}

	//Important: To maintain positions on screen, sort the
	//list NOT BY SELECTION, but from left to right
	//Only affected objects are the ones between the left and right objects
	temp->Sort(listCompareX);

	//All good? Space away!
	//Calculate spacing formula:
	/*
	 SpaceToSplit = (WindowWidth - CombinedWidth)
	 SpaceBetweenObjects = SpaceToSplit / (ObjectCount + 1);
	 SpaceBetweenObjects is the amount of 'Padding' around each side of the objects
	 */
	padding = (windowWidth - combinedWidth) / (temp->Count + 1);
	advance = padding;

	//Space the objects
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			c->Position->X = advance;
			advance += (c->Width + padding);
		}
	}
}


//---------------------------------
//
//Space vertically using objects
//MUST be greater than 2 objects
//
//---------------------------------
void __fastcall spaceVertWithObjects(TControl* Parent, TList* temp)
{
	int windowHeight   = Parent->Height;
	int combinedHeight = 0;
	int top            = 1000000;
	int bottom         = -1000000;
	int padding        = 0;
	int advance        = 0;
	int bottomOfTop    = 0;
	int topOfBottom    = 0;
	int tHeight        = 0;
	int bHeight        = 0;

	//Get the combined height of the objects
	//Also get the highest point and lowest point
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			combinedHeight += c->Height;

			if (c->Position->Y < top)
			{
				top = c->Position->Y;
				bottomOfTop = top + c->Height;
				tHeight = c->Height;
			}

			if ((c->Position->Y + c->Height) > bottom)
			{
				bottom = (c->Position->Y + c->Height);
				topOfBottom = c->Position->Y;
				bHeight = c->Height;
			}
		}
	}

	//Important: To maintain positions on screen, sort the
	//list NOT BY SELECTION, but from top to bottom
	//Only affected objects are the ones between the top and bottom objects
	temp->Sort(listCompareY);

	//The space to work with
	windowHeight = (topOfBottom - bottomOfTop);
	combinedHeight -= (tHeight + bHeight);

	//Do not space if the objects are larger than the window
	if (combinedHeight > windowHeight)
	{
		ShowMessage("Error: Objects vertical coverage with spacing\nis greater than area occupied.");
		//delete temp;
		return;
	}

	//All good? Space away!
	padding = (windowHeight - combinedHeight) / (temp->Count - 1);
	advance = bottomOfTop + padding;

	//Space the objects: First and last are not touched
	for (int i = 1; i < temp->Count - 1; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			c->Position->Y = advance;
			advance += (c->Height + padding);
		}
	}
}


//---------------------------------
//
//Space horizontally using objects
//MUST be greater than 2 objects
//
//---------------------------------
void __fastcall spaceHoriWithObjects(TControl* Parent, TList* temp)
{
	int windowWidth   = Parent->Width;
	int combinedWidth = 0;
	int left          = 1000000;
	int right         = -1000000;
	int padding       = 0;
	int advance       = 0;
	int rightOfLeft   = 0;
	int leftOfRight   = 0;
	int lWidth        = 0;
	int rWidth        = 0;

	//Get the combined with of the objects
	for (int i = 0; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			combinedWidth += c->Width;

			if (c->Position->X < left)
			{
				left = c->Position->X;
				rightOfLeft = left + c->Width;
				lWidth = c->Width;
			}

			if ((c->Position->X + c->Width) > right)
			{
				right = (c->Position->X + c->Width);
				leftOfRight = c->Position->X;
				rWidth = c->Width;
			}
		}
	}

	//Important: To maintain positions on screen, sort the
	//list NOT BY SELECTION, but from left to right
	//Only affected objects are the ones between the left and right objects
	temp->Sort(listCompareX);

	//The space to work with
	windowWidth = (leftOfRight - rightOfLeft);
	combinedWidth -= (lWidth + rWidth);

	//Do not space if the objects are larger than the window
	if (combinedWidth > windowWidth)
	{
		ShowMessage("Error: Objects horizontal coverage with spacing\nis greater than area occupied.");
		//delete temp;
		return;
	}

	//All good? Space away!
	//MUST removed the end shape widths from the final equation
	//Because they do not adjust
	padding = (windowWidth - combinedWidth) / (temp->Count - 1);
	advance = rightOfLeft + padding;

	//Space the objects: First and last are not touched
	for (int i = 1; i < temp->Count - 1; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			c->Position->X = advance;
			advance += (c->Width + padding);
		}
	}
}


//---------------------------------
//
//Align Top/Bottom with First Obj
//
//---------------------------------
void __fastcall alignXxOfObjects(TControl* Parent, TList* temp, int sideOf)
{
	int       top     = 0;
	int       padding = 0;
	int       advance = 0;
	TControl* first   = (TControl *)temp->Items[0];

	//Align the objects: First object selected is the anchor object
	for (int i = 1; i < temp->Count; i++)
	{
		TControl* c = (TControl *)temp->Items[i];

		if (c)
		{
			switch(sideOf)
			{
			case 1:
				//Tops
				c->Position->Y = first->Position->Y; break;
			case 2:
				//Right
				c->Position->X = (first->Position->X + first->Width) - c->Width;
				break;
			case 3:
				//Bottom
				c->Position->Y = (first->Position->Y + first->Height) - c->Height;
				break;
			case 4:
				//Horizontal Center
				c->Position->X = first->Position->X + ((first->Width/2) - (c->Width/2));
				break;
			case 5:
				//Vertical Center
				c->Position->Y = first->Position->Y + ((first->Height/2) - (c->Height/2));
				break;
			case 6:
				//Left of first selected (anchor)
				c->Position->X = (first->Position->X - c->Width);
				break;
			case 7:
				//Top of first selected (anchor)
				c->Position->Y = (first->Position->Y - c->Height);
				break;
			case 8:
				//Right of first selected (anchor)
				c->Position->X = (first->Position->X + first->Width);
				break;
			case 9:
				//Bottom of first selected (anchor)
				c->Position->Y = (first->Position->Y + first->Height);
				break;
			case 0:
			default:
				//Left
				c->Position->X = first->Position->X; break;
				break;
			}
		}
	}
}


//---------------------------------
//
//
//
//---------------------------------
TControl* __fastcall findObjectByName(TObject* parent, AnsiString Name)
{
	//Recursive method
	//If not found or not applicable, return -1
	TFmxObject* Parent = (TFmxObject *)parent;
	TList*      list   = new TList();
	int i;

	try
	{
		if (Parent && Name != "")
		{
			for (int i = 0; i < Parent->ChildrenCount; i++)
			{
				//loop through all children
				if (Parent->Children->Items[i] && Parent->Children->Items[i]->Name == Name)
				{
					return (TControl*)Parent->Children->Items[i];
				}

				TControl* p = (TControl *)Parent->Children->Items[i];
				if (p->ChildrenCount)
				{
					//Check these children
					TControl* Child = findObjectByName(p, Name);

					if (Child)
						return Child;
				}
			}
		}
	}
	catch(...)//ignore all errors
	{


	}

	return NULL;
}


//---------------------------------
//
//Build a string list of object
//names on a parent
//
//---------------------------------
TStringList* __fastcall getObjectsList(TFmxObject* Parent)
{

	//If not found or not applicable, return -1
	TStringList* list = NULL;

	try
	{
		if (Parent && Parent->ClassType() != __classid(TTextEx))
		{
			list = new TStringList();

			//For selecting nothing
			//list->Add("(None)");

			for (int i = 0; i < Parent->ChildrenCount; i++)
			{
				//loop through all children   list
				if ((Parent->Children->Items[i] && Parent->Children->Items[i] != Parent) &&
					(Parent->Children->Items[i]->ClassType() == __classid(TTextEx) || Parent->Children->Items[i]->ClassType() == __classid(TRectangleEx)))
				{
					if (((TControl*)Parent->Children->Items[i])->HitTest)
						list->Add(Parent->Children->Items[i]->Name);

					//Check for sub children
					if (Parent->Children->Items[i]->ChildrenCount)
					{
						TStringList* l = getObjectsList(Parent->Children->Items[i]);

						if (l && l->Count)
						{
							list->AddStrings(l);
							delete l;
						}
					}
				}
			}
		}
	}
	catch(...)//ignore all errors
	{


	}

	return list;
}


//---------------------------------
//
//Build a complete list of objects
//on a parent
//
//---------------------------------
void __fastcall getAllObjectOnParent(TFmxObject* Parent, TList* list)
{
	try
	{
		if (Parent && Parent->ClassType() != __classid(TTextEx) && list)
		{
			for (int i = 0; i < Parent->ChildrenCount; i++)
			{
				TControl* c = dynamic_cast<TControl *>(Parent->Children->Items[i]);

				//loop through all children
				if (c)
				{
					list->Add(c);

					if (c->ChildrenCount)
					{
						//Recursive search
						getAllObjectOnParent(c, list);
					}
				}
			}
		}
	}
	catch(...)//ignore all errors
	{


	}
}

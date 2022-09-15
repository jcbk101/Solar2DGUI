//---------------------------------------------------------------------------

#pragma hdrstop

#include "EditFeatures.h"
#include "CustomControls\TRectangleEx.h"
#include "CustomControls\TTextEx.h"

#pragma package(smart_init)


//--------------------------------------------------------
//
//Cut objects from a parent window
//
//--------------------------------------------------------
void __fastcall TEditFeature::editCut(TControl* c)
{
	if (c)
	{
		//Each item is removed from their parent and made invisible
		//Until a paste action is performed
		this->Add(c);
		c->Parent = NULL;
		c->Visible = false;

		//Signal to PASTE that the data in the list is CUT. :)
		this->IsCut = true;
	}
}


//--------------------------------------------------------
//
//Copy objects from a parent window
//
//--------------------------------------------------------
void __fastcall TEditFeature::editCopy(TControl* c)
{
	if (c)
	{
		//Each item is saved in the list. Once PASTE called, new instance of
		//the object is created and then 'Assigned' the value from the list
		//Afterward, each NEW item is re-named according to the new parent. IMPORTANT!!!
		//Do nothing until a paste action is performed.
		this->Add(c);

		//Signal to PASTE that the data in the list is COPY. :)
		this->IsCut = false;
	}
}


//--------------------------------------------------------
//
//Paste objects to a parent window: c is the Parent window
//
//--------------------------------------------------------
void __fastcall TEditFeature::editPaste(TControl* c)
{
	AnsiString type;

	//When pasting TO an object, only pasting to a WINDOW is supported
	//This check must be performed here
	if (c && (c->ClassType() == __classid(TRectangleEx) || c->ClassType() == __classid(TRectangle)))
	{
		//We can paste the data to the window
		for (int i = 0; i < this->Count; i++)
		{
			TControl* item = (TControl*)this->Items[ i ];

			if (item)
			{
				//Set type of object to Paste
				type = (item->ClassType() == __classid(TTextEx) ? "TEXT":"RECT");
				TGlowEffect* e = dynamic_cast< TGlowEffect * >(item->TagObject);

				if (e)
				{
					delete e;
					e = NULL;
					c->TagObject = NULL;
				}

				if (IsCut)
				{
					//Cut from another parent and pasting to a new parent
					//item->Parent = c;
					item->Visible = true;
					//Handle renaming the object with new parent info
					CutChangeParentInfo(c, item, type);
				}
				else
				{
					// //Copying an object
					//if (type == "TEXT")
					//{
					//TTextEx* object = new TTextEx(c);
					// //object = new TTextEx(c);
					//object->Clone(item);
					//object->Text = ((TTextEx*)item)->Text;
					//
					// //Handle renaming the object with new parent info
					//CopyChangeParentInfo(c, object, item, type);
					//}
					//else
					//{
					//TRectangleEx* object;
					//object = new TRectangleEx(c);
					//object->Clone(item);
					//
					// //Handle renaming the object with new parent info
					//CopyChangeParentInfo(c, object, item, type);
					//}
				}

				wasPasted = true;
			}
		}

		//Clear the 'Cut' flag
		IsCut = false;
	}
}


//--------------------------------------------------------
//
//Rename Cut object according to structure rules
//
//--------------------------------------------------------
void __fastcall TEditFeature::CutChangeParentInfo(TControl* Parent, TControl* item, AnsiString type)
{
	//Recursive method
	AnsiString s            = UpperCase(type);
	AnsiString nameSpace    = "";
	int        controlCount = -1;

	//Get the object count for the parent form
	if (Parent)
	{ //Must subtract what was just added as a direct child
		controlCount = Parent->ChildrenCount - 1;

		//Create a "prefix" for each object per parent
		if (Parent->Name != "deviceScreen")
			nameSpace = Parent->Name + "_";
	}

	//
	controlCount++;

	//Rename item per structure rule
	item->Parent = Parent;
	item->Name = nameSpace + type + IntToStr(controlCount);

	//Must reposition in the widow. Coordinates can be way off!
	item->Position->X = (controlCount * 2);
	item->Position->Y = (controlCount * 2);

	//Test recursive requirement: TTextEx does not hold children
	//Test anyway
	if (item->ChildrenCount && (item->ClassType() == __classid(TRectangle) || item->ClassType() == __classid(TRectangleEx)))
	{
		for (int i = 0; i < item->ChildrenCount; i++)
		{
			TControl* subItem = (TControl*)item->Children->Items[ i ];

			if (subItem)
			{
				//Set type of object to Paste
				CutChangeParentInfo(item, subItem, (subItem->ClassType() == __classid(TTextEx) ? "TEXT":"RECT"));
			}
		}
	}
}


//--------------------------------------------------------
//
//Rename Copy object according to structure rules
//
//--------------------------------------------------------
void __fastcall TEditFeature::CopyChangeParentInfo(TControl* Parent, TControl* item, TControl* src, AnsiString type)
{
	// //Recursive method
	//AnsiString s            = UpperCase(type);
	//AnsiString nameSpace    = "";
	//int        controlCount = -1;
	//
	// //Get the object count for the parent form
	//if (Parent)
	//{ //Must subtract what was just added as a direct child
	//controlCount = Parent->ChildrenCount - 1;
	//
	// //Create a "prefix" for each object per parent
	//if (Parent->Name != "deviceScreen")
	//nameSpace = Parent->Name + "_";
	//}
	//
	// //
	//controlCount++;
	//
	// //Rename item per structure rule
	//item->Parent = Parent;
	//item->Name = nameSpace + type + IntToStr(controlCount);
	//
	// //Must reposition in the widow. Coordinates can be way off!
	//item->Position->X = (controlCount * 2);
	//item->Position->Y = (controlCount * 2);
	//
	//UIObjects* n = new UIObject();
	//UIObjects* o = (UIObject *)src->Tag;
	//if (o)
	//{
	//memcpy(n, o, sizeof(UIObjects));
	//}
	//
	// //
	//item->Tag = (long)n;
	//
	// //Common properties
	//item->Scale = src->Scale;
	//
	// //Event listeners
	//if (type == "TEXT")
	//{
	//item->OnMouseDown = src->OnMouseDown;
	//item->OnMouseUp = src->OnMouseUp;
	//item->OnMouseMove = src->OnMouseMove;
	//item->OnMouseLeave = src->OnMouseLeave;
	//
	//TTextEx* s = dynamic_cast< TTextEx * >(src);
	//TTextEx* d = dynamic_cast< TTextEx * >(item);
	//
	// //Copy all properties needed
	//if (s && d)
	//{
	//
	//}
	//}
	//else
	//{
	//item->OnMouseDown = src->OnMouseDown;
	//item->OnMouseUp = src->OnMouseUp;
	//item->OnMouseMove = src->OnMouseMove;
	//item->OnMouseLeave = src->OnMouseLeave;
	//item->OnPainting = src->OnPaint;
	//item->OnResize = src->OnResize;
	//
	// //9-Patch info
	//TRectangeEx* s = dynamic_cast< TRectangleEx * >(src);
	//TRectangeEx* d = dynamic_cast< TRectangleEx * >(item);
	//
	// //Copy all properties needed
	//if (s && d)
	//{
	//
	//}
	//}
	//
	//item->PopupMenu = Parent->PopupMenu;
	//
	// //Parent->Repaint();
	//
	// //Test recursive requirement: TTextEx does not hold children
	// //Test anyway
	//if (item->ChildrenCount && (item->ClassType() == __classid(TRectangle) || item->ClassType() == __classid(TRectangleEx)))
	//{
	//for (int i = 0; i < item->ChildrenCount; i++)
	//{
	//TControl* subItem = (TControl*)item->Children->Items[ i ];
	//
	//if (subItem)
	//{
	// //Set type of object to Paste
	//type = (subItem->ClassType() == __classid(TTextEx) ? "TEXT":"RECT");
	//
	//CopyChangeParentInfo(item, subItem, type);
	//}
	//}
	//}
}

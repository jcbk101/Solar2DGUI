//---------------------------------------------------------------------------

#pragma hdrstop

#include "ZOrder.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//--------------------------------------------------------
//
//Get the Z position for all objects initially
//
//--------------------------------------------------------
void calcControlZOrder(TObject* parent)
{
	//if not found or not applicable, return -1
	TFmxObject* Parent = (TFmxObject *)parent;

	try
	{
		if (Parent)
		{
			for (int i = 0; i < Parent->ChildrenCount; i++)
			{
				//loop through all children
				if (Parent->Children->Items[ i ])//found a control
				{
					UIObject* o = (UIObject *)Parent->Children->Items[ i ]->Tag;

					if (!o)
					{
						o = new UIObject();
						memset(o, 0, sizeof(UIObject));
					}

					o->zPosition = i;
				}
			}
		}
	}
	catch(...)//ignore all errors
	{


	}
}


//--------------------------------------------------------
//
//Get the Z position for all objects initially
//
//--------------------------------------------------------
int GetControlZOrder(TObject* aControl)
{
	//if not found or not applicable, return -1
	TList* zList;
	TFmxObject* Parent;
	int result = -1; //flag for not found or not relevent

	try
	{
		TControl* c = dynamic_cast< TControl * >(aControl);

		if (c)
		{
			if ( /* c->Parent == TComponent && */ c->Parent != NULL /* && c->Parent == TFmxObject */)
			{
				Parent = (TFmxObject *)c->Parent;

				//determine current position in z-order
				for (int i = 0; i < Parent->ChildrenCount; i++)
				{
					//loop through all children
					if (Parent->Children->Items[ i ] == aControl)//found the control
					{
						UIObject* o = (UIObject *)Parent->Children->Items[ i ]->Tag;
						//Actually position
						if (o)
							return o->zPosition;
						else
							return 0;
					}

				}
			}
		}
	}
	catch(...)//ignore all errors
	{


	}

	return -1;
}


//--------------------------------------------------------
//
//Set the Z position of an object
//
//--------------------------------------------------------
bool SetControlZOrder(TObject* aControl, int newZOrder)
{
	//Set the Z order of the control
	//If a control already exists at this Z Order, the control will be placed underneath
	//It (pushing the other control up)
	//
	//return TRUE if change was applied
	//return FALSE if no changes

	TControl* c = dynamic_cast< TControl * >(aControl);
	TList* zOrderList;
	TFmxObject* Parent;
	int  currentZ = 0;
	bool result   = false;

	if (c)
	{
		if (newZOrder >= 0 && c->Parent != NULL)
		{
			zOrderList = new TList();

			try
			{
				Parent = (TFmxObject *)c->Parent;


				//Populate list of controls that need to be raised to the top
				for (int i = 0; i < Parent->ChildrenCount; i++)
					zOrderList->Add(Parent->Children->Items[ i ]);

				//Remove from the current list  zOrderList->List
				zOrderList->Remove(aControl);

				//Add to the position requested
				if (newZOrder <= zOrderList->Count)
					zOrderList->Insert(newZOrder, aControl);
				else
					zOrderList->Add(aControl);

				//Loop through controls and bring each one to the front
				for (int i = 0; i < zOrderList->Count; i++)
				{
					((TControl*)zOrderList->Items[ i ])->BringToFront();
				}


				//Show th eupdated Z Positions
				if (dynamic_cast< TControl * >(Parent) != NULL)
				{
					((TControl*)Parent)->Repaint();
				}

				result = true;
			}
			__finally
			{
				delete zOrderList;
			}
		}
	}

	return result;
}

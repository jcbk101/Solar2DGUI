//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TexAtlas.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TtexAtlasForm *texAtlasForm;


void __fastcall TtexAtlasForm::FormCreate(TObject *Sender)
{
	//Don't need an open dialog if a file exist already. Just build the list
	if (*this->atlasFile != "")
	{
		texAtlasFile->Text = *this->atlasFile;
		baseDir->Text = ExtractFilePath(*this->atlasFile);
		FOpen->FileName = *this->atlasFile;
		loadTexAtlasClick(NULL);
	}
}


void __fastcall TtexAtlasForm::baseDirChange(TObject *Sender)
{
	AnsiString dir = baseDir->Text;

	if (dir != "")
	{
		if (!DirectoryExists(dir, false))
		{
			ShowMessage("Error: This directory does not exist.");
		}
	}
}


//--------------------------------------------------------
//
//Convenience function used to retrieve a tuxture
//
//--------------------------------------------------------
void __fastcall TtexAtlasForm::getTexture(AnsiString fileName, TBitmap *bmp, int index)
{
	//Don't need an open dialog if a file exist already
	//indexMUST be greater than ZERO since it must be decreased by one
	if (fileName != "" && bmp && index > 0)
	{
		FOpen->FileName = fileName;
		loadTexAtlasClick(NULL);

		if (ListBox1->Items->Count)
		{
			//Select the item wanted
			TListBoxItem *item = dynamic_cast<TListBoxItem *>(ListBox1->ListItems[index - 1]);

			if (item)
			{
				for (int i = 0; i < item->ChildrenCount; i++)
				{
					AnsiString name = item->Children->Items[i]->ClassName();
					TImage *   img  = dynamic_cast<TImage *>(item->Children->Items[i]);

					if (img)
					{
						bmp->Assign(img->Bitmap);
						//For Solar2D, imagesheet indexes start at '1'
						break;
					}
				}
			}
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
void __fastcall TtexAtlasForm::loadTexAtlasClick(TObject *Sender)
{
	FOpen->Filter = "LUA Sheet Info|*.lua";
	if (FOpen->InitialDir == "")
		FOpen->InitialDir = GetCurrentDir();

	//Try to open dialog
	if (FOpen->FileName != "" || FOpen->Execute())
	{
		AnsiString png = FOpen->FileName;
		AnsiString lua = FOpen->FileName;

		png = ChangeFileExt(png, ".png");

		//Process the data if it all exist
		if (FileExists(png))
		{
			TMemo *m    = new TMemo(texAtlasForm);
			TList *temp = new TList();

			AnsiString buf;
			int index = 0;
			char buffer[256];
			char *p, *s;
			bool canProcess = false;

			//
			m->Visible = false;
			m->Lines->LoadFromFile(lua);
			m->BeginUpdate();

			for (int i = 0; i < m->Lines->Count; i++)
			{
				buf = AnsiString(m->Lines->Strings[i]);
				strcpy(buffer, AnsiString(UpperCase(buf)).c_str());

				//
				if (!canProcess && strstr(buffer, "FRAMES"))
				{
					canProcess = true;
					continue;
				}

				//Only if 'canProcess' is true
				if (!canProcess)
					continue;

				//
				//Process all the frame info
				//
				strcpy(buffer, AnsiString(Trim(m->Lines->Strings[i])).c_str());

				if (buffer[0] == '{')
				{
					i = processTileInformation(m, temp, i);
				}
			}

			m->EndUpdate();


			//-------------------------------------
			//
			//Convert the pointers to tiles
			//
			//-------------------------------------
			//Create a temp Bitmap to hold image that will be processed
			TBitmap *bm = new TBitmap(png);
			TRect src;
			TRect dst;

			//Clear old data
			ListBox1->Clear();

			//Load the Imagelist with image data
			for (int i = 0; i < temp->Count; i++)
			{
				UIFrame *frame = (UIFrame *)(temp->Items[i]);
				//TListBoxItem *item  = new TListBoxItem(ListBox1);
				TListBoxItem *parent = new TListBoxItem(ListBox1);
				TImage *      item   = new TImage(parent);
				TBitmap *img;// = new TBitmap();

				if (frame)
				{
					//img = item->Fill->Bitmap->Bitmap;
					img = item->Bitmap;

					if (frame->sW && frame->sH)
					{
						//Where is the data on the full image data bitmap?
						src = Rect(frame->x, frame->y, (frame->x + frame->w), (frame->y + frame->h));
						//dst = Rect(frame->sX, frame->sY, (frame->sX + frame->w), (frame->sY + frame->h));

						img->Width = frame->sW;
						img->Height = frame->sH;
						//img->Clear(0);
						//
						//img->Canvas->DrawBitmap(bm, src, dst, 1, false);
						img->CopyFromBitmap(bm, src, frame->sX, frame->sY);
					}
					else
					{
						//Where is the data on the full image data bitmap?
						src = Rect(frame->x, frame->y, (frame->x + frame->w), (frame->y + frame->h));
						//dst = Rect(0, 0, frame->w, frame->h);

						img->Width = frame->w;
						img->Height = frame->h;

						//img->Clear(0);
						//
						img->CopyFromBitmap(bm, src, 0, 0);
					}

					//Add the BMP to the list
					parent->Parent = ListBox1;
					parent->Padding->Rect = Rect(4,4,4,4);
					item->Parent = parent;
					item->HitTest = false;

					//item->Stroke->Kind = TBrushKind::None;
					item->Align = TAlignLayout::Client;
					item->Tag = i;
				}
			}

			//File name saved
			texAtlasFile->Text = lua;
			//this->fileName = lua;
			*this->atlasFile = lua;
			baseDir->Text = ExtractFilePath(lua);

			//Memory clean up
			delete m;
			delete temp;
			delete bm;
		}
		else
		{
			ShowMessage("Error: LUA Sheet Info file found, but related '*.png' file not found.");
		}
	}
}


//--------------------------------------------------------
//
//
//
//--------------------------------------------------------
int __fastcall TtexAtlasForm::processTileInformation(TMemo *m, TList *list, int currentIndex)
{
	//Frames data
	UIFrame *frame = new UIFrame();
	int      i     = 0;
	char buffer[256];
	char *p = NULL;
	char var[256];
	int value = 0;

	//Clear garbage
	memset(frame, 0, sizeof(UIFrame));

	//
	for (i = currentIndex; i < m->Lines->Count; i++)
	{
		//
		//Process all the frame info
		//
		strcpy(buffer, AnsiString(Trim(m->Lines->Strings[i])).c_str());

		if (buffer[0] == '}')
		{
			if ((frame->x + frame->w) && (frame->y + frame->h))
				list->Add(frame);
			//
			//delete frame;

			return i;
		}

		//Values to gather
		if ((p = strchr(buffer, '=')))
		{
			p[0] = ' ';

			//
			strcpy(buffer, AnsiString(UpperCase(buffer)).c_str());

			if (buffer[0] == 'X')
				sscanf(p, "%d", &frame->x);
			if (buffer[0] == 'Y')
				sscanf(p, "%d", &frame->y);
			if (strstr(buffer, "WIDTH"))
				sscanf(p, "%d", &frame->w);
			if (strstr(buffer, "HEIGHT"))
				sscanf(p, "%d", &frame->h);
			//
			if (strstr(buffer, "SOURCEX"))
				sscanf(p, "%d", &frame->sX);
			if (strstr(buffer, "SOURCEY"))
				sscanf(p, "%d", &frame->sY);
			if (strstr(buffer, "SOURCEWIDTH"))
				sscanf(p, "%d", &frame->sW);
			if (strstr(buffer, "SOURCEHEIGHT"))
				sscanf(p, "%d", &frame->sH);
		}

	}

	delete frame;

	return i;
}


void __fastcall TtexAtlasForm::ListBox1DblClick(TObject *Sender)
{
	//Select the item wanted
	AnsiString    name = ListBox1->ListItems[ListBox1->ItemIndex]->ClassName();
	TListBoxItem *item = dynamic_cast<TListBoxItem *>(ListBox1->ListItems[ListBox1->ItemIndex]);

	if (item)
	{
		for (int i = 0; i < item->ChildrenCount; i++)
		{
			AnsiString name = item->Children->Items[i]->ClassName();
			TImage *   img  = dynamic_cast<TImage *>(item->Children->Items[i]);

			if (img)
			{
				this->bmp->Assign(img->Bitmap);
				//For Solar2D, imagesheet indexes start at '1'
				*this->imageIndex = ListBox1->ItemIndex + 1;
				break;
			}
		}
	}

	Close();
}

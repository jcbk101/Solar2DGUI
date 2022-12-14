//---------------------------------------------------------------------------

#ifndef TexAtlasH
#define TexAtlasH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.ImgList.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.ImageList.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Objects.hpp>


//---------------------------------------------------------------------------
class TtexAtlasForm : public TForm
{
__published: //IDE-managed Components
	TLabel *          baseDirButton;
	TEdit *           baseDir;
	TGridPanelLayout *GridPanelLayout1;
	TCornerButton *   loadTexAtlas;
	TEdit *           texAtlasFile;
	TOpenDialog *     FOpen;
	TListBox *        ListBox1;

	void __fastcall baseDirChange(TObject *Sender);
	void __fastcall loadTexAtlasClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ListBox1DblClick(TObject *Sender);

private: //User declarations
	int __fastcall processTileInformation(TMemo *memo, TList *list, int currentIndex);



public: //User declarations
	//Called when only the Texture data is required on a project reload
	__fastcall TtexAtlasForm(TComponent *Owner, TBitmap *bmp, AnsiString *fileName, int *index) : TForm(Owner)
	{
		if (bmp)
			this->bmp = bmp;
		else
			this->bmp = new TBitmap();

		if (fileName)
			this->atlasFile = fileName;
		else
			this->atlasFile = &dummyStr;

		if (index)
			this->imageIndex = index;
		else
			this->imageIndex = &dummyInt;

	}

	void __fastcall getTexture(AnsiString fileName, TBitmap *bmp, int index);

	TBitmap *   bmp;
	AnsiString *atlasFile;
	AnsiString  dummyStr;
	int         dummyInt;
	int *       imageIndex;
};


//-----------------------------------------------
//
//Sub class
//
//-----------------------------------------------
class UIFrame : public TObject
{
private:



public:
	__fastcall UIFrame()
	{

	}

	int x, y;
	int w, h;
	int sX, sY;
	int sW, sH;
};

//---------------------------------------------------------------------------
extern PACKAGE TtexAtlasForm *texAtlasForm;
//---------------------------------------------------------------------------
#endif

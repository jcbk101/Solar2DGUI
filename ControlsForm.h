//---------------------------------------------------------------------------

//This software is Copyright (c) 2015 Embarcadero Technologies, Inc.
//You may only use this software if you are an authorized licensee
//of an Embarcadero developer tools product.
//This software is considered a Redistributable as defined under
//the software license agreement that comes with the Embarcadero Products
//and is subject to that software license agreement.

//---------------------------------------------------------------------------

#ifndef ControlsFormH
#define ControlsFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Dialogs.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Menus.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
#include <FMX.ExtCtrls.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Memo.hpp>
#include <FMX.StdCtrls.hpp>
#include <System.Rtti.hpp>
#include <FMX.ScrollBox.hpp>
#include <System.Threading.hpp>
#include <FMX.Graphics.hpp>
#include <System.UITypes.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.BufferedLayout.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.Objects3D.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.MultiView.hpp>
#include <FMX.Edit.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.NumberBox.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <FMX.Header.hpp>
#include <FMX.Colors.hpp>
#include <FMX.ComboEdit.hpp>
#include <FMX.ImgList.hpp>
#include <System.ImageList.hpp>
#include <FMX.Styles.Objects.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <System.Bindings.Outputs.hpp>
#include <FMX.Filter.Effects.hpp>
#include <System.JSON.Writers.hpp>
#include <System.JSON.Readers.hpp>
#include <registry.hpp>

#include <System.Math.hpp>
#include <System.IOUtils.hpp>
#include <FMX.WebBrowser.hpp>
#include <algorithm>

#include "LUADisplay\LUADisplay.h"
#include "AboutForm.h"
#include "MessageBox.h"
#include "FontForm.h"
#include "EditFeatures.h"

#include "CustomControls\TRectangleEx.h"
#include "CustomControls\TTextEx.h"
//#include "CustomControls\TControlEx.h"

#include "CodeView\CodeView.h"
#include "TexAtlas\TexAtlas.h"

#include "ObjectAlign.h"
#include "DataProcessing.h"
#include "UIAnimation.h"
#include <FMX.Ani.hpp>

#define MAXSAVESTATES 10


//---------------------------------------------------------------------------
class Tsolar2DForm : public TForm
{
__published: //IDE-managed Components
	TOpenDialog *         FOpen;
	TStatusBar *          StatusBar1;
	TTrackBar *           ScaleTrack;
	TLabel *              Text1;
	TLabel *              statusLabel;
	TLabel *              TextScale;
	TLayout *             ControlRoot;
	TLayout *             ControlLayout;
	TTabControl *         mainTab;
	TTabItem *            designTabItem;
	TTabItem *            codeTabItem;
	TMainMenu *           MainMenu1;
	TMenuItem *           editMenu;
	TMenuItem *           viewMenu;
	TMenuItem *           fileMenu;
	TMenuItem *           openProj;
	TMenuItem *           saveProj;
	TMenuItem *           exitApp;
	TMenuItem *           saveAs;
	TMenuItem *           toolsMenu;
	TMenuItem *           exportLua;
	TMenuItem *           helpMenu;
	TMenuItem *           aboutItem;
	TStyleBook *          StyleBook1;
	TColorComboBox *      colorArray;
	TPresentedScrollBox * deviceScrollBox;
	TRectangle *          deviceScreen;
	TRectangle *          selectionRect;
	TRectangle *          R;
	TRectangle *          L;
	TRectangle *          BR;
	TRectangle *          BC;
	TRectangle *          BL;
	TRectangle *          TL;
	TRectangle *          TR;
	TRectangle *          TC;
	TSplitter *           splitterVertL;
	TSplitter *           splitterVertR;
	TFramedScrollBox *    tabCtrlBox;
	TTabControl *         inspectorTab;
	TTabItem *            objectProperties;
	TFramedVertScrollBox *FramedVertScrollBox1;
	TPanel *              Panel2;
	TFramedVertScrollBox *gridScrollBox;
	TSplitter *           alignSplitter;
	TGridPanelLayout *    propsGridL;
	TLabel *              alpha;
	TLabel *              anchorX;
	TLabel *              anchorY;
	TLabel *              color;
	TLabel *              font;
	TLabel *              fontColor;
	TLabel *              fontSize;
	TLabel *              height;
	TLabel *              image;
	TLabel *              isVisible;
	TLabel *              name;
	TLabel *              positionX;
	TLabel *              positionY;
	TLabel *              rotation;
	TLabel *              scaleX;
	TLabel *              scaleY;
	TLabel *              text;
	TLabel *              textAlign;
	TLabel *              width;
	TGridPanelLayout *    propsGridR;
	TNumberBox *          widthNum;
	TNumberBox *          heightNum;
	TNumberBox *          posXNum;
	TNumberBox *          posYNum;
	TNumberBox *          scaleXNum;
	TNumberBox *          anchorXNum;
	TEdit *               nameEdit;
	TNumberBox *          scaleYNum;
	TNumberBox *          anchorYNum;
	TNumberBox *          rotateNum;
	TCheckBox *           visibleCheck;
	TNumberBox *          alphaNum;
	TCornerButton *       imageButton;
	TColorComboBox *      shapeColorSelect;
	TEdit *               enterText;
	TComboBox *           alignTextSelector;
	TColorComboBox *      fontColorSelect;
	TNumberBox *          fontSizeNum;
	TPanel *              Panel1;
	TCornerButton *       fontDialogButton;
	TSplitter *           propsSplitter;
	TGridPanelLayout *    alignGridL;
	TLabel *              bottomParent;
	TLabel *              horiParent;
	TLabel *              leftParent;
	TLabel *              rightParent;
	TLabel *              topParentt;
	TLabel *              vertParent;
	TLabel *              anchorLeft;
	TLabel *              anchorTop;
	TLabel *              anchorRight;
	TLabel *              anchorBottom;
	TLabel *              above;
	TLabel *              bottomWith;
	TLabel *              horiWith;
	TLabel *              leftWith;
	TLabel *              rightWith;
	TLabel *              topWith;
	TLabel *              vertWith;
	TLabel *              below;
	TLabel *              leftOf;
	TLabel *              rightOf;
	TGridPanelLayout *    alignGridR;
	TCheckBox *           bottomParentChk;
	TCheckBox *           horiParentChk;
	TCheckBox *           leftParentChk;
	TCheckBox *           rightParentChk;
	TCheckBox *           topParentChk;
	TCheckBox *           vertParentChk;
	TCheckBox *           anchorLeftChk;
	TCheckBox *           anchorTopChk;
	TCheckBox *           anchorRightChk;
	TCheckBox *           anchorBottomChk;
	TComboBox *           aboveCombo;
	TComboBox *           bottomWithCombo;
	TComboBox *           horiWithCombo;
	TComboBox *           leftWithCombo;
	TComboBox *           rightWithCombo;
	TComboBox *           topWithCombo;
	TComboBox *           vertWithCombo;
	TComboBox *           belowCombo;
	TComboBox *           leftOfCombo;
	TComboBox *           rightOfCombo;
	TSplitter *           Splitter1;
	TTreeView *           controlTree;
	TPanel *              propsSizedPanel;
	TPanel *              alignSizedPanel;
	TImageList *          ImageList1;
	TSpeedButton *        raiseToTop;
	TSpeedButton *        raiseOneLevel;
	TSpeedButton *        lowerToBottom;
	TSpeedButton *        lowerOneLevel;
	TSpeedButton *        newWindow;
	TSpeedButton *        newText;
	TSpeedButton *        deleteSelected;
	TSpeedButton *        spaceVert;
	TSpeedButton *        spaceHori;
	TSpeedButton *        rotateDevice;
	TSpeedButton *        alignLSideWin;
	TSpeedButton *        alignHCenterWin;
	TSpeedButton *        alignTSideWin;
	TSpeedButton *        alignRSideWin;
	TSpeedButton *        alignBSideWin;
	TSpeedButton *        alignVCenterWin;
	TSpeedButton *        spaceVEqually;
	TSpeedButton *        spaceHEqually;
	TSpeedButton *        expandV;
	TSpeedButton *        expandH;
	TSpeedButton *        expandWin;
	TSpeedButton *        collapseWin;
	TMonochromeEffect *   raiseToTopEffect;
	TMonochromeEffect *   raiseOneLevelEffect;
	TMonochromeEffect *   lowerToBottomEffect;
	TMonochromeEffect *   lowerOneLevelEffect;
	TMonochromeEffect *   newWindowEffect;
	TMonochromeEffect *   newTextEffect;
	TMonochromeEffect *   deleteSelectedEffect;
	TMonochromeEffect *   spaceVertEffect;
	TMonochromeEffect *   spaceHoriEffect;
	TMonochromeEffect *   alignLSideWinEffect;
	TMonochromeEffect *   alignTSideWinEffect;
	TMonochromeEffect *   alignRSideWinEffect;
	TMonochromeEffect *   alignBSideWinEffect;
	TMonochromeEffect *   alignVCenterWinEffect;
	TMonochromeEffect *   alignHCenterWinEffect;
	TMonochromeEffect *   spaceVEquallyEffect;
	TMonochromeEffect *   spaceHEquallyEffect;
	TMonochromeEffect *   expandVEffect;
	TMonochromeEffect *   expandHEffect;
	TMonochromeEffect *   expandWinEffect;
	TMonochromeEffect *   collapseWinEffect;
	TPopupMenu *          devicePopup;
	TMenuItem *           setBG;
	TMenuItem *           clearBG;
	TMenuItem *           MenuItem3;
	TMenuItem *           addNewWin;
	TMenuItem *           addNewText;
	TMenuItem *           clearImage;
	TMenuItem *           pasteObj;
	TMenuItem *           cutObj;
	TMenuItem *           copyObj;
	TMenuItem *           MenuItem1;
	TMenuItem *           display0;
	TMenuItem *           display1;
	TMenuItem *           display2;
	TMenuItem *           display3;
	TMenuItem *           display4;
	TMenuItem *           display5;
	TMenuItem *           display6;
	TMenuItem *           display7;
	TMenuItem *           display8;
	TMenuItem *           display9;
	TMenuItem *           display10;
	TMenuItem *           display11;
	TMenuItem *           display12;
	TMenuItem *           display13;
	TMenuItem *           display14;
	TMenuItem *           display15;
	TMenuItem *           display16;
	TMenuItem *           display17;
	TMenuItem *           display18;
	TMenuItem *           display19;
	TMenuItem *           display20;
	TMenuItem *           display21;
	TMenuItem *           display22;
	TMenuItem *           display23;
	TMenuItem *           display24;
	TMenuItem *           display25;
	TMenuItem *           display26;
	TMenuItem *           display27;
	TMenuItem *           display28;
	TMenuItem *           display29;
	TMenuItem *           display30;
	TMenuItem *           display31;
	TMenuItem *           display32;
	TMenuItem *           display33;
	TMenuItem *           display34;
	TMenuItem *           display35;
	TMenuItem *           display36;
	TMenuItem *           display37;
	TMenuItem *           display38;
	TMenuItem *           display39;
	TMenuItem *           display40;
	TMenuItem *           display41;
	TMenuItem *           display42;
	TMenuItem *           display43;
	TMenuItem *           display44;
	TMenuItem *           display45;
	TMenuItem *           display46;
	TMenuItem *           display47;
	TMenuItem *           display48;
	TMenuItem *           display49;
	TMenuItem *           display50;
	TMenuItem *           borderlessSubMenu;
	TRectangle *          deviceFrame;
	TSpeedButton *        alignLSide;
	TSpeedButton *        alignTSide;
	TSpeedButton *        alignRSide;
	TSpeedButton *        alignBSide;
	TSpeedButton *        alignVCenter;
	TSpeedButton *        alignHCenter;
	TMonochromeEffect *   alignLSideEffect;
	TMonochromeEffect *   alignTSideEffect;
	TMonochromeEffect *   alignRSideEffect;
	TMonochromeEffect *   alignBSideEffect;
	TMonochromeEffect *   alignVCenterEffect;
	TMonochromeEffect *   alignHCenterEffect;
	TSaveDialog *         FSave;
	TMenuItem *           options;
	TMenuItem *           borderToggle;
	TMenuItem *           MenuItem4;
	TSpeedButton *        setLeftOf;
	TMonochromeEffect *   setLeftOfEffect;
	TSpeedButton *        setTopOf;
	TMonochromeEffect *   setTopOfEffect;
	TSpeedButton *        setRightOf;
	TMonochromeEffect *   setRightOfEffect;
	TSpeedButton *        setBottomOf;
	TMonochromeEffect *   setBottomOfEffect;
	TTabItem *            animationsTab;
	TMenuItem *           addAnim;
	TMenuItem *           delAnim;
	TMenuItem *           MenuItem2;
	TMenuItem *           MenuItem5;
	TPopupMenu *          animationPopup;
	TPanel *              Panel3;
	TSplitter *           animHSplitter;
	TListBox *            animListBox;
	TFramedVertScrollBox *animScrollBox;
	TPanel *              animSizedPanel;
	TGridPanelLayout *    animGridL;
	TLabel *              timeLabel;
	TLabel *              delayLabel;
	TLabel *              iterationsLabel;
	TLabel *              tagLabel;
	TLabel *              deltaLabel;
	TLabel *              loopLabel;
	TLabel *              easeLabel;
	TLabel *              interpolationLabel;
	TGridPanelLayout *    animGridR;
	TCheckBox *           deltaChk;
	TNumberBox *          timeNum;
	TNumberBox *          delayNum;
	TNumberBox *          iterationNum;
	TComboBox *           easeBox;
	TComboBox *           interpolationBox;
	TEdit *               tagEdit;
	TCheckBox *           loopChk;
	TSplitter *           Splitter4;
	TLabel *              animNameLabel;
	TEdit *               animRefName;
	TLabel *              transition;
	TComboBox *           transBox;
	TCheckBox *           xChk;
	TCheckBox *           yChk;
	TCheckBox *           widthChk;
	TCheckBox *           heightChk;
	TCheckBox *           rotationChk;
	TCheckBox *           xScaleChk;
	TCheckBox *           yScaleChk;
	TCheckBox *           alphaChk;
	TCheckBox *           fontSizeChk;
	TNumberBox *          animXNum;
	TNumberBox *          animYNum;
	TNumberBox *          animWidthNum;
	TNumberBox *          animHeightNum;
	TNumberBox *          animRotationNum;
	TNumberBox *          animXScaleNum;
	TNumberBox *          animYScaleNum;
	TNumberBox *          animAlphaNum;
	TNumberBox *          animFSNum;
	TMenuItem *           MenuItem6;
	TMenuItem *           runAnimation;
	TMenuItem *           stopAnimation;
	TMenuItem *           addAnimation;
	TMenuItem *           pauseAnimation;
	TMenuItem *           MenuItem7;
	TMenuItem *           saveSettings;
	TMenuItem *           reloadSettings;
	TMenuItem *           crossHair;
	TMenuItem *           MenuItem9;
	TMenuItem *           newProj;
	TMemo *               codeMemo;
	TMenuItem *           setImage;
	TMenuItem *           setImageSheet;
	TMenuItem *           editUndo;
	TMenuItem *           editRedo;
	TMenuItem *           MenuItem11;

	void __fastcall ScaleTrackChange(TObject *Sender);

	void __fastcall objectMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y);
	void __fastcall objectMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y);
	void __fastcall objectMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
	void __fastcall objectMouseLeave(TObject *Sender);

	void __fastcall adjustHandles(bool isVisible, TRectangle *handle);
	void __fastcall setHandleColors(TControl *Sender);
	void __fastcall objectResize(TObject *Sender);
	void __fastcall exitAppClick(TObject *Sender);
	void __fastcall aboutItemClick(TObject *Sender);
	void __fastcall alignWithChange(TObject *Sender);

	TObject *__fastcall createNewControl(TObject *parent,AnsiString type);
	void __fastcall deviceScreenMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y);
	void __fastcall deviceScreenMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
	void __fastcall fontDialogButtonClick(TObject *Sender);
	void __fastcall objectNumChange(TObject *Sender);
	void __fastcall nameEditChange(TObject *Sender);

	void __fastcall visibleCheckChange(TObject *Sender);
	void __fastcall enterTextChange(TObject *Sender);
	void __fastcall alphaNumClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall aboveComboChange(TObject *Sender);
	void __fastcall nameEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);

	void __fastcall objectZOrderClick(TObject *Sender);
	void __fastcall objectActionClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall objectNumKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);

	void __fastcall controlTreeClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall deviceScreenResize(TObject *Sender);
	void __fastcall objectColorSelectChange(TObject *Sender);

	void __fastcall addNewTextClick(TObject *Sender);
	void __fastcall devicePopupPopup(TObject *Sender);
	void __fastcall objectTextSelectorChange(TObject *Sender);
	void __fastcall object9PatchPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall screenResolutionClick(TObject *Sender);
	void __fastcall deviceScreenPainting(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall deviceFrameMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
	void __fastcall deviceFrameMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y);
	void __fastcall saveProjClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall openProjClick(TObject *Sender);
	void __fastcall saveAsClick(TObject *Sender);
	void __fastcall borderToggleClick(TObject *Sender);
	void __fastcall MenuItem4Click(TObject *Sender);

	void __fastcall animationPopupPopup(TObject *Sender);
	void __fastcall addAnimClick(TObject *Sender);
	void __fastcall animListBoxClick(TObject *Sender);
	void __fastcall xChkChange(TObject *Sender);
	void __fastcall animRefNameChange(TObject *Sender);
	void __fastcall runAnimationClick(TObject *Sender);
	void __fastcall stopAnimationClick(TObject *Sender);
	void __fastcall pauseAnimationClick(TObject *Sender);
	void __fastcall onAnimationFinish(TObject *Sender);
	void __fastcall saveSettingsClick(TObject *Sender);
	void __fastcall reloadSettingsClick(TObject *Sender);

	void __fastcall textPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall objectPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall crossHairClick(TObject *Sender);
	void __fastcall deviceFrameMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, float X, float Y);
	void __fastcall newProjClick(TObject *Sender);
	void __fastcall imageButtonClick(TObject *Sender);
	void __fastcall codeTabItemClick(TObject *Sender);
	void __fastcall exportLuaClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall editUndoClick(TObject *Sender);


private: //User declarations

public: //User declarations
	__fastcall Tsolar2DForm(TComponent *Owner);

	//int __fastcall GetControlZOrder(TObject* aControl);
	//bool __fastcall SetControlZOrder(TObject* aControl, int aNewZOrder);
	void __fastcall updateUndoStack(String state, bool isAnimation);
	void __fastcall updateInspector();
	void __fastcall updateObjectData();
	void __fastcall loadAlignmentComboBoxes(bool clearOnly);
	void __fastcall clipToParent(TControl *c);
	void __fastcall imageLoader(int type, AnsiString fileName);
	void __fastcall loadImageFromSheet();

	void __fastcall reloadImageFromSheet(TRectangleEx *c, AnsiString fileName, int index);
	void __fastcall alignObjects();
	void __fastcall showAlignmentSelections(TObject *Sender);
	void __fastcall clearObjectSelections();
	void __fastcall clearAllCursors(TObject *Sender);
	void __fastcall updateTreeView();

	void __fastcall buildTreeBranches(TObject *Parent, TTreeViewItem *Child);
	void __fastcall calculate9PatchOffsets(TRectangleEx *Sender, int X, int Y, int Width, int Height);
	void __fastcall setScreenDisplay(int displayIndex);
	void __fastcall applyDisplayMetrics();
	void __fastcall setAllAlignments(TControl *DeviceParent);

	void __fastcall load9PatchImage(TRectangleEx *c, AnsiString fileName);
	void __fastcall buildAnimationList(TControl *Control);
	void __fastcall clearAnimationInspector();
	void __fastcall onAnimationProcessScaleX(TObject *Sender);
	void __fastcall onAnimationProcessScaleY(TObject *Sender);
	void __fastcall onAnimationProcessWidth(TObject *Sender);
	void __fastcall onAnimationProcessHeight(TObject *Sender);


	TCursor __fastcall getMouseCursor();
	//TControl* __fastcall findObjectByName(TObject* parent, AnsiString Name);
	//TStringList* __fastcall getObjectsList(TFmxObject* Sender);
	TPointF __fastcall getNestedPosition(TControl *Child);
	TFloatAnimation *__fastcall buildFloatAnimation(TFmxObject *Sender, UIAnimation *animation, int property, int fontSize);


	float      screenOrientation;
	AnsiString projectName;
	AnsiString exportDir;
	AnsiString exePath;
	AnsiString atlasFile;
	bool       isModified;
	bool       allowObjectUpdates;
};


//int __fastcall listCompareX(void* Item1, void* Item2);
//int __fastcall listCompareY(void* Item1, void* Item2);


//---------------------------------------------------------------------------
extern PACKAGE Tsolar2DForm *solar2DForm;
//---------------------------------------------------------------------------
#endif


/* if (temp && font)
 {
 //Choose who gets the requested changes
 if (selectionRect->TagObject)
 {
 temp->Add(selectionRect->TagObject);
 }
 else if (selectionsList->Count)
 {
 for (int i = 0; i < selectionsList->Count; i++)
 {
 TTextEx* c = dynamic_cast< TTextEx * >(findObjectByName(deviceScreen, AnsiString(selectionsList->Strings[ i ])));

 if (c)
 temp->Add(c);
 }
 }

 //
 TtransForm* trans = new TtransForm(Application, &easeIndex, &easeType);
 trans->ShowModal();
 trans->DisposeOf();

 if (temp->Count && trans->ModalResult == mrOk)
 {
 if (easeIndex > -1 && easeType > -1)
 {
 for (int i = 0; i < temp->Count; i++)
 {
 if (temp->Items[ i ]->ClassType() == __classid(TRectangleEx))
 {
 TRectangleEx* c = dynamic_cast< TRectangleEx * >(temp->Items[ i ]);
 }
 else if (temp->Items[ i ]->ClassType() == __classid(TTextEx))
 {
 TRectangleEx* c = dynamic_cast< TRectangleEx * >(temp->Items[ i ]);
 }
 }

 //Project has changed
 isModified = true;

 //Updates
 updateInspector();
 //updateObjectData();
 //deviceScreen->Repaint();
 }
 }
 }

 //Clean up
 delete temp;
 * */


/*
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
 */


/*
 If performing an Edit->Cut / Copy, MUST clear the list every time
 if (m->Tag == 7 || m->Tag == 8)
 {
 //Test to see if cut data exist
 if (editList->IsCut)
 {
 TmsgForm *msgForm = new TmsgForm(Application);

 msgForm->Caption = AnsiString("Objects in 'Cut' pending processing") + (selectionsList->Count > 1 ? "s...": "...");
 msgForm->messageText->Text =
 AnsiString("Warning! There are objects in the 'Cut' list pending processing. Should you continue, these items will be lost forever. Is this your intended action?");
 msgForm->ShowModal();

 //What result did we receive?
 if (msgForm->ModalResult == mrYes)
 {
 //Iterate thru the list
 for (int i = 0; i < editList->Count; i++)
 {
 delete((TControl*)editList->Items[i]);
 }

 //
 updateTreeView();
 clearObjectSelections();
 updateInspector();
 adjustHandles(false, NULL);
 }

 //Just leave and wait for thenext command
 if (msgForm->ModalResult == mrNo || msgForm->ModalResult == mrCancel)
 {
 //Rid of the form
 msgForm->DisposeOf();
 delete temp;
 return;
 }

 //Rid of the form
 msgForm->DisposeOf();
 }

 //
 editList->Clear();
 }
 */

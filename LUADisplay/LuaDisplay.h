#ifndef LUADisplayH
#define LUADisplayH

#include <fmx.h>


class LUADisplay
{
private:

public:
	bool     hasAccelerometer;
	bool     isUprightOrientationPortrait;
	bool     supportsScreenRotation;
	char*    device;
	char*    displayManufacturer;
	char*    displayName;
	char*    windowTitleBarName;
	float    androidDisplayApproximateDpi;
	float    defaultFontSize;
	float    iosPointHeight;
	float    iosPointWidth;
	float    safeLandscapeScreenInsetBottom;
	float    safeLandscapeScreenInsetLeft;
	float    safeLandscapeScreenInsetRight;
	float    safeLandscapeScreenInsetStatusBar;
	float    safeLandscapeScreenInsetTop;
	float    safeScreenInsetBottom;
	float    safeScreenInsetLeft;
	float    safeScreenInsetRight;
	float    safeScreenInsetStatusBar;
	float    safeScreenInsetTop;
	float    screenHeight;
	float    screenOriginX;
	float    screenOriginY;
	float    screenWidth;
	TBitmap* deviceImage;
	TBitmap* screenDressing;
	TBitmap* statusBarBlack;
	TBitmap* statusBarDarkTransparent;
	TBitmap* statusBarDefault;
	TBitmap* statusBarLightTransparent;
	TBitmap* statusBarTranslucent;

	int selectedDisplay;
};

void __fastcall initFunctionCalls();
void __fastcall initBitmapData(LUADisplay* display);
void __fastcall clearDisplayValues(LUADisplay* display);

extern void(*luaDisplay[ 52 ])(LUADisplay *display);


void initDisplay0(LUADisplay* display);
void initDisplay1(LUADisplay* display);
void initDisplay2(LUADisplay* display);
void initDisplay3(LUADisplay* display);
void initDisplay4(LUADisplay* display);
void initDisplay5(LUADisplay* display);
void initDisplay6(LUADisplay* display);
void initDisplay7(LUADisplay* display);
void initDisplay8(LUADisplay* display);
void initDisplay9(LUADisplay* display);
void initDisplay10(LUADisplay* display);
void initDisplay11(LUADisplay* display);
void initDisplay12(LUADisplay* display);
void initDisplay13(LUADisplay* display);
void initDisplay14(LUADisplay* display);
void initDisplay15(LUADisplay* display);
void initDisplay16(LUADisplay* display);
void initDisplay17(LUADisplay* display);
void initDisplay18(LUADisplay* display);
void initDisplay19(LUADisplay* display);
void initDisplay20(LUADisplay* display);
void initDisplay21(LUADisplay* display);
void initDisplay22(LUADisplay* display);
void initDisplay23(LUADisplay* display);
void initDisplay24(LUADisplay* display);
void initDisplay25(LUADisplay* display);
void initDisplay26(LUADisplay* display);
void initDisplay27(LUADisplay* display);
void initDisplay28(LUADisplay* display);
void initDisplay29(LUADisplay* display);
void initDisplay30(LUADisplay* display);
void initDisplay31(LUADisplay* display);
void initDisplay32(LUADisplay* display);
void initDisplay33(LUADisplay* display);
void initDisplay34(LUADisplay* display);
void initDisplay35(LUADisplay* display);
void initDisplay36(LUADisplay* display);
void initDisplay37(LUADisplay* display);
void initDisplay38(LUADisplay* display);
void initDisplay39(LUADisplay* display);
void initDisplay40(LUADisplay* display);
void initDisplay41(LUADisplay* display);
void initDisplay42(LUADisplay* display);
void initDisplay43(LUADisplay* display);
void initDisplay44(LUADisplay* display);
void initDisplay45(LUADisplay* display);
void initDisplay46(LUADisplay* display);
void initDisplay47(LUADisplay* display);
void initDisplay48(LUADisplay* display);
void initDisplay49(LUADisplay* display);
void initDisplay50(LUADisplay* display);
void initDisplay51(LUADisplay* display);

#endif

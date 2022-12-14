#include "LUADisplay.h"

extern void(*luaDisplay[52])(LUADisplay *display) =
{0};


//---------------------------------------------------
//
//Init the function pointers
//
//---------------------------------------------------
void __fastcall initFunctionCalls()
{
	luaDisplay[0] = initDisplay0;
	luaDisplay[1] = initDisplay1;
	luaDisplay[2] = initDisplay2;
	luaDisplay[3] = initDisplay3;
	luaDisplay[4] = initDisplay4;
	luaDisplay[5] = initDisplay5;
	luaDisplay[6] = initDisplay6;
	luaDisplay[7] = initDisplay7;
	luaDisplay[8] = initDisplay8;
	luaDisplay[9] = initDisplay9;
	luaDisplay[10] = initDisplay10;
	luaDisplay[11] = initDisplay11;
	luaDisplay[12] = initDisplay12;
	luaDisplay[13] = initDisplay13;
	luaDisplay[14] = initDisplay14;
	luaDisplay[15] = initDisplay15;
	luaDisplay[16] = initDisplay16;
	luaDisplay[17] = initDisplay17;
	luaDisplay[18] = initDisplay18;
	luaDisplay[19] = initDisplay19;
	luaDisplay[20] = initDisplay20;
	luaDisplay[21] = initDisplay21;
	luaDisplay[22] = initDisplay22;
	luaDisplay[23] = initDisplay23;
	luaDisplay[24] = initDisplay24;
	luaDisplay[25] = initDisplay25;
	luaDisplay[26] = initDisplay26;
	luaDisplay[27] = initDisplay27;
	luaDisplay[28] = initDisplay28;
	luaDisplay[29] = initDisplay29;
	luaDisplay[30] = initDisplay30;
	luaDisplay[31] = initDisplay31;
	luaDisplay[32] = initDisplay32;
	luaDisplay[33] = initDisplay33;
	luaDisplay[34] = initDisplay34;
	luaDisplay[35] = initDisplay35;
	luaDisplay[36] = initDisplay36;
	luaDisplay[37] = initDisplay37;
	luaDisplay[38] = initDisplay38;
	luaDisplay[39] = initDisplay39;
	luaDisplay[40] = initDisplay40;
	luaDisplay[41] = initDisplay41;
	luaDisplay[42] = initDisplay42;
	luaDisplay[43] = initDisplay43;
	luaDisplay[44] = initDisplay44;
	luaDisplay[45] = initDisplay45;
	luaDisplay[46] = initDisplay46;
	luaDisplay[47] = initDisplay47;
	luaDisplay[48] = initDisplay48;
	luaDisplay[49] = initDisplay49;
	luaDisplay[50] = initDisplay50;
	luaDisplay[51] = initDisplay51;
}


//---------------------------------------------------
//
//Clear all old data
//
//---------------------------------------------------
void __fastcall clearDisplayValues(LUADisplay* display)
{
	display->hasAccelerometer = false;
	display->isUprightOrientationPortrait = false;
	display->supportsScreenRotation = false;
	display->device = NULL;
	display->displayManufacturer = NULL;
	display->displayName = NULL;
	display->windowTitleBarName = NULL;
	display->androidDisplayApproximateDpi = 0.0;
	display->defaultFontSize = 0.0;
	display->iosPointHeight = 0.0;
	display->iosPointWidth = 0.0;
	display->safeLandscapeScreenInsetBottom = 0.0;
	display->safeLandscapeScreenInsetLeft = 0.0;
	display->safeLandscapeScreenInsetRight = 0.0;
	display->safeLandscapeScreenInsetStatusBar = 0.0;
	display->safeLandscapeScreenInsetTop = 0.0;
	display->safeScreenInsetBottom = 0.0;
	display->safeScreenInsetLeft = 0.0;
	display->safeScreenInsetRight = 0.0;
	display->safeScreenInsetStatusBar = 0.0;
	display->safeScreenInsetTop = 0.0;
	display->screenHeight = 0.0;
	display->screenOriginX = 0.0;
	display->screenOriginY = 0.0;
	display->screenWidth = 0.0;

	//
	delete display->deviceImage;
	delete display->screenDressing;
	delete display->statusBarBlack;
	delete display->statusBarDarkTransparent;
	delete display->statusBarDefault;
	delete display->statusBarLightTransparent;
	delete display->statusBarTranslucent;

	display->deviceImage = NULL;
	display->screenDressing = NULL;
	display->statusBarBlack = NULL;
	display->statusBarDarkTransparent = NULL;
	display->statusBarDefault = NULL;
	display->statusBarLightTransparent = NULL;
	display->statusBarTranslucent = NULL;

	//Re-init the bitmaps
	initBitmapData(display);
}


//---------------------------------------------------
//
//Initialize TBitmap data
//
//---------------------------------------------------
void __fastcall initBitmapData(LUADisplay* display)
{
	display->deviceImage = new TBitmap();
	display->screenDressing = new TBitmap();
	display->statusBarBlack = new TBitmap();
	display->statusBarDarkTransparent = new TBitmap();
	display->statusBarDefault = new TBitmap();
	display->statusBarLightTransparent = new TBitmap();
	display->statusBarTranslucent = new TBitmap();
}


//---------------------------------------------------
//
//Display data for: AmazonFireTV1080
//
//---------------------------------------------------
void initDisplay0(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tv";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Amazon";
	display->displayName = "AFTB";
	display->isUprightOrientationPortrait = false;
	display->supportsScreenRotation = false;
	display->windowTitleBarName = "Amazon Fire TV (Gen. 1)";
	display->safeScreenInsetTop = 96;
	display->safeScreenInsetLeft = 54;
	display->safeScreenInsetBottom = 96;
	display->safeScreenInsetRight = 54;
	display->safeLandscapeScreenInsetTop = 54;
	display->safeLandscapeScreenInsetLeft = 96;
	display->safeLandscapeScreenInsetBottom = 54;
	display->safeLandscapeScreenInsetRight = 96;
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: AmazonFireTV720
//
//---------------------------------------------------
void initDisplay1(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tv";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Amazon";
	display->displayName = "AFTB";
	display->isUprightOrientationPortrait = false;
	display->supportsScreenRotation = false;
	display->windowTitleBarName = "Amazon Fire TV (Gen. 1)";
	display->safeScreenInsetTop = 64;
	display->safeScreenInsetLeft = 36;
	display->safeScreenInsetBottom = 64;
	display->safeScreenInsetRight = 36;
	display->safeLandscapeScreenInsetTop = 36;
	display->safeLandscapeScreenInsetLeft = 64;
	display->safeLandscapeScreenInsetBottom = 36;
	display->safeLandscapeScreenInsetRight = 64;
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: AppleTV
//
//---------------------------------------------------
void initDisplay2(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "tvos-tv";
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->displayManufacturer = "Apple";
	display->displayName = "Apple TV";
	display->windowTitleBarName = "Apple TV";
	display->defaultFontSize = 17 * 2;
	display->supportsScreenRotation = false;
	display->isUprightOrientationPortrait = false;
	display->safeScreenInsetTop = 90;
	display->safeScreenInsetLeft = 60;
	display->safeScreenInsetBottom = 90;
	display->safeScreenInsetRight = 60;
	display->safeLandscapeScreenInsetTop = 60;
	display->safeLandscapeScreenInsetLeft = 90;
	display->safeLandscapeScreenInsetBottom = 60;
	display->safeLandscapeScreenInsetRight = 90;
}


//---------------------------------------------------
//
//Display data for: Galaxy_Note20_Ultra
//
//---------------------------------------------------
void initDisplay3(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-phone";
	display->screenOriginX = 35;
	display->screenOriginY = 80;
	display->screenWidth = 1440;
	display->screenHeight = 3088;
	display->safeScreenInsetTop = 100;
	display->safeScreenInsetLeft = 0 * 3;
	display->safeScreenInsetBottom = 30;
	display->safeScreenInsetRight = 0 * 3;
	display->safeLandscapeScreenInsetTop = 0 * 3;
	display->safeLandscapeScreenInsetLeft = 100;
	display->safeLandscapeScreenInsetBottom = 0 * 3;
	display->safeLandscapeScreenInsetRight = 80;
	display->androidDisplayApproximateDpi = 420; //xxhdpi
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRA", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "samsung";
	display->displayName = "SM-N981x";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASTATUSBARGRAY", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASTATUSBARTRANSPARENT", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "GALAXY_NOTE20_ULTRASCREENDRESSING", RT_RCDATA);
	display->screenDressing->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "Galaxy Note20 Ultra";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1080x1920_2
//
//---------------------------------------------------
void initDisplay4(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 416; //2.6 x mdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (2.6 * mdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1080x1920_xhdpi
//
//---------------------------------------------------
void initDisplay5(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1080x1920_xxhdpi
//
//---------------------------------------------------
void initDisplay6(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 480; //xxhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xxhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1200x1920_xhdpi
//
//---------------------------------------------------
void initDisplay7(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1200;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1200x1920_xxhdpi
//
//---------------------------------------------------
void initDisplay8(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1200;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 480; //xxhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xxhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1440x2560_3
//
//---------------------------------------------------
void initDisplay9(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1440;
	display->screenHeight = 2560;
	display->androidDisplayApproximateDpi = 560; //3.5 * mdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (3.5 * mdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1440x2560_xxhdpi
//
//---------------------------------------------------
void initDisplay10(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1440;
	display->screenHeight = 2560;
	display->androidDisplayApproximateDpi = 480; //xxhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xxhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1440x2560_xxxhdpi
//
//---------------------------------------------------
void initDisplay11(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1440;
	display->screenHeight = 2560;
	display->androidDisplayApproximateDpi = 640; //xxxhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xxxhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1536x2048
//
//---------------------------------------------------
void initDisplay12(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1536;
	display->screenHeight = 2048;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_1600x2560
//
//---------------------------------------------------
void initDisplay13(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1600;
	display->screenHeight = 2560;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_320x480
//
//---------------------------------------------------
void initDisplay14(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 320;
	display->screenHeight = 480;
	display->androidDisplayApproximateDpi = 160; //mdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (mdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_480x800
//
//---------------------------------------------------
void initDisplay15(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 480;
	display->screenHeight = 800;
	display->androidDisplayApproximateDpi = 240; //hdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (hdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_480x854
//
//---------------------------------------------------
void initDisplay16(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 400;
	display->screenHeight = 854;
	display->androidDisplayApproximateDpi = 240; //hdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (hdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_720x1280
//
//---------------------------------------------------
void initDisplay17(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_768x1280
//
//---------------------------------------------------
void initDisplay18(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 768;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_800x1280_hdpi
//
//---------------------------------------------------
void initDisplay19(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 800;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 240; //hdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (hdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_800x1280_mdpi
//
//---------------------------------------------------
void initDisplay20(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 800;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 160; //mdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (mdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_800x1280_tvdpi
//
//---------------------------------------------------
void initDisplay21(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 800;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 213; //tvdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (tvdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_Android_800x1280_xhdpi
//
//---------------------------------------------------
void initDisplay22(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-android-device";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 800;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericAndroidDevice";
	display->windowTitleBarName = "Android (xhdpi)";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Generic_iPad_1536x2048
//
//---------------------------------------------------
void initDisplay23(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-tablet";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 768*2;
	display->screenHeight = 1024*2;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad @2x";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_iPad_2048x2732
//
//---------------------------------------------------
void initDisplay24(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-tablet";
	display->screenWidth = 2048;
	display->screenHeight = 2732;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad @3x";
	display->defaultFontSize = 17 * 2;
}


//---------------------------------------------------
//
//Display data for: Generic_iPad_768x1024
//
//---------------------------------------------------
void initDisplay25(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-tablet";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 768;
	display->screenHeight = 1024;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->safeLandscapeScreenInsetStatusBar = 20;
	display->safeScreenInsetStatusBar = 20;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad @1x";
	display->defaultFontSize = 17;
}


//---------------------------------------------------
//
//Display data for: Generic_iPhone_1080x1920
//
//---------------------------------------------------
void initDisplay26(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-phone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->iosPointWidth = 414;
	display->iosPointHeight = 736;
	display->safeLandscapeScreenInsetStatusBar = 20 * 3;
	display->safeScreenInsetStatusBar = 20 * 3;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone @3x";
	display->defaultFontSize = 17 * 3; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_iPhone_1125x2436
//
//---------------------------------------------------
void initDisplay27(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-phone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1125;
	display->screenHeight = 2436;
	display->safeScreenInsetTop = 44 * 3;
	display->safeScreenInsetLeft = 0 * 3;
	display->safeScreenInsetBottom = 34 * 3;
	display->safeScreenInsetRight = 0 * 3;
	display->safeLandscapeScreenInsetTop = 0 * 3;
	display->safeLandscapeScreenInsetLeft = 44 * 3;
	display->safeLandscapeScreenInsetBottom = 21 * 3;
	display->safeLandscapeScreenInsetRight = 44 * 3;
	display->iosPointWidth = 375;
	display->iosPointHeight = 812;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSCREENDRESSINGBORDERLESS", RT_RCDATA);
	display->screenDressing->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone @3x";
	display->defaultFontSize = 17 * 3; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_iPhone_640x1136
//
//---------------------------------------------------
void initDisplay28(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-phone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 640;
	display->screenHeight = 1136;
	display->iosPointWidth = 320;
	display->iosPointHeight = 568;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone @2x";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_iPhone_640x960
//
//---------------------------------------------------
void initDisplay29(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-phone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 640;
	display->screenHeight = 960;
	display->iosPointWidth = 320;
	display->iosPointHeight = 480;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone @2x";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_iPhone_750x1334
//
//---------------------------------------------------
void initDisplay30(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-ios-phone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 750;
	display->screenHeight = 1334;
	display->iosPointWidth = 375;
	display->iosPointHeight = 667;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone @2x";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Generic_WindowsPhone_480x800
//
//---------------------------------------------------
void initDisplay31(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-winphone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 480;
	display->screenHeight = 800;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericWindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "Windows Phone";
	display->defaultFontSize = 25.333;
}


//---------------------------------------------------
//
//Display data for: Generic_WindowsPhone_720x1280
//
//---------------------------------------------------
void initDisplay32(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-winphone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericWindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "Windows Phone";
	display->defaultFontSize = 25.333 * 1.5; //Apply a 720p scale factor.
}


//---------------------------------------------------
//
//Display data for: Generic_WindowsPhone_768x1280
//
//---------------------------------------------------
void initDisplay33(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "borderless-winphone";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 768;
	display->screenHeight = 1280;
	display->displayManufacturer = "Corona Labs Inc.";
	display->displayName = "GenericWindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "Windows Phone";
	display->defaultFontSize = 25.333 * 1.6; //Apply a WXGA scale factor.
}


//---------------------------------------------------
//
//Display data for: HTC_8S
//
//---------------------------------------------------
void initDisplay34(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "winphone-phone";
	display->screenOriginX = 48;
	display->screenOriginY = 144;
	display->screenWidth = 480;
	display->screenHeight = 800;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "HTC_8S", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Not Supported Yet"; //Unsupported on WP8, return the stub implementation
	display->displayName = "WindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "HTC Windows Phone 8S";
	display->defaultFontSize = 25.333;
}


//---------------------------------------------------
//
//Display data for: iPadAir
//
//---------------------------------------------------
void initDisplay35(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-tablet";
	display->screenOriginX = 117;
	display->screenOriginY = 235;
	display->screenWidth = 768*2;
	display->screenHeight = 1024*2;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADAIR", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad Air";
	display->defaultFontSize = 17 * 2;
}


//---------------------------------------------------
//
//Display data for: iPadMini
//
//---------------------------------------------------
void initDisplay36(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-tablet";
	display->screenOriginX = 47;
	display->screenOriginY = 131;
	display->screenWidth = 768;
	display->screenHeight = 1024;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINI", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADMINISTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad mini";
	display->defaultFontSize = 17 * 2;
}


//---------------------------------------------------
//
//Display data for: iPadPro
//
//---------------------------------------------------
void initDisplay37(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-tablet";
	display->screenOriginX = 123;
	display->screenOriginY = 231;
	display->screenWidth = 2048;
	display->screenHeight = 2732;
	display->iosPointWidth = 768;
	display->iosPointHeight = 1024;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPRO", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPad";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPADPROSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPad Pro";
	display->defaultFontSize = 17 * 2;
}


//---------------------------------------------------
//
//Display data for: iPhone4S
//
//---------------------------------------------------
void initDisplay38(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-phone";
	display->screenOriginX = 64;
	display->screenOriginY = 256;
	display->screenWidth = 640;
	display->screenHeight = 960;
	display->iosPointWidth = 320;
	display->iosPointHeight = 480;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4S", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE4STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone 4S";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: iPhone5
//
//---------------------------------------------------
void initDisplay39(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-phone";
	display->screenOriginX = 64;
	display->screenOriginY = 228;
	display->screenWidth = 640;
	display->screenHeight = 1136;
	display->iosPointWidth = 320;
	display->iosPointHeight = 568;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE5STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone 5";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: iPhone6
//
//---------------------------------------------------
void initDisplay40(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-phone";
	display->screenOriginX = 55;
	display->screenOriginY = 215;
	display->screenWidth = 750;
	display->screenHeight = 1334;
	display->iosPointWidth = 375;
	display->iosPointHeight = 667;
	display->safeLandscapeScreenInsetStatusBar = 20 * 2;
	display->safeScreenInsetStatusBar = 20 * 2;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6STATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone 6";
	display->defaultFontSize = 17 * 2; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: iPhone6Plus
//
//---------------------------------------------------
void initDisplay41(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-phone";
	display->screenOriginX = 82;
	display->screenOriginY = 298;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->iosPointWidth = 414;
	display->iosPointHeight = 736;
	display->safeLandscapeScreenInsetStatusBar = 20 * 3;
	display->safeScreenInsetStatusBar = 20 * 3;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUS", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONE6PLUSSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone 6 Plus";
	display->defaultFontSize = 17 * 3; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: iPhoneX
//
//---------------------------------------------------
void initDisplay42(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "ios-phone";
	display->screenOriginX = 85;
	display->screenOriginY = 77;
	display->screenWidth = 1125;
	display->screenHeight = 2436;
	display->safeScreenInsetTop = 44 * 3;
	display->safeScreenInsetLeft = 0 * 3;
	display->safeScreenInsetBottom = 34 * 3;
	display->safeScreenInsetRight = 0 * 3;
	display->safeLandscapeScreenInsetTop = 0 * 3;
	display->safeLandscapeScreenInsetLeft = 44 * 3;
	display->safeLandscapeScreenInsetBottom = 21 * 3;
	display->safeLandscapeScreenInsetRight = 44 * 3;
	display->iosPointWidth = 375;
	display->iosPointHeight = 812;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEX", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Apple";
	display->displayName = "iPhone";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSCREENDRESSING", RT_RCDATA);
	display->screenDressing->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARWHITE", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "IPHONEXSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "iPhone X";
	display->defaultFontSize = 17 * 3; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: KindleFireHD7
//
//---------------------------------------------------
void initDisplay43(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tablet";
	display->screenOriginX = 72;
	display->screenOriginY = 70;
	display->screenWidth = 800;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 240; //hdpi
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "KINDLEFIREHD7", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Amazon";
	display->displayName = "KFTT";
	display->statusBarDefault = NULL;          //No fName loaded
	display->statusBarTranslucent = NULL;      //No fName loaded
	display->statusBarBlack = NULL;            //No fName loaded
	display->statusBarLightTransparent = NULL; //No fName loaded
	display->statusBarDarkTransparent = NULL;  //No fName loaded
	display->windowTitleBarName = "Kindle Fire HD 7\" (2012)",
		display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: KindleFireHD9
//
//---------------------------------------------------
void initDisplay44(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tablet";
	display->screenOriginX = 102;
	display->screenOriginY = 100;
	display->screenWidth = 1200;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 240; //hdpi
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "KINDLEFIREHD9", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Amazon";
	display->displayName = "KFJWI";
	display->statusBarDefault = NULL;          //No fName loaded
	display->statusBarTranslucent = NULL;      //No fName loaded
	display->statusBarBlack = NULL;            //No fName loaded
	display->statusBarLightTransparent = NULL; //No fName loaded
	display->statusBarDarkTransparent = NULL;  //No fName loaded
	display->windowTitleBarName = "Kindle Fire HD 8.9\" (2012)",
		display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Nokia_Lumia_920
//
//---------------------------------------------------
void initDisplay45(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "winphone-phone";
	display->screenOriginX = 122;
	display->screenOriginY = 315;
	display->screenWidth = 768;
	display->screenHeight = 1280;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "NOKIA_LUMIA_920", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Not Supported Yet"; //Unsupported on WP8, return the stub implementation
	display->displayName = "WindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "Nokia Lumia 920";
	display->defaultFontSize = 25.333 * 1.6; //Apply a WXGA scale factor.
}


//---------------------------------------------------
//
//Display data for: NX_Switch
//
//---------------------------------------------------
void initDisplay46(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "nx64";
	display->screenOriginX = 92;
	display->screenOriginY = 420;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "NX_SWITCH", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Nintendo";
	display->displayName = "Switch";
	display->windowTitleBarName = "NX Switch";
	display->defaultFontSize = 17; //Converts default font point size to pixels.
}


//---------------------------------------------------
//
//Display data for: Ouya1080
//
//---------------------------------------------------
void initDisplay47(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tv";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 1080;
	display->screenHeight = 1920;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "OUYA";
	display->displayName = "OUYA Console";
	display->supportsScreenRotation = false;
	display->isUprightOrientationPortrait = false;
	display->windowTitleBarName = "Ouya";
	display->safeScreenInsetTop = 96;
	display->safeScreenInsetLeft = 54;
	display->safeScreenInsetBottom = 96;
	display->safeScreenInsetRight = 54;
	display->safeLandscapeScreenInsetTop = 54;
	display->safeLandscapeScreenInsetLeft = 96;
	display->safeLandscapeScreenInsetBottom = 54;
	display->safeLandscapeScreenInsetRight = 96;
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Ouya720
//
//---------------------------------------------------
void initDisplay48(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-tv";
	display->screenOriginX = 0;
	display->screenOriginY = 0;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	display->androidDisplayApproximateDpi = 320; //xhdpi
	display->displayManufacturer = "OUYA";
	display->displayName = "OUYA Console";
	display->supportsScreenRotation = false;
	display->isUprightOrientationPortrait = false;
	display->windowTitleBarName = "Ouya";
	display->safeScreenInsetTop = 64;
	display->safeScreenInsetLeft = 36;
	display->safeScreenInsetBottom = 64;
	display->safeScreenInsetRight = 36;
	display->safeLandscapeScreenInsetTop = 36;
	display->safeLandscapeScreenInsetLeft = 64;
	display->safeLandscapeScreenInsetBottom = 36;
	display->safeLandscapeScreenInsetRight = 64;
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Pixel_4a
//
//---------------------------------------------------
void initDisplay49(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-phone";
	display->screenOriginX = 75;
	display->screenOriginY = 110;
	display->screenWidth = 1080;
	display->screenHeight = 2340;
	display->safeScreenInsetTop = 130;
	display->safeScreenInsetLeft = 0;
	display->safeScreenInsetBottom = 80;
	display->safeScreenInsetRight = 0;
	display->safeLandscapeScreenInsetTop = 0 * 3;
	display->safeLandscapeScreenInsetLeft = 130;
	display->safeLandscapeScreenInsetBottom = 0 * 3;
	display->safeLandscapeScreenInsetRight = 80;
	display->androidDisplayApproximateDpi = 420; //xxhdpi
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4A", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "google";
	display->displayName = "GA02099";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASTATUSBARTRANSPARENT", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASTATUSBARTRANSPARENT", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "PIXEL_4ASCREENDRESSING", RT_RCDATA);
	display->screenDressing->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "Pixel 4a";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}


//---------------------------------------------------
//
//Display data for: Samsung_ATIV_S
//
//---------------------------------------------------
void initDisplay50(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "winphone-phone";
	display->screenOriginX = 58;
	display->screenOriginY = 152;
	display->screenWidth = 720;
	display->screenHeight = 1280;
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNG_ATIV_S", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "Not Supported Yet"; //Unsupported on WP8, return the stub implementation
	display->displayName = "WindowsPhone";
	display->supportsScreenRotation = true;
	display->hasAccelerometer = true;
	display->windowTitleBarName = "Samsung ATIV S";
	display->defaultFontSize = 25.333 * 1.5; //Apply a 720p scale factor.
}


//---------------------------------------------------
//
//Display data for: Samsung_Galaxy_S21
//
//---------------------------------------------------
void initDisplay51(LUADisplay* display)
{
	//Used to load image resources from 'LUADisplay.res'
	TResourceStream* imgStream;

	//Clear all variables for a clean start
	clearDisplayValues(display);


	display->device = "android-phone";
	display->screenOriginX = 50;
	display->screenOriginY = 65;
	display->screenWidth = 1080;
	display->screenHeight = 2400;
	display->safeScreenInsetTop = 30*3;
	display->safeScreenInsetLeft = 0 * 3;
	display->safeScreenInsetBottom = 30 * 3;
	display->safeScreenInsetRight = 0 * 3;
	display->safeLandscapeScreenInsetTop = 0 * 3;
	display->safeLandscapeScreenInsetLeft = 30 * 3;
	display->safeLandscapeScreenInsetBottom = 0 * 3;
	display->safeLandscapeScreenInsetRight = 30 * 3;
	display->androidDisplayApproximateDpi = 420; //xxhdpi
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYS21", RT_RCDATA);
	display->deviceImage->LoadFromStream(imgStream);
	delete imgStream;

	display->displayManufacturer = "samsung";
	display->displayName = "SM-G991x";
	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYSTATUSBARGRAY", RT_RCDATA);
	display->statusBarDefault->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYSTATUSBARGRAY", RT_RCDATA);
	display->statusBarTranslucent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYSTATUSBARBLACK", RT_RCDATA);
	display->statusBarBlack->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYSTATUSBARWHITE", RT_RCDATA);
	display->statusBarLightTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXYSTATUSBARBLACK", RT_RCDATA);
	display->statusBarDarkTransparent->LoadFromStream(imgStream);
	delete imgStream;

	//Load image from stream
	imgStream = new TResourceStream((int)HInstance, "SAMSUNGGALAXY21SCREENDRESSING", RT_RCDATA);
	display->screenDressing->LoadFromStream(imgStream);
	delete imgStream;

	display->windowTitleBarName = "Samsung Galaxy S21";
	display->defaultFontSize = 18.0 * (display->androidDisplayApproximateDpi / 160);
}

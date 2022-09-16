//---------------------------------------------------------------------------

#ifndef UIAnimationH
#define UIAnimationH

#include <System.Classes.hpp>


class UIAnimation
{
public:
	//AnsiString Name;
	AnsiString Name;
	int        time;
	int        delay;
	int        iterations;
	int        transition;
	int        easeType;
	int        interpolation;
	AnsiString tag;
	bool       delta;
	bool       looping;
	bool       xChk;
	bool       yChk;
	bool       widthChk;
	bool       heightChk;
	bool       rotationChk;
	bool       xScaleChk;
	bool       yScaleChk;
	bool       alphaChk;
	bool       fontSizeChk;
	float      x;
	float      y;
	float      width;
	float      height;
	float      rotation;
	float      xScale;
	float      yScale;
	float      alpha;
	int        fontSize;
};

#endif

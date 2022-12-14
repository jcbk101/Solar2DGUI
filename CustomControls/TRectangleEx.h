//---------------------------------------------------------------------------

#ifndef TRectangleExH
#define TRectangleExH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Forms.hpp>
#include <System.UITypes.hpp>


//--------------------------------------
//
//Extend TRectangle to support 9-Patch
//
//--------------------------------------
class TRectangleEx: public TRectangle
{
private:



public:
	//overload
	//__fastcall TRectangleEx(TControl* AOwner): TRectangle(AOwner)
	//{
	//
	//}
	__fastcall TRectangleEx(TControl *AOwner) : TRectangle(AOwner)
	{
		this->animations = new TList();
	}


	virtual __fastcall ~TRectangleEx()
	{
		this->animations->Clear();
		delete this->animations;
		this->animations = NULL;
	}


//	//Setters
//	void setScaleX(float X)
//	{
//		Scale->X = X;
//	}
//
//
//	void setScaleY(float Y)
//	{
//		Scale->Y = Y;
//	}
//
//
//	void setRotationAngle(float Angle)
//	{
//		RotationAngle = Angle;
//	}
//
//
//	void setRotationCenterX(float X)
//	{
//		RotationCenter->X = X;
//	}
//
//
//	void setRotationCenterY(float Y)
//	{
//		RotationCenter->Y = Y;
//	}
//
//
//	//Getters
//	float getScaleX()
//	{
//		return Scale->X;
//	}
//
//
//	float getScaleY()
//	{
//		return Scale->Y;
//	}
//
//
//	float getRotationAngle()
//	{
//		return RotationAngle;
//	}
//
//
//	float getRotationCenterX()
//	{
//		return RotationCenter->X;
//	}
//
//
//	float getRotationCenterY()
//	{
//		return RotationCenter->Y;
//	}

	enum
	{
		Standard = 0,
		NinePatch,
		ImageSheet
	};

	//These values hold the copied image data offsets
	//for each loaded 9-Patch file.
	TRect TL, TC, TR;
	TRect L, C, R;
	TRect BL, BC, BR;

	TAlphaColor oldColor;

	TList *animations;

	float oldW;
	float oldH;
	float oldX;
	float oldY;

	int imageType;
	int imageIndex;
};

#endif

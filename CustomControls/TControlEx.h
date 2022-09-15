//---------------------------------------------------------------------------

#ifndef TControlExH
#define TControlExH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Forms.hpp>
#include <System.UITypes.hpp>


//--------------------------------------
//
//Accessor for protected data
//
//--------------------------------------
class TControlEx: public TControl
{
private:



public:
	//Setters
	void setScaleX(float X)
	{
		Scale->X = X;
	}


	void setScaleY(float Y)
	{
		Scale->Y = Y;
	}


	void setRotationAngle(float Angle)
	{
		RotationAngle = Angle;
	}


	void setRotationCenterX(float X)
	{
		RotationCenter->X = X;
	}


	void setRotationCenterY(float Y)
	{
		RotationCenter->Y = Y;
	}


	//Getters
	float getScaleX()
	{
		return Scale->X;
	}


	float getScaleY()
	{
		return Scale->Y;
	}


	float getRotationAngle()
	{
		return RotationAngle;
	}


	float getRotationCenterX()
	{
		return RotationCenter->X;
	}


	float getRotationCenterY()
	{
		return RotationCenter->Y;
	}

};

#endif

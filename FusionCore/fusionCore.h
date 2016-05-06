#pragma once

#include "..\Dynamo.h"


extern Ptr<Application> app;
extern Ptr<UserInterface> ui;

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DYNAMO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DYNAMO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FUSION_EXPORTS
#define FUSION_API __declspec(dllexport)
#else
#define FUSION_API __declspec(dllimport)
#endif

class FUSION_API FusionCore
{
	
public:
	Ptr<SketchPoint> static pointByCoordinates(double x, double y, double z);
	Ptr<SketchCircle> static circleByPointRadius( Ptr<Point3D> point, double r);
};

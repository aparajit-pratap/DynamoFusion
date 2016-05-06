// This is the main DLL file.

#include "stdafx.h"

#include "FusionManagedWrapper.h"

using namespace FusionManagedWrapper;

FusionCurve::FusionCurve(SketchCircle* pCurve) 
{
	m_pCurve = pCurve;
}

FusionCurve^ FusionCurve::createCircle(double x, double y, double z, double r)
{
	Ptr<Point3D> point = Point3D::create(x, y, z);
	Ptr<SketchCircle> circle = FusionCore::circleByPointRadius(point, r);
	
	return gcnew FusionCurve(circle.detach());
}

FusionCurve::~FusionCurve()
{
	m_pCurve->deleteMe();
}


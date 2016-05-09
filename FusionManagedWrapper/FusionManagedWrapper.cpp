// This is the main DLL file.

#include "stdafx.h"

#include "FusionManagedWrapper.h"

using namespace FusionManagedWrapper;

cli::array<FusionEntity^>^ FusionEntity::getSelectedEntities()
{
	auto bodies = FusionCore::getSelectedEntities();
	int size = bodies.size();
	cli::array<FusionEntity^>^ bodyArray = gcnew cli::array<FusionEntity^>(size);
	
	for (int i = 0; i < size; i++)
	{
		bodyArray[i] = gcnew FusionSolid(bodies[i].detach());
	}
	return bodyArray;
}

FusionEntity::~FusionEntity()
{
	//m_pEntity->deleteMe();
}

///////////////////////////////////////////////////////////////////////////////

FusionSolid::FusionSolid(BRepBody* pSolid)
{
	m_pSolid = pSolid;
}

FusionSolid::~FusionSolid()
{
	m_pSolid->deleteMe();
}


//////////////////////////////////////////////////////////////////////////////

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


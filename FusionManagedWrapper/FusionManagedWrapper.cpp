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

cli::array<Geometry^>^ FusionSolid::Decompose()
{
	
	auto faces = m_pSolid->faces();
	//auto edges = m_pSolid->edges();
	return BRepFacesInfo(faces);
}

FusionSolid::~FusionSolid()
{
	m_pSolid->deleteMe();
}

cli::array<Geometry^>^ FusionSolid::BRepFacesInfo(Ptr<BRepFaces> faces)
{
	//std::vector<Geometry^> entities;
	size_t count = faces->count();
	cli::array<Geometry^>^ entities = gcnew cli::array<Geometry^>(count);
	
	for (size_t j = 0; j < count; j++) {
		Ptr<BRepFace> face = faces->item(j);
		Ptr<adsk::core::Surface> someFace = face->geometry();

		if (Ptr<adsk::core::Cone> coneGeom = someFace) {
			Ptr<Point3D> origin;
			Ptr<Vector3D> axis;
			double radius = 0.0;
			double halfAngle = 0.0;
			bool result = coneGeom->getData(origin, axis, radius, halfAngle);
			if (!result)
				continue;

			//auto cone = gcnew FusionCone(face, origin, axis, radius, halfAngle);
			Point^ startPoint = Point::ByCoordinates(origin->x(), origin->y(), origin->z());
			double ht = radius / tan(halfAngle);
			if (!axis->scaleBy(ht))
				continue;

			auto origVector = origin->asVector();
			if (!origVector->add(axis))
				continue;

			Point^ endPoint = Point::ByCoordinates(origVector->x(), origVector->y(), origVector->z());
			Autodesk::DesignScript::Geometry::Cone^ cone = Autodesk::DesignScript::Geometry::Cone::ByPointsRadius(
				startPoint, endPoint, radius);
			entities[j] = cone;
		}
		else if (Ptr<adsk::core::Cylinder> cylinderGeom = someFace) {
			Ptr<Point3D> origin;
			Ptr<Vector3D> axis;
			double radius = 0.0;
			bool result = cylinderGeom->getData(origin, axis, radius);
			if (!result)
				continue;
		}
		else if (Ptr<EllipticalCone> ellipticalConeGeom = someFace) {
			Ptr<Point3D> origin;
			Ptr<Vector3D> axis;
			Ptr<Vector3D> majorAxisDirection;
			double majorRadius, minorRadius, halfAngle;
			bool result = ellipticalConeGeom->getData(origin, axis, majorAxisDirection, majorRadius, minorRadius, halfAngle);
			if (!result)
				continue;
		}
		else if (Ptr<EllipticalCylinder> ellipticalCylinderGeom = someFace) {
			Ptr<Point3D> origin;
			Ptr<Vector3D> axis;
			Ptr<Vector3D> majorAxis;
			double majorRadius, minorRadius;
			bool result = ellipticalCylinderGeom->getData(origin, axis, majorAxis, majorRadius, minorRadius);
			if (!result)
				continue;
		}
		else if (Ptr<adsk::core::NurbsSurface> nurbsSurfaceGeom = someFace) {
			entities[j] = FusionSurface::CreateNurbsSurface(nurbsSurfaceGeom);
		}
		else if (Ptr<adsk::core::Plane> planeGeom = someFace) {
			planeGeom->uDirection();
			planeGeom->vDirection();
		}
		else if (Ptr<adsk::core::Sphere> sphereGeom = someFace) {
			Ptr<Point3D> origin;
			double radius = 0.0;
			bool result = sphereGeom->getData(origin, radius);
			if (!result)
				continue;
		}
		else if (Ptr<Torus> torusGeom = someFace) {
			Ptr<Point3D> origin;
			Ptr<Vector3D> axis;
			double majorRadius, minorRadius;
			bool result = torusGeom->getData(origin, axis, majorRadius, minorRadius);
			if (!result)
				continue;
		}

	}
	return entities;
}
//////////////////////////////////////////////////////////////////////////////

FusionCone::FusionCone(Ptr<BRepFace> face, Ptr<Point3D> origin, Ptr<Vector3D> axis, double radius, double halfAngle)
	: FusionSolid(face->body().detach())
{
	m_pCone = face.detach();
}

//////////////////////////////////////////////////////////////////

FusionSurface::FusionSurface(Ptr<BRepFace> face) : FusionSolid(face->body().detach())
{
	m_pFace = face.detach();
}

Autodesk::DesignScript::Geometry::NurbsSurface^ FusionSurface::CreateNurbsSurface(Ptr<adsk::core::NurbsSurface> nurbsSurfaceGeom)
{
	int degreeU, degreeV, controlPointCountU, controlPointCountV;
	std::vector<Ptr<Point3D>> controlPoints;
	std::vector<double> knotsU;
	std::vector<double> knotsV;
	std::vector<double> weights;
	NurbsSurfaceProperties propertiesU, propertiesV;

	bool result = nurbsSurfaceGeom->getData(
		degreeU, degreeV, controlPointCountU, controlPointCountV,
		controlPoints, knotsU, knotsV, weights,
		propertiesU, propertiesV
		);
	if (!result)
		return nullptr;

	int cpSize = controlPoints.size();
	int wsize = weights.size();

	cli::array<array<Point^>^>^ ctrlPts = gcnew cli::array<array<Point^>^>(controlPointCountU);
	for (int i = 0; i < controlPointCountU; i++)
	{
		ctrlPts[i] = gcnew cli::array<Point^>(controlPointCountV);
		for (int j = 0; j < controlPointCountV; j++)
		{
			auto pt = controlPoints[i*controlPointCountV + j];
			ctrlPts[i][j] = Point::ByCoordinates(pt->x(), pt->y(), pt->z());
		}
	}

	cli::array<array<double>^>^ wts = gcnew cli::array<array<double>^>(controlPointCountU);
	for (int i = 0; i < controlPointCountU; i++)
	{
		wts[i] = gcnew cli::array<double>(controlPointCountV);
		for (int j = 0; j < controlPointCountV; j++)
		{
			if (wsize == 0)
				wts[i][j] = 1.0;
			else
			{
				auto wt = weights[i*controlPointCountV + j];
				wts[i][j] = wt;
			}
		}
	}

	int numUknots = knotsU.size();
	auto uKnots = gcnew array<double>(numUknots);
	for (int i = 0; i < numUknots; i++)
	{
		uKnots[i] = knotsU[i];
	}

	int numVknots = knotsV.size();
	auto vKnots = gcnew array<double>(numVknots);
	for (int i = 0; i < numVknots; i++)
	{
		vKnots[i] = knotsV[i];
	}

	return Autodesk::DesignScript::Geometry::NurbsSurface::ByControlPointsWeightsKnots(
		ctrlPts, wts, uKnots, vKnots, degreeU, degreeV);
}

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


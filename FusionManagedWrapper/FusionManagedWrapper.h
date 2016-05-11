// FusionManagedWrapper.h

#pragma once

#include "fusionCore.h"

#using<ProtoGeometry.dll>

using namespace System;

using namespace Autodesk::DesignScript::Geometry;

namespace FusionManagedWrapper {

	/////////////////////////////////////////
	public ref class FusionEntity
	{
	public:
		static cli::array<FusionEntity^>^ getSelectedEntities();

	protected:
		~FusionEntity();
		//Base* m_pEntity;
	};

	//////////////////////////////////////////////
	public ref class FusionSolid : FusionEntity
	{
	public:
		FusionSolid(BRepBody* pSolid);
		cli::array<Geometry^>^ Decompose();
	protected:
		~FusionSolid();

	private:
		//static std::vector<Geometry^> BRepFacesInfo(Ptr<BRepFaces> faces);
		cli::array<Geometry^>^ BRepFacesInfo(Ptr<BRepFaces> faces);
		BRepBody* m_pSolid;
	};
	///////////////////////////////////////////////

	public ref class FusionCone : FusionSolid
	{
	public:
		FusionCone(Ptr<BRepFace> pFace, Ptr<Point3D> origin, Ptr<Vector3D> axis, double radius, double halfAngle);

	private:
		BRepFace* m_pCone;
	};
	////////////////////////////////////////////////

	public ref class FusionSurface : FusionSolid
	{
	public:
		FusionSurface(Ptr<BRepFace> pFace);
		static Autodesk::DesignScript::Geometry::NurbsSurface^ CreateNurbsSurface(Ptr<adsk::core::NurbsSurface> nurbsSurface);
	
	private:
		BRepFace* m_pFace;

	};

	public ref class FusionCurve : FusionEntity
	{
	public:
		static FusionCurve^ createCircle(double x, double y, double z, double r);
	protected:
		FusionCurve(SketchCircle* pCurve);
		~FusionCurve();

	private:
		SketchCircle* m_pCurve;

	};

}

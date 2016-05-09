// FusionManagedWrapper.h

#pragma once

#include "fusionCore.h"

using namespace System;

namespace FusionManagedWrapper {

	public ref class FusionEntity
	{
	public:
		static cli::array<FusionEntity^>^ getSelectedEntities();

	protected:
		~FusionEntity();
		//Base* m_pEntity;
	};

	public ref class FusionSolid : FusionEntity
	{
	public:
		FusionSolid(BRepBody* pSolid);

	protected:
		~FusionSolid();

	private:
		
		BRepBody* m_pSolid;
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

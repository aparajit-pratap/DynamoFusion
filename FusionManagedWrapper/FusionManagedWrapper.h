// FusionManagedWrapper.h

#pragma once

#include "fusionCore.h"

using namespace System;

namespace FusionManagedWrapper {

	public ref class FusionCurve 
	{
	public:
		static FusionCurve^ createCircle(double x, double y, double z, double r);
		~FusionCurve();

	private:
		FusionCurve(SketchCircle* pCurve);
		SketchCircle* m_pCurve;

	};
}

#include "fusionCore.h"

Ptr<SketchPoint> FusionCore::pointByCoordinates(double x, double y, double z)
{
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();

	Ptr<Sketches> sketches = rootComponent->sketches();
	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
	Ptr<Sketch> sketch = sketches->add(xyPlane);

	return sketch->sketchPoints()->add(Point3D::create(x, y, z));
}

Ptr<SketchCircle> FusionCore::circleByPointRadius( Ptr<Point3D> point, double r)
{
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();

	Ptr<Sketches> sketches = rootComponent->sketches();
	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
	Ptr<Sketch> sketch = sketches->add(xyPlane);

	Ptr<SketchCircles> circles = sketch->sketchCurves()->sketchCircles();
	Ptr<SketchCircle> circle = circles->addByCenterRadius(
		Point3D::create(point->x(), point->y(), point->z()), r);

	return circle;
}
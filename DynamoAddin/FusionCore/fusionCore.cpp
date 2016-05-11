#include <objbase.h>

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

std::vector<Ptr<BRepBody>> FusionCore::getSelectedEntities()
{
	//
	// Create array for storing objects
	//
	std::vector<Ptr<BRepBody>> selected_objects;

	/*if (selectionInput == nullptr)
		return objects;

	int selCount = selectionInput->selectionCount();

	//
	// Store the objects in the array
	//
	for (size_t i = 0; i < selCount; i++)
	{
		auto entity = selectionInput->selection(i)->entity();
		
		objects.push_back(entity);
	}*/

	for (int i = 0; i < selCount; i++)
	{
		selected_objects.push_back(objects[i]);
	}
	return selected_objects;
}


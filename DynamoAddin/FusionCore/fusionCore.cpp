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

//Ptr<SketchCircle> FusionCore::circleByPointRadius( Ptr<Point3D> point, double r)
//{
//	Ptr<Design> design = app->activeProduct();
//	Ptr<Component> rootComponent = design->rootComponent();
//
//	Ptr<Sketches> sketches = rootComponent->sketches();
//	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
//	/*Ptr<Sketch> sketch = sketches->item(0);
//	if(!sketch)*/
//	Ptr<Sketch>	sketch = sketches->add(xyPlane);
//
//	Ptr<SketchCircles> circles = sketch->sketchCurves()->sketchCircles();
//	Ptr<SketchCircle> circle = circles->addByCenterRadius(
//		Point3D::create(point->x(), point->y(), point->z()), r);
//
//	return circle;
//}

Ptr<SketchCircle> FusionCore::circleByPointRadius(Ptr<Point3D> point, double r, int idNumber)
{
	std::string id = "id" + std::to_string(idNumber);
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();
	//
	// Get all exisiting sketches
	//
	Ptr<Sketches> sketches = rootComponent->sketches();

	Ptr<SketchCircle> circle;
	Ptr<SketchCircles> circles;
	Ptr<SketchCurves> sketchCurves;
	//
	// Check if attribute exist in the first place or not.
	//
	std::vector<Ptr<Attribute>> attributes = design->findAttributes("Dynamo-SketchCircle", id);
	if (attributes.size() > 0) {

		//
		// Check each sketch in design
		//
		for (size_t j = 0; j < sketches->count(); j++) {
			Ptr<Sketch> sketch = sketches->item(j);

			sketchCurves = sketch->sketchCurves();
			if (!sketchCurves) continue;

			circles = sketchCurves->sketchCircles();
			if (!circles) continue;

			//
			// Check each circle in each sketch.
			//
			for (size_t i = 0; i < circles->count(); i++) {

				circle = circles->item(i);

				Ptr<Attributes> attrib = circle->attributes();
				if (!attrib) continue;

				if (attrib->itemByName("Dynamo-SketchCircle", id)) {
					//
					// Set New Point through translation
					//
					Ptr<Point3D> currPoint = circle->geometry()->center();
					Ptr<Point3D> pt = Point3D::create(point->x() - currPoint->x(), point->y() - currPoint->y(), point->z() - currPoint->z());
					if (!pt->isEqualTo(Point3D::create(0, 0, 0)))
						circle->centerSketchPoint()->move(pt->asVector());
					//
					// Set New Radius
					//
					circle->radius(r);
					return circle;
				}
			}

		}
	}

	//
	// No such attribute exist; create new circle in new sketch.
	//
	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
	Ptr<Sketch> sketch = sketches->add(xyPlane);

	sketchCurves = sketch->sketchCurves();
	if (!sketchCurves) return nullptr;

	circles = sketchCurves->sketchCircles();
	if (!circles) nullptr;

	circle = circles->addByCenterRadius(
		Point3D::create(point->x(), point->y(), point->z()), r);

	circle->attributes()->add("Dynamo-SketchCircle", id, "1");

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


//#include "Dynamo2.cpp"
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include <sstream>

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

std::string Arc3DGeometryInfo(Ptr<Arc3D> arcGeom)
{
	std::string arcInfo;
	if (!arcGeom)
		return arcInfo;

	Ptr<Point3D> center;
	Ptr<Vector3D> axis;
	Ptr<Vector3D> refVector;
	double radius = 0.0;
	double startAngle = 0.0;
	double endAngle = 0.0;
	bool result = arcGeom->getData(center, axis, refVector, radius, startAngle, endAngle);
	if (!result)
		return arcInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Center: " << center->x() << ", " << center->y() << ", " << center->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Reference vector: " << refVector->x() << ", " << refVector->y() << ", " << refVector->z() << "\n";
	ss << "Radius: " << radius << "\n";
	ss << "Start angle: " << startAngle << "\n";
	ss << "End angle: " << endAngle << "\n";

	arcInfo += ss.str();
	return arcInfo;
}

std::string Circle3DGeometryInfo(Ptr<Circle3D> circGeom)
{
	std::string circleInfo;
	if (!circGeom)
		return circleInfo;

	Ptr<Point3D> center;
	Ptr<Vector3D> axis;
	double radius = 0.0;
	bool result = circGeom->getData(center, axis, radius);
	if (!result)
		return circleInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Center: " << center->x() << ", " << center->y() << ", " << center->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Radius: " << radius << "\n";

	circleInfo += ss.str();
	return circleInfo;
}

std::string Ellipse3DGeometryInfo(Ptr<Ellipse3D> circGeom)
{
	std::string ellipse3DInfo;
	if (!circGeom)
		return ellipse3DInfo;

	Ptr<Point3D> center;
	Ptr<Vector3D> normal;
	Ptr<Vector3D> majorAxis;
	double majorRadius, minorRadius;
	bool result = circGeom->getData(center, normal, majorAxis, majorRadius, minorRadius);
	if (!result)
		return ellipse3DInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Center: " << center->x() << ", " << center->y() << ", " << center->z() << "\n";
	ss << "Normal: " << normal->x() << ", " << normal->y() << ", " << normal->z() << "\n";
	ss << "Major Axis: " << majorAxis->x() << ", " << majorAxis->y() << ", " << majorAxis->z() << "\n";
	ss << "Major Radius: " << majorRadius << "\n";
	ss << "Minor Radius: " << minorRadius << "\n";

	ellipse3DInfo += ss.str();
	return ellipse3DInfo;
}

std::string EllipticalArc3DGeometryInfo(Ptr<EllipticalArc3D> circGeom)
{
	std::string ellipticalArc3DInfo;
	if (!circGeom)
		return ellipticalArc3DInfo;

	Ptr<Point3D> center;
	Ptr<Vector3D> normal;
	Ptr<Vector3D> majorAxis;
	double majorRadius, minorRadius, startAngle, endAngle;
	bool result = circGeom->getData(center, normal, majorAxis, majorRadius, minorRadius, startAngle, endAngle);
	if (!result)
		return ellipticalArc3DInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Center: " << center->x() << ", " << center->y() << ", " << center->z() << "\n";
	ss << "Normal: " << normal->x() << ", " << normal->y() << ", " << normal->z() << "\n";
	ss << "Major Axis: " << majorAxis->x() << ", " << majorAxis->y() << ", " << majorAxis->z() << "\n";
	ss << "Major Radius: " << majorRadius << "\n";
	ss << "Minor Radius: " << minorRadius << "\n";
	ss << "Start Angle: " << startAngle << "\n";
	ss << "End Angle: " << endAngle << "\n";

	ellipticalArc3DInfo += ss.str();
	return ellipticalArc3DInfo;
}

std::string InfiniteLine3DGeometryInfo(Ptr<InfiniteLine3D> circGeom)
{
	std::string infiniteLine3DInfo;
	if (!circGeom)
		return infiniteLine3DInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> direction;
	bool result = circGeom->getData(origin, direction);
	if (!result)
		return infiniteLine3DInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Direction: " << direction->x() << ", " << direction->y() << ", " << direction->z() << "\n";

	infiniteLine3DInfo += ss.str();
	return infiniteLine3DInfo;
}

std::string Line3DGeometryInfo(Ptr<Line3D> circGeom)
{
	std::string line3DInfo;
	if (!circGeom)
		return line3DInfo;

	Ptr<Point3D> startPoint;
	Ptr<Point3D> endPoint;
	bool result = circGeom->getData(startPoint, endPoint);
	if (!result)
		return line3DInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Start Point: " << startPoint->x() << ", " << startPoint->y() << ", " << startPoint->z() << "\n";
	ss << "End Point: " << endPoint->x() << ", " << endPoint->y() << ", " << endPoint->z() << "\n";

	line3DInfo += ss.str();
	return line3DInfo;
}

std::string NurbsCurve3DGeometryInfo(Ptr<NurbsCurve3D> circGeom)
{
	std::string nurbsCurve3DInfo;
	if (!circGeom)
		return nurbsCurve3DInfo;

	std::vector<Ptr<Point3D>> controlPoints;
	int degree;
	std::vector<double> knots;
	bool isRational;
	std::vector<double> weights;
	bool isPeriodic;
	bool result = circGeom->getData(controlPoints, degree, knots, isRational, weights, isPeriodic);
	if (!result)
		return nurbsCurve3DInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	for (Ptr<Point3D> & controlPoint : controlPoints) {
		ss << "Control Point: " << controlPoint->x() << ", " << controlPoint->y() << ", " << controlPoint->z() << "\n";
	}
	ss << "Degree: " << degree << "\n";
	for (double & knot : knots) {
		ss << "Knot: " << knot << "\n";
	}
	ss << "isRational: " << isRational << "\n";
	for (double & weight : weights) {
		ss << "Weight: " << weight << "\n";
	}
	ss << "isPeriodic: " << isPeriodic << "\n";

	nurbsCurve3DInfo += ss.str();
	return nurbsCurve3DInfo;
}

void BRepEdgesGeometryInfo(Ptr<UserInterface> ui, Ptr<BRepEdges> edges) {
	size_t count = edges->count();
	for (size_t j = 0; j < count; j++) {

		Ptr<Curve3D> someCurve = edges->item(j)->geometry();

		if (Ptr<Arc3D> arcGeom = someCurve) {
			std::string arc3DInfo = Arc3DGeometryInfo(arcGeom);
			ui->messageBox(arc3DInfo, "Arc3D Info");
		}
		else if (Ptr<Circle3D> circGeom = someCurve) {
			std::string circle3DInfo = Circle3DGeometryInfo(circGeom);
			ui->messageBox(circle3DInfo, "Circle3D Info");
		}
		else if (Ptr<Ellipse3D> circGeom = someCurve) {
			std::string ellipse3DInfo = Ellipse3DGeometryInfo(circGeom);
			ui->messageBox(ellipse3DInfo, "Ellipse3D Info");
		}
		else if (Ptr<EllipticalArc3D> circGeom = someCurve) {
			std::string ellipticalArc3DInfo = EllipticalArc3DGeometryInfo(circGeom);
			ui->messageBox(ellipticalArc3DInfo, "EllipticalArc3D Info");
		}
		else if (Ptr<InfiniteLine3D> circGeom = someCurve) {
			std::string infiniteLine3DInfo = InfiniteLine3DGeometryInfo(circGeom);
			ui->messageBox(infiniteLine3DInfo, "InfiniteLine3D Info");
		}
		else if (Ptr<Line3D> circGeom = someCurve) {
			std::string line3DInfo = Line3DGeometryInfo(circGeom);
			ui->messageBox(line3DInfo, "Line3D Info");
		}
		else if (Ptr<NurbsCurve3D> circGeom = someCurve) {
			std::string nurbsCurve3DInfo = NurbsCurve3DGeometryInfo(circGeom);
			ui->messageBox(nurbsCurve3DInfo, "NurbsCurve3D Info");
		}

	}
}

std::string ConeGeometryInfo(Ptr<Cone> coneGeom)
{
	std::string coneInfo;
	if (!coneGeom)
		return coneInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> axis;
	double radius = 0.0;
	double halfAngle = 0.0;
	bool result = coneGeom->getData(origin, axis, radius, halfAngle);
	if (!result)
		return coneInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Radius: " << radius << "\n";
	ss << "Half Angle: " << halfAngle << "\n";

	coneInfo += ss.str();
	return coneInfo;
}

std::string CylinderGeometryInfo(Ptr<Cylinder> cylinderGeom)
{
	std::string cylinderInfo;
	if (!cylinderGeom)
		return cylinderInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> axis;
	double radius = 0.0;
	bool result = cylinderGeom->getData(origin, axis, radius);
	if (!result)
		return cylinderInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Radius: " << radius << "\n";

	cylinderInfo += ss.str();
	return cylinderInfo;
}

std::string EllipticalConeGeometryInfo(Ptr<EllipticalCone> ellipticalConeGeom)
{
	std::string ellipticalConeInfo;
	if (!ellipticalConeGeom)
		return ellipticalConeInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> axis;
	Ptr<Vector3D> majorAxisDirection;
	double majorRadius, minorRadius, halfAngle;
	bool result = ellipticalConeGeom->getData(origin, axis, majorAxisDirection, majorRadius, minorRadius, halfAngle);
	if (!result)
		return ellipticalConeInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Major Axis Direction: " << majorAxisDirection->x() << ", " << majorAxisDirection->y() << ", " << majorAxisDirection->z() << "\n";
	ss << "Major Radius: " << majorRadius << "\n";
	ss << "Minor Radius: " << minorRadius << "\n";
	ss << "Half Radius: " << halfAngle << "\n";

	ellipticalConeInfo += ss.str();
	return ellipticalConeInfo;
}

std::string EllipticalCylinderGeometryInfo(Ptr<EllipticalCylinder> ellipticalCylinderGeom)
{
	std::string ellipticalCylinderInfo;
	if (!ellipticalCylinderGeom)
		return ellipticalCylinderInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> axis;
	Ptr<Vector3D> majorAxis;
	double majorRadius, minorRadius;
	bool result = ellipticalCylinderGeom->getData(origin, axis, majorAxis, majorRadius, minorRadius);
	if (!result)
		return ellipticalCylinderInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Major Axis: " << majorAxis->x() << ", " << majorAxis->y() << ", " << majorAxis->z() << "\n";
	ss << "Major Radius: " << majorRadius << "\n";
	ss << "Minor Radius: " << minorRadius << "\n";

	ellipticalCylinderInfo += ss.str();
	return ellipticalCylinderInfo;
}

std::string NurbsSurfaceGeometryInfo(Ptr<NurbsSurface> nurbsSurfaceGeom)
{
	std::string nurbsSurfaceInfo;
	if (!nurbsSurfaceGeom)
		return nurbsSurfaceInfo;

	
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
		return nurbsSurfaceInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Degree U: " << degreeU << "\n";
	ss << "Degree V: " << degreeV << "\n";
	ss << "Control Point Count U: " << controlPointCountU << "\n";
	ss << "Control Point Count U: " << controlPointCountV << "\n";
	for (Ptr<Point3D> & controlPoint : controlPoints) {
		ss << "Control Point: " << controlPoint->x() << ", " << controlPoint->y() << ", " << controlPoint->z() << "\n";
	}
	for (double & knot : knotsU) {
		ss << "Knot U: " << knot << "\n";
	}
	for (double & knot : knotsV) {
		ss << "Knot V: " << knot << "\n";
	}
	for (double & weight : weights) {
		ss << "Weight: " << weight << "\n";
	}

	nurbsSurfaceInfo += ss.str();
	return nurbsSurfaceInfo;
}

std::string PlaneGeometryInfo(Ptr<Plane> planeGeom)
{
	std::string planeInfo;
	if (!planeGeom)
		return planeInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "U Direction: " << planeGeom->uDirection() << "\n";
	ss << "V Direction: " << planeGeom->vDirection() << "\n";

	planeInfo += ss.str();
	return planeInfo;
}

std::string SphereGeometryInfo(Ptr<Sphere> sphereGeom)
{
	std::string sphereInfo;
	if (!sphereGeom)
		return sphereInfo;

	Ptr<Point3D> origin;
	double radius = 0.0;
	bool result = sphereGeom->getData(origin, radius);
	if (!result)
		return sphereInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Radius: " << radius << "\n";

	sphereInfo += ss.str();
	return sphereInfo;
}

std::string TorusGeometryInfo(Ptr<Torus> torusGeom)
{
	std::string torusInfo;
	if (!torusGeom)
		return torusInfo;

	Ptr<Point3D> origin;
	Ptr<Vector3D> axis;
	double majorRadius, minorRadius;
	bool result = torusGeom->getData(origin, axis, majorRadius, minorRadius);
	if (!result)
		return torusInfo;

	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Origin: " << origin->x() << ", " << origin->y() << ", " << origin->z() << "\n";
	ss << "Axis: " << axis->x() << ", " << axis->y() << ", " << axis->z() << "\n";
	ss << "Major Radius: " << majorRadius << "\n";
	ss << "Minor Radius: " << minorRadius << "\n";

	torusInfo += ss.str();
	return torusInfo;
}

void BRepFacesGeometryInfo(Ptr<UserInterface> ui, Ptr<BRepFaces> faces) {
	size_t count = faces->count();
	for (size_t j = 0; j < count; j++) {

		Ptr<Surface> someFace = faces->item(j)->geometry();

		if (Ptr<Cone> coneGeom = someFace) {
			std::string coneInfo = ConeGeometryInfo(coneGeom);
			ui->messageBox(coneInfo, "Cone Info");
		}
		else if (Ptr<Cylinder> cylinderGeom = someFace) {
			std::string cylinderInfo = CylinderGeometryInfo(cylinderGeom);
			ui->messageBox(cylinderInfo, "Cylinder Info");
		}
		else if (Ptr<EllipticalCone> ellipticalConeGeom = someFace) {
			std::string ellipticalConeInfo = EllipticalConeGeometryInfo(ellipticalConeGeom);
			ui->messageBox(ellipticalConeInfo, "EllipticalCone Info");
		}
		else if (Ptr<EllipticalCylinder> ellipticalCylinderGeom = someFace) {
			std::string ellipticalCylinderInfo = EllipticalCylinderGeometryInfo(ellipticalCylinderGeom);
			ui->messageBox(ellipticalCylinderInfo, "EllipticalCylinder Info");
		}
		else if (Ptr<NurbsSurface> nurbsSurfaceGeom = someFace) {
			std::string nurbsSurfaceInfo = NurbsSurfaceGeometryInfo(nurbsSurfaceGeom);
			ui->messageBox(nurbsSurfaceInfo, "NurbsSurface Info");
		}
		else if (Ptr<Plane> planeGeom = someFace) {
			std::string planeInfo = PlaneGeometryInfo(planeGeom);
			ui->messageBox(planeInfo, "Plane Info");
		}
		else if (Ptr<Sphere> sphereGeom = someFace) {
			std::string sphereInfo = SphereGeometryInfo(sphereGeom);
			ui->messageBox(sphereInfo, "Sphere Info");
		}
		else if (Ptr<Torus> torusGeom = someFace) {
			std::string torusInfo = TorusGeometryInfo(torusGeom);
			ui->messageBox(torusInfo, "Torus Info");
		}

	}
}

void BRepBodyGeometryInfo(Ptr<UserInterface> ui, Ptr<BRepBody> body) {


	Ptr<BRepFaces> faces = body->faces();
	BRepFacesGeometryInfo(ui, faces);

	Ptr<BRepEdges> edges = body->edges();
	BRepEdgesGeometryInfo(ui, edges);

}
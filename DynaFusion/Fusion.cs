using Autodesk.DesignScript.Geometry;
using Autodesk.DesignScript.Runtime;
using System.Collections.Generic;
using System.Linq;

using FusionManagedWrapper;

// This is the Dynamo-Fusion node library for Fusion specific nodes in Dynamo
namespace DynaFusion
{
    public static class Fusion
    {
        [IsVisibleInDynamoLibrary(false)]
        public static IEnumerable<Geometry> SelectEntity()
        {
            var geometries = new List<Geometry>();
            var entities = FusionEntity.getSelectedEntities();
            foreach(var entity in entities)
            {
                geometries.AddRange(ToDynamoGeometry(entity));
            }
            return geometries;
        }

        public static IEnumerable<FusionEntity> ImportGeometry(IEnumerable<Geometry> geometries)
        {
            var entities = new List<FusionEntity>();
            foreach (var geometry in geometries)
            {
                var curve = geometry as Curve;
                if (curve != null)
                {
                    entities.Add(ToFusionCurve(curve));
                }
            }
            return entities;
        }

        private static FusionCurve ToFusionCurve(Curve curve)
        {
            var cv = curve as Circle;
            if(cv != null)
            {
                var point = cv.CenterPoint;
                return FusionCurve.createCircle(point.X, point.Y, point.Z, cv.Radius);
            }
            return null;
        }

        private static IEnumerable<Geometry> ToDynamoGeometry(FusionEntity entity)
        {
            var geometries = new List<Geometry>();
            var fusionSolid = entity as FusionSolid;
            if(fusionSolid != null)
            {
                geometries.AddRange(fusionSolid.Decompose());
            }
            return geometries;
        }
    }
}

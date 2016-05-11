using Autodesk.DesignScript.Geometry;
using Autodesk.DesignScript.Runtime;
using Dynamo.Graph.Nodes;
using ProtoCore.AST.AssociativeAST;
using System;
using System.Collections.Generic;

namespace DynamoFusionNodeLib
{
    [NodeName("Fusion.SelectEntity")]
    [IsDesignScriptCompatible]
    public class SelectEntityNode : NodeModel
    {
        public SelectEntityNode()
        {
            OutPortData.Add(new PortData("Geometry", DynamoFusionNodeLib.Properties.Resource.SelectionEntityNodePortDataOutputToolTip));

            RegisterAllPorts();
        }

        [IsVisibleInDynamoLibrary(false)]
        public override IEnumerable<AssociativeNode> BuildOutputAst(
            List<AssociativeNode> inputAstNodes)
        {
            var args = new List<AssociativeNode>();
            var funcNode = AstFactory.BuildFunctionCall(
                new Func<IEnumerable<Geometry>>(DynaFusion.Fusion.SelectEntity), args);

            return new[] {
                AstFactory.BuildAssignment(
                    GetAstIdentifierForOutputIndex(0), funcNode) };
        }
    }
}

/*
   Simple STEP PCB Export demo

   T.W. 2015
*/

#include <cstdio>
#include <string>
#include <vector>
#include <map>

#include <boost/foreach.hpp>

#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>

#include <STEPControl_Writer.hxx>

#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Cut.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>

#include <Interface_Static.hxx>
#include <Handle_StepData_StepModel.hxx>
#include <StepData_StepModel.hxx>
#include <Handle_StepRepr_Representation.hxx>
#include <StepRepr_Representation.hxx>
#include <StepShape_ShapeRepresentation.hxx>

#include <Handle_Transfer_TransientProcess.hxx>
#include <Transfer_TransientProcess.hxx>
#include <Transfer_TransientMapper.hxx>
#include <XSControl_WorkSession.hxx>
#include <XSControl_TransferReader.hxx>
#include <XSControl_TransferWriter.hxx>

#include <Transfer_TransientProcess.hxx>
#include <TransferBRep.hxx>
#include <Transfer_FinderProcess.hxx>
#include <TransferBRep_ShapeMapper.hxx>

#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <TDocStd_Document.hxx>
#include <TDataStd_Name.hxx>

#include <StepShape_ShapeDefinitionRepresentation.hxx>
#include <TransferBRep_ShapeMapper.hxx>
#include <StepShape_ShapeDefinitionRepresentation.hxx>
#include <StepRepr_PropertyDefinition.hxx>
#include <StepBasic_ProductDefinition.hxx>
#include <StepBasic_Product.hxx>
#include <StepBasic_ProductDefinitionFormation.hxx>

#include <Quantity_Color.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Handle_XCAFDoc_ColorTool.hxx>


class Board {


private:

	double m_thick;
	std::vector <TopoDS_Edge> edges;
	std::vector <TopoDS_Shape> holes;
	std::map <std::string, TopoDS_Shape> footprints;
	std::vector <TopoDS_Shape> components;

	struct ShapeName
	{
		TopoDS_Shape sh;
		std::string name;
	};

	std::vector<ShapeName> shapeNames;
	std::vector<std::string> designators;

public:

	void setShapeName( const TopoDS_Shape& sh, std::string name )
	{
		BOOST_FOREACH(ShapeName n, shapeNames)
		{
			if(n.sh == sh)
			{
				n.name = name;
				return;
			}
		}

		ShapeName n;
		n.sh = sh;
		n.name = name;
		shapeNames.push_back(n);
	}

	const std::string findShapeName ( const TopoDS_Shape& sh )
	{
		BOOST_FOREACH(ShapeName n, shapeNames)
		{
			if(n.sh == sh)
			{
				return n.name;
			}
		}
		return "<unknown>";
	}

	void setThickness (double t)
	{
		m_thick = t;
	}

	void addSegment(double x1, double y1, double x2, double y2)
	{
		TopoDS_Edge e = BRepBuilderAPI_MakeEdge( gp_Pnt (x1, y1, 0.0), gp_Pnt(x2, y2, 0.0 ) );
		edges.push_back ( e );
	}

	void addRoundHole (double x, double y, double r)
	{
		TopoDS_Shape s = BRepPrimAPI_MakeCylinder(r, m_thick * 3).Shape();
		gp_Trsf xlate;
		xlate. SetTranslation( gp_Vec( x, y, -m_thick ) );
 	  	BRepBuilderAPI_Transform xform (s, xlate);
		holes.push_back(xform.Shape());
	}

	const TopoDS_Shape addFootprint ( const std::string fileName, const std::string footprintName )
	{
		if (footprints.find(footprintName) != footprints.end())
			return footprints[footprintName]; // already loaded

		Interface_Static::SetIVal("read.stepcaf.subshapes.name",1);


		STEPCAFControl_Reader reader;
		reader.SetNameMode(true);
	    reader.SetColorMode(true);
	    reader.SetMatMode(true);

		reader.ReadFile(fileName.c_str());
		reader.ChangeReader().TransferRoots();
		TopoDS_Shape sh = reader.Reader().OneShape();
		printf("loading '%s', shapes: %d\n", fileName.c_str(), reader.Reader().NbShapes());

		footprints[footprintName] = sh;
		setShapeName ( sh, footprintName );
		return sh;
	}

	void addComponent ( const std::string footprintName, const std::string& designator, double x, double y, double z)
	{
		gp_Trsf xform;
		xform.SetTranslation ( gp_Vec( x, y, z ) );
		TopLoc_Location location ( xform );

		TopoDS_Shape shape = footprints[footprintName].Located(location);
		components.push_back ( shape );
		designators.push_back (designator);
		//setShapeName ( shape, designator );
	}


#if 1

	void stepSetShapeName(STEPCAFControl_Writer *myWriter, const TopoDS_Shape& shape, std::string name, std::string id)
	{
		Handle(XSControl_WorkSession) WS = myWriter->Writer().WS();
		Handle(Interface_InterfaceModel) Model = WS->Model();
   		Handle(XSControl_TransferWriter) TW = WS->TransferWriter();
   		Handle(Transfer_FinderProcess) FP = TW->FinderProcess();
		Handle(StepShape_ShapeDefinitionRepresentation) SDR;
    	Handle(TransferBRep_ShapeMapper) mapper = TransferBRep::ShapeMapper ( FP, shape );
    	if ( ! FP->FindTypedTransient ( mapper, STANDARD_TYPE(StepShape_ShapeDefinitionRepresentation), SDR ) ) {
      		cout << "Warning: Cannot find SDR for " << shape.TShape()->DynamicType()->Name() << endl;
      		return;
    	}

	    // set the name to the PRODUCT
	    Handle(StepRepr_PropertyDefinition) PropD = SDR->Definition().PropertyDefinition();
	    if ( PropD.IsNull() ) {printf("propd-null\n");}
	    Handle(StepBasic_ProductDefinition) PD = PropD->Definition().ProductDefinition();
	    if ( PD.IsNull() ) {printf("pd-null\n");}
	    Handle(StepBasic_Product) Prod = PD->Formation()->OfProduct();


	    Handle(TCollection_HAsciiString) n1 = new TCollection_HAsciiString (name.c_str());
	    Handle(TCollection_HAsciiString) i1 = new TCollection_HAsciiString (id.c_str());

		Prod->SetName(n1);
		Prod->SetId(i1);

	}

#endif

	void buildHierarchy(Handle(TDocStd_Document)& aDoc)
	{
	    // Create label and add our shape
	    Handle (XCAFDoc_ShapeTool) myShapeTool =
	           XCAFDoc_DocumentTool::ShapeTool(aDoc->Main());


		BRepBuilderAPI_MakeWire wiremaker;

		for(int i = 0; i< edges.size(); i++)
			wiremaker.Add(edges[i]);

	    BRep_Builder builder;

 	  	TopoDS_Compound compoundShape;

 	  	builder.MakeCompound(compoundShape);

 	  // put together all holes
		for(int i = 0; i< holes.size(); i++)
		{
			builder.Add(compoundShape, holes[i]);
		}

	// make a planar face with the board outline
		TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace( wiremaker );

	// extrude it to make the board
		TopoDS_Shape board_body = BRepPrimAPI_MakePrism(myFaceProfile,  gp_Vec(0, 0, m_thick));

		// cut the holes in the PCB body
		board_body = BRepAlgoAPI_Cut(board_body, compoundShape);//, holes[i]);

		TDF_Label bodyLabel = myShapeTool->NewShape();
		myShapeTool->SetShape (bodyLabel, board_body);

		std::vector<TDF_Label> comps;

		for(int i = 0; i< components.size(); i++)
		{

			TDF_Label lab2 = myShapeTool->NewShape();
			myShapeTool->SetShape(lab2, components[i]);

			TDF_Label lab1 = myShapeTool->NewShape();

			TopLoc_Location loc;

			TDF_Label compLabel = myShapeTool->AddComponent (lab1, lab2, loc);
			TDataStd_Name::Set(lab1, designators[i].c_str());


			comps.push_back(lab1);
		}

		TDF_Label topLabel = myShapeTool->NewShape();
		TDataStd_Name::Set(topLabel, "Kicad-PCB");

		TopLoc_Location loc;
		TDataStd_Name::Set(bodyLabel, "PCB-Body");

		Quantity_Color green(0,0.4,0, Quantity_TOC_RGB);


		myShapeTool->AddComponent (topLabel, bodyLabel, loc);

		XCAFDoc_DocumentTool::ColorTool (topLabel)->AddColor(green);
		XCAFDoc_DocumentTool::ColorTool (topLabel)->SetColor(bodyLabel, green, XCAFDoc_ColorGen);

		for(int i = 0; i< comps.size(); i++)
		{
				myShapeTool->AddComponent (topLabel, comps[i], loc);
		}
	}

	void write(const std::string& filename)
	{


	    Handle(TDocStd_Document) aDoc;
	    Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
	    anApp->NewDocument("MDTV-XCAF",aDoc);


	    buildHierarchy( aDoc );


		Interface_Static::SetIVal("write.step.assembly",1);
		Interface_Static::SetIVal("write.step.schema",4);
		Interface_Static::SetIVal("write.stepcaf.subshapes.name",1);


    // Write as STEP file
	    printf("Writing '%s'\n", filename.c_str());
	    STEPCAFControl_Writer *myWriter = new STEPCAFControl_Writer();
	    myWriter->SetNameMode(true);
	    myWriter->SetColorMode(true);
		Handle(XSControl_WorkSession) WS = myWriter->Writer().WS();
	    myWriter->Transfer( aDoc, STEPControl_AsIs );


		for(int i = 0; i< shapeNames.size(); i++)
		{
			stepSetShapeName(myWriter, shapeNames[i].sh, shapeNames[i].name, shapeNames[i].name);

		}


	#if 0
		for(int i = 0; i< components.size(); i++)
		{
			char buf[1024];
			sprintf(buf,"comp%d", i+1);

			stepSetShapeName(myWriter, components[i], buf,"component");
		}
		#endif


	    myWriter->Write(filename.c_str());

	}

};

main()
{


	Board out;
	out.setThickness(5.0);

	out.addSegment(0, 0, 100, 0);
	out.addSegment(100, 0, 100, 100);
	out.addSegment(100, 100, 50, 150);
	out.addSegment(50, 150, 0, 100);
	out.addSegment(0, 100, 0, 0);

	for(int x = 70; x < 90; x+=5)
	for(int y = 30; y < 60; y+=5)
		{
			out.addRoundHole(x, y, 2);
		}

	for(int x = 20; x < 50; x+=5)
	for(int y = 5; y < 60; y+=5)
	{
			out.addRoundHole(x, y, 2);
		}

	out.addFootprint("chip.stp", "CHIP");
	out.addFootprint("res.stp", "RESISTOR");

	out.addComponent("CHIP", "IC1", 10, 10, 5);
	out.addComponent("CHIP", "IC2", 10, 30, 5);
	out.addComponent("RESISTOR", "R1", 10, 90, 5);
	out.addComponent("RESISTOR", "R2", 20, 90, 5);
	out.addComponent("RESISTOR", "R3", 30, 90, 5);

	out.write("output.stp");

	return 0;
}


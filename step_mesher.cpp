/* 
   Simple STEP/IGES File Mesher 

   Based on OpenCascade VRML conversion code, subject to OpenCascade license.

   T.W. 2013
*/

#include <cstdio>
#include <string>
#include <vector>

#include <AIS_Shape.hxx>

#include <IGESControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <IGESCAFControl_Reader.hxx>

#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>

#include <XCAFApp_Application.hxx>
#include <Handle_XCAFApp_Application.hxx>

#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <Handle_XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include <TDocStd_Document.hxx>

#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp_Explorer.hxx>

#include <Quantity_Color.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_PolygonOnTriangulation.hxx>
#include <Precision.hxx>

#include <TDF_LabelSequence.hxx>
#include <TDF_ChildIterator.hxx>

#include "step_mesher.h"
#include "s3d_model.h"

OCC_STEP_MESHER::OCC_STEP_MESHER() :
     m_scale(1.0),
     m_deflectionFactor (0.01),
     m_deflectionAngle (20.*3.141/180.)
{

}

OCC_STEP_MESHER::~OCC_STEP_MESHER()
{

}

Quantity_Color OCC_STEP_MESHER::queryColor(const TopoDS_Shape& shape)
{
     Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(m_doc->Main());
     Quantity_Color color;
     TDF_Label L;

     color.SetValues(0.5,0.5,0.5, Quantity_TOC_RGB);
     if ( ! colorTool->ShapeTool()->Search ( shape, L ) )
          return color;
  
     if(colorTool->GetColor(shape, XCAFDoc_ColorGen, color))
          return color;  
     else if(colorTool->GetColor(shape, XCAFDoc_ColorCurv, color))
          return color;
     else if(colorTool->GetColor(shape, XCAFDoc_ColorSurf, color))
          return color;
    
    return color;
}

int OCC_STEP_MESHER::Load( const std::string& aFileName, FormatType fmt )
{
     XSControl_Reader reader;

     m_app = XCAFApp_Application::GetApplication();
     m_app -> NewDocument("MDTV-XCAF", m_doc);

	if(fmt == FMT_IGES)
     {
          IGESCAFControl_Reader igesReader;
          igesReader.ReadFile( (Standard_CString) aFileName.c_str() );
          igesReader.SetColorMode(true);
          igesReader.SetNameMode(true);
          igesReader.SetLayerMode(true);
          if ( !igesReader.Transfer( m_doc ) )
          {
               m_doc->Close();
               return -1;
          }
          reader = igesReader;//.Reader();
     } else if (fmt == FMT_STEP) {
          STEPCAFControl_Reader stepReader;
          stepReader.ReadFile((Standard_CString) aFileName.c_str() );

          stepReader.SetColorMode(true);
          stepReader.SetNameMode(true);
          stepReader.SetLayerMode(true);

          if ( !stepReader.Transfer( m_doc ) )
          {
               m_doc->Close();
               return -1;
          }     

          reader = stepReader.Reader();
     } else
      return -1;
          
      m_model = new S3D_MODEL();

     for(int shape_id = 1; shape_id <= reader.NbShapes(); shape_id++ )
     {
          TopoDS_Shape shape = reader.Shape( shape_id );
          
          if ( shape.IsNull() )
               continue;

          meshShape (shape);
     }
     return 0;
}



void OCC_STEP_MESHER::meshShape ( TopoDS_Shape& theShape )
{
  
  Handle(XCAFDoc_ColorTool) colorTool = XCAFDoc_DocumentTool::ColorTool(m_doc->Main());
  Handle(XCAFDoc_ShapeTool) shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_doc->Main());

  TDF_Label top_label = shapeTool->FindShape ( theShape );


   if(top_label.IsNull() != Standard_True && top_label.HasChild())
   {
     TDF_ChildIterator it;
     
      for(it.Initialize(top_label); it.More(); it.Next())
      {
        TopExp_Explorer explorer (shapeTool->GetShape (it.Value()), TopAbs_FACE);
        for (; explorer.More(); explorer.Next()) 
        {
          const TopoDS_Shape& shape = explorer.Current();

            if (shape.ShapeType() == TopAbs_FACE) 
            {
        	 Quantity_Color localColor = queryColor(shapeTool->GetShape (it.Value())); 

              S3D_FACE *my_face = new S3D_FACE;
              my_face->SetColor ( S3D_COLOR ( localColor.Red(), localColor.Green(), localColor.Blue() ) );

              if (processFace ( TopoDS::Face (shape), my_face ))
                m_model->AddFace(my_face);
              else
                delete my_face;
            }
          }
        }
    } else {

        TopExp_Explorer explorer (theShape, TopAbs_FACE);
        for (; explorer.More(); explorer.Next()) 
        {
          const TopoDS_Shape& shape = explorer.Current();

            if (shape.ShapeType() == TopAbs_FACE) 
            {
              S3D_FACE *my_face = new S3D_FACE;
              my_face->SetColor ( S3D_COLOR ( 0.5, 0.5, 0.5 ) );

              if (processFace ( TopoDS::Face (shape), my_face ))
                m_model->AddFace(my_face);
              else
                delete my_face;
            }
          }
    

    }
}

bool OCC_STEP_MESHER::processFace( const TopoDS_Face& face, S3D_FACE *meshData )
{
  TopLoc_Location loc;
  const Standard_Real deflection = m_deflectionFactor < 0.0001 ? 0.0001 : m_deflectionFactor;
  Standard_Boolean isTessellate (Standard_False);

  if(face.IsNull() == Standard_True)   
    return false;

  Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation (face, loc);
  if (triangulation.IsNull())
    isTessellate = Standard_True;
  // Check the existing deflection
  else if (triangulation->Deflection() > deflection + Precision::Confusion())
    isTessellate = Standard_True;
  
  if (isTessellate) 
  {
    BRepMesh_IncrementalMesh IM (face, deflection, Standard_False, m_deflectionAngle);
    triangulation = BRep_Tool::Triangulation (face, loc);
  }

  if (triangulation.IsNull() == Standard_True)
    return false;
  
  const TColgp_Array1OfPnt&    arrPolyNodes = triangulation->Nodes();
  const Poly_Array1OfTriangle& arrTriangles = triangulation->Triangles();
  Standard_Boolean isReverse = (face.Orientation() == TopAbs_REVERSED);

  for(int i = 0; i < triangulation->NbNodes(); i++)
  {
    gp_XYZ v (arrPolyNodes(i+1).Coord());
    meshData->AddVertex( S3D_VERTEX ( v.X(), v.Y(), v.Z() ) );
  }

  for(int i = 0; i < triangulation->NbTriangles(); i++)
  {
    int a, b, c;
    arrTriangles(i+1).Get(a, b, c);
    a--;
    if(isReverse)
    {
      int tmp = b - 1;
      b = c - 1;
      c = tmp;
    } else {
      b--;
      c--;
    }
    meshData->AddTriangle (a, b, c);
  }
  return true;
}

S3D_MODEL *LoadStepModel( const std::string& filename )
{
  OCC_STEP_MESHER mesher;
  if (mesher.Load(filename, OCC_STEP_MESHER::FMT_STEP) < 0)
    return NULL;
  return mesher.GetModel();
}

S3D_MODEL *LoadIgesModel( const std::string& filename )
{
  OCC_STEP_MESHER mesher;
  if (mesher.Load(filename, OCC_STEP_MESHER::FMT_IGES) < 0)
    return NULL;
  return mesher.GetModel();
}
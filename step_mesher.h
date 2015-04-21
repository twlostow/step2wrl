/* 
   Simple STEP/IGES File Mesher 

   Based on OpenCascade VRML conversion code, subject to OpenCascade license.

   T.W. 2013
*/

#ifndef OCC_STEP_MESHER_H
#define OCC_STEP_MESHER_H

#include <TDocStd_Document.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <Quantity_Color.hxx>

class S3D_MODEL;
class S3D_FACE;

class VrmlData_Scene;
class TopoDS_Shape;
class TopoDS_Face;
class Handle_Poly_Triangulation;
class Handle_Poly_Polygon3D;
class Handle_VrmlData_Coordinate;
/**
 * Algorithm converting one shape or a set of shapes to VrmlData_Scene.
 */

class OCC_STEP_MESHER
{
 public:

  enum FormatType {
    FMT_STEP = 0,
    FMT_IGES = 1
  };

  OCC_STEP_MESHER();
  ~OCC_STEP_MESHER();

  int Load( const std::string& aFileName, FormatType fmt );

  S3D_MODEL *GetModel()
  {
    return m_model;
  }

 private:
  void meshShape ( TopoDS_Shape& theShape );
  bool processFace ( const TopoDS_Face& face, S3D_FACE *meshData );
          
  Quantity_Color queryColor(const TopoDS_Shape& shape);

/*  void triToIndexedFaceSet(
    const Handle_Poly_Triangulation&,
    const TopoDS_Face& );*/

  //VrmlData_Scene                        m_scene;
  Standard_Real                         m_scale;
  Standard_Real                         m_deflectionAngle;
  Standard_Real                         m_deflectionFactor;
  Handle(TDocStd_Document)              m_doc;
  Handle(XCAFApp_Application)           m_app;

  S3D_MODEL*                            m_model;
};

#endif




OCC_DIR=/usr/local/oce
OCC_LIB = -lPTKernel \
-lTKAdvTools \
-lTKBin \
-lTKBinL \
-lTKBinTObj \
-lTKBinXCAF \
-lTKBO \
-lTKBool \
-lTKBRep \
-lTKCAF \
-lTKCDF \
-lTKernel \
-lTKFeat \
-lTKFillet \
-lTKG2d \
-lTKG3d \
-lTKGeomAlgo \
-lTKGeomBase \
-lTKHLR \
-lTKIGES \
-lTKLCAF \
-lTKMath \
-lTKMesh \
-lTKMeshVS \
-lTKNIS \
-lTKOffset \
-lTKPCAF \
-lTKPLCAF \
-lTKPrim \
-lTKPShape \
-lTKShapeSchema \
-lTKShHealing \
-lTKStdLSchema \
-lTKStdSchema \
-lTKSTEP209 \
-lTKSTEP \
-lTKSTEPAttr \
-lTKSTEPBase \
-lTKSTL \
-lTKTObj \
-lTKTopAlgo \
-lTKVoxel \
-lTKVRML \
-lTKXCAF \
-lTKXCAFSchema \
-lTKXDEIGES \
-lTKXDESTEP \
-lTKXMesh \
-lTKXml \
-lTKXmlL \
-lTKXmlTObj \
-lTKXmlXCAF \
-lTKXSBase \
-lTKService \
-lTKOpenGl \
-lTKV3d

OBJS = step_mesher.o s3d_model.o vrml_writer.o
CXXFLAGS = -I$(OCC_DIR)/include/oce -DLIN -DLININTEL -DHAVE_CONFIG_H -DHAVE_IOSTREAM -DHAVE_FSTREAM -DHAVE_LIMITS_H -I. -g $(WX_CFLAGS)
LDFLAGS =   $(WX_LIBS) -L$(OCC_DIR)/lib $(OCC_LIB) $(OCC_LIB) $(OCC_LIB)  $(OCC_LIB)  -lX11 -ldl -lfreetype -lpthread -lGL

all:	$(OBJS)
			g++ -o step2wrl $(OBJS) $(LDFLAGS)

clean:
			rm -f step2wrl $(OBJS)


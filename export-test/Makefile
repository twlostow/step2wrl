OCE_LIB_DIR=/usr/local/oce/lib


OCE_LIBS=$(OCE_LIB_DIR)/libFWOSPlugin.so \
$(OCE_LIB_DIR)/libPTKernel.so \
$(OCE_LIB_DIR)/libTKBinL.so \
$(OCE_LIB_DIR)/libTKBin.so \
$(OCE_LIB_DIR)/libTKBinTObj.so \
$(OCE_LIB_DIR)/libTKBinXCAF.so \
$(OCE_LIB_DIR)/libTKBool.so \
$(OCE_LIB_DIR)/libTKBO.so \
$(OCE_LIB_DIR)/libTKBRep.so \
$(OCE_LIB_DIR)/libTKCAF.so \
$(OCE_LIB_DIR)/libTKCDF.so \
$(OCE_LIB_DIR)/libTKernel.so \
$(OCE_LIB_DIR)/libTKFeat.so \
$(OCE_LIB_DIR)/libTKFillet.so \
$(OCE_LIB_DIR)/libTKG2d.so \
$(OCE_LIB_DIR)/libTKG3d.so \
$(OCE_LIB_DIR)/libTKGeomAlgo.so \
$(OCE_LIB_DIR)/libTKGeomBase.so \
$(OCE_LIB_DIR)/libTKHLR.so \
$(OCE_LIB_DIR)/libTKIGES.so \
$(OCE_LIB_DIR)/libTKLCAF.so \
$(OCE_LIB_DIR)/libTKMath.so \
$(OCE_LIB_DIR)/libTKMesh.so \
$(OCE_LIB_DIR)/libTKMeshVS.so \
$(OCE_LIB_DIR)/libTKNIS.so \
$(OCE_LIB_DIR)/libTKOffset.so \
$(OCE_LIB_DIR)/libTKOpenGl.so \
$(OCE_LIB_DIR)/libTKPCAF.so \
$(OCE_LIB_DIR)/libTKPLCAF.so \
$(OCE_LIB_DIR)/libTKPrim.so \
$(OCE_LIB_DIR)/libTKPShape.so \
$(OCE_LIB_DIR)/libTKService.so \
$(OCE_LIB_DIR)/libTKShapeSchema.so \
$(OCE_LIB_DIR)/libTKShHealing.so \
$(OCE_LIB_DIR)/libTKStdLSchema.so \
$(OCE_LIB_DIR)/libTKStdSchema.so \
$(OCE_LIB_DIR)/libTKSTEP209.so \
$(OCE_LIB_DIR)/libTKSTEPAttr.so \
$(OCE_LIB_DIR)/libTKSTEPBase.so \
$(OCE_LIB_DIR)/libTKSTEP.so \
$(OCE_LIB_DIR)/libTKSTL.so \
$(OCE_LIB_DIR)/libTKTObj.so \
$(OCE_LIB_DIR)/libTKTopAlgo.so \
$(OCE_LIB_DIR)/libTKV3d.so \
$(OCE_LIB_DIR)/libTKVoxel.so \
$(OCE_LIB_DIR)/libTKVRML.so \
$(OCE_LIB_DIR)/libTKXCAFSchema.so \
$(OCE_LIB_DIR)/libTKXCAF.so \
$(OCE_LIB_DIR)/libTKXDEIGES.so \
$(OCE_LIB_DIR)/libTKXDESTEP.so \
$(OCE_LIB_DIR)/libTKXMesh.so \
$(OCE_LIB_DIR)/libTKXmlL.so \
$(OCE_LIB_DIR)/libTKXml.so \
$(OCE_LIB_DIR)/libTKXmlTObj.so \
$(OCE_LIB_DIR)/libTKXmlXCAF.so \
$(OCE_LIB_DIR)/libTKXSBase.so



OBJS = step-export-demo.o
CXXFLAGS = -I/usr/local/oce/include/oce -DLIN -DLININTEL -DHAVE_CONFIG_H -DHAVE_IOSTREAM -DHAVE_FSTREAM -DHAVE_LIMITS_H -I. -g
LDFLAGS =  -L/usr/local/oce/lib $(OCE_LIBS) $(OCE_LIBS) $(OCE_LIBS) $(OCE_LIBS) -g -lpthread -lX11 -lfreetype -ldl -lGL

all:	$(OBJS)
			g++ -o step-export-demo $(OBJS) $(LDFLAGS)

clean:
			rm -f step-export-demo $(OBJS)


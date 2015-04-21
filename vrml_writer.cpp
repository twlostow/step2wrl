/* Really crappy VRML writer. Public domain. */

#include <cstdio>
#include <string>
#include "s3d_model.h"

// this is crap, but works ;)

int writeVRML ( const std::string &fname, S3D_MODEL *mdl )
{
    FILE *f = fopen(fname.c_str(), "wb");

    if(!f)
    	return -1;
    fprintf (f, "#VRML V2.0 utf8\n#Exported from step2vrml v0.0.1\nDEF body Transform {\n children [\n");

    for(int i = 0; i<mdl->FaceCount(); i++)
    {
	S3D_FACE *face = mdl->Face(i);
	S3D_MATERIAL m = face->Material();

	fprintf(f,"Shape {\n appearance Appearance {\nmaterial DEF base Material {\n");
	fprintf(f,"diffuseColor %.5f %.5f %.5f \n", m.diffuse.r,m.diffuse.g,m.diffuse.b);
	fprintf(f,"specularColor %.5f %.5f %.5f \n", m.specular.r,m.specular.g,m.specular.b);
	fprintf(f,"shininess %.5f\n", m.shininess);
	fprintf(f,"}\n}\n");

	fprintf(f,"geometry IndexedFaceSet {\nnormalPerVertex FALSE\ncolorPerVertex FALSE\ncoord Coordinate { point [\n");

	for(int j = 0; j<face->VertexCount(); j++)
	{
	    fprintf(f,"%.20f %.20f %.20f", face->Vertex(j).x, face->Vertex(j).y, face->Vertex(j).z );
	    if(j != face->VertexCount() - 1)
	    fprintf(f,",\n");
    	}
        fprintf(f,"] }\n");
	fprintf(f,"coordIndex [\n");

	for(int j = 0; j<face->TriCount(); j++)
	{
	    fprintf(f,"%d, %d, %d, -1", face->Tri(j).v[0], face->Tri(j).v[1], face->Tri(j).v[2] );
	    if(j != face->TriCount() - 1)
	    fprintf(f,",\n");
	
	}
	fprintf(f,"]\n");
        fprintf(f,"}\n}");
	if(i!=mdl->FaceCount()-1)
	    fprintf(f,",\n");
    }

    fprintf(f,"]\n}\n");
    fclose(f);

    return 0;

}

S3D_MODEL *LoadStepModel( const std::string& filename );

main(int argc, char *argv[])
{
    if(argc < 3)
    {
	fprintf(stderr,"usage: %s input.step output.wrl\n", argv[0]);
	return 0;
    }

    S3D_MODEL *mdl = LoadStepModel( argv[1] );

    mdl->Scale(1.0 / 2.54);

 	if(!mdl)
 	{
 		fprintf(stderr,"Error parsing STEP file.\n");
 		return -1;
 	}
 	if(   writeVRML(argv[2], mdl) < 0)
 	{
 		fprintf(stderr,"Error writing VRML file.\n");
 		return -1;
 	}
    delete mdl;
}
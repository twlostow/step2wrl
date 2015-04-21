#ifndef __S3D_MODEL_H
#define __S3D_MODEL_H

#include <cmath>
#include <cstdio>
#include <vector>

struct S3D_VERTEX {
	S3D_VERTEX(float _x=0, float _y=0, float _z=0): x(_x), y(_y), z(_z) {};
	float x, y, z;
};

struct S3D_COLOR {
	S3D_COLOR(float _r = 0, float _g = 0, float _b = 0, float _a=1.0):
		r(_r), g(_g), b(_b), a(_a) {};
	
	float r, g, b, a;
};

struct S3D_TRIANGLE {
	int v[3];
};

struct S3D_MATERIAL {
	S3D_MATERIAL();

	S3D_COLOR diffuse, specular;
	float shininess;
};

class S3D_FACE {
public:

	S3D_FACE(int aVertices = 0, int aTris = 0)
	{
		m_vertices.reserve (aVertices);
		m_tris.reserve(aTris);
		m_material = NULL;
	}

	~S3D_FACE();

	void AddVertex ( const S3D_VERTEX &v ) 
	{
		m_vertices.push_back(v);
	}

	void AddTriangle ( int a, int b, int c )
	{
		S3D_TRIANGLE t;
		t.v[0] = a;
		t.v[1] = b;
		t.v[2] = c;
		m_tris.push_back(t);
	}

	void SetMaterial ( S3D_MATERIAL * aMat )
	{
		m_material = aMat;
	}

	void SetColor( S3D_COLOR color ) {
		if(!m_material)
		{
			m_material = new S3D_MATERIAL;
			m_material->diffuse = color; // lame
		}
	}

	void BuildGLGeometry();

	S3D_VERTEX& Vertex(int aIndex) { return m_vertices[aIndex]; }
	S3D_TRIANGLE& Tri(int aIndex) { return m_tris[aIndex]; }

	S3D_MATERIAL& Material() { return *m_material; }
	
	int VertexCount() { return m_vertices.size(); }
	int TriCount() { return m_tris.size(); }

	void Scale ( float scale );
	void Rotate ( float x, float y, float z);

private:
	void calcNormals();

	std::vector<S3D_VERTEX> m_vertices;
	std::vector<S3D_VERTEX> m_normals;
	std::vector<S3D_TRIANGLE> m_tris;

	S3D_MATERIAL *m_material;
};

class S3D_MODEL 
{
public:
	S3D_MODEL();
	~S3D_MODEL();

	void BuildGLGeometry();
	void AddFace ( S3D_FACE *face );

	void Render();
	int FaceCount() { return m_faces.size(); }
	S3D_FACE *Face(int i) { return m_faces[i]; }

	void Scale ( float scale );
	void Rotate ( float x, float y, float z);

private:

	std::vector<S3D_FACE *> m_faces;
	unsigned int m_glHandle;
};

#endif
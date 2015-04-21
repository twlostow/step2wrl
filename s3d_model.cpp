#include "s3d_model.h"

#include <GL/gl.h>

S3D_MODEL::S3D_MODEL()
{
	m_glHandle = 0;
}

S3D_MODEL::~S3D_MODEL()
{
}


void S3D_FACE::calcNormals()
{
	m_normals.clear();
	for(int i = 0; i < m_vertices.size(); i++)
		m_normals.push_back(S3D_VERTEX());

	for(int i = 0; i < m_tris.size(); i++)
	{
		const S3D_TRIANGLE& t = m_tris[i];
		const S3D_VERTEX& a = m_vertices[t.v[0]]; 
		const S3D_VERTEX& b = m_vertices[t.v[1]]; 
		const S3D_VERTEX& c = m_vertices[t.v[2]]; 

		S3D_VERTEX v1 (a.x - b.x, a.y - b.y, a.z - b.z);
		S3D_VERTEX v2 (c.x - b.x, c.y - b.y, c.z - b.z);
		
		S3D_VERTEX n (	v1.y * v2.z - v1.z * v2.y,
						v1.z * v2.x - v1.x * v2.z, 
						v1.x * v2.y - v1.y * v2.x);

		for(int j = 0; j < 3; j ++)
		{
			m_normals[t.v[j]].x += n.x; 
			m_normals[t.v[j]].y += n.y; 
			m_normals[t.v[j]].z += n.z; 
		}
	}

	for(int i = 0; i < m_normals.size(); i++)
	{
		S3D_VERTEX n = m_normals[i];
		
		float len = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);

		if(len > 0.0)
		{
			n.x /= len;
			n.y /= len;
			n.z /= len;
		}
		m_normals[i] = n;
	}
}

void S3D_MODEL::AddFace ( S3D_FACE *face )
{
	m_faces.push_back(face);
}

S3D_FACE::~S3D_FACE()
{

}


S3D_MATERIAL::S3D_MATERIAL()
{
	specular = S3D_COLOR(1.0, 1.0, 1.0, 1.0);
	shininess = 1.0;
}

void S3D_FACE::Scale(float scale)
{
	for(int i = 0; i < m_vertices.size(); i++)
	{
	    m_vertices[i].x *= scale;
	    m_vertices[i].y *= scale;
	    m_vertices[i].z *= scale;
	}
    
}

void S3D_FACE::Rotate(float x, float y, float z)
{

}


void S3D_MODEL::Scale(float scale)
{
	for(int i = 0; i < m_faces.size(); i++)
	{
	    m_faces[i]->Scale(scale);
	}
    
}

void S3D_MODEL::Rotate(float x, float y, float z)
{

}

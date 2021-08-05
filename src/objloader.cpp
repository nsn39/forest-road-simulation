#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "../headers/objloader.hpp"


bool import_obj_mesh( 
	const std::string & pFile,
	int & no_meshes,
	all_vertices & out_vertices,
	all_UVs & out_uvs,
	all_normals & out_normals,
	std::vector<std::string> & name_list
) {
	// Create an instance of the Importer class
	Assimp::Importer importer;

	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile( pFile,
		aiProcess_CalcTangentSpace       |
		aiProcess_Triangulate            |
		aiProcess_FlipUVs				 |
		aiProcess_SortByPType);

	// If the import failed, report it
	if( !scene) {
		printf("Couldn't load the scene\n");
		return false;
	}	

	int size = scene->mNumMeshes;
	no_meshes = size;
	//printf("The no of meshes: %d\n", size);

	int tsize = scene->mNumTextures;
	//printf("The no of textures: %d\n", tsize);

	aiMesh** meshes = scene->mMeshes;
	

	//Iterate for each mesh and add it to our data structure.
	for(int i=0; i<size; i++)
	{
		aiMesh* temp = meshes[i];
		//unsigned int x = (temp->GetNumUVChannels());
		unsigned int y = temp->mNumVertices;
		
		//char* mesh_name = const_cast<char*>(temp->mName.C_Str());
		std::string m_name = std::string(temp->mName.C_Str());
		name_list.push_back(m_name);
		
		//printf("%d Vertices %d object\n", temp->mNumVertices, i);

		std::vector<glm::vec3> mesh_i_vertices;
		std::vector<glm::vec2> mesh_i_uvs;
		std::vector<glm::vec3> mesh_i_normals;

		//For vertices
		aiVector3D* vertices = temp->mVertices;
		for(unsigned int j=0; j<y; j++)
		{
			aiVector3D point = vertices[j];
			glm::vec3 mesh_i_vertex_j;
			mesh_i_vertex_j.x = point.x;
			mesh_i_vertex_j.y = point.y;
			mesh_i_vertex_j.z = point.z;
			mesh_i_vertices.push_back(mesh_i_vertex_j);
			//printf("Length: %lf\n", point.Length());

			//printf("%f %f %f\n", point.x, point.y, point.z);
		}

		//For UVs
		aiVector3D* uvs = temp->mTextureCoords[0];
		for(unsigned int j=0; j<y; j++)
		{
			aiVector3D point = uvs[j];

			glm::vec2 mesh_i_uv_j;
			mesh_i_uv_j.x = point.x;
			mesh_i_uv_j.y = point.y;
			mesh_i_uvs.push_back(mesh_i_uv_j);
		}

		//For normals
		aiVector3D* normals = temp->mNormals;
		for(unsigned int j=0; j<y; j++)
		{
			aiVector3D point = normals[j];

			glm::vec3 mesh_i_normal_j;
			mesh_i_normal_j.x = point.x;
			mesh_i_normal_j.y = point.y;
			mesh_i_normal_j.z = point.z;
			mesh_i_normals.push_back(mesh_i_normal_j);
		}

		out_vertices.push_back(mesh_i_vertices);
		out_uvs.push_back(mesh_i_uvs);
		out_normals.push_back(mesh_i_normals);
	}

	// We're done. Everything will be cleaned up by the importer destructor
	return true;
}
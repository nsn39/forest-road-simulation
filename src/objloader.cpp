#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "../headers/objloader.hpp"

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices; 
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	
	}
	fclose(file);
	return true;
}

bool DoTheImportThing( 
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
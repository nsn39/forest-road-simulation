#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


typedef std::vector<std::vector<glm::vec3>> all_vertices;
typedef std::vector<std::vector<glm::vec2>> all_UVs;
typedef std::vector<std::vector<glm::vec3>> all_normals;

bool loadOBJ(
	const char * path, 
	std::vector<glm::vec3> & out_vertices, 
	std::vector<glm::vec2> & out_uvs, 
	std::vector<glm::vec3> & out_normals
);

bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
);

bool DoTheImportThing( 
	const std::string & pFile,
	int & no_meshes,
	all_vertices & out_vertices,
	all_UVs & out_uvs,
	all_normals & out_normals
);

#endif

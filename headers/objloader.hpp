#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


typedef std::vector<std::vector<glm::vec3>> all_vertices;
typedef std::vector<std::vector<glm::vec2>> all_UVs;
typedef std::vector<std::vector<glm::vec3>> all_normals;

bool import_obj_mesh( 
	const std::string & pFile,
	int & no_meshes,
	all_vertices & out_vertices,
	all_UVs & out_uvs,
	all_normals & out_normals,
	std::vector<std::string> & name_list
);

#endif

#include "SceneImporter.h"

#include <Assimp\Importer.hpp>
#include <Assimp\scene.h>
#include <Assimp\postprocess.h>

bool ImportScene(std::string const & file, std::vector<Vertex>& outVerices, std::vector<uint32_t>& outIndices)
{
	// what a name a importer named importer
	// bet noone saw that one coming
	Assimp::Importer importer;

	// wow assimp half your stuff is in a namespace and the other half is prefixed with ai
	uint32_t flags = aiPostProcessSteps::aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;

	// as we all know by now. i have no creativity with names
	aiScene const* scene = importer.ReadFile(file, flags);

	if(!scene)
		return false;

	// clean up on isle Vertex
	outVerices.clear();
	outIndices.clear();

	// one shot, one mesh
	for(uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes ; ++meshIndex)
	{ 
		// what did i tell you ZERO creativity
		aiMesh* mesh = scene->mMeshes[meshIndex];

		uint32_t indexOffset = static_cast<uint32_t >(outVerices.size());

		for(uint32_t vIndex = 0; vIndex < mesh->mNumVertices; ++vIndex)
		{
			Vertex v; // this might have other data at some point
			aiVector3D aiPos = mesh->mVertices[vIndex];

			v.pos.x = aiPos.x;
			v.pos.y = aiPos.y;
			v.pos.z = aiPos.z;
			v.pos.w = 1;

			// You got to push
			outVerices.push_back(v);
		}

		for(uint32_t fIndex = 0; fIndex < mesh->mNumFaces; ++fIndex)
		{
			// what did you expect at this point
			aiFace face = mesh->mFaces[fIndex];

			// better be 3, but i trust this code... i am so fucked
			outIndices.push_back(face.mIndices[0] + indexOffset);
			outIndices.push_back(face.mIndices[1] + indexOffset);
			outIndices.push_back(face.mIndices[2] + indexOffset);
		}
	}

	importer.FreeScene();
	// shh i know the code is slow up there

	return true;
}

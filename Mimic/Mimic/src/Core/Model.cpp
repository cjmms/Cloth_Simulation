#include "Model.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "../ResourceManager.h"
#include <assimp/pbrmaterial.h>




Model::Model(const char* path)
{
    loadModel(path);
}

// iterate all meshes, draw all meshes
void Model::Draw(Shader& shader) const
{
	for (auto mesh : meshes) mesh.Draw(shader);
}




void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate );

    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    std::cout << "Loading model, model address: " << path << std::endl;
    double before = glfwGetTime();
    processNode(scene->mRootNode, scene);
    std::cout << "Model loading complete, loading time: " <<  glfwGetTime() - before << "s " << std::endl;
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


void Model::setVec3(glm::vec3& des, aiVector3D& source) const
{
    des.x = source.x;
    des.y = source.y;
    des.z = source.z;
}


std::vector<Vertex> Model::processVertices(aiMesh* mesh) const
{
    std::vector<Vertex> vertices;
    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // positions
        setVec3(vertex.Position, mesh->mVertices[i]);

        // normals
        if (mesh->HasNormals())
            setVec3(vertex.Normal, mesh->mNormals[i]);

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;

            // tangent
            //setVec3(vertex.Tangent, mesh->mTangents[i]);

            // bitangent
            //setVec3(vertex.Bitangent, mesh->mBitangents[i]);
        }
        else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    return vertices;
}


std::vector<unsigned int> Model::processIndices(aiMesh* mesh) const
{
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    return indices;
}



/*
* Assimp will bind "keyword" in .mtl file with tis own aiTextureType
* For example: 
* map_Kd albedo.jpg in .mtl file 
* assimp will treat albedo.jpg a texture with aiTextureType_DIFFUSE 
* 
* Unfortunately, the matching I find online are incorrect between keyword and aiTextureType
* 
* After many testings these matchings have been validated:
* map_Kd aiTextureType_DIFFUSE
* map_Bump aiTextureType_HEIGHT
* map_Ka aiTextureType_AMBIENT
* map_Ns aiTextureType_SHININESS
*/
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
    std::vector<Vertex> vertices = processVertices(mesh);
    std::vector<unsigned int> indices = processIndices(mesh);

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


    std::vector<Texture> textures;
    // mapped to map_Kd in .mtl
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_albedo");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // mapped to map_Bump in .mtl
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // mapped to map_Ka in .mtl
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_metallic");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // mapped to map_Ns in .mtl
    std::vector<Texture> tMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_roughness");
    textures.insert(textures.end(), tMaps.begin(), tMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, textures, indices);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) 
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = ResourceManager::TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            std::cout << "Loading " << texture.path << " as " << typeName << std::endl;
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}
#include "HeightMapTerrainGenerator.hpp"

Model HeightMapTerrainGenerator::generateTerrain() {
    Image image = GenImagePerlinNoise(512, 512, 0, 0, 2);
    Texture2D texture = LoadTextureFromImage(image);

    Mesh mesh = GenMeshHeightmap(image, Vector3 {128, 32, 128});
    Model model = LoadModelFromMesh(mesh);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    UnloadImage(image);

    return model;
}

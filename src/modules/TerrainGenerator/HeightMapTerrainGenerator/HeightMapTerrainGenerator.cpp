#include "HeightMapTerrainGenerator.hpp"

Model HeightMapTerrainGenerator::generateTerrain() {
    Image image = LoadImage("resources/height-map.png");
    Texture2D texture = LoadTextureFromImage(image);

    Mesh mesh = GenMeshHeightmap(image, Vector3 {16, 8, 16});
    Model model = LoadModelFromMesh(mesh);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    UnloadImage(image);

    return model;
}

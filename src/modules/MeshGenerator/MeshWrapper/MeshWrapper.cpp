#include "MeshWrapper.hpp"

#include <raymath.h>

Vector3 MeshWrapper::computeNormal(
    const Vector3& point,
    const Vector3& leftNeighbor,
    const Vector3& rightNeighbor
) {
    Vector3 leftVector = Vector3Subtract(leftNeighbor, point);
    Vector3 rightVector = Vector3Subtract(rightNeighbor, point);

    return Vector3Normalize(Vector3CrossProduct(leftVector, rightVector));
}

MeshWrapper::MeshWrapper(const Vector2& resolution) {
    const uint32_t squareCount = (resolution.x - 1) * (resolution.y - 1);
    const uint32_t triangleCount = squareCount * 2;
    const uint32_t vertexCount = triangleCount * 3;

    const uint8_t coordsPerVertex = 3;
    const uint8_t coordsPerTexture = 2;
    const uint8_t colorChannelsPerVertex = 4;

    mesh_.triangleCount = triangleCount;
    mesh_.vertexCount = vertexCount;

    mesh_.vertices =
        (float*) MemAlloc(vertexCount * coordsPerVertex * sizeof(float));

    mesh_.normals =
        (float*) MemAlloc(vertexCount * coordsPerVertex * sizeof(float));

    mesh_.texcoords =
        (float*) MemAlloc(vertexCount * coordsPerTexture * sizeof(float));

    mesh_.colors = (uint8_t*) MemAlloc(
        vertexCount * colorChannelsPerVertex * sizeof(uint8_t)
    );
}

void MeshWrapper::addNormal(const Vector3 coords) {
    mesh_.normals[normalIndex + 0] = coords.x;
    mesh_.normals[normalIndex + 1] = coords.y;
    mesh_.normals[normalIndex + 2] = coords.z;

    normalIndex += 3;
}

void MeshWrapper::addColor(const Color& color) {
    mesh_.colors[colorIndex + 0] = color.r;
    mesh_.colors[colorIndex + 1] = color.g;
    mesh_.colors[colorIndex + 2] = color.b;
    mesh_.colors[colorIndex + 3] = color.a;

    colorIndex += 4;
}

void MeshWrapper::addTextureCoords(
    const Vector2& index,
    const Vector2& resolution
) {
    mesh_.texcoords[textureCoordIndex + 0] =
        (index.x / (resolution.x - 1));

    mesh_.texcoords[textureCoordIndex + 1] =
        (index.y / (resolution.y - 1));

    textureCoordIndex += 2;
}

void MeshWrapper::addVertex(const Vector3& coords) {
    mesh_.vertices[vertexIndex + 0] = coords.x;
    mesh_.vertices[vertexIndex + 1] = coords.y;
    mesh_.vertices[vertexIndex + 2] = coords.z;

    vertexIndex += 3;
}

void MeshWrapper::addTriangle(
    const Vector3& pointA,
    const Vector3& pointB,
    const Vector3& pointC
) {
    addVertex(pointA);
    addVertex(pointB);
    addVertex(pointC);
}

void MeshWrapper::addTextureTriangle(
    const Vector2& pointAIndex,
    const Vector2& pointBIndex,
    const Vector2& pointCIndex,
    const Vector2& resolution
) {
    addTextureCoords(pointAIndex, resolution);
    addTextureCoords(pointBIndex, resolution);
    addTextureCoords(pointCIndex, resolution);
}

void MeshWrapper::addLighting(
    const TerrainData& terrainData,
    const Vector3& lightPosition
) {
    // const Vector3 normalA = computeNormal(pointA, pointB, pointC);

    // addNormal(normalA);

    // const Vector3 pointALight =
    //     Vector3Normalize(Vector3Subtract(lightPosition, pointA));

    // const float dotA =
    //     Clamp(Vector3DotProduct(pointALight, normalA), 0.0f, 1.0f);

    // const uint8_t pointAIntensity = uint8_t(dotA * 255);

    // addColor({pointAIntensity, pointAIntensity, pointAIntensity, 255});
}

Mesh& MeshWrapper::getMesh() {
    return mesh_;
}
#include "MeshWrapper.hpp"

#include <raymath.h>

MeshWrapper::MeshWrapper(const Vector2& resolution) {
    const uint32_t squareCount = (resolution.x - 1) * (resolution.y - 1);
    const uint32_t triangleCount = squareCount * 2;
    const uint32_t vertexCount = triangleCount * 3;

    const uint8_t coordsPerVertex = 3;
    const uint8_t coordsPerTexture = 2;

    mesh_.triangleCount = triangleCount;
    mesh_.vertexCount = vertexCount;

    mesh_.vertices =
        (float*) MemAlloc(vertexCount * coordsPerVertex * sizeof(float));

    mesh_.normals =
        (float*) MemAlloc(vertexCount * coordsPerVertex * sizeof(float));

    mesh_.texcoords =
        (float*) MemAlloc(vertexCount * coordsPerTexture * sizeof(float));

    mesh_.colors = nullptr;
}

void MeshWrapper::addNormal(const Vector3 coords) {
    mesh_.normals[normalIndex + 0] = coords.x;
    mesh_.normals[normalIndex + 1] = coords.y;
    mesh_.normals[normalIndex + 2] = coords.z;

    normalIndex += 3;
}

void MeshWrapper::addTextureCoords(
    const Vector3& coords,
    const Vector2& resolution
) {
    mesh_.texcoords[textureCoordIndex + 0] =
        (coords.x / float(resolution.x - 1));

    mesh_.texcoords[textureCoordIndex + 1] =
        (coords.z / float(resolution.y - 1));

    textureCoordIndex += 2;
}

void MeshWrapper::addVertex(const Vector3& coords) {
    mesh_.vertices[vertexIndex + 0] = coords.x;
    mesh_.vertices[vertexIndex + 1] = coords.y;
    mesh_.vertices[vertexIndex + 2] = coords.z;

    vertexIndex += 3;
}

void MeshWrapper::addTraingle(
    const Vector3& pointA,
    const Vector3& pointB,
    const Vector3& pointC,
    const Vector2 resolution
) {
    addVertex(pointA);
    addVertex(pointB);
    addVertex(pointC);

    addTextureCoords(pointA, resolution);
    addTextureCoords(pointB, resolution);
    addTextureCoords(pointC, resolution);

    const Vector3 vectorAB = Vector3Subtract(pointB, pointA);
    const Vector3 vectorAC = Vector3Subtract(pointC, pointA);
    const Vector3 normal =
        Vector3Normalize(Vector3CrossProduct(vectorAB, vectorAC));

    addNormal(normal);
    addNormal(normal);
    addNormal(normal);
}

Mesh& MeshWrapper::getMesh() {
    return mesh_;
}
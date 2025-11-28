#include "MeshWrapper.hpp"

#include <raymath.h>

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

    const Vector3 vectorAB = Vector3Subtract(pointB, pointA);
    const Vector3 vectorAC = Vector3Subtract(pointC, pointA);
    const Vector3 normal =
        Vector3Normalize(Vector3CrossProduct(vectorAB, vectorAC));

    const Vector3 lightPosition {0, 45, 0};

    const Vector3 pointALight =
        Vector3Normalize(Vector3Subtract(lightPosition, pointA));
    const Vector3 pointBLight =
        Vector3Normalize(Vector3Subtract(lightPosition, pointB));
    const Vector3 pointCLight =
        Vector3Normalize(Vector3Subtract(lightPosition, pointC));

    const float dotA =
        Clamp(Vector3DotProduct(pointALight, normal), 0.0f, 1.0f);
    const float dotB =
        Clamp(Vector3DotProduct(pointBLight, normal), 0.0f, 1.0f);
    const float dotC =
        Clamp(Vector3DotProduct(pointCLight, normal), 0.0f, 1.0f);

    const uint8_t pointAIntensity = uint8_t(dotA * 255);
    const uint8_t pointBIntensity = uint8_t(dotB * 255);
    const uint8_t pointCIntensity = uint8_t(dotC * 255);

    addColor({pointAIntensity, pointAIntensity, pointAIntensity, 255});
    addColor({pointBIntensity, pointBIntensity, pointBIntensity, 255});
    addColor({pointCIntensity, pointCIntensity, pointCIntensity, 255});
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

Mesh& MeshWrapper::getMesh() {
    return mesh_;
}
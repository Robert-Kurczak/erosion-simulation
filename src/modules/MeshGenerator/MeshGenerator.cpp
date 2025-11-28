#include "MeshGenerator.hpp"

#include "MeshWrapper/MeshWrapper.hpp"

#include <cmath>
#include <cstring>
#include <iostream>
#include <raymath.h>

void MeshGenerator::setupVertices(
    Mesh& mesh,
    const TerrainData& data,
    const Vector3& worldSize
) {
    const uint8_t coordsPerVertex = 3;
    const uint32_t vertexCount = data.heightMap.size();

    const Vector3 scale {
        worldSize.x / (data.resolutionX - 1),
        worldSize.y,
        worldSize.z / (data.resolutionZ - 1)
    };

    const Vector3 offset {
        -float(data.resolutionX - 1) / 2.0f,
        0.0f,
        -float(data.resolutionZ - 1) / 2.0f
    };

    mesh.vertexCount = vertexCount;
    mesh.vertices =
        (float*) MemAlloc(vertexCount * coordsPerVertex * sizeof(float));

    uint32_t vertexIndex = 0;
    for (uint32_t z = 0; z < data.resolutionZ; z++) {
        for (uint32_t x = 0; x < data.resolutionX; x++) {
            mesh.vertices[vertexIndex + 0] =
                (float(x) + offset.x) * scale.x;
            mesh.vertices[vertexIndex + 1] =
                (float(data.heightAt(x, z)) + offset.y) * scale.y;
            mesh.vertices[vertexIndex + 2] =
                (float(z) + offset.z) * scale.z;

            vertexIndex += 3;
        }
    }
}

void MeshGenerator::setupTriangles(Mesh& mesh, const TerrainData& data) {
    // A-----B
    // |   / |
    // | /   |
    // C-----D
    //
    // CounterClockwise
    // A, C, B
    // B, C, D
    const uint32_t squareCount =
        (data.resolutionX - 1) * (data.resolutionZ - 1);

    const uint32_t triangleCount = squareCount * 2;

    mesh.triangleCount = triangleCount;
    mesh.indices =
        (uint16_t*) MemAlloc(triangleCount * 3 * sizeof(uint16_t));

    uint16_t triangleIndex = 0;
    for (uint32_t z = 0; z < data.resolutionZ - 1; z++) {
        for (uint32_t x = 0; x < data.resolutionX - 1; x++) {
            const uint32_t vertexAIndex = x + data.resolutionX * z;
            const uint32_t vertexBIndex = (x + 1) + data.resolutionX * z;
            const uint32_t vertexCIndex = x + data.resolutionX * (z + 1);
            const uint32_t vertexDIndex =
                (x + 1) + data.resolutionX * (z + 1);

            mesh.indices[triangleIndex + 0] = vertexAIndex;
            mesh.indices[triangleIndex + 1] = vertexCIndex;
            mesh.indices[triangleIndex + 2] = vertexBIndex;

            triangleIndex += 3;

            mesh.indices[triangleIndex + 0] = vertexBIndex;
            mesh.indices[triangleIndex + 1] = vertexCIndex;
            mesh.indices[triangleIndex + 2] = vertexDIndex;

            triangleIndex += 3;
        }
    }
}

void MeshGenerator::setupTextureCoordinates(
    Mesh& mesh,
    const TerrainData& data
) {
    const uint32_t vertexCount = data.heightMap.size();
    const uint8_t coordsPerTexture = 2;

    mesh.texcoords =
        (float*) MemAlloc(vertexCount * coordsPerTexture * sizeof(float));

    uint32_t textCoordIndex = 0;
    for (uint32_t z = 0; z < data.resolutionZ; z++) {
        for (uint32_t x = 0; x < data.resolutionX; x++) {
            const float xCoord = float(x) / float(data.resolutionX - 1);
            const float yCoord = float(z) / float(data.resolutionZ - 1);

            mesh.texcoords[textCoordIndex + 0] = xCoord;
            mesh.texcoords[textCoordIndex + 1] = yCoord;

            textCoordIndex += 2;
        }
    }
}

void MeshGenerator::setupLighting(Mesh& mesh, const TerrainData& data) {}

void MeshGenerator::updateVertices(Mesh& mesh, const TerrainData& data) {
    uint32_t vertexIndex = 0;
    for (uint32_t z = 0; z < data.resolutionZ; z++) {
        for (uint32_t x = 0; x < data.resolutionX; x++) {
            mesh.vertices[vertexIndex + 1] = data.heightAt(x, z);

            vertexIndex += 3;
        }
    }
}

void MeshGenerator::updateLighting(Mesh& mesh, const TerrainData& data) {}

Mesh MeshGenerator::generateMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    Mesh mesh {};

    setupVertices(mesh, terrainData, worldSize);
    setupTriangles(mesh, terrainData);
    setupTextureCoordinates(mesh, terrainData);
    setupLighting(mesh, terrainData);

    return mesh;
}

void MeshGenerator::updateMesh(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    // updateVertices(mesh, terrainData);
    // updateLighting(mesh, terrainData);
}
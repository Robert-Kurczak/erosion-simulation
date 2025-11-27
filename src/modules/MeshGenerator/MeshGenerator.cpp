#include "MeshGenerator.hpp"

#include "MeshWrapper/MeshWrapper.hpp"

#include <cmath>
#include <iostream>
#include <raymath.h>

Vector3 MeshGenerator::getPoint(
    const Vector2& index,
    const TerrainData& terrainData,
    const Vector3& scale,
    const Vector3& offset
) {
    const Vector3 point {
        .x = index.x * scale.x,
        .y = float(terrainData.heightAt(index) * scale.y),
        .z = index.y * scale.z
    };

    return Vector3Add(point, offset);
}

Mesh MeshGenerator::generateMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    const Vector2 resolution {
        .x = float(terrainData.resolutionX),
        .y = float(terrainData.resolutionZ)
    };

    const Vector3 scale {
        .x = worldSize.x / resolution.x,
        .y = worldSize.y,
        .z = worldSize.z / resolution.y
    };

    const Vector3 centerOffset {
        .x = -(resolution.x - 1) * scale.x / 2.0f,
        .y = 0.0f,
        .z = -(resolution.y - 1) * scale.z / 2.0f
    };

    MeshWrapper meshWrapper {resolution};

    for (uint32_t z = 0; z < terrainData.resolutionZ - 1; z++) {
        for (uint32_t x = 0; x < terrainData.resolutionX - 1; x++) {
            // 6 Vertices per quad
            // A, C, B
            // B, C, D
            //
            // A-----B
            // |   / |
            // | /   |
            // C-----D

            const Vector2 pointAIndex {.x = float(x), .y = float(z)};
            const Vector3 pointA =
                getPoint(pointAIndex, terrainData, scale, centerOffset);

            const Vector2 pointBIndex {.x = float(x) + 1, .y = float(z)};
            const Vector3 pointB =
                getPoint(pointBIndex, terrainData, scale, centerOffset);

            const Vector2 pointCIndex {.x = float(x), .y = float(z) + 1};
            const Vector3 pointC =
                getPoint(pointCIndex, terrainData, scale, centerOffset);

            const Vector2 pointDIndex {
                .x = float(x) + 1, .y = float(z) + 1
            };
            const Vector3 pointD =
                getPoint(pointDIndex, terrainData, scale, centerOffset);

            meshWrapper.addTriangle(pointA, pointC, pointB);
            meshWrapper.addTextureTriangle(
                pointAIndex, pointCIndex, pointBIndex, resolution
            );

            meshWrapper.addTriangle(pointB, pointC, pointD);
            meshWrapper.addTextureTriangle(
                pointBIndex, pointCIndex, pointDIndex, resolution
            );
        }
    }

    return meshWrapper.getMesh();
}

void MeshGenerator::updateMesh(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    const Vector3 scale {.x = 1, .y = worldSize.y, .z = 1};
    const Vector3 offset {.x = 0, .y = 0, .z = 0};

    uint32_t vertexIndex = 0;
    for (uint32_t z = 0; z < terrainData.resolutionZ - 1; z++) {
        for (uint32_t x = 0; x < terrainData.resolutionX - 1; x++) {
            // 6 Vertices per quad
            // A, C, B
            // B, C, D
            //
            // A-----B
            // |   / |
            // | /   |
            // C-----D

            const Vector2 pointAIndex {.x = float(x), .y = float(z)};
            const Vector3 pointA =
                getPoint(pointAIndex, terrainData, scale, offset);

            const Vector2 pointBIndex {.x = float(x) + 1, .y = float(z)};
            const Vector3 pointB =
                getPoint(pointBIndex, terrainData, scale, offset);

            const Vector2 pointCIndex {.x = float(x), .y = float(z) + 1};
            const Vector3 pointC =
                getPoint(pointCIndex, terrainData, scale, offset);

            const Vector2 pointDIndex {
                .x = float(x) + 1, .y = float(z) + 1
            };
            const Vector3 pointD =
                getPoint(pointDIndex, terrainData, scale, offset);

            mesh.vertices[vertexIndex + 1] = pointA.y;
            vertexIndex += 3;

            mesh.vertices[vertexIndex + 1] = pointC.y;
            vertexIndex += 3;

            mesh.vertices[vertexIndex + 1] = pointB.y;
            vertexIndex += 3;

            mesh.vertices[vertexIndex + 1] = pointB.y;
            vertexIndex += 3;

            mesh.vertices[vertexIndex + 1] = pointC.y;
            vertexIndex += 3;

            mesh.vertices[vertexIndex + 1] = pointD.y;
            vertexIndex += 3;
        }
    }
}
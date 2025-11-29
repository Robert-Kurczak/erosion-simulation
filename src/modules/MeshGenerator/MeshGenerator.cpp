#include "MeshGenerator.hpp"

#include <raymath.h>

uint8_t MeshGenerator::calculateLightIntensity(
    uint32_t xIndex,
    uint32_t zIndex,
    const TerrainData& data,
    const Vector3& worldSize,
    const Vector3& lightPosition,
    const Vector3& point
) {
    const uint32_t leftIndex = xIndex == 0 ? 0 : xIndex - 1;
    const uint32_t rightIndex =
        xIndex == data.resolutionX - 1 ? xIndex : xIndex + 1;
    const uint32_t upIndex =
        zIndex == data.resolutionZ - 1 ? zIndex : zIndex + 1;
    const uint32_t downIndex = zIndex == 0 ? 0 : zIndex - 1;

    const float heightLeft = data.heightAt(leftIndex, zIndex);
    const float heightRight = data.heightAt(rightIndex, zIndex);
    const float heightUp = data.heightAt(xIndex, upIndex);
    const float heightDown = data.heightAt(xIndex, downIndex);

    const Vector3 scale {
        worldSize.x / float(data.resolutionX - 1),
        worldSize.y,
        worldSize.z / float(data.resolutionZ - 1)
    };

    const Vector3 xDifference = {
        2.0f * scale.x, (heightRight - heightLeft) * scale.y, 0
    };

    const Vector3 zDifference = {
        0, (heightUp - heightDown) * scale.y, 2.0f * scale.z
    };

    const Vector3 pointNormal =
        Vector3Normalize(Vector3CrossProduct(zDifference, xDifference));

    const Vector3 lightVector =
        Vector3Normalize(Vector3Subtract(lightPosition, point));

    const float dotProduct =
        Clamp(Vector3DotProduct(lightVector, pointNormal), 0.0f, 1.0f);

    return uint8_t(dotProduct * 255);
}

void MeshGenerator::setupMesh(
    Mesh& mesh,
    const TerrainData& data,
    uint32_t totalQuadsNumber
) {
    mesh.vertexCount = totalQuadsNumber * VERTICES_PER_QUAD;
    mesh.vertices = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_VERTEX * sizeof(float)
    );

    mesh.texcoords = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_TEXTURE * sizeof(float)
    );

    mesh.colors = (uint8_t*) MemAlloc(
        mesh.vertexCount * CHANNELS_PER_COLOR * sizeof(uint8_t)
    );
}

void MeshGenerator::addTriangles(
    Mesh& mesh,
    const TerrainData& data,
    const Vector3& worldSize,
    uint32_t startQuad,
    uint32_t endQuad
) {
    // A-----B
    // |   / |
    // | /   |
    // C-----D
    //
    // CounterClockwise
    // A, C, B
    // B, C, D

    const Vector3 scale {
        worldSize.x / float(data.resolutionX - 1),
        worldSize.y,
        worldSize.z / float(data.resolutionZ - 1)
    };

    const Vector3 offset {
        -(float(data.resolutionX) - 1) / 2.0f,
        0.0f,
        -(float(data.resolutionZ) - 1) / 2.0f
    };

    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t vertexIndex =
        startQuad * VERTICES_PER_QUAD * COORDS_PER_VERTEX;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // C
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;

        // B
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // B
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] = (float(z) + offset.z) * scale.z;

        vertexIndex += 3;

        // C
        mesh.vertices[vertexIndex + 0] = (float(x) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;

        // D
        mesh.vertices[vertexIndex + 0] =
            (float(x + 1) + offset.x) * scale.x;
        mesh.vertices[vertexIndex + 1] =
            (float(data.heightAt(x + 1, z + 1)) + offset.y) * scale.y;
        mesh.vertices[vertexIndex + 2] =
            (float(z + 1) + offset.z) * scale.z;

        vertexIndex += 3;
    }
}

void MeshGenerator::addTextureCoords(
    Mesh& mesh,
    const TerrainData& data,
    uint32_t startQuad,
    uint32_t endQuad
) {
    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t coordIndex =
        startQuad * VERTICES_PER_QUAD * COORDS_PER_TEXTURE;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // C
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // B
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // B
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // C
        mesh.texcoords[coordIndex + 0] =
            float(x) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;

        // D
        mesh.texcoords[coordIndex + 0] =
            float(x + 1) / float(data.resolutionX - 1);
        mesh.texcoords[coordIndex + 1] =
            float(z + 1) / float(data.resolutionZ - 1);

        coordIndex += 2;
    }
}

void MeshGenerator::addLighting(
    Mesh& mesh,
    const TerrainData& data,
    const Vector3& worldSize,
    const Vector3& lightPosition,
    uint32_t startQuad,
    uint32_t endQuad
) {
    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t colorIndex =
        startQuad * VERTICES_PER_QUAD * CHANNELS_PER_COLOR;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        uint32_t vertexIndex =
            startQuad * VERTICES_PER_QUAD * COORDS_PER_VERTEX;
        const Vector3 pointA {
            mesh.vertices[vertexIndex + 0],
            mesh.vertices[vertexIndex + 1],
            mesh.vertices[vertexIndex + 2]
        };
        const uint8_t pointAIntensity = calculateLightIntensity(
            x, z, data, worldSize, lightPosition, pointA
        );
        vertexIndex += 3;

        const Vector3 pointC {
            mesh.vertices[vertexIndex + 0],
            mesh.vertices[vertexIndex + 1],
            mesh.vertices[vertexIndex + 2]
        };
        const uint8_t pointCIntensity = calculateLightIntensity(
            x, z + 1, data, worldSize, lightPosition, pointC
        );

        vertexIndex += 3;

        const Vector3 pointB {
            mesh.vertices[vertexIndex + 0],
            mesh.vertices[vertexIndex + 1],
            mesh.vertices[vertexIndex + 2]
        };
        const uint8_t pointBIntensity = calculateLightIntensity(
            x + 1, z, data, worldSize, lightPosition, pointB
        );

        vertexIndex += 3;
        // B
        vertexIndex += 3;
        // C
        vertexIndex += 3;
        const Vector3 pointD {
            mesh.vertices[vertexIndex + 0],
            mesh.vertices[vertexIndex + 1],
            mesh.vertices[vertexIndex + 2]
        };
        const uint8_t pointDIntensity = calculateLightIntensity(
            x + 1, z + 1, data, worldSize, lightPosition, pointD
        );

        // A
        mesh.colors[colorIndex + 0] = pointAIntensity;
        mesh.colors[colorIndex + 1] = pointAIntensity;
        mesh.colors[colorIndex + 2] = pointAIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // C
        mesh.colors[colorIndex + 0] = pointCIntensity;
        mesh.colors[colorIndex + 1] = pointCIntensity;
        mesh.colors[colorIndex + 2] = pointCIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // B
        mesh.colors[colorIndex + 0] = pointBIntensity;
        mesh.colors[colorIndex + 1] = pointBIntensity;
        mesh.colors[colorIndex + 2] = pointBIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // B
        mesh.colors[colorIndex + 0] = pointBIntensity;
        mesh.colors[colorIndex + 1] = pointBIntensity;
        mesh.colors[colorIndex + 2] = pointBIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // C
        mesh.colors[colorIndex + 0] = pointCIntensity;
        mesh.colors[colorIndex + 1] = pointCIntensity;
        mesh.colors[colorIndex + 2] = pointCIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;

        // D
        mesh.colors[colorIndex + 0] = pointDIntensity;
        mesh.colors[colorIndex + 1] = pointDIntensity;
        mesh.colors[colorIndex + 2] = pointDIntensity;
        mesh.colors[colorIndex + 3] = 255;

        colorIndex += 4;
    }
}

Mesh MeshGenerator::generateMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize
) {
    Mesh mesh {};

    const uint32_t totalQuadsNumber =
        (terrainData.resolutionX - 1) * (terrainData.resolutionZ - 1);

    setupMesh(mesh, terrainData, totalQuadsNumber);

    addTriangles(mesh, terrainData, worldSize, 0, totalQuadsNumber);
    addTextureCoords(mesh, terrainData, 0, totalQuadsNumber);

    const Vector3 lightPosition {0, 100, 0};
    addLighting(
        mesh, terrainData, worldSize, lightPosition, 0, totalQuadsNumber
    );

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
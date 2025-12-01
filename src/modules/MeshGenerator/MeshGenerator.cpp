#include "MeshGenerator.hpp"

#include <algorithm>
#include <iostream>
#include <raymath.h>

inline Vector3 MeshGenerator::getWorldPoint(
    uint32_t xIndex,
    uint32_t zIndex,
    const TerrainData& data,
    const Vector3& offset,
    const Vector3& scale
) const {
    return Vector3 {
        (float(xIndex) + offset.x) * scale.x,
        float(data.heightAt(xIndex, zIndex) * scale.y),
        (float(zIndex) + offset.z) * scale.z
    };
}

inline Vector2 MeshGenerator::getTextureCoord(
    uint32_t xIndex,
    uint32_t zIndex,
    const TerrainData& data
) const {
    return Vector2 {
        float(xIndex) / float(data.resolutionX - 1),
        float(zIndex) / float(data.resolutionZ - 1)
    };
}

inline Vector3 MeshGenerator::getVertexFromQuad(
    Mesh& mesh,
    uint32_t quadIndex,
    uint32_t vertexIndexInQuad
) {
    const uint8_t vertexCoordIndexInQuad =
        vertexIndexInQuad * COORDS_PER_VERTEX_;

    const uint32_t vertexOffset =
        quadIndex * VERTICES_PER_QUAD_ * COORDS_PER_VERTEX_ +
        vertexCoordIndexInQuad;

    return Vector3 {
        mesh.vertices[vertexOffset + 0],
        mesh.vertices[vertexOffset + 1],
        mesh.vertices[vertexOffset + 2]
    };
}

inline void MeshGenerator::addVertex(
    Mesh& mesh,
    uint32_t index,
    const Vector3& coords
) {
    mesh.vertices[index + 0] = coords.x;
    mesh.vertices[index + 1] = coords.y;
    mesh.vertices[index + 2] = coords.z;
}

inline void MeshGenerator::addTextureCoord(
    Mesh& mesh,
    uint32_t index,
    const Vector2& coords
) {
    mesh.texcoords[index + 0] = coords.x;
    mesh.texcoords[index + 1] = coords.y;
}

inline void MeshGenerator::addColor(
    Mesh& mesh,
    uint32_t index,
    const Color& color
) {
    mesh.colors[index + 0] = color.r;
    mesh.colors[index + 1] = color.g;
    mesh.colors[index + 2] = color.b;
    mesh.colors[index + 3] = color.a;
}

inline Color MeshGenerator::calculateLightIntensity(
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

    const uint8_t intensity = dotProduct * 255;

    return Color {intensity, intensity, intensity, 255};
}

void MeshGenerator::setupMesh(Mesh& mesh, uint32_t totalQuadsNumber) {
    mesh.vertexCount = totalQuadsNumber * VERTICES_PER_QUAD_;
    mesh.vertices = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_VERTEX_ * sizeof(float)
    );

    mesh.texcoords = (float*) MemAlloc(
        mesh.vertexCount * COORDS_PER_TEXTURE_ * sizeof(float)
    );

    mesh.colors = (uint8_t*) MemAlloc(
        mesh.vertexCount * CHANNELS_PER_COLOR_ * sizeof(uint8_t)
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
        startQuad * VERTICES_PER_QUAD_ * COORDS_PER_VERTEX_;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        const Vector3 pointA = getWorldPoint(x, z, data, offset, scale);
        const Vector3 pointB =
            getWorldPoint(x + 1, z, data, offset, scale);
        const Vector3 pointC =
            getWorldPoint(x, z + 1, data, offset, scale);
        const Vector3 pointD =
            getWorldPoint(x + 1, z + 1, data, offset, scale);

        addVertex(mesh, vertexIndex, pointA);
        vertexIndex += COORDS_PER_VERTEX_;

        addVertex(mesh, vertexIndex, pointC);
        vertexIndex += COORDS_PER_VERTEX_;

        addVertex(mesh, vertexIndex, pointB);
        vertexIndex += COORDS_PER_VERTEX_;

        addVertex(mesh, vertexIndex, pointB);
        vertexIndex += COORDS_PER_VERTEX_;

        addVertex(mesh, vertexIndex, pointC);
        vertexIndex += COORDS_PER_VERTEX_;

        addVertex(mesh, vertexIndex, pointD);
        vertexIndex += COORDS_PER_VERTEX_;
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
        startQuad * VERTICES_PER_QUAD_ * COORDS_PER_TEXTURE_;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        const Vector2 coordsA = getTextureCoord(x, z, data);
        const Vector2 coordsB = getTextureCoord(x + 1, z, data);
        const Vector2 coordsC = getTextureCoord(x, z + 1, data);
        const Vector2 coordsD = getTextureCoord(x + 1, z + 1, data);

        addTextureCoord(mesh, coordIndex, coordsA);
        coordIndex += COORDS_PER_TEXTURE_;

        addTextureCoord(mesh, coordIndex, coordsC);
        coordIndex += COORDS_PER_TEXTURE_;

        addTextureCoord(mesh, coordIndex, coordsB);
        coordIndex += COORDS_PER_TEXTURE_;

        addTextureCoord(mesh, coordIndex, coordsB);
        coordIndex += COORDS_PER_TEXTURE_;

        addTextureCoord(mesh, coordIndex, coordsC);
        coordIndex += COORDS_PER_TEXTURE_;

        addTextureCoord(mesh, coordIndex, coordsD);
        coordIndex += COORDS_PER_TEXTURE_;
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
        startQuad * VERTICES_PER_QUAD_ * CHANNELS_PER_COLOR_;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        // A, C, B | B, C, D
        const uint8_t vertexAIndexInQuad = 0;
        const uint8_t vertexBIndexInQuad = 2;
        const uint8_t vertexCIndexInQuad = 1;
        const uint8_t vertexDIndexInQuad = 5;

        const Vector3 vertexA =
            getVertexFromQuad(mesh, quad, vertexAIndexInQuad);
        const Vector3 vertexB =
            getVertexFromQuad(mesh, quad, vertexBIndexInQuad);
        const Vector3 vertexC =
            getVertexFromQuad(mesh, quad, vertexCIndexInQuad);
        const Vector3 vertexD =
            getVertexFromQuad(mesh, quad, vertexDIndexInQuad);

        const Color vertexALight = calculateLightIntensity(
            x, z, data, worldSize, lightPosition, vertexA
        );
        const Color vertexBLight = calculateLightIntensity(
            x + 1, z, data, worldSize, lightPosition, vertexB
        );
        const Color vertexCLight = calculateLightIntensity(
            x, z + 1, data, worldSize, lightPosition, vertexC
        );
        const Color vertexDLight = calculateLightIntensity(
            x + 1, z + 1, data, worldSize, lightPosition, vertexD
        );

        addColor(mesh, colorIndex, vertexALight);
        colorIndex += CHANNELS_PER_COLOR_;

        addColor(mesh, colorIndex, vertexCLight);
        colorIndex += CHANNELS_PER_COLOR_;

        addColor(mesh, colorIndex, vertexBLight);
        colorIndex += CHANNELS_PER_COLOR_;

        addColor(mesh, colorIndex, vertexBLight);
        colorIndex += CHANNELS_PER_COLOR_;

        addColor(mesh, colorIndex, vertexCLight);
        colorIndex += CHANNELS_PER_COLOR_;

        addColor(mesh, colorIndex, vertexDLight);
        colorIndex += CHANNELS_PER_COLOR_;
    }
}

void MeshGenerator::updateTrianglesHeight(
    Mesh& mesh,
    const TerrainData& data,
    const Vector3& worldSize,
    uint32_t startQuad,
    uint32_t endQuad
) {
    const uint32_t quadsPerRow = data.resolutionX - 1;

    uint32_t vertexIndex =
        startQuad * VERTICES_PER_QUAD_ * COORDS_PER_VERTEX_;

    for (uint32_t quad = startQuad; quad < endQuad; quad++) {
        const uint32_t x = quad % quadsPerRow;
        const uint32_t z = quad / quadsPerRow;

        const float pointAHeight = data.heightAt(x, z) * worldSize.y;
        const float pointBHeight = data.heightAt(x + 1, z) * worldSize.y;
        const float pointCHeight = data.heightAt(x, z + 1) * worldSize.y;
        const float pointDHeight =
            data.heightAt(x + 1, z + 1) * worldSize.y;

        mesh.vertices[vertexIndex + 1] = pointAHeight;
        vertexIndex += COORDS_PER_VERTEX_;

        mesh.vertices[vertexIndex + 1] = pointCHeight;
        vertexIndex += COORDS_PER_VERTEX_;

        mesh.vertices[vertexIndex + 1] = pointBHeight;
        vertexIndex += COORDS_PER_VERTEX_;

        mesh.vertices[vertexIndex + 1] = pointBHeight;
        vertexIndex += COORDS_PER_VERTEX_;

        mesh.vertices[vertexIndex + 1] = pointCHeight;
        vertexIndex += COORDS_PER_VERTEX_;

        mesh.vertices[vertexIndex + 1] = pointDHeight;
        vertexIndex += COORDS_PER_VERTEX_;
    }
}

std::thread MeshGenerator::createGeneratorThread(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize,
    const Vector3& lightPosition,
    uint32_t startQuad,
    uint32_t endQuad
) {
    return std::thread {([this,
                          &mesh,
                          &terrainData,
                          &worldSize,
                          &lightPosition,
                          startQuad,
                          endQuad]() mutable {
        addTriangles(mesh, terrainData, worldSize, startQuad, endQuad);

        addTextureCoords(mesh, terrainData, startQuad, endQuad);

        addLighting(
            mesh,
            terrainData,
            worldSize,
            lightPosition,
            startQuad,
            endQuad
        );
    })};
}

std::thread MeshGenerator::createUpdaterThread(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize,
    const Vector3& lightPosition,
    uint32_t startQuad,
    uint32_t endQuad
) {
    return std::thread {([this,
                          &mesh,
                          &terrainData,
                          &worldSize,
                          &lightPosition,
                          startQuad,
                          endQuad]() mutable {
        updateTrianglesHeight(
            mesh, terrainData, worldSize, startQuad, endQuad
        );

        addLighting(
            mesh,
            terrainData,
            worldSize,
            lightPosition,
            startQuad,
            endQuad
        );
    })};
}

MeshGenerator::MeshGenerator() :
    threads_amount_(std::max(2u, std::thread::hardware_concurrency())) {
    generatorThreads_.resize(threads_amount_);
    updaterThreads_.resize(threads_amount_);

    std::cout << "Threads in Mesh Generator: " << threads_amount_ << "\n";
}

Mesh MeshGenerator::generateIlluminatedMesh(
    const TerrainData& terrainData,
    const Vector3& worldSize,
    const Vector3& lightPosition
) {
    Mesh mesh {};

    const uint32_t totalQuadsNumber =
        (terrainData.resolutionX - 1) * (terrainData.resolutionZ - 1);

    const uint32_t quadsPerThread = totalQuadsNumber / threads_amount_;

    setupMesh(mesh, totalQuadsNumber);

    for (uint8_t i = 0; i < generatorThreads_.size() - 1; i++) {
        const uint32_t startQuad = i * quadsPerThread;
        const uint32_t endQuad = startQuad + quadsPerThread;

        generatorThreads_[i] = createGeneratorThread(
            mesh,
            terrainData,
            worldSize,
            lightPosition,
            startQuad,
            endQuad
        );
    }

    const uint32_t startQuad =
        (generatorThreads_.size() - 1) * quadsPerThread;

    const uint32_t endQuad = totalQuadsNumber;

    generatorThreads_.back() = createGeneratorThread(
        mesh, terrainData, worldSize, lightPosition, startQuad, endQuad
    );

    for (std::thread& thread : generatorThreads_) {
        thread.join();
    }

    return mesh;
}

void MeshGenerator::updateIlluminatedMesh(
    Mesh& mesh,
    const TerrainData& terrainData,
    const Vector3& worldSize,
    const Vector3& lightPosition
) {
    const uint32_t totalQuadsNumber =
        (terrainData.resolutionX - 1) * (terrainData.resolutionZ - 1);

    const uint32_t quadsPerThread = totalQuadsNumber / threads_amount_;

    for (uint8_t i = 0; i < threads_amount_ - 1; i++) {
        const uint32_t startQuad = i * quadsPerThread;
        const uint32_t endQuad = startQuad + quadsPerThread;

        updaterThreads_[i] = createUpdaterThread(
            mesh,
            terrainData,
            worldSize,
            lightPosition,
            startQuad,
            endQuad
        );
    }

    const uint32_t startQuad =
        (updaterThreads_.size() - 1) * quadsPerThread;

    const uint32_t endQuad = totalQuadsNumber;

    updaterThreads_.back() = createUpdaterThread(
        mesh, terrainData, worldSize, lightPosition, startQuad, endQuad
    );

    for (std::thread& thread : updaterThreads_) {
        thread.join();
    }
}
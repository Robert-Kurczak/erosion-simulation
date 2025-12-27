#include "TerrainData.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

TerrainData::TerrainData(
    const uint32_t resolutionX,
    const uint32_t resolutionZ,
    const Vector3& worldSize,
    const Vector3& worldPosition,
    uint32_t rainDropsAmount
) :
    resolutionX_(resolutionX),
    resolutionZ_(resolutionZ),
    worldSize_(worldSize),
    worldPosition_(worldPosition),
    boundingBox_ {
        .min {-worldSize_.x / 2.0f, 0.0f,        -worldSize_.z / 2.0f},
        .max {worldSize_.x / 2.0f,  worldSize.y, worldSize_.z / 2.0f }
} {
    const uint32_t sampleSize = resolutionX_ * resolutionZ_;

    heightMap_.resize(sampleSize);
    colorMap_.resize(sampleSize);
    rainMap_.resize(rainDropsAmount);
}

uint32_t TerrainData::getResolutionX() const {
    return resolutionX_;
}

uint32_t TerrainData::getResolutionZ() const {
    return resolutionZ_;
}

const Vector3& TerrainData::getWorldSize() const {
    return worldSize_;
}

const Vector3& TerrainData::getWorldPosition() const {
    return worldPosition_;
}

const BoundingBox& TerrainData::getBoundingBox() const {
    return boundingBox_;
}

const std::vector<double>& TerrainData::getHeightMap() const {
    return heightMap_;
}

std::vector<double>& TerrainData::getHeightMap() {
    return heightMap_;
}

const std::vector<Color>& TerrainData::getColorMap() const {
    return colorMap_;
}

std::vector<Color>& TerrainData::getColorMap() {
    return colorMap_;
}

const std::vector<RainDrop>& TerrainData::getRainMap() const {
    return rainMap_;
}

std::vector<RainDrop>& TerrainData::getRainMap() {
    return rainMap_;
}

bool TerrainData::isInsideBoundingBox(const Vector2& worldPosition
) const {
    const bool withinLeftBorder = worldPosition.x >= boundingBox_.min.x;
    const bool withinRightBorder = worldPosition.x <= boundingBox_.max.x;
    const bool withinBottomBorder = worldPosition.y >= boundingBox_.min.z;
    const bool withinTopBorder = worldPosition.y <= boundingBox_.max.z;

    return withinLeftBorder && withinRightBorder && withinBottomBorder &&
           withinTopBorder;
}

Vector2 TerrainData::worldPositionToIndices(const Vector2& worldPosition
) const {
    const uint32_t x =
        (worldPosition.x / worldSize_.x + 0.5f) * (resolutionX_ - 1);
    const uint32_t z =
        (worldPosition.y / worldSize_.z + 0.5f) * (resolutionZ_ - 1);

    return Vector2 {float(x), float(z)};
}

Vector2 TerrainData::indicesToWorldPosition(uint32_t x, uint32_t z)
    const {
    return Vector2 {
        (float(x) / float(resolutionX_ - 1) - 0.5f) * worldSize_.x,
        (float(z) / float(resolutionZ_ - 1) - 0.5f) * worldSize_.z,
    };
}

double TerrainData::heightAt(uint32_t index) const {
    assert(index < resolutionX_ * resolutionZ_);

    return heightMap_[index];
}

double TerrainData::heightAt(uint32_t x, uint32_t z) const {
    assert(x <= resolutionX_ - 1 && z <= resolutionZ_ - 1);

    return heightMap_[z * resolutionX_ + x];
}

Color TerrainData::colorAt(uint32_t index) const {
    assert(index < resolutionX_ * resolutionZ_);

    return colorMap_[index];
}

Color TerrainData::colorAt(uint32_t x, uint32_t z) const {
    assert(x <= resolutionX_ - 1 && z <= resolutionZ_ - 1);

    return colorMap_[z * resolutionX_ + x];
}

Color& TerrainData::mutableColorAtWorld(const Vector2& worldPosition) {
    const Vector2 indices = worldPositionToIndices(worldPosition);
    assert(
        indices.x <= resolutionX_ - 1 && indices.y <= resolutionZ_ - 1
    );

    return colorMap_[indices.y * resolutionX_ + indices.x];
}
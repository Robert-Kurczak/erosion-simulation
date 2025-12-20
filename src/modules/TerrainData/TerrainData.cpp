#include "TerrainData.hpp"

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
    worldPosition_(worldPosition) {
    const uint32_t sampleSize = resolutionX_ * resolutionZ_;

    heightMap_.reserve(sampleSize);
    colorMap_.reserve(sampleSize);
    rainMap_.reserve(rainDropsAmount);
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

Vector2 TerrainData::worldPositionToIndices(const Vector2& worldPosition
) const {
    const uint32_t x =
        (worldPosition.x / worldSize_.x + 0.5f) * (resolutionX_ - 1);
    const uint32_t z =
        (worldPosition.y / worldSize_.z + 0.5f) * (resolutionZ_ - 1);

    assert(x <= resolutionX_ - 1);
    assert(z <= resolutionZ_ - 1);

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
    assert(index < heightMap_.size());

    return heightMap_[index];
}

double TerrainData::heightAt(uint32_t x, uint32_t z) const {
    assert(x <= resolutionX_ - 1 && z <= resolutionZ_ - 1);

    return heightMap_[z * resolutionX_ + x];
}

double TerrainData::heightAtWorld(const Vector2& worldPosition) const {
    const Vector2 indices = worldPositionToIndices(worldPosition);

    return heightMap_[indices.y * resolutionX_ + indices.x];
}

double& TerrainData::mutableHeightAtWorld(const Vector2& worldPosition) {
    const Vector2 indices = worldPositionToIndices(worldPosition);

    return heightMap_[indices.y * resolutionX_ + indices.x];
}

Color TerrainData::colorAt(uint32_t index) const {
    assert(index < colorMap_.size());

    return colorMap_[index];
}

Color TerrainData::colorAt(uint32_t x, uint32_t z) const {
    assert(x <= resolutionX_ - 1 && z <= resolutionZ_ - 1);

    return colorMap_[z * resolutionX_ + x];
}

Color TerrainData::colorAtWorld(const Vector2& worldPosition) const {
    const Vector2 indices = worldPositionToIndices(worldPosition);

    return colorMap_[indices.y * resolutionX_ + indices.x];
}

Color& TerrainData::mutableColorAtWorld(const Vector2& worldPosition) {
    const Vector2 indices = worldPositionToIndices(worldPosition);

    return colorMap_[indices.y * resolutionX_ + indices.x];
}
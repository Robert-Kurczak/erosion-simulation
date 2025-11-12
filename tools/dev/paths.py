from pathlib import Path

# === Base ===
APPLICATION_NAME = "erosion-simulator"

DEV_DIRECTORY_PATH = Path(__file__).parent
REPOSITORY_ROOT_PATH = DEV_DIRECTORY_PATH.parent.parent
DOCKERFILES_DIRECTORY_PATH = DEV_DIRECTORY_PATH.joinpath("docker")
# ===

# === Files ===
APPLICATION_BINARY_PATH = REPOSITORY_ROOT_PATH.joinpath(
    f"build/{APPLICATION_NAME}"
)
# ===

# === Dockerfiles ===
BUILDER_IMAGE_NAME = "raylib-builder"
BUILDER_DOCKERFILE_PATH = DOCKERFILES_DIRECTORY_PATH.joinpath(
    f"{BUILDER_IMAGE_NAME}.dockerfile"
)
# ===

# === Docker volumes ===
REPOSITORY_ROOT_MOUNT_PATH = Path(f"/opt/{APPLICATION_NAME}")
BUILD_FOLDER_MOUNT_PATH = REPOSITORY_ROOT_MOUNT_PATH.joinpath("build")
SOURCE_FOLDER_MOUNT_PATH = REPOSITORY_ROOT_MOUNT_PATH.joinpath("src")
# ===

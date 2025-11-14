from pathlib import Path

# === Base ===
APPLICATION_NAME = "erosion-simulator"

DEV_DIRECTORY_PATH = Path(__file__).parent
REPOSITORY_ROOT_PATH = DEV_DIRECTORY_PATH.parent.parent
# ===

# === Files ===
APPLICATION_BINARY_PATH = REPOSITORY_ROOT_PATH.joinpath(
    f"build/package/{APPLICATION_NAME}"
)
# ===

# === Dockerfiles ===
BUILDER_IMAGE_NAME = "erosion-simulator-dev"
BUILDER_DOCKERFILE_PATH = REPOSITORY_ROOT_PATH.joinpath(".devcontainer/Dockerfile")
# ===

# === Docker volumes ===
REPOSITORY_ROOT_MOUNT_PATH = Path(f"/opt/{APPLICATION_NAME}")
BUILD_FOLDER_MOUNT_PATH = REPOSITORY_ROOT_MOUNT_PATH.joinpath("build")
SOURCE_FOLDER_MOUNT_PATH = REPOSITORY_ROOT_MOUNT_PATH.joinpath("src")
# ===

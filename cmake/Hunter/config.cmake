hunter_config(
    Eigen
    VERSION 3.4.0
)

hunter_config(
    Sophus
    VERSION 1.22.10
    URL "https://github.com/luxonis/Sophus/archive/refs/heads/hunterized.zip"
    SHA1 "ff17d3dce1bcadabbc9fb83845a29974377d7f95" 
    CMAKE_ARGS
        BUILD_SOPHUS_TESTS=OFF
        BUILD_SOPHUS_EXAMPLES=OFF
)

hunter_config(
    cereal
    VERSION 1.3.0
    URL "https://github.com/USCiLab/cereal/archive/64f50dbd5cecdaba785217e2b0aeea3a4f1cdfab.zip"
    SHA1 "6587337e36045f1b5f68c902f5e04a76d8999e02"
)
hunter_config(
    Eigen
    VERSION 3.4.0
    CMAKE_ARGS
        EIGEN_DONT_PARALLELIZE=ON
        EIGEN_INITIALIZE_MATRICES_BY_NAN=ON
)

hunter_config(
    Sophus
    VERSION 1.22.10
    URL "https://github.com/luxonis/Sophus/archive/54e9b230edc4df47f819cef0d15b1fcc165342df.zip"
    SHA1 "53493ab699bf1ef5d3d8ab2892f8eaa80cf1dfc3" 
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
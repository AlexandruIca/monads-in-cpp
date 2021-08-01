add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

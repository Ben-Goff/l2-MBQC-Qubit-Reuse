cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(qubit_reuse)

add_library(circuit ${PROJECT_SOURCE_DIR}/src/qcircuit.cxx)
add_library(gate ${PROJECT_SOURCE_DIR}/src-shared/gate.cxx)
target_include_directories(circuit PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(gate PUBLIC ${PROJECT_SOURCE_DIR}/src-shared)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cxx)
target_link_libraries(${PROJECT_NAME} circuit)
target_link_libraries(${PROJECT_NAME} gate)
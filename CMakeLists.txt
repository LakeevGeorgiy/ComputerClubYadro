cmake_minimum_required(VERSION 3.12)
project(ComputerClubYadro)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(src/domain)
add_subdirectory(src/data)
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        DomainLayer
        DataLayer
)
enable_testing()
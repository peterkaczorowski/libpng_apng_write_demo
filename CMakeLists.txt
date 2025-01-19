cmake_minimum_required(VERSION 3.15)
project(APNGWriteDemo)

set(CMAKE_CXX_STANDARD 17)

# Enable export of compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Znajdź wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Znajdź libpng
find_package(PNG REQUIRED)

# Dodaj plik źródłowy
add_executable(APNGWriteDemo main.cpp)

# Połącz z bibliotekami wxWidgets i libpng
target_link_libraries(APNGWriteDemo PRIVATE ${wxWidgets_LIBRARIES} PNG::PNG)
target_include_directories(APNGWriteDemo PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_compile_definitions(APNGWriteDemo PRIVATE ${wxWidgets_DEFINITIONS})


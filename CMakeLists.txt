cmake_minimum_required(VERSION 3.15)
project(simple_xframework)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)

add_executable(simple_xframework
        src/include/Debug.h
        src/include/EventHandlers.h
        src/include/Form.h
        src/include/Object.h
        src/include/System.h
        src/include/Types.h
        src/include/Component.h
        teste.cpp
        src/Form.cpp
        src/Label.cpp
        src/include/theme/Theme.h
        src/include/theme/KDETheme.h
        src/include/theme/ThemeManager.h
        src/include/Button.h
        src/theme/ThemeManager.cpp

)

target_link_libraries(simple_xframework PRIVATE ${X11_LIBRARIES})
target_include_directories(simple_xframework PRIVATE ${X11_INCLUDE_DIRS})
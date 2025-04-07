
project(Editor)

file(GLOB_RECURSE EDITOR_APP_SRC CONFIGURE_DEPENDS src/*.cpp src/*.h)

add_executable(Editor ${EDITOR_APP_SRC})

target_include_directories(Editor PUBLIC
    src
    ../Engine/src
    ../ThirdParty/imgui
    ../ThirdParty/glm
    ../ThirdParty/GLFW/include
    ${VULKAN_SDK_INCLUDE}
)

target_link_libraries(Editor Engine)

target_compile_definitions(Editor PRIVATE WL_PLATFORM_WINDOWS)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Editor PRIVATE WL_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Editor PRIVATE WL_RELEASE)
endif()

if(WIN32)
    target_link_directories(Editor PUBLIC ${VULKAN_SDK_LIB})
    target_link_libraries(Editor vulkan-1)
endif()

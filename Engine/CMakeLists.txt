
project(Engine)

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp src/*.h)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC
    src
    ../ThirdParty/imgui
    ../ThirdParty/imgui/backends
    ../ThirdParty/glfw/include
    ../ThirdParty/stb_image
    ../ThirdParty/glm
    ../ThirdParty/spdlog
    ${VULKAN_SDK_INCLUDE}
)

target_link_libraries(Engine
    imgui
    glfw
    glm
    stb_image
)

target_compile_definitions(Engine PRIVATE WL_PLATFORM_WINDOWS)

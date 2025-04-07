
project(Engine)

file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/*.cpp src/*.h)

add_library(Engine STATIC ${ENGINE_SRC})

target_include_directories(Engine PUBLIC
    src
    ../ThirdParty/imgui
    ../ThirdParty/GLFW/include
    ../ThirdParty/stb_image
    ../ThirdParty/glm
    ${VULKAN_SDK_INCLUDE}
)

target_link_libraries(Engine
    ImGui
    GLFW
)

if(WIN32)
    target_link_directories(Engine PUBLIC ${VULKAN_SDK_LIB})
    target_link_libraries(Engine vulkan-1)
endif()

target_compile_definitions(Engine PRIVATE WL_PLATFORM_WINDOWS)

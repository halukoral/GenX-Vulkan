#include "Application.h"
#include "Log.h"

#define GLFW_INCLUDE_VULKAN
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


extern bool g_ApplicationRunning;
static Application* s_Instance = nullptr;

void check_vk_result(VkResult err)
{
	if (err == 0)
		return;

	LOG_ERROR("[vulkan] Error: VkResult = {0}", err);

	if (err < 0)
		abort();
}

static void glfw_error_callback(int error, const char* description)
{
	LOG_ERROR("GLFW Error ({0}): {1}", error, description);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Application::Application(AppSpec spec) : m_Spec(std::move(spec))
{
	s_Instance = this;
	Init();
}

Application::~Application()
{
	Shutdown();
	s_Instance = nullptr;
}

Application& Application::Get()
{
	return *s_Instance;
}

void Application::Init()
{
	// Setup GLFW window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		LOG_ERROR("Could not initialize GLFW!");
		return;
	}

	// Setup Vulkan
	if (!glfwVulkanSupported())
	{
		LOG_ERROR("GLFW: Vulkan not supported!");
		return;
	}
	
	/* set a "hint" for the NEXT window created*/
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	/* Vulkan needs no context */
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_WindowHandle = glfwCreateWindow(m_Spec.Width, m_Spec.Height, m_Spec.Name.c_str(), nullptr, nullptr);

	if (!m_WindowHandle)
	{
		LOG_ERROR("Could not create window!");
		glfwTerminate();
		return;
	}

	if (!InitVulkan())
	{
		LOG_ERROR("Could not init Vulkan!");
		glfwTerminate();
		return;
	}
}

bool Application::InitVulkan()
{
	VkResult result = VK_ERROR_UNKNOWN;

	VkApplicationInfo mAppInfo{};
	mAppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	mAppInfo.pNext = nullptr;
	mAppInfo.pApplicationName = m_Spec.Name.c_str();
	mAppInfo.applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 1);
	mAppInfo.pEngineName = "Game Animations Programming";
	mAppInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	mAppInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 1, 0);

	uint32_t extensionCount = 0;
	const char** extensions = glfwGetRequiredInstanceExtensions(&extensionCount);

	if (extensionCount == 0)
	{
		LOG_ERROR("No Vulkan extensions found, need at least 'VK_KHR_surface'");
		return false;
	}

	//Logger::log(1, "%s: Found %u Vulkan extensions\n", __FUNCTION__, extensionCount);
	for (uint32_t i = 0; i < extensionCount; ++i)
	{
		LOG_INFO("{0} : {1}", __FUNCTION__, std::string(extensions[i]).c_str());
	}

	VkInstanceCreateInfo CreateInfo{};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pNext = nullptr;
	CreateInfo.pApplicationInfo = &mAppInfo;
	CreateInfo.enabledExtensionCount = extensionCount;
	CreateInfo.ppEnabledExtensionNames = extensions;
	CreateInfo.enabledLayerCount = 0;

	result = vkCreateInstance(&CreateInfo, nullptr, &mInstance);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("Could not create Vulkan instance");
		return false;
	}

	uint32_t PhysicalDeviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &PhysicalDeviceCount, nullptr);

	if (PhysicalDeviceCount == 0)
	{
		LOG_ERROR("No Vulkan capable GPU found");
		return false;
	}

	std::vector<VkPhysicalDevice> Devices;
	Devices.resize(PhysicalDeviceCount);
	vkEnumeratePhysicalDevices(mInstance, &PhysicalDeviceCount, Devices.data());

	//Logger::log(1, "%s: Found %u physical device(s)\n", __FUNCTION__, physicalDeviceCount);

	result = glfwCreateWindowSurface(mInstance, m_WindowHandle, nullptr, &mSurface);
	if (result != VK_SUCCESS)
	{
		LOG_ERROR("Could not create Vulkan surface");
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	for (auto& layer : m_LayerStack)
		layer->OnDetach();

	m_LayerStack.clear();

	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroyInstance(mInstance, nullptr);

	glfwDestroyWindow(m_WindowHandle);
	glfwTerminate();

	g_ApplicationRunning = false;
}

void Application::Run()
{
	m_Running = true;

	// Main loop
	while (!glfwWindowShouldClose(m_WindowHandle) && m_Running)
	{
		glfwPollEvents();

		for (auto& layer : m_LayerStack)
			layer->OnUpdate(m_TimeStep);

		float time = GetTime();
		m_FrameTime = time - m_LastFrameTime;
		m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
		m_LastFrameTime = time;
	}
}

void Application::Close()
{
	m_Running = false;
}

float Application::GetTime()
{
	return (float)glfwGetTime();
}
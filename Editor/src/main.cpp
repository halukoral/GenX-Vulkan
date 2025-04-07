#include "Application.h"
#include "EntryPoint.h"

class EditorLayer : public Layer
{
public:
    void OnUIRender() override
    {
        ImGui::Begin("Hello");
        ImGui::Button("Button");
        ImGui::End();

        ImGui::ShowDemoWindow();
    }
};

Application* CreateApplication(int argc, char** argv)
{
    AppSpec spec;
    spec.Name = "Editor";

    auto app = new Application(spec);
    app->PushLayer<EditorLayer>();
    app->SetMenubarCallback([app]()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit"))
            {
                app->Close();
            }
            ImGui::EndMenu();
        }
    });
    return app;
}
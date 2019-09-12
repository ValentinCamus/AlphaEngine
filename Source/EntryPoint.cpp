#include <iostream>

#include <Alpha/Core/CoreMinimal.h>

#include <Alpha/Application/Application.h>
#include <Alpha/Engine/Renderer/Renderer.h>

#include <Sandbox/Sandbox.h>

int main(int argc, char *argv[])
{
    ALPHA_UNUSED(argc);
    ALPHA_UNUSED(argv);

    Alpha::Core::Init();
    Alpha::Renderer::Init();

	Alpha::Application application;

	auto sandboxLayer = Alpha::NewPointer<Alpha::SandboxLayer>();
    auto sandboxGuiLayer = Alpha::NewPointer<Alpha::GuiSandboxLayer>();

	application.PushLayer(sandboxLayer);
    application.PushLayer(sandboxGuiLayer);

	application.Run();

    Alpha::Logger::Info("Exiting with exit code 0 (No Error)");
    return EXIT_SUCCESS;
}

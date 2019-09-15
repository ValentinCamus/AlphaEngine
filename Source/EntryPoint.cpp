#include <iostream>

#include <Alpha/Core/CoreMinimal.h>
#include <Alpha/Engine/EngineMinimal.h>

#include <Alpha/Input/Input.h>

#include <Alpha/Application/Application.h>

#include <Sandbox/Sandbox.h>

int main(int argc, char *argv[])
{
    ALPHA_UNUSED(argc);
    ALPHA_UNUSED(argv);

    Alpha::Core::Init();
    Alpha::Input::Init();
    Alpha::Engine::Init();

	Alpha::Application application;

	auto sandboxLayer = Alpha::NewPointer<Alpha::SandboxLayer>();
    auto sandboxGuiLayer = Alpha::NewPointer<Alpha::GuiSandboxLayer>();

	application.PushLayer(sandboxLayer);
    application.PushLayer(sandboxGuiLayer);

    Alpha::Logger::Info("AlphaEngine: Running...");

	application.Run();

    Alpha::Exit(EXIT_SUCCESS);
}

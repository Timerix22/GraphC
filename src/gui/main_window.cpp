#include "gui_internal.hpp"

const char* window_title="GraphC";
bool loop_running=false;
SDL_Window* sdl_window;
SDL_GLContext gl_context;

#define SDL_ERROR_SAFETHROW() { \
    const char* sdl_error=SDL_GetError(); \
    throw_msg(cptr_concat("SDL Error: ", sdl_error)); \
    SDL_ClearError(); \
}
#define SDL_TRY_ZERO(FUNC_CALL) if(FUNC_CALL != 0) SDL_ERROR_SAFETHROW();

Maybe main_window_open(){
    SDL_TRY_ZERO(SDL_Init(SDL_INIT_VIDEO));
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0));
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0));

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif
    
    // Create window with graphics context
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
    SDL_TRY_ZERO( SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI);
    sdl_window = SDL_CreateWindow(window_title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(sdl_window);
    SDL_TRY_ZERO( SDL_GL_MakeCurrent(sdl_window, gl_context));
    SDL_TRY_ZERO( SDL_GL_SetSwapInterval(1)); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if(ImGui_ImplSDL2_InitForOpenGL(sdl_window, gl_context) != true)
        SDL_ERROR_SAFETHROW();
    if(ImGui_ImplOpenGL3_Init(glsl_version) != true)
        SDL_ERROR_SAFETHROW();

    return MaybeNull;
}

Maybe main_window_loop_start(){
    if(loop_running)
        safethrow_msg("loop is already running",;);
    loop_running=true;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // main loop
    while(loop_running){
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                loop_running = false;
            if (event.type==SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdl_window))
                loop_running = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(sdl_window);
    }

    try_cpp(main_window_close(),_999,;);
    return MaybeNull;
}

Maybe main_window_close(){
    if(!loop_running)
        return MaybeNull;
    loop_running=false;
    
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return MaybeNull;
}

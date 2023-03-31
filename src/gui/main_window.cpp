#include "gui_internal.hpp"

ImVec4 clear_color = RGBAHexToF(35,35,50,255);
bool loop_running=false;
SDL_Window* sdl_window;
SDL_GLContext gl_context;

#define SDL_ERROR_SAFETHROW() { \
    const char* sdl_error=SDL_GetError(); \
    safethrow_msg(cptr_concat("SDL Error: ", sdl_error),;); \
    SDL_ClearError(); \
}
#define SDL_TRY_ZERO(FUNC_CALL) if(FUNC_CALL != 0) SDL_ERROR_SAFETHROW();
#define SDL_TRY_ONE(FUNC_CALL) if(FUNC_CALL != 1) SDL_ERROR_SAFETHROW();

Maybe main_window_open(const char* window_title){
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    io.ConfigDockingTransparentPayload = true;
    io.ConfigInputTextCursorBlink = false;
    // io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    if(ImGui_ImplSDL2_InitForOpenGL(sdl_window, gl_context) != true)
        SDL_ERROR_SAFETHROW();
    if(ImGui_ImplOpenGL3_Init(glsl_version) != true)
        SDL_ERROR_SAFETHROW();

    return MaybeNull;
}

// Wait, poll and handle events (inputs, window resize, etc.)
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
Maybe poll_events(bool& frame_needs_update, bool wait){
    SDL_Event event;
    if(wait){
        // waits for first event in cpu-efficient way
        SDL_TRY_ONE(SDL_WaitEvent(&event)); 
    }
    // doesnt wait for event
    else if(!SDL_PollEvent(&event))
        return MaybeNull;
    
    do {
        frame_needs_update|=ImGui_ImplSDL2_ProcessEvent(&event);
        switch(event.type){
            case SDL_QUIT: {
                try_cpp(main_window_close(),_9914,;);
                break;
            }
            case SDL_WINDOWEVENT: {
                if(event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(sdl_window)){
                    try_cpp(main_window_close(),_9915,;);
                }
                break;
            }
        }
    } while (SDL_PollEvent(&event)); // if there are more events, handles them
    return MaybeNull;
}

Maybe draw_frame(){
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();

    // Draw UI
    draw_bg_window();
    draw_demo_windows(io);

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }

    SDL_GL_SwapWindow(sdl_window);

    return MaybeNull;
}

Maybe main_window_loop_start(){
    if(loop_running)
        safethrow_msg("loop is already running",;);

    // draw first frame
    try_cpp(draw_frame(),_2175,;);

    // main loop
    loop_running=true;
    while(loop_running){
        bool frame_needs_update=false;
        // waits for events
        try_cpp(poll_events(frame_needs_update, true),_55415,;);

        if(!frame_needs_update){
            // skips frame rendering if user didn't interacted with anything
            u32 frame_delay_ms=1000/frame_rate_max;
            SDL_Delay(frame_delay_ms);
            continue;
        }
        
        try_cpp(draw_frame(),_2175,;);
    }

    // Cleanup
    main_window_destroy();
    return MaybeNull;
}

Maybe main_window_close(){
    if(!loop_running)
        return MaybeNull;
    loop_running=false;
    return MaybeNull;
}

void main_window_destroy(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

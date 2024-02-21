#include "gui_internal.hpp"

#define default_font_name font_DroidSans
const f32 default_font_size=14.0f;
ImVec4 clear_color = RGBAHexToF(35,35,50,255);
bool loop_running=false;
SDL_Window* sdl_window;
SDL_GLContext gl_context;
bool main_loop_wait_for_input=true;

f32 getMainWindowDPI(){
    int w=0, h=0;
    SDL_GL_GetDrawableSize(sdl_window, &w, &h);
    int sim_w=0, sim_h=0;
    SDL_GetWindowSize(sdl_window, &sim_w, &sim_h);
    f32 wdpi=w/sim_w;
    f32 hdpi=h/sim_h;
    f32 dpi=SDL_sqrtf(wdpi*wdpi + hdpi*hdpi);
    return dpi;
}

Maybe main_window_open(const char* window_title){
    SDL_TRY_ZERO(SDL_Init(SDL_INIT_VIDEO));
    SDL_version v;
    SDL_GetVersion(&v);
    kprintf("SDL version: %u.%u.%u\n", v.major, v.minor, v.patch);
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
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_ALLOW_HIGHDPI);
    sdl_window = SDL_CreateWindow(window_title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, window_flags);
    gl_context = SDL_GL_CreateContext(sdl_window);
    SDL_TRY_ZERO( SDL_GL_MakeCurrent(sdl_window, gl_context));
    SDL_TRY_ZERO( SDL_GL_SetSwapInterval(1)); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    io.ConfigDockingTransparentPayload = true;
    io.ConfigInputTextCursorBlink = false;
    // io.ConfigViewportsNoTaskBarIcon = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Setup Platform/Renderer backends
    if(ImGui_ImplSDL2_InitForOpenGL(sdl_window, gl_context) != true)
        SDL_ERROR_SAFETHROW();
    if(ImGui_ImplOpenGL3_Init(glsl_version) != true)
        SDL_ERROR_SAFETHROW();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    io.FontDefault=ImFont_LoadEmbedded(default_font_name, default_font_size);
    ImFont_LoadEmbedded(font_Cousine_Regular, default_font_size);

    node_editor_create("node editor");
    return MaybeNull;
}

// Wait, poll and handle events (inputs, window resize, etc.)
Maybe poll_events(u16& frame_updates_requested, bool wait){
    SDL_Event event;
    if(wait){
        // waits for first event in cpu-efficient way
        SDL_TRY_ONE(SDL_WaitEvent(&event)); 
    }
    // doesnt wait for event
    else if(!SDL_PollEvent(&event))
        return MaybeNull;
    
    do {
        if(ImGui_ImplSDL2_ProcessEvent(&event))
            frame_updates_requested=2;
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
    draw_debug_window(io, &main_loop_wait_for_input);
    draw_node_editor();

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

    u16 frame_updates_requested=1;
    u64 prev_update_time_ms=SDL_GetTicks64();
    loop_running=true;
    // main loop
    while(loop_running){
        // waits for events
        try_cpp(poll_events(frame_updates_requested, main_loop_wait_for_input),_55415,;);

        if(frame_updates_requested==0)
        {
            u64 update_time_ms=SDL_GetTicks64();
            if(update_time_ms >= prev_update_time_ms + 1000/frame_rate_min){
                // if frame rate < frame_rate_min then requests frame draw
                // works only if main_loop_wait_for_input = false
                frame_updates_requested=1;
                prev_update_time_ms=update_time_ms;
            }
            else {
                // skips frame rendering and waits to limit fps
                u32 frame_delay_ms=1000/frame_rate_max;
                SDL_Delay(frame_delay_ms);
                continue;
            }
        }

        // deaws requested number of frames
        while(frame_updates_requested>0) {
            try_cpp(draw_frame(),_2175,;);
            frame_updates_requested--;
        }   
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
    node_editor_destroy();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

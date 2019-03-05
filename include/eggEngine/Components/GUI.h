#ifndef EGG_GUI_H
#define EGG_GUI_H

#include <eggEngine/Component.h>
#include <eggEngine/IMGUI.h>
#include <eggEngine/ResourceManager.h>
#include <eggEngine/Program.h>
#include <eggEngine/RenderVisitor.h>
#include <eggEngine/GraphicsContext.h>

namespace egg
{

    static void ShowExampleAppMainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                //ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }


    static void ShowDock(const char* name, float x, float y, float w, float h, bool* p_open)
    {
        bool f = false;

        ImGui::SetNextWindowPos(ImVec2(x, y));
        ImGui::SetNextWindowSize(ImVec2(w, h));
        if (ImGui::Begin(name, p_open
            , ImGuiWindowFlags_NoResize
            //| ImGuiWindowFlags_AlwaysAutoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_NoFocusOnAppearing
            | ImGuiWindowFlags_NoNav
            //| ImGuiWindowFlags_ResizeFromAnySide
            | ImGuiWindowFlags_NoTitleBar
        ))
        {

        }
        ImGui::End();
    }

    class EGG_ENGINE_EXPORT GUI
        :public ComponentBase
    {
    public:
        GUI() {}
        virtual ~GUI() {}

        virtual std::string ObjectClassName() override { return "gui"; }

        float mouse_x = 0;
        float mouse_y = 0;
        bool mouse_pressed[3] = { false, false, false };

        virtual void onInput(nv_input* nv) override
        {
            //printf("i");
            egg::WindowEvent we = nv->mWindowEvent;

            //glfwSetMouseButtonCallback(window, ImGui_ImplGlfw_MouseButtonCallback);
            //glfwSetScrollCallback(window, ImGui_ImplGlfw_ScrollCallback);
            //glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
            //glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);

            //记录鼠标坐标
            if (we.mWindowEventType == egg::WET_MOUSE)
            {
                if (we.mMouseType == egg::MT_Move)
                {
                    mouse_x = we.x;
                    mouse_y = we.y;
                }
                if (we.mMouseType == egg::MT_Click)
                {
                    if (we.mMouseButton == egg::MB_Left)
                    {
                        mouse_pressed[0] = (we.mKeyState == egg::KS_DOWN);
                    }
                    if (we.mMouseButton == egg::MB_Middle)
                    {
                        mouse_pressed[1] = (we.mKeyState == egg::KS_DOWN);
                    }
                    if (we.mMouseButton == egg::MB_Right)
                    {
                        mouse_pressed[2] = (we.mKeyState == egg::KS_DOWN);
                    }
                }
            }

        }
        virtual void onUpdate(nv_update* nv) override {}
        virtual void onCull(nv_cull* nv, Eigen::Matrix4f& vm) override
        {
            //printf("c");
        }
        virtual void onRender(nv_render* nv, Eigen::Matrix4f& mv) override
        {
            //printf("r");
        }
        virtual void onGUI(nv_gui* nv) override
        {
            //printf("u");

            //初始化过程 必须在当前context
            if (!isInited)
            {
                // Setup ImGui binding
                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

                //ImGui_ImplGlfwGL2_Init(window, true);
                ImGui_Init(nv);

                isInited = true;
            }

            //设置样式
            // Setup style
            //ImGui::StyleColorsDark();
            ImGui::StyleColorsClassic();
            //ImGui::StyleColorsLight();

            ImGui::GetStyle().Alpha = 0.7f;
            ImGui::GetStyle().WindowRounding = 0.0f;
            ImGui::GetStyle().WindowBorderSize = 1.0f;

            //开始新的frame
            //ImGui_ImplGlfwGL2_NewFrame();
            ImGui_NewFrame(nv);
            {

                //渲染测试窗口
                bool show_demo_window = true;
                ImGui::ShowDemoWindow(&show_demo_window);

                ImGui::ShowUserGuide();

                //主菜单
                ShowExampleAppMainMenuBar();

                //界面布局区域
                bool p_open = true;
                ShowDock("a", 0, 50, 100, 600, &p_open);
                ShowDock("b", 0, 18, nv->viewport.z(), 20, &p_open);
                ShowDock("c", 0, nv->viewport.w() - 20, nv->viewport.z(), 20, &p_open);
                ShowDock("d", nv->viewport.z() - 200, 50, 200, 600, &p_open);

            }
            //完成frame
            ImGui::Render();
            //ImGui_ImplGlfwGL2_RenderDrawData(ImGui::GetDrawData());
            ImGui_RenderDrawData(nv, ImGui::GetDrawData());

        }

    private:

        //初始化标志
        bool isInited = false;

        //字体对象
        egg::handle_texture mFont;

        //
        Program* program = 0;

        bool ImGui_Init(nv_gui* nv)
        {
            //g_Window = window;

            //映射键盘
            ImGuiIO& io = ImGui::GetIO();
            //io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB; 
            //io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
            //io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
            //io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
            //io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
            //io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
            //io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
            //io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
            //io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
            //io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
            //io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
            //io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
            //io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
            //io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
            //io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
            //io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
            //io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
            //io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
            //io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
            //io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
            //io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

            //设置剪切板
            //io.SetClipboardTextFn = ImGui_ImplGlfwGL2_SetClipboardText;
            //io.GetClipboardTextFn = ImGui_ImplGlfwGL2_GetClipboardText;
            //io.ClipboardUserData = g_Window;

            //设置输入法
#ifdef _WIN32
            //io.ImeWindowHandle = glfwGetWin32Window(g_Window);
#endif

            //设置鼠标指针
            // Load cursors
            // FIXME: GLFW doesn't expose suitable cursors for ResizeAll, ResizeNESW, ResizeNWSE. We revert to arrow cursor for those.
            //g_MouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
            //g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

            //绑定输入 用onInput取代
            //if (install_callbacks)
            //    ImGui_ImplGlfw_InstallCallbacks(window);


            //初始化font贴图
            unsigned char* pixels;
            int width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

            //建立贴图对象
            mFont = nv->renderer->createTexture();
            nv->renderer->setTextureData(mFont
                , width, height
                , (char*)pixels
                , egg::PF_RGBA
                , egg::DT_UNSIGNED_BYTE
            );

            // Store our identifier
            io.Fonts->TexID = (void *)(intptr_t)mFont;


            //加载GUI默认材质


            return true;
        }


        void ImGui_NewFrame(nv_gui* nv)
        {
#if 0
            //建立字体对象
            if (!g_FontTexture)
                ImGui_ImplGlfwGL2_CreateDeviceObjects();

            ImGuiIO& io = ImGui::GetIO();

            //设置显示比例
            // Setup display size (every frame to accommodate for window resizing)
            int w, h;
            int display_w, display_h;
            glfwGetWindowSize(g_Window, &w, &h);
            glfwGetFramebufferSize(g_Window, &display_w, &display_h);
            io.DisplaySize = ImVec2((float)w, (float)h);
            io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

            //设置时间
            // Setup time step
            double current_time = glfwGetTime();
            io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
            g_Time = current_time;

            //设置鼠标位置
            // Setup inputs
            // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
            if (glfwGetWindowAttrib(g_Window, GLFW_FOCUSED))
            {
                if (io.WantMoveMouse)
                {
                    glfwSetCursorPos(g_Window, (double)io.MousePos.x, (double)io.MousePos.y);   // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
                }
                else
                {
                    double mouse_x, mouse_y;
                    glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
                    io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
                }
            }
            else
            {
                io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
            }

            //设置鼠标按钮状态
            for (int i = 0; i < 3; i++)
            {
                // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
                io.MouseDown[i] = g_MouseJustPressed[i] || glfwGetMouseButton(g_Window, i) != 0;
                g_MouseJustPressed[i] = false;
            }

            //设置鼠标指针
            // Update OS/hardware mouse cursor if imgui isn't drawing a software cursor
            ImGuiMouseCursor cursor = ImGui::GetMouseCursor();
            if (io.MouseDrawCursor || cursor == ImGuiMouseCursor_None)
            {
                glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else
            {
                glfwSetCursor(g_Window, g_MouseCursors[cursor] ? g_MouseCursors[cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
                glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
#endif

            ImGuiIO& io = ImGui::GetIO();

            //设置默认size 避免断言检测
            io.DisplaySize = ImVec2(
                nv->viewport[2] - nv->viewport[0]
                , nv->viewport[3] - nv->viewport[1]
            );
            io.DisplayFramebufferScale = ImVec2(1, 1);

            //输入鼠标
            //if (io.WantMoveMouse)
            //{
            //    //glfwSetCursorPos(g_Window, (double)io.MousePos.x, (double)io.MousePos.y);   // Set mouse position if requested by io.WantMoveMouse flag (used when io.NavMovesTrue is enabled by user and using directional navigation)
            //}
            //else
            {
                //double mouse_x, mouse_y;
                //glfwGetCursorPos(g_Window, &mouse_x, &mouse_y);
                io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
            }

            for (size_t i = 0; i < 3; i++)
            {
                io.MouseDown[i] = mouse_pressed[i];
            }


            //启动新的frame
            // Start the frame. This call will update the io.WantCaptureMouse, io.WantCaptureKeyboard flag that you can use to dispatch inputs (or not) to your application.
            ImGui::NewFrame();
        }


        void ImGui_RenderDrawData(nv_gui* nv, ImDrawData* draw_data)
        {
            // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
            ImGuiIO& io = ImGui::GetIO();
            int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
            if (fb_width == 0 || fb_height == 0) return;
            draw_data->ScaleClipRects(io.DisplayFramebufferScale);

            //属性地址
            int attr_local_pos = 0;
            int attr_local_uv = 0;
            int attr_local_color = 0;

            //立即设置渲染状态
            if (program == 0)
            {
                std::string abs_program_path = "|assets|shaders|glcore|gui.program";
                ResourceManager* rm = ResourceManager::getInstance();
                Resource* resource = rm->readResource(abs_program_path, false);
                /*Program* */program = dynamic_cast<Program*>(resource);
            }
            program->bind(nv->gc->m_gcID, nv->renderer);
            attr_local_pos = program->mVANameMapping[egg_va_position2];
            attr_local_uv = program->mVANameMapping[egg_va_uv0];
            attr_local_color = program->mVANameMapping[egg_va_color0];

#if 0
            //计算MVP
            Eigen::Matrix4f ViewMatrix;
            Eigen::Matrix4f ProjactiveMatrix;
            ViewMatrix.setIdentity();
            ProjactiveMatrix.setIdentity();
            ProjactiveMatrix = Eigen::ortho<float>(0, io.DisplaySize.x, 0, io.DisplaySize.y, 0, 1);
            ProjactiveMatrix = Eigen::ortho<float>(0, 10000, 0, 10000, 0, 1);
            //ViewMatrix.setIdentity();

            //设置MVP矩阵
            egg::handle_program handle = program->getProgram().getID(nv->gc->m_gcID);
            nv->renderer->setUniformMatrix(handle, egg_ModelViewMatrix, 1, ViewMatrix.data());
            nv->renderer->setUniformMatrix(handle, egg_ProjectionMatrix, 1, ProjactiveMatrix.data());
#endif

            //set viewport lrbt
            egg::handle_program handle = program->getProgram().getID(nv->gc->m_gcID);
            Eigen::Vector4f ViewPort(0.0f, io.DisplaySize.x, 0.0f, io.DisplaySize.y);
            nv->renderer->setUniform(handle, egg_ViewPort, 4, 1, ViewPort.data());
            nv->renderer->setViewport(nv->viewport[0], nv->viewport[1], nv->viewport[2], nv->viewport[3]);

            //
            //nv->renderer->setUniform(handle, egg_tex, 0);
            //nv->renderer->setUniform(handle, "ddd", 99);

            //blend
            nv->renderer->setBlendFunc(egg::Renderer::BF_SRC_ALPHA, egg::Renderer::BF_ONE_MINUS_SRC_ALPHA);
            nv->renderer->setDepthFunc();

            // Render command lists
            for (int n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                const ImDrawVert* vtx_buffer = cmd_list->VtxBuffer.Data;
                const ImDrawIdx* idx_buffer = cmd_list->IdxBuffer.Data;

                //glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, pos)));
                //glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, uv)));
                //glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + IM_OFFSETOF(ImDrawVert, col)));

                //建立VBO
                egg::handle_geometry vbo = nv->renderer->createVBO(egg::VBO_TYPE::RAW
                    , sizeof(ImDrawVert)*cmd_list->VtxBuffer.size(), (char*)vtx_buffer);
                //egg::handle_geometry ibo = nv->renderer->createVBO(egg::VBO_TYPE::INDEX
                //    , sizeof(ImDrawIdx)*cmd_list->IdxBuffer.size(), (char*)idx_buffer);

                //bind 属性
                nv->renderer->bindVBO(vbo, attr_local_pos, 2, egg::DT_FLOAT, false, sizeof(ImDrawVert)); //pos
                nv->renderer->bindVBO(vbo, attr_local_uv, 2, egg::DT_FLOAT, false, sizeof(ImDrawVert), sizeof(ImVec2)); //uv
                nv->renderer->bindVBO(vbo, attr_local_color, 4, egg::DT_UNSIGNED_BYTE, false, sizeof(ImDrawVert), sizeof(ImVec2)*2, true);//color
                //nv->renderer->bindVBO(ibo, 0, 1
                //    , sizeof(ImDrawIdx) == 2 ? egg::DT_UNSIGNED_SHORT : egg::DT_UNSIGNED_INT
                //    , true);

#if 0 //转换类型  
                //直接实用int32作为RGBA的输入无法正确显示
                //所以转换成float类型
                std::vector<float> color_float;
                color_float.resize(cmd_list->VtxBuffer.size() * 4);
                for (size_t i = 0; i < cmd_list->VtxBuffer.size(); i++)
                {
                    ImDrawVert& idv = cmd_list->VtxBuffer.Data[i];
                    color_float[i * 4 + 3] = float((idv.col & 0xff000000) >> 24) / 255.0f;
                    color_float[i * 4 + 2] = float((idv.col & 0xff0000) >> 16) / 255.0f;
                    color_float[i * 4 + 1] = float((idv.col & 0xff00) >> 8) / 255.0f;
                    color_float[i * 4 + 0] = float(idv.col & 0xff) / 255.0f;

                    //color_float[i * 4 + 0] = 1.0;
                    //color_float[i * 4 + 1] = 1.0;
                    //color_float[i * 4 + 2] = 1.0;
                    //color_float[i * 4 + 3] = 1.0;

                }
                egg::handle_geometry vbo_color = nv->renderer->createVBO(egg::VBO_TYPE::RAW
                    , color_float.size(), (char*)&color_float[0]);
                nv->renderer->bindVBO(vbo_color, attr_local_color, 4, egg::DT_FLOAT, false, sizeof(float) * 4);//color
#endif


                for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
                {
                    const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                    if (pcmd->UserCallback)
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else
                    {
                        //glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                        //glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        //glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);

                        //立即修改贴图对象
                        nv->renderer->bindTexture((handle_texture)pcmd->TextureId, 0);
                        nv->renderer->setUniform(handle, egg_texture0, 0);

                        //立即修改渲染区域
                        nv->renderer->setScissor(
                            (int)pcmd->ClipRect.x
                            , (int)(fb_height - pcmd->ClipRect.w)
                            , (int)(pcmd->ClipRect.z - pcmd->ClipRect.x)
                            , (int)(pcmd->ClipRect.w - pcmd->ClipRect.y)
                        );

                        //立即绘制geometry
                        nv->renderer->drawElements(egg::PrimitiveSet::TRIANGLES, pcmd->ElemCount
                            , sizeof(ImDrawIdx) == 2 ? egg::DT_UNSIGNED_SHORT : egg::DT_UNSIGNED_INT
                            , (char*)idx_buffer);
                    }
                    idx_buffer += pcmd->ElemCount;
                    //break;
                }

                nv->renderer->unbindVBO(vbo, attr_local_pos, false);
                nv->renderer->unbindVBO(vbo, attr_local_uv, false);
                nv->renderer->unbindVBO(vbo, attr_local_color, false);

                //nv->renderer->distoryVBO(vbo_color);
                nv->renderer->distoryVBO(vbo);
                //nv->renderer->distoryVBO(ibo);

            }

            //立即退出渲染状态
            program->unBind();
            nv->renderer->setScissor();
            nv->renderer->setBlendFunc();
        }

        

    };

}//namespace egg

#endif//EGG_GUI_H

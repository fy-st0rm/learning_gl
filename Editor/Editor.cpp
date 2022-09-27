#include "Editor.h"


Editor::Editor()
{
    camera.setPos({ 0.0f, 0.0f, 0.0f });
    camera.setSize({ 500.0f, 500.0f });

    spritesheet.create("Res/Textures/spritesheet.png", 0);
    create_map();

    renderer.add_quad();
    renderer.genBuffers();

    int samplers[] = { 0 };
    renderer.shader.SetUniform1iv("u_Textures", 1, samplers);
}

Editor::~Editor()
{

}

void Editor::create_map()
{
	map = new int* [m_Height];
    for (int i = 0; i < m_Width; i++)
    {
        map[i] = new int[m_Width];
        memset(map[i], false, m_Width * sizeof(int));
    }
}

void Editor::print_map()
{
    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++)
            std::cout << map[y][x];
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Editor::add_tiles()
{
    int x = input.mouse_pos.x / 16;
    int y = input.mouse_pos.y / 16;

    std::cout << y << " " << x << std::endl;
    if (x < m_Width && y < m_Height)
    {
        map[y][x] = 1;
    }
}

void Editor::delete_tiles()
{
    int x = input.mouse_pos.x / 16;
    int y = input.mouse_pos.y / 16;

    std::cout << y << " " << x << std::endl;
    if (x < m_Width && y < m_Height)
    {
        map[y][x] = 0;
    }
}

void Editor::draw_tiles()
{
    for (int y = 0; y < m_Height; y++)
    {
        for (int x = 0; x < m_Width; x++)
        {
            if (map[y][x] == 1)
            {
                auto quad = spritesheet.create_quad({ x * 16.0f, y * 16.0f }, { 0.0f, 0.0f }, { 16.0f, 16.0f });
                renderer.push_quad(quad);
            }
        }
    }
}

void Editor::OnUpdate()
{
	renderer.clear({ 0.0f, 0.0f, 0.0f, 1.0f });

    if (input.mouse_button_pressed(GLFW_MOUSE_BUTTON_1))
    {
        add_tiles();
    }

    if (input.mouse_button_pressed(GLFW_MOUSE_BUTTON_2))
    {
        delete_tiles();
    }

    draw_tiles();
    //float x = 5.0f, y = 5.0f;
    //auto quad = spritesheet.create_quad({ x * 16.0f, y * 16.0f }, { 0.0f, 0.0f }, { 16.0f, 16.0f });
    //renderer.push_quad(quad);

    auto mvp = camera.getMVP();
    renderer.shader.SetUniformMat4f("u_MVP", mvp);
    
    renderer.draw();
}

void Editor::OnImGuiRender()
{

}

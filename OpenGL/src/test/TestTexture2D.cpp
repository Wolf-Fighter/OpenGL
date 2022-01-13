#include "TestTexture2D.h"

#include "VertexBufferLayout.h"

#include "imgui/imgui.h"

namespace test {

    TestTexture2D::TestTexture2D()
        : m_TranslateA(200.0f, 300.0f, 0.0f), m_TranslateB(400.0f, 300.0f, 0.0f),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        float positions[4 * 4] = {
            -64, -64, 0.0f, 0.0f, // 0
             64, -64, 1.0f, 0.0f, // 1
             64,  64, 1.0f, 1.0f, // 2
            -64,  64, 0.0f, 1.0f  // 3
        };

        unsigned int indices[2 * 3] = {
            0, 1, 2,
            2, 3, 0
        };

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float), GL_STATIC_DRAW);

        VertexBufferLayout bvl;
        bvl.Push<float>(2);
        bvl.Push<float>(2);
        
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, bvl);
            
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 2 * 3);

        m_Shader = std::make_unique<Shader>("res/shader/Basic.shader");
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("res/texture/image.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

    TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float daltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
        Renderer renderer;
        renderer.Clear();

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslateA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslateB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslateA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslateB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/fram (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
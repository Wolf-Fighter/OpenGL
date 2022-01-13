#include "TestDynamicBatchTexture2D.h"

#include "VertexBufferLayout.h"

#include "imgui/imgui.h"

namespace test {

    static Vertex* CreateQuad(Vertex* vertex, float x, float y, float texID)
    {
        float size = 100.0f;

        vertex->Position = { x, y, 0.0f};
        vertex->Color = {};
        vertex->TexCoord = {0.0f, 0.0f};
        vertex->TexID = texID;
        vertex++;

        vertex->Position = { x + size, y, 0.0f };
        vertex->Color = {};
        vertex->TexCoord = { 1.0f, 0.0f };
        vertex->TexID = texID;
        vertex++;

        vertex->Position = { x + size, y + size, 0.0f };
        vertex->Color = {};
        vertex->TexCoord = { 1.0f, 1.0f };
        vertex->TexID = texID;
        vertex++;

        vertex->Position = { x, y + size, 0.0f };
        vertex->Color = {};
        vertex->TexCoord = { 0.0f, 1.0f };
        vertex->TexID = texID;
        vertex++;

        return vertex;
    }

    TestDynamicBatchTexture2D::TestDynamicBatchTexture2D()
        : m_TranslateA(0.0f, 0.0f, 0.0f), m_TranslateB{ 100.0f, 100.0f },
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        const size_t MaxQuadCount = 1024;
        const size_t MaxVertexCount = MaxQuadCount * 4;
        const size_t MaxIndexCount = MaxQuadCount * 6;

        m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(Vertex), GL_DYNAMIC_DRAW);

        VertexBufferLayout bvl;
        bvl.Push<float>(3);
        bvl.Push<float>(4);
        bvl.Push<float>(2);
        bvl.Push<float>(1);

        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, bvl);

        uint32_t indices[MaxIndexCount];
        uint32_t offset = 0;
        for (size_t i = 0; i < MaxVertexCount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;
            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
            offset += 4;
        }

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, MaxIndexCount);

        m_Shader = std::make_unique<Shader>("res/shader/BatchBasic.shader");
        m_Shader->Bind();
        m_TextureA = std::make_unique<Texture>("res/texture/image.png");
        m_TextureB = std::make_unique<Texture>("res/texture/image1.png");
        int sampler2D[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, sampler2D);
	}

    void TestDynamicBatchTexture2D::OnUpdate(float datlaTime)
    {
        std::array<Vertex, 1024 * 4> vertices;

        Vertex* buffer = vertices.data();
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                float x = i * 100;
                float y = j * 100;
                buffer = CreateQuad(buffer, x, y, (i + j) % 2);
            }
        }
        buffer = CreateQuad(buffer, m_TranslateB[0], m_TranslateB[1], 0.0f);

        m_VertexBuffer->Bind();
        GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data()));
    }

	void TestDynamicBatchTexture2D::OnRender()
	{
        Renderer renderer;
        renderer.Clear();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslateA);
            glm::mat4 vp = m_Proj * m_View;
            m_Shader->Bind();
            m_TextureA->Bind(0);
            m_TextureB->Bind(1);
            m_Shader->SetUniformMat4f("u_ViewProj", vp);
            m_Shader->SetUniformMat4f("u_Transform", model);
            renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
        }
	}

    void TestDynamicBatchTexture2D::OnImGuiRender()
    {
        ImGui::Begin("Controls");
        ImGui::DragFloat2("Quad Position", m_TranslateB, 0.1f);
        ImGui::End();
    }
}
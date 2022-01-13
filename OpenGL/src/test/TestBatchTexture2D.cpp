#include "TestBatchTexture2D.h"

#include "VertexBufferLayout.h"

namespace test {

	TestBatchTexture2D::TestBatchTexture2D()
		: m_TranslateA(0.0f, 0.0f, 0.0f), m_TranslateB(0.0f, 0.0f, 0.0f),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // [x,y,z],[r,g,b,a],[u,v],[texturei]
        float positions[8 * 10] = {
            100.0f, 100.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 0.0f, 0.0f,  // 0
            228.0f, 100.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 0.0f, 0.0f,  // 1
            228.0f, 228.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 1.0f, 1.0f, 0.0f,  // 2
            100.0f, 228.0f, 0.0f, 0.18f, 0.6f, 0.96f, 1.0f, 0.0f, 1.0f, 0.0f,  // 3

            328.0f, 328.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 0.0f, 1.0f,  // 4
            456.0f, 328.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 0.0f, 1.0f,  // 5
            456.0f, 456.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 1.0f, 1.0f, 1.0f,  // 6
            328.0f, 456.0f, 0.0f, 1.0f, 0.93f, 0.24f, 1.0f, 0.0f, 1.0f, 1.0f   // 7
        };

        unsigned int indices[2 * 6] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 10 * sizeof(float), GL_STATIC_DRAW);

        VertexBufferLayout bvl;
        bvl.Push<float>(3);
        bvl.Push<float>(4);
        bvl.Push<float>(2);
        bvl.Push<float>(1);

        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexArray->AddBuffer(*m_VertexBuffer, bvl);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 2 * 6);

        m_Shader = std::make_unique<Shader>("res/shader/BatchBasic.shader");
        m_Shader->Bind();
        m_TextureA = std::make_unique<Texture>("res/texture/image.png");
        m_TextureB = std::make_unique<Texture>("res/texture/image1.png");
        int sampler2D[2] = { 0, 1 };
        m_Shader->SetUniform1iv("u_Textures", 2, sampler2D);
	}

	void TestBatchTexture2D::OnRender()
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
}
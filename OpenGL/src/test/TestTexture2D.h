#pragma once

#include "test/Test.h"

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace test {
	class TestTexture2D : public Test
	{
	private:
		glm::vec3 m_TranslateA, m_TranslateB;
		glm::mat4 m_Proj, m_View;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float daltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
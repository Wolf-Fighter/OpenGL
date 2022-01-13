#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <array>

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexID;
};

namespace test {
	class TestDynamicBatchTexture2D : public Test
	{
	private:
		glm::vec3 m_TranslateA;
		float m_TranslateB[2];
		glm::mat4 m_Proj, m_View;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Texture> m_TextureA, m_TextureB;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

	public:
		TestDynamicBatchTexture2D();

		void OnUpdate(float datlaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}

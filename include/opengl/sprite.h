#pragma once

#include <GL/glew.h>
#include <vector>
#include <iostream>

#include "mvl/mvl.h"

namespace ogl
{
	/*
	 * T - vertex data type
	 * N - sprite dimensions
	 */
	template<typename T, size_t N>
	class Sprite
	{
	public:
		Sprite()
		{
			primitive_type = GL_TRIANGLE_STRIP;
			data_type = GL_FLOAT;
		}

		void setPrimitiveType(GLenum prim_type)
		{
			primitive_type = prim_type;
		}

		void setDataType(GLenum d_type)
		{
			data_type = d_type;
		}
		
		void addVertices(const std::vector<mvl::Vector<T,N>>& vecs)
		{
			// Reserve space once
			vertices.reserve(N * vecs.size());

			// Append every vector to vertices
			for (const auto& v : vecs)
				for (size_t i = 0; i < N; i++)
					vertices.push_back(v[i]);
		}

		void draw(GLint attribute_position)
		{
			glVertexAttribPointer(attribute_position, N, data_type, GL_FALSE, 0, vertices.data());
			glDrawArrays(primitive_type, 0, vertices.size() / N);
		}

	private:
		GLenum primitive_type;
		GLenum data_type;

		std::vector<T> vertices;
	};
}

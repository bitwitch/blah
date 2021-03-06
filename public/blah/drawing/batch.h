#pragma once
#include <blah/graphics/graphics.h>
#include <blah/containers/str.h>
#include <blah/math/vec2.h>
#include <blah/math/rect.h>
#include <blah/math/mat3x2.h>
#include <blah/math/mat4x4.h>
#include <blah/math/color.h>
#include <blah/drawing/subtexture.h>
#include <blah/drawing/spritefont.h>
#include <blah/containers/vector.h>

namespace Blah
{

	enum class ColorMode
	{
		Normal,
		Wash
	};

	enum class TextAlign : char
	{
		Center = 0,
		Left = 1 << 1,
		Right = 1 << 2,
		Top = 1 << 3,
		Bottom = 1 << 4,

		TopLeft = Top | Left,
		TopRight = Top | Right,
		BottomLeft = Bottom | Left,
		BottomRight = Bottom | Right
	};

	inline TextAlign operator|(TextAlign lhs, TextAlign rhs) { return static_cast<TextAlign>(static_cast<char>(lhs) | static_cast<char>(rhs)); }
	inline TextAlign operator&(TextAlign lhs, TextAlign rhs) { return static_cast<TextAlign>(static_cast<char>(lhs) & static_cast<char>(rhs)); }

	class Batch
	{
	public:
		Batch();
		Batch(const Batch& other) = delete;
		Batch& operator=(const Batch& other) = delete;
		~Batch();

		void push_matrix(const Mat3x2& matrix);
		void pop_matrix();
		void push_scissor(const Rect& scissor);
		void pop_scissor();
		void push_blend(const BlendMode& blend);
		void pop_blend();
		void push_material(const MaterialRef& material);
		void pop_material();
		void push_layer(int layer);
		void pop_layer();
		void push_color_mode(ColorMode mode);
		void pop_color_mode();

		void set_texture(const TextureRef& texture);

		void render(const FrameBufferRef& target);
		void render(const FrameBufferRef& target, const Mat4x4& matrix);

		void clear();
		void dispose();

		void line(const Vec2& from, const Vec2& to, float t, Color color);
		void line(const Vec2& from, const Vec2& to, float t, Color fromColor, Color toColor);

		void bezier_line(const Vec2& from, const Vec2& b, const Vec2& to, int steps, float t, Color color);
		void bezier_line(const Vec2& from, const Vec2& b, const Vec2& c, const Vec2& to, int steps, float t, Color color);

		void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, Color color);
		void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, Color col0, Color col1, Color col2);
		void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& tex0, const Vec2& tex1, const Vec2& tex2, Color color);
		void tri(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& tex0, const Vec2& tex1, const Vec2& tex2, Color col0, Color col1, Color col2);

		void tri_line(const Vec2& a, const Vec2& b, const Vec2& c, float t, Color color);

		void rect(const Rect& rect, Color color);
		void rect_line(const Rect& rect, float t, Color color);
		void rect_rounded(const Rect& rect, float radius, int steps, Color color);
		void rect_rounded(const Rect& rect, float rtl, int rtl_steps, float rtr, int rtr_steps, float rbr, int rbr_steps, float rbl, int rbl_steps, Color color);
		void rect_rounded_line(const Rect& rect, float radius, int steps, float t, Color color);
		void rect_rounded_line(const Rect& rect, float rtl, int rtl_steps, float rtr, int rtr_steps, float rbr, int rbr_steps, float rbl, int rbl_steps, float t, Color color);

		void semi_circle(Vec2 center, float start_radians, float end_radians, float radius, int steps, Color centerColor, Color edgeColor);
		void semi_circle(Vec2 center, float start_radians, float end_radians, float radius, int steps, Color color);
		void semi_circle_line(Vec2 center, float start_radians, float end_radians, float radius, int steps, float t, Color color);

		void circle(const Vec2 center, float radius, int steps, Color color);
		void circle_line(const Vec2 center, float raidus, float t, int steps, Color color);

		void quad(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, Color color);
		void quad(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, Color col0, Color col1, Color col2, Color col3);
		void quad(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Vec2& tex0, const Vec2& tex1, const Vec2& tex2, const Vec2& tex3, Color color);
		void quad(const Vec2& pos0, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Vec2& tex0, const Vec2& tex1, const Vec2& tex2, const Vec2& tex3, Color col0, Color col1, Color col2, Color col3);
		void quad_line(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& d, float t, Color color);

		void arrow_head(const Vec2& point_pos, float radians, float side_len, Color color);
		void arrow_head(const Vec2& point_pos, const Vec2& from_pos, float side_len, Color color);

		void tex();
		void tex(const Subtexture& subtexture, const Vec2& pos, Color color);
		void tex(const Subtexture& subtexture, const Vec2& pos, const Vec2& origin, const Vec2& scale, float rotation, Color color);
		void str(const SpriteFont& font, const String& text, const Vec2& pos, Color color);
		void str(const SpriteFont& font, const String& text, const Vec2& pos, TextAlign align, float size, Color color);

		const char* matrix_uniform;
		const char* texture_uniform;

	private:

		struct Vertex
		{
			Vec2 pos;
			Vec2 tex;
			Color col;

			uint8_t mult;
			uint8_t wash;
			uint8_t fill;

			static VertexAttribute attributes[6];
		};

		struct DrawBatch
		{
			int layer;
			int offset;
			int elements;
			MaterialRef material;
			BlendMode blend;
			TextureRef texture;
			bool flip_vertically;
			Rect scissor;

			DrawBatch() :
				layer(0),
				offset(0),
				elements(0),
				blend(BlendMode::Normal),
				flip_vertically(false),
				scissor(0, 0, -1, -1) {}
		};

		static ShaderRef		m_default_shader;
		MaterialRef				m_default_material;
		MeshRef					m_mesh;
		Mat3x2					m_matrix;
		ColorMode				m_color_mode;
		uint8_t					m_tex_mult;
		uint8_t					m_tex_wash;
		DrawBatch				m_batch;
		Vector<Vertex>			m_vertices;
		Vector<int>				m_indices;
		Vector<Mat3x2>			m_matrix_stack;
		Vector<Rect>			m_scissor_stack;
		Vector<BlendMode>		m_blend_stack;
		Vector<MaterialRef>		m_material_stack;
		Vector<ColorMode>		m_color_mode_stack;
		Vector<int>				m_layer_stack;
		Vector<DrawBatch>		m_batches;

		void render_single_batch(RenderCall& call, const DrawBatch& b, const Mat4x4& matrix);
	};
}
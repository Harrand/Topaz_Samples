#include "render.hpp"
#include "gl/imported_shaders.hpp"
#include "core/matrix_transform.hpp"

#include ImportedShaderHeader(draw, vertex)
#include ImportedShaderHeader(draw, fragment)

namespace game
{
	void GameRenderInfo::set_camera(tz::Vec3 cam_pos, tz::Vec3 cam_rot)
	{
		this->camera_pos = cam_pos;
		this->camera_rot = cam_rot;
		this->view_matrix = tz::view(this->camera_pos, this->camera_rot);
	}
	
	void GameRenderInfo::update_dimensions(tz::Vec2 viewport_dimensions)
	{
		this->projection_matrix = tz::perspective(1.27f, viewport_dimensions[0] / viewport_dimensions[1], 0.001f, 100000.0f);
	}

	RenderState::RenderState():
	rinfo(),
	world_mesh(game::get_world_mesh()),
	world_buf(tz::gl2::BufferResource::from_many(this->world_mesh)),
	game_buf(tz::gl2::BufferResource::from_one(GameRenderInfo{}, tz::gl2::ResourceAccess::DynamicFixed)),
	stonebricks(game::get_image(game::ImageName::StoneBricks)),
	stonebricks_res(tz::gl2::ImageResource::from_memory(this->stonebricks.format, this->stonebricks.dimensions, this->stonebricks.image_data)),
	birch(game::get_image(game::ImageName::Birch)),
	birch_res(tz::gl2::ImageResource::from_memory(this->birch.format, this->birch.dimensions, this->birch.image_data)),
	wood(game::get_image(game::ImageName::Wood)),
	wood_res(tz::gl2::ImageResource::from_memory(this->wood.format, this->wood.dimensions, this->wood.image_data)),
	world_buffer_handle(tz::nullhand),
	game_buffer_handle(tz::nullhand)
	{
		tz_assert(stonebricks.success && birch.success && wood.success, "Failed to load one or more game images (Stonebricks = %d, Birch = %d, Wood = %d)", stonebricks.success, birch.success, wood.success);

		this->rinfo.shader().set_shader(tz::gl2::ShaderStage::Vertex, ImportedShaderSource(draw, vertex));
		this->rinfo.shader().set_shader(tz::gl2::ShaderStage::Fragment, ImportedShaderSource(draw, fragment));
		this->world_buffer_handle = this->rinfo.add_resource(this->world_buf);
		this->game_buffer_handle = this->rinfo.add_resource(this->game_buf);
		this->rinfo.add_resource(this->stonebricks_res);
		this->rinfo.add_resource(this->birch_res);
		this->rinfo.add_resource(this->wood_res);
	}

	const tz::gl2::RendererInfo& RenderState::info() const
	{
		return this->rinfo;
	}

	std::size_t RenderState::get_triangle_count() const
	{
		return this->world_mesh.size() / 3;
	}

	GameRenderInfo& RenderState::get_mutable_state(tz::gl2::Renderer& renderer)
	{
		return static_cast<tz::gl2::BufferResource*>(renderer.get_resource(this->game_buffer_handle))->data_as<GameRenderInfo>().front();
	}
}

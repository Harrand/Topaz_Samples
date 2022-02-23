#include "core/tz.hpp"
#include "gl/device.hpp"
#include "gl/resource.hpp"
#include "gl/renderer.hpp"
#include "gl/imported_shaders.hpp"

#include "resource.hpp"

#include ImportedShaderHeader(draw, vertex)
#include ImportedShaderHeader(draw, fragment)

int main()
{
	tz::initialise
	({
		.name = "Sample 001 - House",
		.window = {.flags = {.resizeable = false}}
	});
	{
		std::size_t triangle_count;
		// Create mesh for game world
		// TODO: Give it data!
		game::NaiveMesh world_mesh = game::get_world_mesh();
		const std::size_t world_size_bytes = world_mesh.size() * sizeof(game::TriangleVertexData);
		triangle_count = world_mesh.size() / 3;

		tz::gl2::BufferResource world_buffer = tz::gl2::BufferResource::from_many(world_mesh);

		// Now create all images upfront. We have three.
		samplelib::ImageImportResult stone_bricks_data = game::get_image(game::ImageName::StoneBricks);
		tz_assert(stone_bricks_data.success, "Failed to load game image");
		tz::gl2::ImageResource stone_bricks_resource = tz::gl2::ImageResource::from_memory(stone_bricks_data.format, stone_bricks_data.dimensions, stone_bricks_data.image_data);

		// Create renderer.
		tz::gl2::Device dev;
		tz::gl2::RendererInfo rinfo;
		rinfo.shader().set_shader(tz::gl2::ShaderStage::Vertex, ImportedShaderSource(draw, vertex));
		rinfo.shader().set_shader(tz::gl2::ShaderStage::Fragment, ImportedShaderSource(draw, fragment));
		rinfo.add_resource(world_buffer);
		rinfo.add_resource(stone_bricks_resource);
		tz::gl2::Renderer renderer = dev.create_renderer(rinfo);
		while(!tz::window().is_close_requested())
		{
			tz::window().update();
			renderer.render(triangle_count);
		}
	}
	tz::terminate();
	return 0;
}

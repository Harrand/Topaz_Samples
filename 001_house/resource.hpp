#ifndef SAMPLE_001_HOUSE_RESOURCES_HPP
#define SAMPLE_001_HOUSE_RESOURCES_HPP
#include "image_import.hpp" // samplelib
#include "model_import.hpp" // samplelib
#include "core/vector.hpp"
#include <vector>

namespace game
{
	struct TriangleVertexData
	{
		tz::Vec3 position;
		float pad0;
		tz::Vec2 texcoord;
		float pad1[2];
		tz::Vec3 normal;
		float pad2;
		tz::Vec3 tangent;
		float pad3;
		tz::Vec3 bitangent;
		float pad4;
	};

	using NaiveMesh = std::vector<TriangleVertexData>;

	enum class ImageName
	{
		StoneBricks,
		Birch,
		Wood
	};

	samplelib::ImageImportResult get_image(ImageName name);

	NaiveMesh get_world_mesh();
}

#endif // SAMPLE_001_HOUSE_RESOURCES_HPP

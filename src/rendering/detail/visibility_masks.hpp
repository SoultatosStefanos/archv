// Contains a private module for any package-wide renderable visibility mask.
// Used across viewports to quickly decide what to render.
// Soultatos Stefanos 2022

#ifndef RENDERING_DETAIL_VISIBILITY_MASKS_HPP
#define RENDERING_DETAIL_VISIBILITY_MASKS_HPP

#include <cstdint>

namespace rendering::detail
{

constexpr int32_t vertex_mesh_mask { 0X0001 };
constexpr int32_t vertex_text_mask { 0X0002 };
constexpr int32_t edge_mesh_mask { 0X0004 };
constexpr int32_t edge_text_mask { 0X0008 };
constexpr int32_t edge_tip_mask { 0X0010 };

constexpr int32_t particles_mask { 0X0020 };
constexpr int32_t minimap_mask { 0X0040 };
constexpr int32_t vertex_mask = vertex_mesh_mask | vertex_text_mask;
constexpr int32_t edge_mask = edge_mesh_mask | edge_text_mask | edge_tip_mask;

static_assert((particles_mask & vertex_mask) == 0);
static_assert((particles_mask & vertex_mesh_mask) == 0);
static_assert((particles_mask & vertex_text_mask) == 0);
static_assert((particles_mask & edge_mask) == 0);
static_assert((particles_mask & edge_mesh_mask) == 0);
static_assert((particles_mask & edge_tip_mask) == 0);
static_assert((particles_mask & edge_text_mask) == 0);
static_assert((particles_mask & minimap_mask) == 0);
static_assert((particles_mask & particles_mask) != 0);

static_assert((vertex_mask & vertex_mesh_mask) != 0);
static_assert((vertex_mask & vertex_text_mask) != 0);
static_assert((vertex_mask & edge_mesh_mask) == 0);
static_assert((vertex_mask & edge_text_mask) == 0);
static_assert((vertex_mask & edge_tip_mask) == 0);
static_assert((vertex_mask & edge_mask) == 0);
static_assert((vertex_mask & particles_mask) == 0);
static_assert((vertex_mask & minimap_mask) == 0);

static_assert((edge_mask & edge_mesh_mask) != 0);
static_assert((edge_mask & edge_text_mask) != 0);
static_assert((edge_mask & edge_tip_mask) != 0);
static_assert((edge_mask & vertex_mesh_mask) == 0);
static_assert((edge_mask & vertex_text_mask) == 0);
static_assert((edge_mask & vertex_mask) == 0);
static_assert((edge_mask & particles_mask) == 0);
static_assert((edge_mask & minimap_mask) == 0);

static_assert((vertex_mesh_mask & vertex_text_mask) == 0);
static_assert((edge_mesh_mask & edge_text_mask) == 0);
static_assert((edge_mesh_mask & edge_tip_mask) == 0);
static_assert((edge_text_mask & edge_tip_mask) == 0);

static_assert((vertex_mesh_mask & minimap_mask) == 0);
static_assert((vertex_text_mask & minimap_mask) == 0);
static_assert((edge_mesh_mask & minimap_mask) == 0);
static_assert((edge_text_mask & minimap_mask) == 0);
static_assert((edge_tip_mask & minimap_mask) == 0);

} // namespace rendering::detail

#endif // RENDERING_DETAIL_VISIBILITY_MASKS_HPP

#pragma once

#include <sol/sol.hpp>

#include <noggit/scripting/scripting_tool.hpp>
#include <noggit/scripting/script_image.hpp>
#include <noggit/scripting/script_context.hpp>
#include <noggit/scripting/script_filesystem.hpp>
#include <noggit/scripting/script_math.hpp>
#include <noggit/scripting/script_noise.hpp>
#include <noggit/scripting/script_random.hpp>
#include <noggit/scripting/script_selection.hpp>
#include <noggit/scripting/scripting_tool.hpp>
#include <noggit/scripting/script_brush.hpp>
#include <noggit/scripting/script_misc.hpp>
#include <noggit/scripting/script_chunk.hpp>
#include <noggit/scripting/script_model.hpp>
#include <noggit/scripting/script_vert.hpp>

void register_functions(sol::state * lua)
{
#define FUNC(func,str,...) lua->set_function(str,func)
#define FUNC_RETCLASS(func,str,...) lua->set_function(str,func)

    FUNC(noggit::scripting::rand_int32, "rand_int32", worstDefault);
    FUNC(noggit::scripting::random_from_seed, "random_from_seed", worstDefault);
    FUNC(noggit::scripting::random_from_time, "random_from_time", worstDefault);
    FUNC(noggit::scripting::rand_uint32, "rand_uint32", worstDefault);
    FUNC(noggit::scripting::rand_double, "rand_double", worstDefault);

    // script_noise.hpp
    FUNC(noggit::scripting::noise_get, "noise_get", worstDefault);
    FUNC(noggit::scripting::noise_set, "noise_set", worstDefault);
    FUNC(noggit::scripting::noise_start, "noise_start", worstDefault);
    FUNC(noggit::scripting::noise_width, "width", worstDefault);
    FUNC(noggit::scripting::noise_height, "height", worstDefault);
    FUNC(noggit::scripting::noise_is_highest, "noise_is_highest", worstDefault);
    FUNC(noggit::scripting::make_noise_selection, "make_noise_selection", worstDefault);

    // script_selections.hpp
    FUNC_RETCLASS(noggit::scripting::select_origin, "select_origin", worstDefault);
    FUNC_RETCLASS(noggit::scripting::select_between, "select_between", worstDefault);
    FUNC(noggit::scripting::sel_next_chunk, "sel_next_chunk", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_get_chunk, "sel_get_chunk", worstDefault);
    FUNC(noggit::scripting::sel_reset_chunk_itr, "sel_reset_chunk_itr", worstDefault);
    FUNC(noggit::scripting::sel_next_model, "sel_next_model", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_get_model, "sel_get_model", worstDefault);
    FUNC(noggit::scripting::sel_reset_model_itr, "sel_reset_model_itr", worstDefault);
    FUNC(noggit::scripting::sel_requery_models, "sel_requery_model", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_center, "sel_center", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_min, "sel_min", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_max, "sel_max", worstDefault);
    FUNC_RETCLASS(noggit::scripting::sel_size, "sel_size", worstDefault);

    // script_chunk.hpp
    FUNC(noggit::scripting::chunk_set_hole, "chunk_set_hole", worstDefault);
    FUNC(noggit::scripting::chunk_remove_texture, "chunk_remove_texture", worstDefault);
    FUNC(noggit::scripting::chunk_get_texture, "chunk_get_texture", worstDefault);
    FUNC(noggit::scripting::chunk_add_texture, "chunk_add_texture", worstDefault);
    FUNC(noggit::scripting::chunk_clear_textures, "chunk_clear_textures", worstDefault);
    FUNC(noggit::scripting::chunk_apply_textures, "chunk_apply_textures", worstDefault);
    FUNC(noggit::scripting::chunk_apply_heightmap, "chunk_apply_heightmap", worstDefault);
    FUNC(noggit::scripting::chunk_apply_vertex_color, "chunk_apply_vertex_color", worstDefault);
    FUNC(noggit::scripting::chunk_apply_all, "chunk_apply_all", worstDefault);
    FUNC(noggit::scripting::chunk_set_impassable, "chunk_set_impassable", worstDefault);
    FUNC(noggit::scripting::chunk_get_area_id, "chunk_get_area_id", worstDefault);
    FUNC(noggit::scripting::chunk_set_area_id, "chunk_set_area_id", worstDefault);
    FUNC(noggit::scripting::chunk_next_vert, "chunk_next_vert", worstDefault);
    FUNC(noggit::scripting::chunk_next_tex, "chunk_next_tex", worstDefault);
    FUNC(noggit::scripting::chunk_reset_vert_itr, "chunk_reset_vert_itr", worstDefault);
    FUNC(noggit::scripting::chunk_reset_tex_itr, "chunk_reset_tex_itr", worstDefault);
    FUNC(noggit::scripting::chunk_clear_colors, "chunk_clear_colors", worstDefault);
    FUNC_RETCLASS(noggit::scripting::chunk_get_vert, "chunk_get_vert", worstDefault);
    FUNC_RETCLASS(noggit::scripting::chunk_get_tex, "chunk_get_tex", worstDefault);

    // script_vert.hpp
    FUNC_RETCLASS(noggit::scripting::vert_get_pos, "vert_get_pos", worstDefault);
    FUNC(noggit::scripting::vert_set_height, "vert_set_height", worstDefault);
    FUNC(noggit::scripting::vert_set_hole, "vert_set_hole", worstDefault);
    FUNC(noggit::scripting::vert_set_alpha, "vert_set_alpha", worstDefault);
    FUNC(noggit::scripting::vert_get_alpha, "vert_get_alpha", worstDefault);
    FUNC(noggit::scripting::vert_next_tex, "vert_next_tex", worstDefault);
    FUNC(noggit::scripting::vert_reset_tex, "vert_reset_tex", worstDefault);
    FUNC_RETCLASS(noggit::scripting::vert_get_tex, "vert_get_tex", worstDefault);
    FUNC(noggit::scripting::vert_is_water_aligned, "vert_is_water_aligned", worstDefault);
    FUNC(noggit::scripting::tex_get_alpha, "tex_get_alpha", worstDefault);
    FUNC_RETCLASS(noggit::scripting::tex_get_pos_2d, "tex_get_pos_2d", worstDefault);

    // script_model.hpp
    FUNC_RETCLASS(noggit::scripting::model_get_pos, "model_get_pos", worstDefault);
    FUNC(noggit::scripting::model_set_pos, "model_set_pos", worstDefault);
    FUNC_RETCLASS(noggit::scripting::model_get_rot, "model_get_rot", worstDefault);
    FUNC(noggit::scripting::model_set_rot, "model_set_rot", worstDefault);
    FUNC(noggit::scripting::model_get_scale, "model_get_scale", worstDefault);
    FUNC(noggit::scripting::model_get_uid, "model_get_uid", worstDefault);
    FUNC(noggit::scripting::model_remove, "model_remove", worstDefault);
    FUNC(noggit::scripting::model_get_filename, "model_get_filename", worstDefault);
    FUNC(noggit::scripting::model_replace, "model_replace", worstDefault);

    // script_math.hpp
    FUNC(noggit::scripting::round, "round", worstDefault);
    FUNC(noggit::scripting::pow, "pow", worstDefault);
    FUNC(noggit::scripting::log10, "log10", worstDefault);
    FUNC(noggit::scripting::log, "log", worstDefault);
    FUNC(noggit::scripting::ceil, "ceil", worstDefault);
    FUNC(noggit::scripting::floor, "floor", worstDefault);
    FUNC(noggit::scripting::exp, "exp", worstDefault);
    FUNC(noggit::scripting::cbrt, "cbrt", worstDefault);
    FUNC(noggit::scripting::acosh, "acosh", worstDefault);
    FUNC(noggit::scripting::asinh, "asinh", worstDefault);
    FUNC(noggit::scripting::atanh, "atanh", worstDefault);
    FUNC(noggit::scripting::cosh, "cosh", worstDefault);
    FUNC(noggit::scripting::sinh, "sinh", worstDefault);
    FUNC(noggit::scripting::tanh, "tanh", worstDefault);
    FUNC(noggit::scripting::acos, "acos", worstDefault);
    FUNC(noggit::scripting::asin, "asin", worstDefault);
    FUNC(noggit::scripting::atan, "atan", worstDefault);
    FUNC(noggit::scripting::cos, "cos", worstDefault);
    FUNC(noggit::scripting::sin, "sin", worstDefault);
    FUNC(noggit::scripting::tan, "tan", worstDefault);
    FUNC(noggit::scripting::sqrt, "sqrt", worstDefault);
    FUNC(noggit::scripting::dist_2d, "dist_2d", worstDefault);
    FUNC(noggit::scripting::dist_2d_compare, "dist_2d_compare", worstDefault);

    // script_context.hpp
    FUNC_RETCLASS(noggit::scripting::pos, "pos", worstDefault);
    FUNC(noggit::scripting::brush_change_terrain, "brush_change_terrain", worstDefault);
    FUNC(noggit::scripting::add_wmo, "add_wmo", worstDefault);
    FUNC(noggit::scripting::get_map_id, "get_map_id", worstDefault);
    FUNC(noggit::scripting::get_area_id, "get_area_id", worstDefault);
    FUNC(noggit::scripting::brush_set_area_id, "brush_set_area_id", worstDefault);
    FUNC(noggit::scripting::brush_change_vertex_color, "brush_change_vertex_color", worstDefault);
    FUNC_RETCLASS(noggit::scripting::brush_get_vertex_color, "brush_get_vertex_color", worstDefault);
    FUNC(noggit::scripting::brush_flatten_terrain, "brush_flatten_terrain", worstDefault);
    FUNC(noggit::scripting::brush_blur_terrain, "brush_blur_terrain", worstDefault);
    FUNC(noggit::scripting::brush_erase_textures, "brush_erase_texture", worstDefault);
    FUNC(noggit::scripting::brush_clear_shadows, "brush_clear_shadows", worstDefault);
    FUNC(noggit::scripting::brush_clear_textures, "brush_clear_textures", worstDefault);
    FUNC(noggit::scripting::brush_clear_height, "brush_clear_height", worstDefault);
    FUNC(noggit::scripting::brush_set_hole, "brush_set_hole", worstDefault);
    FUNC(noggit::scripting::brush_set_hole_adt, "brush_set_hole_adt", worstDefault);
    FUNC(noggit::scripting::brush_deselect_vertices, "brush_deselect_vertices", worstDefault);
    FUNC(noggit::scripting::brush_clear_vertex_selection, "brush_clear_vertex_selection", worstDefault);
    FUNC(noggit::scripting::brush_move_vertices, "brush_move_vertices", worstDefault);
    FUNC(noggit::scripting::brush_flatten_vertices, "brush_flatten_vertices", worstDefault);
    FUNC(noggit::scripting::brush_update_vertices, "brush_update_vertices", worstDefault);
    FUNC(noggit::scripting::brush_paint_texture, "brush_paint_texture", worstDefault);
    FUNC(noggit::scripting::cam_pitch, "cam_pitch", worstDefault);
    FUNC(noggit::scripting::cam_yaw, "cam_yaw", worstDefault);
    FUNC(noggit::scripting::outer_radius, "outer_radius", worstDefault);
    FUNC(noggit::scripting::inner_radius, "inner_radius", worstDefault);
    FUNC(noggit::scripting::holding_alt, "holding_alt", worstDefault);
    FUNC(noggit::scripting::holding_shift, "holding_shift", worstDefault);
    FUNC(noggit::scripting::holding_ctrl, "holding_ctrl", worstDefault);
    FUNC(noggit::scripting::holding_space, "holding_space", worstDefault);
    FUNC(noggit::scripting::dt, "dt", worstDefault);
}
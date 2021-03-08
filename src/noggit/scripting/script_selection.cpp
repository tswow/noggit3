// This file is part of Noggit3, licensed under GNU General Public License (version 3).
#include <daScript/daScript.h>

#include <noggit/scripting/script_selection.hpp>
#include <noggit/scripting/script_context.hpp>
#include <noggit/scripting/script_heap.hpp>
#include <noggit/scripting/script_exception.hpp>
#include <noggit/World.h>

namespace noggit
{
  namespace scripting
  {
    static selection select_between_int(const char* caller,math::vector_3d const& point1, math::vector_3d const& point2)
    {
      selection sel;
      sel._world = get_ctx(caller)->_world;
      sel._min = math::vector_3d(
        std::min(point1.x, point2.x),
        std::min(point1.y, point2.y),
        std::min(point1.z, point2.z));

      sel._max = math::vector_3d(
        std::max(point1.x, point2.x),
        std::max(point1.y, point2.y),
        std::max(point1.z, point2.z));

      sel._size = sel._max - sel._min;
      sel._half_size = sel._size/2;
      sel._center = sel._min + sel._half_size;
      sel._models = model_iterator(sel._world, sel._min, sel._max);
      return sel;
    }

    selection select_origin(math::vector_3d const& origin, float xRadius, float zRadius)
    {
      return select_between_int("select_origin",
               math::vector_3d(origin.x - xRadius, 0, origin.z - zRadius),
               math::vector_3d(origin.x + xRadius, 0, origin.z + zRadius));
    }

    selection select_between(math::vector_3d const& point1, math::vector_3d const& point2)
    {
      return select_between_int("select_between",point1,point2);
    }

    math::vector_3d sel_center(selection const& sel) { return sel._center; }
    math::vector_3d sel_min(selection const& sel) { return sel._min; }
    math::vector_3d sel_max(selection const& sel) { return sel._max; }
    math::vector_3d sel_size(selection const& sel) { return sel._size; }

    static bool is_on_chunk(selection& sel)
    {
      return sel._cur_chunk < sel._chunks_size;
    }

    bool sel_next_chunk(selection& sel, das::Context* ctx)
    {
      if (!sel._initialized_chunks)
      {
        std::vector<MapChunk*> chunks;
        sel._world->select_all_chunks_between(sel._min, sel._max, chunks);
        sel._chunks = script_calloc(sizeof(MapChunk*) * chunks.size(), ctx);
        sel._chunks_size = chunks.size();
        memcpy(sel.get_chunks(), chunks.data(), chunks.size() * sizeof(MapChunk*));
        sel._initialized_chunks = true;
      }

      ++sel._cur_chunk;
      return is_on_chunk(sel);
    }

    void sel_reset_chunk_itr(selection& sel)
    {
      sel._cur_chunk = -1;
    }

    chunk sel_get_chunk(selection& sel)
    {
      if(!is_on_chunk(sel))
      {
        throw script_exception(
          "sel_get_chunk",
          "accessing invalid chunk: iterator is done");
      }
      return chunk(sel.get_chunks()[sel._cur_chunk]);
    }

    bool sel_next_model(selection& sel, das::Context * ctx)
    {
      return sel._models.next(ctx);
    }

    model sel_get_model(selection& sel)
    {
      return sel._models.get();
    }

    void sel_reset_model_itr(selection& sel)
    {
      sel._models.reset_itr();
    }

    void sel_requery_models(selection& sel, das::Context* ctx)
    {
      sel._models.query(ctx);
    }
  } // namespace scripting
} // namespace noggit

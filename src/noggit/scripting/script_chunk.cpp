// This file is part of Noggit3, licensed under GNU General Public License (version 3).
#include <daScript/daScript.h> // must be on top

#include <noggit/scripting/script_chunk.hpp>
#include <noggit/scripting/script_selection.hpp>
#include <noggit/scripting/script_context.hpp>
#include <noggit/scripting/script_heap.hpp>
#include <noggit/scripting/script_exception.hpp>
#include <noggit/MapChunk.h>
#include <noggit/MapHeaders.h>
#include <noggit/World.h>

#include <boost/algorithm/string/predicate.hpp>

namespace noggit
{
  namespace scripting
  {
    chunk::chunk(selection* sel, MapChunk* chunk)
    {
      _sel = sel;
      _chunk = chunk;
    }

    void chunk_set_hole(chunk& chunk, bool hole)
    {
      chunk._chunk->setHole(math::vector_3d(0, 0, 0), true, hole);
    }

    int chunk_add_texture(chunk& chunk, char const* texture)
    {
      if(texture==nullptr)
      {
        throw script_exception(
          "chunk_add_texture",
          "empty texture");
      }
      std::string tex = std::string(texture);
      return chunk._chunk->texture_set->addTexture(scoped_blp_texture_reference(tex));
    }

    void chunk_clear_textures(chunk& chunk)
    {
      chunk._chunk->texture_set->eraseTextures();
    }

    void chunk_remove_texture(chunk& chunk, int index)
    {
      if(index<0||index>3) 
      {
        throw script_exception(
          "chunk_remove_texture",
          "invalid texture index, must be between 0-3");
      }
      chunk._chunk->texture_set->eraseTexture(index);
    }

    char const* chunk_get_texture(chunk const& chunk, int index, das::Context * ctx)
    {
      if(index<0||index>3)
      {
        throw script_exception(
          "chunk_get_texture",
          "invalid texture index, must be between 0-3");
      }
      return script_calloc_string(chunk._chunk->texture_set->texture(index)->filename,ctx);
    }

    void chunk_apply_heightmap(chunk& chunk)
    {
      chunk._chunk->updateVerticesData();
      get_ctx("chunk_apply_heightmap")->_world->recalc_norms(chunk._chunk);
    }

    void chunk_apply_textures(chunk& chunk)
    {
      chunk._chunk->texture_set->apply_alpha_changes();
    }

    void chunk_apply_vertex_color(chunk& chunk)
    {
      chunk._chunk->UpdateMCCV();
    }

    void chunk_apply_all(chunk& chunk)
    {
      chunk_apply_heightmap(chunk);
      chunk_apply_textures(chunk);
      chunk_apply_vertex_color(chunk);
    }

    int chunk_get_area_id(chunk const& chunk)
    {
      return chunk._chunk->getAreaID();
    }

    void chunk_set_area_id(chunk& chunk, int value)
    {
      return chunk._chunk->setAreaID(value);
    }

    void chunk_set_impassable(chunk& chunk, bool add)
    {
      chunk._chunk->setFlag(add, 0x2);
    }

    static bool is_on_vert(chunk& chunk)
    {
      return chunk._vert_index < 145;
    }

    static bool is_on_tex(chunk& chunk)
    {
      return chunk._tex_index < 4096;
    }

    static void skip_vertices(chunk& chunk)
    {
      auto sel = chunk._sel;
      if (sel == nullptr)
        return;
      while (is_on_vert(chunk))
      {
        auto& vert = chunk._chunk->mVertices[chunk._vert_index];
        if (vert.x <= sel->_min.x || vert.x >= sel->_max.x ||
          vert.z <= sel->_min.z || vert.z >= sel->_max.z)
        {
          ++chunk._vert_index;
        }
        else
        {
          break;
        }
      }
    }

    bool chunk_next_vert(chunk& chunk)
    {
      ++chunk._vert_index;
      if (is_on_vert(chunk))
      {
        skip_vertices(chunk);
        return is_on_vert(chunk);
      }
      else
      {
        return false;
      }
    }

    bool chunk_next_tex(chunk& chunk)
    {
      ++chunk._tex_index;
      return is_on_tex(chunk);
    }

    void chunk_reset_vert_itr(chunk& chunk)
    {
      chunk._vert_index = -1;
    }

    void chunk_reset_tex_itr(chunk& chunk)
    {
      chunk._tex_index = -1;
    }

    vert chunk_get_vert(chunk const& chunk)
    {
      return vert(chunk._chunk, chunk._vert_index);
    }

    tex chunk_get_tex(chunk const& chunk)
    {
      return tex(chunk._chunk, chunk._tex_index);
    }

    void chunk_clear_colors(chunk& chunk)
    {
      std::fill (
        chunk._chunk->mccv, 
        chunk._chunk->mccv + mapbufsize, 
        math::vector_3d (1.f, 1.f, 1.f)
      );
    }
    
    //! \todo should probably be tied to the context
    static std::set<std::pair<float,float>> overlay_chunks;

    static unsigned long dirty_counter = 0;
    void chunk_set_overlay(chunk& chunk, bool draw)
    {
      if(draw)
      {
        overlay_chunks.insert(std::make_pair(chunk._chunk->xbase,chunk._chunk->zbase));
      }
      else
      {
        overlay_chunks.erase(std::make_pair(chunk._chunk->xbase,chunk._chunk->zbase));
      }
      ++dirty_counter;
    }

    void chunk_clear_overlays()
    {
      overlay_chunks.clear();
      ++dirty_counter;
    }

    bool chunk_has_overlay(chunk& chnk)
    {
      return chunk_has_overlay_int(chnk._chunk);
    }

    bool chunk_has_overlay_int(MapChunk* chnk)
    {
      if(chnk->_script_overlay_dirty_ctr==dirty_counter)
      {
        return chnk->_script_overlay_last_draw;
      }
      bool value = overlay_chunks.find(std::make_pair(chnk->xbase,chnk->zbase)) 
        != overlay_chunks.end();
      chnk->_script_overlay_last_draw = value;
      return value;
    }
  } // namespace scripting
} // namespace noggit
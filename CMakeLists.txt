# This file is part of Noggit3, licensed under GNU General Public License (version 3).

cmake_minimum_required(VERSION 3.18)
cmake_policy (SET CMP0057 NEW) # "Support new IN_LIST if() operator."
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(Noggit)


include (CheckCXXCompilerFlag)
macro (add_compiler_flag_if_supported _VAR _FLAG)
  string (MAKE_C_IDENTIFIER "CXX_COMPILER_SUPPORTS_${_FLAG}" _test_variable)
  check_cxx_compiler_flag ("${_FLAG}" ${_test_variable})
  if (${_test_variable})
    list (APPEND ${_VAR} "${_FLAG}")
  endif()
endmacro()

set (noggit_warning_flags "")

add_compiler_flag_if_supported (noggit_warning_flags -fcolor-diagnostics)

add_compiler_flag_if_supported (noggit_warning_flags -Werror=reorder)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=conditional-uninitialized)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=unused)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=unused-parameter)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=unused-private-field)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=missing-variable-declarations)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=missing-prototypes)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=deprecated)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=missing-braces)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=inconsistent-missing-override)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=parentheses)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=shift-sign-overflow)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=string-plus-int)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=self-assign-field)

# covered by CMAKE_CXX_STANDARD
add_compiler_flag_if_supported (noggit_warning_flags -Wno-c++98-compat)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-c++98-compat-pedantic)

# covered by compilers used
add_compiler_flag_if_supported (noggit_warning_flags -Wno-gnu-anonymous-struct)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-variadic-macros)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-vla)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-vla-extension)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-zero-length-array)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-gnu-zero-variadic-macro-arguments)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-nested-anon-types)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-four-char-constants)

# we assume that our constructors and destructors do not access other global state
add_compiler_flag_if_supported (noggit_warning_flags -Wno-exit-time-destructors)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-global-constructors)

# is fine with GNU, required due to our libstdc
add_compiler_flag_if_supported (noggit_warning_flags -Wno-disabled-macro-expansion)

# we can live with the compilation unit containing the vtable not being fixed
add_compiler_flag_if_supported (noggit_warning_flags -Wno-weak-vtables)
add_compiler_flag_if_supported (noggit_warning_flags -Wno-weak-template-vtables)

# __DATE__ and __TIME__ not being reproducible is exactly why they exist.
add_compiler_flag_if_supported (noggit_warning_flags -Wno-date-time)

# we don't care for a few bytes
add_compiler_flag_if_supported (noggit_warning_flags -Wno-padded)

# msvc++ mangles struct/class into name, thus symbols may be called differently
# with a bad forward-decl. we want compilation to fail, not linking.
add_compiler_flag_if_supported (noggit_warning_flags /we4099)
add_compiler_flag_if_supported (noggit_warning_flags -Werror=mismatched-tags)

# yes, we intend to use multi-character character constants
add_compiler_flag_if_supported (noggit_warning_flags -Wno-multichar)

if(WIN32)
  OPTION(NAME_REUSE_AS_ERROR "Make name reuse warnings errors ?" OFF)
  if(NAME_REUSE_AS_ERROR)
    # declaration of 'identifier' hides previous:
    add_compiler_flag_if_supported (noggit_warning_flags /we4456) # local declaration
    add_compiler_flag_if_supported (noggit_warning_flags /we4457) # function parameter
    add_compiler_flag_if_supported (noggit_warning_flags /we4458) # class members
    add_compiler_flag_if_supported (noggit_warning_flags /we4459) # global declaration
  endif()
endif()

# better exception handling for visual studio, particularly for the asynchronous stuff
add_compiler_flag_if_supported (noggit_warning_flags /EHa)
# multi core building for visual studio
add_compiler_flag_if_supported (noggit_warning_flags /MP)

if(WIN32)
  # Disable opengl error log
  OPTION(ADDITIONAL_OPTIMIZATION_FLAGS "Enable OpenGL error check ?" OFF)
  IF(ADDITIONAL_OPTIMIZATION_FLAGS)
    MESSAGE( STATUS "Enabled additional optimization flags for msvc.")
    add_compiler_flag_if_supported (CMAKE_CXX_FLAGS /Ob2) # inline any suitable functions
    add_compiler_flag_if_supported (CMAKE_CXX_FLAGS /Oi)  # enable intrasic functions
    add_compiler_flag_if_supported (CMAKE_CXX_FLAGS /Ot)  # favor fast code
    add_compiler_flag_if_supported (CMAKE_CXX_FLAGS /GL)  # whole program optimization
  ENDIF()
endif(WIN32)


SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBARY_OUTPUT_PATH bin)

OPTION(USE_SQL "Enable sql uid save ? (require mysql installed)" OFF)

macro(includePlattform SUFFIX)
  if(UNIX)
    if(APPLE)
      include("${CMAKE_SOURCE_DIR}/cmake/apple_${SUFFIX}.cmake")
    else(APPLE)
      include("${CMAKE_SOURCE_DIR}/cmake/linux_${SUFFIX}.cmake")
    endif(APPLE)
  else(UNIX)
    if(WIN32)
      include("${CMAKE_SOURCE_DIR}/cmake/win32_${SUFFIX}.cmake")
      # adds for library repo
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/../Noggit3libs/Boost/lib/")
      #storm lib
      set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/../Noggit3libs/StormLib/include/")
      #boost
      include_directories (SYSTEM "${CMAKE_SOURCE_DIR}/../Noggit3libs/Boost/")
      set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/../Noggit3libs/Boost/libs/")
      set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/../Noggit3libs/Boost/")
	  endif(WIN32)
  endif(UNIX)
endmacro(includePlattform)

OPTION(VALIDATE_OPENGL_PROGRAMS "Validate Opengl programs" ON)


if(VALIDATE_OPENGL_PROGRAMS)
  add_definitions ( -DVALIDATE_OPENGL_PROGRAMS)
  
  if(APPLE)
    message(WARNING "Noggit will most likely not work on a mac with this option enabled.")
  endif()
endif()

includePlattform("prefind")

FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( Boost 1.60 COMPONENTS thread filesystem system unit_test_framework REQUIRED )
find_package (Qt5 COMPONENTS Widgets OpenGL OpenGLExtensions)

if (USE_SQL)
  find_library(MYSQL_LIBRARY
    NAMES libmysql
    HINTS "${CMAKE_SOURCE_DIR}/../Noggit3libs/mysql")
  find_library(MYSQLCPPCONN_LIBRARY
    NAMES mysqlcppconn
    HINTS "${CMAKE_SOURCE_DIR}/../Noggit3libs/mysql/connector")
  find_path (MYSQLCPPCONN_INCLUDE
    NAMES cppconn/driver.h
    HINTS "${CMAKE_SOURCE_DIR}/../Noggit3libs/mysql/connector")

  if (MYSQL_LIBRARY AND MYSQLCPPCONN_LIBRARY AND MYSQLCPPCONN_INCLUDE)
    add_definitions ( -DUSE_MYSQL_UID_STORAGE )

    set ( mysql_sources
          src/mysql/mysql.cpp
        )

    set ( mysql_headers
          src/mysql/mysql.h
        )
    source_group("mysql"  FILES ${mysql_sources} ${mysql_headers})
  else()
     message (FATAL_ERROR "MySQL lib or connector not found")
  endif()
endif()

add_subdirectory (src/external/qt-color-widgets)

# Add the found include directories to our include list.
include_directories (SYSTEM "${CMAKE_SOURCE_DIR}/include/")

OPTION(NOGGIT_ALL_WARNINGS "Enable all warnings?" OFF)

# Log to console for easier debugging.
OPTION( NOGGIT_LOGTOCONSOLE "Log to console instead of log.txt?" OFF )
IF( NOGGIT_LOGTOCONSOLE )
  MESSAGE( STATUS "And writing log to console instead of log.txt" )
  ADD_DEFINITIONS( -DDEBUG__LOGGINGTOCONSOLE )
ENDIF( NOGGIT_LOGTOCONSOLE )

# Disable opengl error log
OPTION(NOGGIT_OPENGL_ERROR_CHECK "Enable OpenGL error check ?" ON)
IF(NOT NOGGIT_OPENGL_ERROR_CHECK )
  MESSAGE( STATUS "OpenGL error check disabled." )
  ADD_DEFINITIONS( -DNOGGIT_DO_NOT_CHECK_FOR_OPENGL_ERRORS )
ENDIF()

includePlattform("postfind")

option (NOGGIT_WITH_SCRIPTING "Enable scripting support?" ON)

include_directories ("${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/tmp")

# And do the job.
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src" )

set ( noggit_root_sources
      src/noggit/AsyncLoader.cpp
      src/noggit/Brush.cpp
      src/noggit/ChunkWater.cpp
      src/noggit/cursor_render.cpp
      src/noggit/DBC.cpp
      src/noggit/DBCFile.cpp
      src/noggit/Log.cpp
      src/noggit/MPQ.cpp
      src/noggit/MapChunk.cpp
      src/noggit/MapTile.cpp
      src/noggit/MapView.cpp
      src/noggit/Misc.cpp
      src/noggit/Model.cpp
      src/noggit/ModelInstance.cpp
      src/noggit/ModelManager.cpp
      src/noggit/Particle.cpp
      src/noggit/Sky.cpp
      src/noggit/TextureManager.cpp
      src/noggit/TileWater.cpp
      src/noggit/WMO.cpp
      src/noggit/WMOInstance.cpp
      src/noggit/World.cpp
      src/noggit/alphamap.cpp
      src/noggit/application.cpp
      src/noggit/camera.cpp
      src/noggit/error_handling.cpp
      src/noggit/liquid_layer.cpp
      src/noggit/liquid_render.cpp
      src/noggit/map_horizon.cpp
      src/noggit/map_index.cpp
      src/noggit/texture_set.cpp
      src/noggit/uid_storage.cpp
      src/noggit/wmo_liquid.cpp
      src/noggit/world_model_instances_storage.cpp
      src/noggit/world_tile_update_queue.cpp
    )

set ( noggit_ui_sources
      src/noggit/ui/About.cpp
      src/noggit/ui/clickable_label.cpp
      src/noggit/ui/CurrentTexture.cpp
      src/noggit/ui/CursorSwitcher.cpp
      src/noggit/ui/DetailInfos.cpp
      src/noggit/ui/FlattenTool.cpp
      src/noggit/ui/font_awesome.cpp
      src/noggit/ui/font_noggit.cpp
      src/noggit/ui/Help.cpp
      src/noggit/ui/HelperModels.cpp
      src/noggit/ui/ModelImport.cpp
      src/noggit/ui/ObjectEditor.cpp
      src/noggit/ui/RotationEditor.cpp
      src/noggit/ui/TexturePicker.cpp
      src/noggit/ui/texture_swapper.cpp
      src/noggit/ui/TexturingGUI.cpp
      src/noggit/ui/texturing_tool.cpp
      src/noggit/ui/Toolbar.cpp
      src/noggit/ui/SettingsPanel.cpp
      src/noggit/ui/Water.cpp
      src/noggit/ui/ZoneIDBrowser.cpp
      src/noggit/ui/main_window.cpp
      src/noggit/ui/minimap_widget.cpp
      src/noggit/ui/shader_tool.cpp
      src/noggit/ui/terrain_tool.cpp
      src/noggit/ui/uid_fix_window.cpp
      src/noggit/ui/texture_palette_small.cpp
      src/noggit/ui/TextureList.cpp
    )

set ( math_sources
      src/math/bounding_box.cpp
      src/math/frustum.cpp
      src/math/matrix_4x4.cpp
      src/math/ray.cpp
      src/math/vector_2d.cpp
    )

set ( opengl_sources
      src/opengl/context.cpp
      src/opengl/primitives.cpp
      src/opengl/shader.cpp
      src/opengl/texture.cpp
    )

if (NOT APPLE)
  set_property (SOURCE "src/opengl/context.cpp" PROPERTY
    COMPILE_DEFINITIONS NOGGIT_OPENGL_SUPPORTS_CPU_INDICES_IN_DRAW_ELEMENTS)
endif()

set ( util_sources
      src/util/exception_to_string.cpp
    )

set ( noggit_root_headers
      src/noggit/Animated.h
      src/noggit/AsyncLoader.h
      src/noggit/AsyncObject.h
      src/noggit/Brush.h
      src/noggit/camera.hpp
      src/noggit/ChunkWater.hpp
      src/noggit/cursor_render.hpp
      src/noggit/DBC.h
      src/noggit/DBCFile.h
      src/noggit/Log.h
      src/noggit/MPQ.h
      src/noggit/map_enums.hpp
      src/noggit/MapChunk.h
      src/noggit/MapHeaders.h
      src/noggit/MapTile.h
      src/noggit/MapView.h
      src/noggit/Misc.h
      src/noggit/Model.h
      src/noggit/ModelHeaders.h
      src/noggit/ModelInstance.h
      src/noggit/ModelManager.h
      src/noggit/Particle.h
      src/noggit/Selection.h
      src/noggit/Sky.h
      src/noggit/TextureManager.h
      src/noggit/TileWater.hpp
      src/noggit/WMO.h
      src/noggit/WMOInstance.h
      src/noggit/World.h
      src/noggit/alphamap.hpp
      src/noggit/errorHandling.h
      src/noggit/liquid_layer.hpp
      src/noggit/liquid_render.hpp
      src/noggit/map_horizon.h
      src/noggit/map_index.hpp
      src/noggit/multimap_with_normalized_key.hpp
      src/noggit/texture_set.hpp
      src/noggit/tile_index.hpp
      src/noggit/tool_enums.hpp
      src/noggit/uid_storage.hpp
      src/noggit/wmo_liquid.hpp
      src/noggit/world_model_instances_storage.hpp
      src/noggit/world_tile_update_queue.hpp
    )

set ( noggit_ui_headers
      src/noggit/ui/About.h
      src/noggit/ui/clickable_label.hpp
      src/noggit/ui/CurrentTexture.h
      src/noggit/ui/CursorSwitcher.h
      src/noggit/ui/DetailInfos.h
      src/noggit/ui/FlattenTool.hpp
      src/noggit/ui/font_awesome.hpp
      src/noggit/ui/font_noggit.hpp
      src/noggit/ui/Help.h
      src/noggit/ui/HelperModels.h
      src/noggit/ui/ModelImport.h
      src/noggit/ui/ObjectEditor.h
      src/noggit/ui/RotationEditor.h
      src/noggit/ui/TexturePicker.h
      src/noggit/ui/texture_swapper.hpp
      src/noggit/ui/TexturingGUI.h
      src/noggit/ui/texturing_tool.hpp
      src/noggit/ui/Toolbar.h
      src/noggit/ui/SettingsPanel.h
      src/noggit/ui/Water.h
      src/noggit/ui/ZoneIDBrowser.h
      src/noggit/ui/main_window.hpp
      src/noggit/ui/minimap_widget.hpp
      src/noggit/ui/shader_tool.hpp
      src/noggit/ui/terrain_tool.hpp
      src/noggit/ui/uid_fix_window.hpp
      src/noggit/ui/texture_palette_small.hpp
      src/noggit/ui/TextureList.hpp
    )

set ( math_headers
      src/math/bounding_box.hpp
      src/math/constants.hpp
      src/math/frustum.hpp
      src/math/interpolation.hpp
      src/math/matrix_4x4.hpp
      src/math/projection.hpp
      src/math/quaternion.hpp
      src/math/ray.hpp
      src/math/trig.hpp
      src/math/vector_2d.hpp
      src/math/vector_3d.hpp
      src/math/vector_4d.hpp
    )

set ( scripting_sources
    src/noggit/scripting/script_brush.cpp
    src/noggit/scripting/script_chunk.cpp
    src/noggit/scripting/script_context.cpp
    src/noggit/scripting/script_filesystem.cpp
    src/noggit/scripting/script_heap.cpp
    src/noggit/scripting/script_image.cpp
    src/noggit/scripting/script_loader-noggit_module.ipp
    src/noggit/scripting/script_loader.cpp
    src/noggit/scripting/script_math.cpp
    src/noggit/scripting/script_misc.cpp
    src/noggit/scripting/script_model.cpp
    src/noggit/scripting/script_noise.cpp
    src/noggit/scripting/script_random.cpp
    src/noggit/scripting/script_selection.cpp
    src/noggit/scripting/script_vert-script_texture_index.ipp
    src/noggit/scripting/script_vert.cpp
    src/noggit/scripting/scripting_tool.cpp
  )

set ( scripting_headers
    src/noggit/scripting/script_filesystem.hpp
    src/noggit/scripting/script_noise.hpp
    src/noggit/scripting/script_chunk.hpp
    src/noggit/scripting/script_model.hpp
    src/noggit/scripting/script_vert.hpp
    src/noggit/scripting/script_random.hpp
    src/noggit/scripting/script_selection.hpp
    src/noggit/scripting/scripting_tool.hpp
    src/noggit/scripting/script_context.hpp
    src/noggit/scripting/script_misc.hpp
    src/noggit/scripting/script_brush.hpp
    src/noggit/scripting/script_image.hpp
    src/noggit/scripting/script_loader.hpp
    src/noggit/scripting/script_math.hpp
    src/noggit/scripting/script_heap.hpp
  )

set ( opengl_headers
      src/opengl/context.hpp
      src/opengl/primitives.hpp
      src/opengl/scoped.hpp
      src/opengl/shader.fwd.hpp
      src/opengl/shader.hpp
      src/opengl/texture.hpp
      src/opengl/types.hpp
    )

set ( shaders
      src/glsl/m2_vert.glsl
      src/glsl/m2_frag.glsl
      src/glsl/m2_box_vert.glsl
      src/glsl/m2_box_frag.glsl
      src/glsl/particle_vert.glsl
      src/glsl/particle_frag.glsl
      src/glsl/ribbon_vert.glsl
      src/glsl/ribbon_frag.glsl
      src/glsl/terrain_vert.glsl
      src/glsl/terrain_frag.glsl
      src/glsl/wmo_vert.glsl
      src/glsl/wmo_frag.glsl
      src/glsl/liquid_vert.glsl
      src/glsl/liquid_frag.glsl
      src/glsl/mfbo_vert.glsl
      src/glsl/mfbo_frag.glsl
      src/glsl/cursor_vert.glsl
      src/glsl/cursor_frag.glsl
      src/glsl/horizon_vert.glsl
      src/glsl/horizon_frag.glsl
    )

IF(WIN32)
  set ( os_sources
        include/win/StackWalker.cpp
      )

  set ( os_headers
        include/win/StackWalker.h
      )
ENDIF(WIN32)


list (APPEND headers_to_moc
  src/noggit/MapView.h
  src/noggit/bool_toggle_property.hpp
  src/noggit/ui/terrain_tool.hpp
  src/noggit/ui/TexturePicker.h
  src/noggit/ui/TexturingGUI.h
  src/noggit/ui/Water.h
  src/noggit/ui/ZoneIDBrowser.h
  src/noggit/ui/clickable_label.hpp
  src/noggit/ui/minimap_widget.hpp
  src/noggit/ui/uid_fix_window.hpp
  src/noggit/ui/widget.hpp
  src/noggit/ui/texture_palette_small.hpp
  src/noggit/ui/TextureList.hpp
  src/noggit/ui/main_window.hpp
  src/noggit/unsigned_int_property.hpp
  src/noggit/ui/CurrentTexture.h
)
qt5_wrap_cpp (moced ${headers_to_moc} ${headers_to_moc})

source_group("noggit"  FILES ${noggit_root_sources} ${noggit_root_headers})
source_group("noggit\\ui"  FILES ${noggit_ui_sources} ${noggit_ui_headers})
source_group("opengl"  FILES ${opengl_sources} ${opengl_headers})
source_group("math"  FILES ${math_sources} ${math_headers})
source_group("external"  FILES ${external_sources} ${external_headers})
source_group("os"  FILES ${os_sources} ${os_headers})
source_group("util"  FILES ${util_sources})
source_group("glsl"  FILES ${shaders})
if (NOGGIT_WITH_SCRIPTING)
  source_group ("scripting" FILES ${scripting_sources} ${scripting_headers})
endif()

qt5_add_resources (compiled_resource_files "resources/resources.qrc")

ADD_EXECUTABLE ( noggit
                  WIN32
                  MACOSX_BUNDLE
                  ${noggit_root_sources}
                  ${noggit_ui_sources}
                  ${opengl_sources}
                  ${math_sources}
                  ${external_sources}
                  ${mysql_sources}
                  ${os_sources}
                  ${util_sources}
                  ${noggit_root_headers}
                  ${noggit_ui_headers}
                  ${opengl_headers}
                  ${math_headers}
                  ${external_headers}
                  ${mysql_headers}
                  ${os_headers}
                  ${ResFiles}
                  ${moced}
                  ${compiled_resource_files}
                  ${shaders}
                  ${force_update_file}
                )

target_compile_options (noggit PRIVATE ${noggit_warning_flags})

if (NOGGIT_WITH_SCRIPTING)
  target_sources (noggit PRIVATE ${scripting_sources} ${scripting_headers})
  add_compile_definitions (NOGGIT_HAS_SCRIPTING)
endif()

TARGET_LINK_LIBRARIES (noggit
  ${OPENGL_LIBRARIES}
  Boost::thread
  Boost::filesystem
  Boost::system
  Qt5::Widgets
  Qt5::OpenGL
  Qt5::OpenGLExtensions
  ColorWidgets-qt5
)

set (_noggit_revision_output_dir "${CMAKE_BINARY_DIR}/revision_output")
set (_noggit_revision_template_file "${CMAKE_SOURCE_DIR}/cmake/revision.h.in")
set (_noggit_revision_output_file "${_noggit_revision_output_dir}/revision.h")
set (_noggit_revision_state_file "${CMAKE_BINARY_DIR}/revision.state")
set (_noggit_revision_script_file "${CMAKE_SOURCE_DIR}/cmake/GenerateRevision.cmake")

include_directories ("${_noggit_revision_output_dir}")

find_package (Git)
if (GIT_FOUND)
  add_custom_target (update_git_revision
    ALL
    DEPENDS "${_noggit_revision_template_file}"
            "${_noggit_revision_script_file}"
    BYPRODUCTS "${_noggit_revision_output_file}"
               "${_noggit_revision_state_file}"
    COMMAND ${CMAKE_COMMAND}
              -D_noggit_revision_template_file="${_noggit_revision_template_file}"
              -D_noggit_revision_output_file="${_noggit_revision_output_file}"
              -D_noggit_revision_state_file="${_noggit_revision_state_file}"
              -DGIT_EXECUTABLE="${GIT_EXECUTABLE}"
              -P "${_noggit_revision_script_file}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

  add_dependencies (noggit update_git_revision)
else()
  message (WARNING "Failed to find a Git executable, will NOT produce a "
    "useful version string. Crash logs will be useless. Do NOT distribute.")

  set (NOGGIT_GIT_VERSION_STRING "UNKNOWN")
  configure_file ("${_noggit_revision_template_file}"
                  "${_noggit_revision_output_file}" @ONLY)
endif()

if(APPLE)
  TARGET_LINK_LIBRARIES (noggit
    "-framework Cocoa"
    "-framework AppKit"
    "-framework Foundation"
  )
endif()

if (MYSQL_LIBRARY AND MYSQLCPPCONN_LIBRARY AND MYSQLCPPCONN_INCLUDE)
  target_link_libraries (noggit ${MYSQL_LIBRARY} ${MYSQLCPPCONN_LIBRARY})
  target_include_directories (noggit SYSTEM PRIVATE ${MYSQLCPPCONN_INCLUDE})
endif()

if (NOGGIT_LOGTOCONSOLE AND WIN32)
  set_property (TARGET noggit APPEND PROPERTY LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
  set_property (TARGET noggit APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:"_CONSOLE">)
endif()

includePlattform("pack")

add_library (noggit-math STATIC
  "src/math/matrix_4x4.cpp"
  "src/math/vector_2d.cpp"
)
add_library (noggit::math ALIAS noggit-math)
target_compile_options (noggit-math PRIVATE ${noggit_warning_flags})

include (CTest)
enable_testing()

add_executable (math-vector_2d.test test/math/vector_2d.cpp)
target_compile_definitions (math-vector_2d.test PRIVATE "-DBOOST_TEST_MODULE=\"math\"")
target_link_libraries (math-vector_2d.test Boost::unit_test_framework noggit::math)
add_test (NAME math-vector_2d COMMAND $<TARGET_FILE:math-vector_2d.test>)

add_executable (math-trig.test test/math/trig.cpp)
target_compile_definitions (math-trig.test PRIVATE "-DBOOST_TEST_MODULE=\"math\"")
target_link_libraries (math-trig.test Boost::unit_test_framework noggit::math)
add_test (NAME math-trig COMMAND $<TARGET_FILE:math-trig.test>)

add_executable (math-matrix_4x4.test test/math/matrix_4x4.cpp)
target_compile_definitions (math-matrix_4x4.test PRIVATE "-DBOOST_TEST_MODULE=\"math\"")
target_link_libraries (math-matrix_4x4.test Boost::unit_test_framework noggit::math)
add_test (NAME math-matrix_4x4 COMMAND $<TARGET_FILE:math-matrix_4x4.test>)

include (FetchContent)

# Dependency: StormLib

FetchContent_Declare (storm
  GIT_REPOSITORY https://github.com/ladislav-zezula/StormLib.git
  GIT_TAG 60c2dd0fa16d3506722e25fee9b6fa4082b0c1e5
)
FetchContent_GetProperties (storm)
if (NOT storm_POPULATED)
  message (STATUS "Installing StormLib...")
  FetchContent_Populate (storm)
endif()
add_subdirectory (${storm_SOURCE_DIR} ${storm_BINARY_DIR} EXCLUDE_FROM_ALL)
set (storm_warning_disable_flags "")
add_compiler_flag_if_supported (storm_warning_disable_flags -Wno-implicit-function-declaration)
target_compile_options (storm PRIVATE ${storm_warning_disable_flags})

target_link_libraries (noggit
  storm
)

if (NOGGIT_WITH_SCRIPTING)
  # The macros to define and use modules are missing prototypes but also don't work well with anon namespaces/static.
  set (dascript_loader_warning_flags "")
  add_compiler_flag_if_supported (dascript_loader_warning_flags -Wno-missing-prototypes)
  if( NOT dascript_loader_warning_flags STREQUAL "")
    set_source_files_properties ("src/noggit/scripting/script_loader.cpp" PROPERTIES COMPILE_FLAGS ${dascript_loader_warning_flags})
  endif()

  # Dependency: json.hpp
  FetchContent_Declare (json
    GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
    GIT_TAG v3.9.1
  )
  FetchContent_GetProperties (json)
  if (NOT json_POPULATED)
    message (STATUS "Installing json.hpp...")
    FetchContent_Populate (json)
  endif()
  add_subdirectory (${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)

  # Dependency: lodepng
  FetchContent_Declare (lodepng
    GIT_REPOSITORY https://github.com/lvandeve/lodepng.git
    GIT_TAG 7fdcc96a5e5864eee72911c3ca79b1d9f0d12292
  )
  FetchContent_GetProperties (lodepng)
  if (NOT lodepng_POPULATED)
    message (STATUS "Installing lodepng...")
    FetchContent_Populate (lodepng)
  endif()
  add_library (lodepng "${lodepng_SOURCE_DIR}/lodepng.cpp")
  target_include_directories (lodepng SYSTEM PUBLIC ${lodepng_SOURCE_DIR})

  # Dependency: FastNoise2
  FetchContent_Declare (fastnoise2
    GIT_REPOSITORY https://github.com/tswow/FastNoise2.git
    GIT_TAG v0.0.1-heightmap
    PATCH_COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/cmake/deps/patch_fastnoise2.cmake"
    UPDATE_DISCONNECTED true
  )
  FetchContent_GetProperties (fastnoise2)
  if (NOT fastnoise2_POPULATED)
    message (STATUS "Installing FastNoise2... (big repo, large download)")
    FetchContent_Populate (fastnoise2)
  endif()
  add_subdirectory (${fastnoise2_SOURCE_DIR} ${fastnoise2_BINARY_DIR} EXCLUDE_FROM_ALL)

  # Dependency: daScript
  FetchContent_Declare (dascript
    GIT_REPOSITORY https://github.com/GaijinEntertainment/daScript.git
    GIT_TAG 7b93bd95db57009856473216048c2612fbb6ca84
    # don't fetch submodules
    GIT_SUBMODULES ""
    PATCH_COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/cmake/deps/patch_dascript.cmake"
    UPDATE_DISCONNECTED true
  )
  FetchContent_GetProperties (dascript)
  if (NOT dascript_POPULATED)
    message (STATUS "Installing daScript...")
    FetchContent_Populate (dascript)
  endif()
  add_subdirectory (${dascript_SOURCE_DIR} ${dascript_BINARY_DIR} EXCLUDE_FROM_ALL)
  target_include_directories (libDaScript SYSTEM PUBLIC "${dascript_SOURCE_DIR}/include")

  target_link_libraries (noggit
    libDaScript
    lodepng
    FastNoise
    nlohmann_json::nlohmann_json
  )

  # If a source of this library fails to compile, the daScript API
  # changed and needs adaption in our wrapper header.
  add_library (DaScriptWrapperVerify STATIC)
  target_link_libraries (DaScriptWrapperVerify PRIVATE libDaScript Qt5::Core)
  file (GLOB_RECURSE _das_wrapper_headers LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/das/*")
  foreach (_das_wrapper_header IN LISTS _das_wrapper_headers)
    file (GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${_das_wrapper_header}.cpp"
      CONTENT "#include <QtCore/QtGlobal>
      #include <${_das_wrapper_header}>"
    )
    target_sources (DaScriptWrapperVerify PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${_das_wrapper_header}.cpp")
  endforeach()
endif()

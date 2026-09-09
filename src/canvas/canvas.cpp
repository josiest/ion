#include "ion/canvas.hpp"
#include <SDL3/SDL_video.h>

#include <array>

#include "shader_manager.hpp"

std::optional<ion::canvas> ion::canvas::create(const shader_manager & shaders, SDL_Window * window)
{
   if (const SDL_WindowFlags window_flags = SDL_GetWindowFlags(window);
       (window_flags & SDL_WINDOW_OPENGL) == 0u)
   {
      return std::nullopt;
   }
   const auto solid_fill = shaders.find_shader<shader::solid_fill>();
   if (not solid_fill) { return std::nullopt; }

   canvas local_canvas(*solid_fill);
   SDL_GetWindowSize(window, &local_canvas._width, &local_canvas._height);
   local_canvas.update_clip_space();
   return std::optional<canvas>(std::in_place, std::move(local_canvas));
}

ion::canvas::canvas(const shader::solid_fill & solid_fill)
   : solid_fill(solid_fill)
{
}

void ion::canvas::triangle(float x1, float y1, float x2, float y2, float x3, float y3)
{
   std::array verts{ x1, y1, x2, y2, x3, y3 } ;
   triangle_mesh.set_vertices(verts);
   solid_fill.use_program();
   triangle_mesh.render();
}

void ion::canvas::rect(float x, float y, float w, float h)
{
   std::array verts{ x, y, x, y+h, x+w, y, x+w, y+h };
   rect_mesh.set_vertices(verts);
   solid_fill.use_program();
   rect_mesh.render();
}

void ion::canvas::background_internal(const SDL_FColor & color)
{
   glClearColor(color.r, color.g, color.b, color.a);
   glClear(GL_COLOR_BUFFER_BIT);
}

void ion::canvas::fill_internal(const SDL_FColor & color)
{
   solid_fill.color = to_linear<glm::vec4>(color);
}

void ion::canvas::update_clip_space()
{
   clip_from_world = glm::ortho(0.f, width_as<float>(), 0.f, height_as<float>(), 0.f, 1000.f);
   solid_fill.transform = clip_from_world;
}

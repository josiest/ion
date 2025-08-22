#include "entities/tile.hpp"
#include <ion/ion.hpp>
#include <ranges>

namespace prefab {

tile::tile(std::string_view images_path)

    : loaded_tiles{ Pipes::TileMap::load_all(ion::asset_loader{}, images_path) }
{
}
}

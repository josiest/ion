#pragma once
#include <SDL2/SDL_render.h>

#include <concepts>
#include <ranges>
#include <iterator>

#include <unordered_map>
#include <string>
#include <utility>

namespace ion {

template<typename key_t>
concept hashable = requires(key_t key) {
    { std::hash<key_t>{}(key) } -> std::convertible_to<std::size_t>;
};

template<typename key_t>
concept generatable = hashable<key_t> && std::weakly_incrementable<key_t>;

template<generatable key_t>
class default_keygen {
public:
    key_t operator()() { return _next_key++; }
private:
    key_t _next_key = 0;
};

/**
 * A simple resource manager for SDL_Textures
 */
template<hashable key_t>
class texture_map {
public:
    texture_map(const texture_map &) = delete;
    ~texture_map() { clear(); }

    void clear()
    {
        for (auto & texture : _textures | std::views::values) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
        _textures.clear();
    }

    std::pair<key_t, bool> load(key_t const & key, std::string const & filepath)
    {
        bool emplaced = _textures.try_emplace(key, _load(filepath))->second;
        return std::make_pair(key, emplaced);
    }

    std::pair<key_t, bool>
    load_or_replace(key_t const & key, std::string const & filepath)
    {
        // load the texture and try to associate it with the key
        auto texture = _load(filepath);
        auto [it, emplaced] = _textures.try_emplace(key, texture);

        // if the key already exists, free the associated texture and replace
        if (not emplaced) {
            SDL_DestroyTexture(it->second);
            it->second = texture;
        }
        return std::make_pair(key, not emplaced);
    }

    bool erase(key_t const & key)
    {
        // see if the key is associated with a texture
        auto search = _textures.find(key);
        bool const erased = search != _textures.end();

        // if so, destroy its resources and deassociate it
        if (erased) {
            SDL_DestroyTexture(search->second);
            _textures.erase(search);
        }
        return erased;
    }

    bool contains(key_t const & key) { return _textures.contains(key); }
    SDL_Texture * at(key_t const & key)
    {
        // see if the key is associated with a texture
        auto search = _textures.find(key);

        // if so, return it
        if (search != _textures.end()) {
            return search->second;
        }
        // otherwise throw error
        throw std::out_of_range{"invalid key!"};
    }
private:
    std::unordered_map<key_t, SDL_Texture *> _textures;
};
}

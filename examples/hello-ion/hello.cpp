#include <ion/editor.hpp>
#include <ion/engine.hpp>

int main(int argc, char * argv[])
{
    const auto editor = ion::editor::initialize();
    if (not editor) { return EXIT_FAILURE; }

    while (not editor->has_quit())
    {
        sdl_events::poll();
    }
    return EXIT_SUCCESS;
}

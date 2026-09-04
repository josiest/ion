#include <ion/engine.hpp>

int main(int argc, char * argv[])
{
    const auto engine = ion::engine::initialize();
    if (not engine) { return EXIT_FAILURE; }

    while (not engine->has_quit())
    {
        ion::sdl_events::poll();
    }
    return EXIT_SUCCESS;
}

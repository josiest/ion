#include "ion.hpp"
#include "SDL2/SDL.h"

int main(int argc, char * argv[])
{
    ion::app simple{"A simple window", 800, 600};
    simple.run();
    return 0;
}

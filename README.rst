ion
===

:code:`ion` is a small-scoped library meant to make working with SDL easier
using modern C++ (23 and up!)

The library is development-driven, which means that utilities are added to the
library as needed by yours truly, who makes games with it.

Installation
------------

*Dependencies*:

* SDL2
* OpenGL
* EnTT

.. code:: console

    git clone https://github.com/josiest/ion.git /home/<username>/ion
    mkdir ion/build
    cd ion/build
    cmake ..
    cmake --build .

Because ion is still in the very early stages of development, I'd recommended
you install the library on a per-project basis. This might look something likes
this

.. code:: console

    cmake --install . --prefix=/home/<username>/my-cool-game

Finally, add the following to your :code:`CMakeLists.txt`

.. code:: cmake

    find_package(ion REQUIRED 0.9.0) # version is important b/c of instability!
    ...
    target_link_libraries(<project-name> PRIVATE ion::ion)

Because ion uses opengl, when you build you may get a warning saying something
about GLVND. If this happens, you can suppress this by adding this line to your
:code:`CmakeLists.txt`

.. code:: cmake

    set(OpenGL_GL_PREFERENCE GLVND) 
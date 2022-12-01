ion
===

:code:`ion` is a small-scoped library meant to make working with SDL easier
using modern C++ (23 and up!)

The library is development-driven, which means that utilities are added to the
library as needed by yours truly, who makes games with it.

Installation
------------

Not gonna lie, building and installing is a bit of a mess right now. A big
reason for this is that it depends on my own custom fork of yaml-cpp (I know,
yikes - but I *really* wanted support for :code:`std::expected`, and was
impatient). So you'll need to download
`that repository <https://github.com/josiest/yaml-cpp-expect>`_ and build and
install it to your local copy of *this* repository. That might look like this

.. code:: console

    $ git clone https://github.com/josiest/ion.git /home/<username>/ion
    $ git clone https://github.com/josiest/yaml-cpp-expect /home/<username>/yaml-cpp-expect
    $
    $ cd /home/<username>/yaml-cpp-expect
    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .
    $ cmake --install . --prefix /home/<username>/ion
    $
    $ cd /home/<username>/ion
    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .

In addition to my own yaml fork, :code:`ion` also has the following dependencies

* SDL2
* OpenGL
* EnTT

Because ion is still in the very early stages of development, I'd recommended
you install the library on a per-project basis. This might look something likes
this

.. code:: console

    $ cmake --install . --prefix=/home/<username>/my-cool-game

Finally, add the following to your :code:`CMakeLists.txt`

.. code:: cmake

    find_package(ion REQUIRED 0.9.0) # version is important b/c of unstableness!
    ...
    target_link_libraries(<project-name> PRIVATE ion::ion)

Because ion uses opengl, when you build you may get a warning saying something
about GLVND. If this happens, you can supress this by adding this line to your
:code:`CmakeLists.txt`

.. code:: cmake

    set(OpenGL_GL_PREFERENCE GLVND) 
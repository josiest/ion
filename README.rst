ion
===

:code:`ion` is a small-scoped library meant to make working with SDL easier.
It serves two primary functions

1. it result-style resource handlers for various SDL and OpenGL components
2. it provides a higher level event-handling interface than pure SDL

:code:`ion` is still in the very early stages of development. Since this is
currently a personal project, more features will be added as I need them.

Installation
------------

The goal is to eventually add ion to different c++ package managers, but I'm
just a mere student and I don't have the time to do that right now. For now
you'll need to download the source code and build it with cmake, then install
it in a place that other CMake projects can find it

.. code:: console

    $ git clone https://github.com/josiest/ion.git <path/to/ion> && cd <path/to/ion>
    $ mkdir build && cd build
    $ cmake ..
    $ cmake --build .

Then if you're on linux, run :code:`$ sudo cmake --install .`. This will install
the ion into :code:`/usr/include`

If you're on windows, run `cmake --install .` as administrator. This will
install ion into :code:`C:\Program Files\ion`

Finally, add the following to your :code:`CMakeLists.txt`

.. code:: cmake

    find_package(ion REQUIRED)
    ...
    include_directories(ion::ion)
    target_link_libraries(<project-name> PRIVATE ion::ion)

Examples
--------

Here's a simple example of how you might use :code:`ion`. This example renders
a fibonacci-like gradient pattern to the screen.

.. include:: examples/simple/simple.cpp
   :language: c++

To run this example from the ion project directory run the following code

.. code:: console

    $ mkdir examples/simple/build && cd examples/simple/build
    $ cmake ..
    $ cmake --build .
    $ ./simple

.. image:: images/simple-example.png
   :alt: simple example window

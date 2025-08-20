# install the header files for ion core
install(FILES ${CMAKE_SOURCE_DIR}/include/ion/ion.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/time.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/isotope.hpp
              ${CMAKE_SOURCE_DIR}/include/ion/math.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion)

install(FILES ${CMAKE_SOURCE_DIR}/include/ion/time/clock.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ion/time)

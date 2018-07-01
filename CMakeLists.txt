cmake_minimum_required(VERSION 2.6)

project(CV_DIA)

#OPEN CV libraries
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})
#include_directories(/usr/local/share/opencv/haarcascade/)
link_directories(${OPENCV_LIBRARY_DIRS})
set (LIBS ${LIBS} ${OpenCV_LIBS})
include_directories(${CV_DIA_LIB_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-g -Wall")

IF (NOT DESTDIR)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/uv_libs)
ELSE (NOT DESTDIR)
  set(CMAKE_INSTALL_PREFIX ${DESTDIR})
ENDIF (NOT DESTDIR)


add_library(cvDia cvDiaPeopleDetection.c histogramms.c)

INSTALL(TARGETS cvDia
        DESTINATION "lib"
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

INSTALL(FILES cvDia.h
        DESTINATION "include"
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

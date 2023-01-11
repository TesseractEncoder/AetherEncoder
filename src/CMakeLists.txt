cmake_minimum_required(VERSION 3.8)
project(AetherEncoder)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTLIBS REQUIRED
        gobject-2.0
        glib-2.0
        gstreamer-check-1.0
        )

pkg_check_modules(JSONCPP jsoncpp)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)


# add extra include directories
# gst-config.h moved to /usr/lib/x86_64-linux-gnu/gstreamer-1.0/include
include_directories(
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        /usr/include/glib-2.0
        /usr/local/include/gstreamer-1.0
        /usr/local/include/json
       )

#[[link_libraries(gstreamer-1.0
        gobject-2.0
        glib-2.0
        json-glib-1.0)
        ]]

set(SOURCE_FILES AetherEncoder.cpp gstreamer/gst.cpp json/jsonParser.cpp)

link_directories(${GSTLIBS_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIRS})

add_executable(AetherEncoder ${SOURCE_FILES})

target_link_libraries(AetherEncoder ${GSTLIBS_LIBRARIES} ${JSONCPP_LIBRARIES} Threads::Threads)
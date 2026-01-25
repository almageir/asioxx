cmake_minimum_required(VERSION 3.21)

project(dataio)

include(FetchContent)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG        asio-1-36-0
)
FetchContent_MakeAvailable(asio)

add_library(dataio STATIC)
add_library(dataio::dataio ALIAS dataio)

if (UNIX)
    find_package(Threads)
    target_compile_definitions(dataio PRIVATE ASIO_HAS_IO_URING)
    target_link_libraries(dataio PRIVATE uring Threads::Threads)
endif()

target_sources(dataio
        PRIVATE
            src/file_sink.cpp

        PUBLIC
        FILE_SET HEADERS
        FILES
            include/dataio/sink/sink.h
            include/dataio/sink/file_sink.h
            include/dataio/sink/console_sink.h
)

target_compile_features(dataio PRIVATE cxx_std_17)

if(UNIX)
    target_sources(dataio PRIVATE src/linux/console_sink.cpp)
else()
    target_sources(dataio PRIVATE src/windows/console_sink.cpp)
    target_compile_definitions(dataio PRIVATE "_WIN32_WINNT=0x0A00")
endif()

target_include_directories(dataio
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${asio_SOURCE_DIR}/asio/include>
)

include(GNUInstallDirs)
install(TARGETS dataio
        EXPORT dataioTargets
        FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT dataioTargets
        NAMESPACE dataio::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dataio
)
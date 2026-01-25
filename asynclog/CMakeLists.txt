cmake_minimum_required(VERSION 3.21)

project(asynclog)

add_library(asynclog STATIC)
add_library(asynclog::asynclog ALIAS asynclog)

target_sources(asynclog
    PRIVATE
        src/logger.cpp
        src/log_manager.cpp
        src/log_provider.cpp

    PUBLIC
        FILE_SET publicHeaders
        TYPE HEADERS
        FILES
            include/asynclog/logger.h

    PRIVATE
        FILE_SET internalHeaders
        TYPE HEADERS
        FILES
            include/asynclog/common.h
            include/asynclog/log_manager.h
            include/asynclog/log_provider.h
)

target_compile_features(asynclog PRIVATE cxx_std_17)
target_link_libraries(asynclog PRIVATE dataio::dataio)

if(WIN32)
    target_compile_definitions(asynclog PRIVATE "_WIN32_WINNT=0x0A00")
else()
    target_compile_definitions(asynclog PRIVATE ASIO_HAS_IO_URING)
endif()

target_include_directories(asynclog
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/>
)

include(GNUInstallDirs)
install(TARGETS asynclog
        EXPORT asynclogTargets
        FILE_SET publicHeaders
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT asynclogTargets
        NAMESPACE asynclog::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/asynclog
)

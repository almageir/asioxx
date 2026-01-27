cmake_minimum_required(VERSION 3.21)

project(asynclog)

add_library(asynclog STATIC)
add_library(asynclog::asynclog ALIAS asynclog)

target_sources(asynclog
    PRIVATE
        src/global_logger.cpp
        src/scoped_logger.cpp
        src/log_manager.cpp
        src/context_bound_log_backend.cpp
        src/standalone_log_backend.cpp
        src/logger_factory.cpp

    PUBLIC
        FILE_SET publicHeaders
        TYPE HEADERS
        FILES
            include/asynclog/global_logger.h
            include/asynclog/scoped_logger.h
            include/asynclog/logger_factory.h
            include/asynclog/log_manager.h
            include/asynclog/log_manager_interface.h

    PRIVATE
        FILE_SET internalHeaders
        TYPE HEADERS
        FILES
            src/detail/log_backend_interface.h
            src/detail/standalone_log_backend.h
            src/detail/context_bound_log_backend.h
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
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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

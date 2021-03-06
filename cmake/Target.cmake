cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0054 NEW)

# NOTE(Wuxiang): Include guard.
if (FALCON_ENGINE_TARGET_INITIALIZED)
    return()
endif()

set(FALCON_ENGINE_TARGET_INITIALIZED TRUE)

# Set up solution dependency
set(FALCON_ENGINE_INCLUDE_DIR ${FALCON_ENGINE_ROOT_DIR}/include)
set(FALCON_ENGINE_LIBRARY_DIR ${FALCON_ENGINE_ROOT_DIR}/lib)

# Set up solution output
set(FALCON_ENGINE_ARCHIVE_OUTPUT_DIR ${FALCON_ENGINE_ROOT_DIR}/bin)
set(FALCON_ENGINE_LIBRARY_OUTPUT_DIR ${FALCON_ENGINE_ROOT_DIR}/bin)
set(FALCON_ENGINE_RUNTIME_OUTPUT_DIR ${FALCON_ENGINE_ROOT_DIR}/bin)

# Set up library type
fe_assert_defined(FALCON_ENGINE_BUILD_DYNAMIC)
if(FALCON_ENGINE_BUILD_DYNAMIC)
set(FALCON_ENGINE_LIBRARY_TYPE "SHARED")
else()
set(FALCON_ENGINE_LIBRARY_TYPE "STATIC")
endif()
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

project("sonia_embed_toolkit")

###############################################################################
## File Globbing ##############################################################
###############################################################################

file(GLOB_RECURSE sources src/*.cpp include/*.h)

file(GLOB_RECURSE sources_test tests/*.cpp)

set(INCLUDE_HEADERS include/)

###############################################################################
## Target Definitions #########################################################
###############################################################################

add_library(${PROJECT_NAME} SHARED ${sources})
# add_executable(${PROJECT_NAME} ${sources})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++1y -Wall -Wfloat-conversion)

target_include_directories(${PROJECT_NAME} PUBLIC src include tests)

# file(COPY ${INCLUDE_HEADERS} DESTINATION include)

###############################################################################
## Dependencies ###############################################################
###############################################################################

# find_package(Boost 1.36.0 COMPONENTS filesystem system REQUIRED)

# target_link_libraries(example PUBLIC
#   ${Boost_LIBRARIES}
#   # here you can add any library dependencies
# )


###############################################################################
## Testing ####################################################################
###############################################################################

# this is for our testing framework
# we don't add REQUIRED because it's just for testing

find_package(GTest)
if(GTEST_FOUND)
  include(CTest)
#   enable_testing()

  add_executable(unittests ${sources_test} ${sources})

  # we add this define to prevent collision with the main
  # this might be better solved by not adding the source with the main to the
  # testing target
  # target_link_libraries(unittests gmock gtest pthread)

  target_compile_definitions(unittests PUBLIC UNIT_TESTS)

  # this allows us to use our executable as a link library
  # therefore we can inherit all compiler options and library dependencies
  set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS on)

  target_link_libraries(unittests PUBLIC
    ${PROJECT_NAME}
    ${GTEST_BOTH_LIBRARIES}
    pthread
  )

  add_test(
    NAME unittests
    COMMAND $<TARGET_FILE:unittests>
    )

  target_include_directories(unittests PUBLIC
    ${GTEST_INCLUDE_DIRS} # doesn't do anything on Linux
  )

  add_custom_target(coverage
    COMMAND rm -rf coverage
    COMMAND mkdir coverage
    COMMAND cd .. && gcovr -f src -f include --exclude-unreachable-branches --exclude-throw-branches --html-details ${PROJECT_BINARY_DIR}/coverage/coverage.html
    )
endif()

###############################################################################
## Packaging ##################################################################
###############################################################################


# all install commands get the same destination. this allows us to use paths
# relative to the executable.
# install(DIRECTORY cpp/include DESTINATION include)
# this is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell CMake that we want it
# in the package
# install(DIRECTORY resources DESTINATION example_destination)

# now comes everything we need, to create a package
# there are a lot more variables you can set, and some
# # you need to set for some package types, but we want to
# # be minimal here
# set(CPACK_PACKAGE_NAME "MyExample")
# set(CPACK_PACKAGE_VERSION "1.0.0")

# # # we don't want to split our program up into several things
# set(CPACK_MONOLITHIC_INSTALL 1)

# # # This must be last
# include(CPack)
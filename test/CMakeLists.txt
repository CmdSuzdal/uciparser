# Shall be included by a top level project that defines
# the CTGT, PROJECT_NAME and CMAKE_MODULE_PATH variables
# (the last to search for the GTEST support)
enable_testing()
include(CTest)

# Add gtest/gmock support
include(addgtestsupport)

set(PROJECT_TESTBNAME ${CTGT}test)

# ----------------------------------------------------------------
add_executable(${PROJECT_TESTBNAME}_init uciinittest.cpp)
target_include_directories(${PROJECT_TESTBNAME}_init PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_TESTBNAME}_init PRIVATE ${CTGT})
target_compile_options(${PROJECT_TESTBNAME}_init PRIVATE -Werror)
target_compile_features(${PROJECT_TESTBNAME}_init PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_TESTBNAME}_init PRIVATE gtest gmock_main)
add_test(NAME UCIParserInitTest COMMAND ${PROJECT_TESTBNAME}_init)

file(GLOB ALL_TESTS
     "test_*.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY tests)

foreach(_TEST ${ALL_TESTS})
    get_filename_component(TESTNAME ${_TEST} NAME_WE)
    add_executable(
      ${TESTNAME}
      ${_TEST}
      "../src/binary_neural_acid.cpp"
      // "../src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp"
    )
    # target_include_directories(${TESTNAME} PUBLIC
    #   "../src/"
    #   "../src.wsjcpp/"
    # )
    add_test(
      NAME ${TESTNAME}
      COMMAND $<TARGET_FILE:${TESTNAME}>
    )
    message(STATUS "Found test file: ${TESTNAME}")
    set_target_properties (${TESTNAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY tests)
endforeach()

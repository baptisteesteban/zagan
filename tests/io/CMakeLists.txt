zagan_add_test(nifti_reader)
target_compile_definitions(test_nifti_reader PRIVATE
    ZAGAN_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/io/fixtures"                                                       
)
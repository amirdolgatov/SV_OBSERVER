add_executable(multiply_test main.cpp)
add_executable(effective_val_test effective_val_test.cpp)

#target_include_directories(multiply_test )
target_link_libraries(multiply_test
        PUBLIC
        SV_OBSERVER
        gtest)

target_link_libraries(effective_val_test
        PUBLIC
        SV_OBSERVER
        gtest)

add_test(multiply_gtests multiply_test)
add_test(effective_val effective_val_test)
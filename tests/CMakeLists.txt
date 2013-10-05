CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE_DIRECTORIES(
    ../src
)

ADD_EXECUTABLE(
    talloc_test
    talloc_test.c
)

TARGET_LINK_LIBRARIES(
    talloc_test
    talloc
)

ADD_TEST(talloc_test talloc_test)

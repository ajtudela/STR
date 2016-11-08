PROJECT(List)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wcomment -Wformat -Wignored-qualifiers -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point -Wreturn-type -Wunused-function -Wunused-variable -Wuninitialized -Warray-bounds -Wundef -Wpointer-arith -Wtype-limits -Wcast-qual -Wwrite-strings -Wempty-body -Wsign-compare -Waddress -Wlogical-op -Wredundant-decls -Winline -Wvla -std=c89")

ADD_EXECUTABLE(${PROJECT_NAME} list.c)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
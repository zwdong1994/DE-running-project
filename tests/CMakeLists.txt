cmake_minimum_required(VERSION 3.5)

SET(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

ADD_DEFINITIONS(-g -W -lpthread -Wall -lrt -lcrypto -lstdc++ -D_GLIBCXX_USE_CXX11_ABI=0 -fpermissive)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

include_directories(../include /usr/include)
link_libraries("/usr/lib/x86_64-linux-gnu/librt.so")
link_libraries("/usr/lib/x86_64-linux-gnu/libcrypto.a")
link_directories(/lib/x86_64-linux-gnu)

message("${CMAKE_SOURCE_DIR}")

set(ATTACHED_FILES ../src/bloom_func.cpp ../include/bloom_func.h ../src/cache.cpp ../include/cache.h
                    ../src/com_t.cpp ../include/com_t.h ../src/mt.cpp ../include/mt.h ../include/dedup.h
                    ../src/dedup.cpp ../src/bch.c ../include/bch.h ../src/new_cache.cpp ../include/new_cache.h)
set(SOURCE_FILES test_bloom.cpp test_cache.cpp test_mt.cpp test_dedup.cpp test_new_cache.cpp)

foreach(TEST_FILE ${SOURCE_FILES})
    get_filename_component(TEST_FILE_WE ${TEST_FILE} NAME_WE)
    set(TEST_FILE ${TEST_FILE} ${ATTACHED_FILES})
    add_executable(${TEST_FILE_WE} ${TEST_FILE})
endforeach()
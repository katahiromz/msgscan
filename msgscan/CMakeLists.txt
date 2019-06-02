# msgscan32.exe/msgscan64.exe
add_executable(msgscan msgscan.c)
target_link_libraries(msgscan shlwapi psapi)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(msgscan PROPERTIES OUTPUT_NAME "msgscan64")
else()
    set_target_properties(msgscan PROPERTIES OUTPUT_NAME "msgscan32")
endif()

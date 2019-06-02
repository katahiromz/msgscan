# MsgGet32.dll/MsgGet64.dll
add_library(MsgGetDLL SHARED MsgGetDLL.c MsgGetDLL.def)
set_target_properties(MsgGetDLL PROPERTIES PREFIX "")

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set_target_properties(MsgGetDLL PROPERTIES OUTPUT_NAME "MsgGet64")
else()
    set_target_properties(MsgGetDLL PROPERTIES OUTPUT_NAME "MsgGet32")
endif()

if (MSVC)
    target_link_options(MsgGetDLL PRIVATE /DYNAMICBASE:NO /FIXED)
endif()

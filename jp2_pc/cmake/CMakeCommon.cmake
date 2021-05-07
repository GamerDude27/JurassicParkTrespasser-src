function(add_common_options)

add_compile_definitions(
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:BUILDVER_MODE=MODE_DEBUG>
    $<$<CONFIG:Release>:BUILDVER_MODE=MODE_RELEASE>
    $<$<CONFIG:Final>:BUILDVER_MODE=MODE_FINAL>
    $<$<CONFIG:Release>:_RELEASE>
    $<$<CONFIG:Final>:_RELEASE>
    
    $<$<CONFIG:Debug>:TARGET_PROCESSOR=PROCESSOR_PENTIUM>
    $<$<CONFIG:Release>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO>
    $<$<CONFIG:Final>:TARGET_PROCESSOR=PROCESSOR_PENTIUMPRO>
    
    $<$<CONFIG:Debug>:NOMINMAX>
    $<$<CONFIG:Release>:NOMINMAX>
    $<$<CONFIG:Final>:NOMINMAX>
)

#In CMake, the regular Release configuration has no debug info
#We turn Release and Final into RelWithDebInfo so that we can have it anyway
#While keeping the configuration name "Release"

set(CMAKE_CXX_FLAGS_FINAL ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} PARENT_SCOPE)
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} PARENT_SCOPE)


set(CMAKE_EXE_LINKER_FLAGS_TMP "${CMAKE_EXE_LINKER_FLAGS} /MAP:${PROJECT_NAME}.map")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_TMP}" PARENT_SCOPE)
set(CMAKE_SHARED_LINKER_FLAGS_TMP "${CMAKE_SHARED_LINKER_FLAGS} /MAP:${PROJECT_NAME}.map")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_TMP}" PARENT_SCOPE)


set(CMAKE_EXE_LINKER_FLAGS_RELEASE_TMP ${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO})
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_TMP ${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO})


#Incremental linking bloats the binary with padding
#Optimizing references means dropping unused symbols from binary
#Tip: list the dropped symbols with /VERBOSE:REF
set(CMAKE_EXE_LINKER_FLAGS_RELEASE_TMP "${CMAKE_EXE_LINKER_FLAGS_RELEASE_TMP} /INCREMENTAL:NO /OPT:REF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE_TMP "${CMAKE_SHARED_LINKER_FLAGS_RELEASE_TMP} /INCREMENTAL:NO /OPT:REF")


set(CMAKE_EXE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE_TMP} PARENT_SCOPE)
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${CMAKE_SHARED_LINKER_FLAGS_RELEASE_TMP} PARENT_SCOPE)
set(CMAKE_EXE_LINKER_FLAGS_FINAL ${CMAKE_EXE_LINKER_FLAGS_RELEASE_TMP} PARENT_SCOPE)
set(CMAKE_SHARED_LINKER_FLAGS_FINAL ${CMAKE_SHARED_LINKER_FLAGS_RELEASE_TMP} PARENT_SCOPE)


endfunction()


function (add_pch PCHHeader PCHCode)

endfunction()

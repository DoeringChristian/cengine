
function(FileEmbedSetup)

    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}file_embed)
    endif ()

    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c)
        file(WRITE ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c "")
    endif ()

    add_library(file_embed ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c)
    target_include_directories(cengine PUBLIC ${CMAKE_BINARY_DIR}/file_embed)

endfunction()

function(FileEmbedAdd file)
    FileEmbedGenerate(${file} var)
    target_sources(file_embed PUBLIC ${var})

    add_custom_command(
            OUTPUT ${var}
            COMMAND ${CMAKE_COMMAND}
            -DRUN_FILE_EMBED_GENERATE=1
            -DFILE_EMBED_GENERATE_PATH=${file}
            -P ${CMAKE_SOURCE_DIR}/cmake/FileEmbed.cmake
            MAIN_DEPENDENCY ${file}
    )
endfunction()

function(FileEmbedGenerate file generated_c)

    get_filename_component(base_filename ${file} NAME)
    set(output_filename ${base_filename})
    string(MAKE_C_IDENTIFIER ${base_filename} c_name)
    file(READ ${file} content HEX)
    #message(${content})

    # Separate into individual bytes.
    string(REGEX MATCHALL "([A-Fa-f0-9][A-Fa-f0-9])" SEPARATED_HEX ${content})

    set(output_c "")

    set(counter 0)
    foreach (hex IN LISTS SEPARATED_HEX)
        string(APPEND output_c "0x${hex},")
        MATH(EXPR counter "${counter}+1")
        if (counter GREATER 16)
            string(APPEND output_c "\n    ")
            set(counter 0)
        endif ()
    endforeach ()


    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}file_embed)
    endif ()


    file(WRITE ${CMAKE_BINARY_DIR}/file_embed/${base_filename}
            ${output_c})


    set(${generated_c} ${CMAKE_BINARY_DIR}/file_embed/${base_filename} PARENT_SCOPE)

endfunction()


function(FileEmbedAddMulti files)
    foreach(file ${files})
        FileEmbedAdd(${file})
    endforeach()
endfunction()

if (RUN_FILE_EMBED_GENERATE)
    FileEmbedGenerate(${FILE_EMBED_GENERATE_PATH} var)
endif ()

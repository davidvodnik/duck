function(add_component name)
    message(STATUS "ARGN=${ARGN}")
    add_library(${name} INTERFACE)
    target_sources(${name} INTERFACE
        ${name}.h
        ${name}.cc
        )
    target_include_directories(${name} INTERFACE .)
    target_link_libraries(${name} INTERFACE ${ARGN})

    add_executable(test_${name}
        ${name}.t.cc
        )
    target_link_libraries(test_${name} PRIVATE ${ARGV0} catch)
    add_test(NAME test_${name} COMMAND test_${name})
endfunction()

add_component(tokenizer)
add_component(parser tokenizer)
add_component(generate_interface fmt::fmt)
add_component(print_error fmt::fmt)

add_executable(duck
    duck.cc
    )
target_link_libraries(duck PRIVATE parser generate_interface print_error)
if(EMSCRIPTEN)
target_link_options(duck PRIVATE "SHELL: -s EXPORT_ES6=1 -s MODULARIZE=1 --bind")
endif()

install(TARGETS duck DESTINATION bin)

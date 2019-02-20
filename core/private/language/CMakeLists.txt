find_package(Java COMPONENTS Runtime)

function(generate_grammars GRAMMAR_LIBRARY_NAME GRAMMAR_SOURCE_FILES)
    set(ANTLR4CPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/externals/antlr4/runtime/Cpp/runtime/src")
    set(ANTLR4CPP_LIBS antlr4_static)
    set(ANTLR_VERSION 4.7.2)
    set(LOCAL_ANTLR_GENERATOR_JAR "${CMAKE_CURRENT_BINARY_DIR}/antlr-complete-${ANTLR_VERSION}.jar")
    file(DOWNLOAD "http://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar" "${LOCAL_ANTLR_GENERATOR_JAR}")

    ${CMAKE_CURRENT_SOURCE_DIR}/L-Code.g4
    foreach(GRAMMAR_SOURCE_FILE ${GRAMMAR_SOURCE_FILES})
        get_filename_component(GRAMMAR_NAME ${GRAMMAR_SOURCE_FILE} NAME_WE)
        message(STATUS "grammar: ${GRAMMAR_NAME} -> ${GRAMMAR_NAME}GENERATED_SRC")

        set(GENERATED_INCLUDES
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}BaseListener.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Listener.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Parser.h"
                )

        set(GENERATED_SOURCES
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}BaseListener.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Listener.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Parser.cpp"
                )

        set(GENERATED_META
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.interp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.tokens"
                )

        set(GENERATED_SRC
            ${GENERATED_INCLUDES}
            ${GENERATED_SOURCES}
            )

        foreach(GENERATED_FILE ${GENERATED_SRC})
            message(STATUS "generated file: ${GENERATED_FILE}")
            # Avoid CMake warning related to AUTOMOC and AUTOUI.
            # Theses files shouldn't be processed by the Qt machinery because
            # ANTLR generate code with something called "emit" in it.
            set_property(SOURCE ${GENERATED_FILE} PROPERTY SKIP_AUTOGEN ON)
        endforeach()
        set(GENERATION_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
        add_custom_command(OUTPUT ${GENERATED_SRC}
                COMMAND "${Java_JAVA_EXECUTABLE}" -cp "${LOCAL_ANTLR_GENERATOR_JAR}" org.antlr.v4.Tool
                -Dlanguage=Cpp ${GRAMMAR_SOURCE_FILE} -o ${GENERATION_OUTPUT_DIR}
                DEPENDS ${GRAMMAR_SOURCE_FILE}
                COMMENT "Generate language for grammar ${GRAMMAR_NAME}")
    endforeach(GRAMMAR_SOURCE_FILE)
    add_library(${GRAMMAR_LIBRARY_NAME} ${GENERATED_SOURCES})
    target_include_directories(${GRAMMAR_LIBRARY_NAME} PUBLIC ${GENERATION_OUTPUT_DIR} ${ANTLR4CPP_INCLUDE_DIRS})
    target_link_libraries(${GRAMMAR_LIBRARY_NAME} PUBLIC ${ANTLR4CPP_LIBS})
endfunction()

set(GRAMMAR_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/LCode.g4
        ${CMAKE_CURRENT_SOURCE_DIR}/LSystem.g4
        )
add_library(lindenmayer-langage ${GRAMMAR_GENERATED_SOURCES})
generate_grammars(grammar-library "${GRAMMAR_SOURCE_FILES}" Cpp)

target_include_directories(lindenmayer-langage PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${ANTLR4CPP_INCLUDE_DIRS}")
target_link_libraries(lindenmayer-langage PUBLIC "${ANTLR4CPP_LIBS}" grammar-library)
target_compile_definitions(lindenmayer-langage PUBLIC ANTLR4CPP_STATIC)





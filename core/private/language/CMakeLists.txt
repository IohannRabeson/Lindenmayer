find_package(Java COMPONENTS Runtime)

# Generate a library called by content of GRAMMAR_LIBRARY_NAME
# This library provide everything needed to build a program using ANTLR4
# This function will download ANTLR jar from the antlr website https://www.antlr.org
function(generate_grammars GRAMMAR_LIBRARY_NAME GRAMMAR_SOURCE_FILES)
    set(ANTLR4CPP_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/externals/antlr4/runtime/Cpp/runtime/src")
    set(ANTLR4CPP_LIBS antlr4_static)
    set(ANTLR_VERSION 4.7.2)
    set(LOCAL_ANTLR_GENERATOR_JAR "${CMAKE_CURRENT_BINARY_DIR}/antlr-complete-${ANTLR_VERSION}.jar")
    file(DOWNLOAD "https://www.antlr.org/download/antlr-${ANTLR_VERSION}-complete.jar" "${LOCAL_ANTLR_GENERATOR_JAR}")

    foreach(GRAMMAR_SOURCE_FILE ${GRAMMAR_SOURCE_FILES})
        get_filename_component(GRAMMAR_NAME ${GRAMMAR_SOURCE_FILE} NAME_WE)
        message(STATUS "grammar: ${GRAMMAR_NAME} -> ${GRAMMAR_NAME}GENERATED_SRC")

        set(GENERATED_INCLUDES
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}BaseListener.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Listener.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Parser.h"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Visitor.h"
                )

        set(GENERATED_SOURCES
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}BaseListener.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Listener.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Parser.cpp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Visitor.cpp"
                )

        set(GENERATED_META
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.interp"
                "${CMAKE_CURRENT_BINARY_DIR}/generated/${GRAMMAR_NAME}Lexer.tokens"
                )

        set(GENERATED_SRC
            ${GENERATED_INCLUDES}
            ${GENERATED_SOURCES}
            ${GENERATED_META}
            )

        list(APPEND SOURCES ${GENERATED_INCLUDES})
        list(APPEND SOURCES ${GENERATED_SOURCES})
        foreach(GENERATED_FILE ${GENERATED_SRC})
            # Avoid CMake warning related to AUTOMOC and AUTOUI.
            # Theses files shouldn't be processed by the Qt machinery because
            # ANTLR generate code with something called "emit" in it.
            set_property(SOURCE ${GENERATED_FILE} PROPERTY SKIP_AUTOGEN ON)
        endforeach()
        set(GENERATION_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
        # Custom command to define how to generate files listed by GENERATED_SRC
        add_custom_command(OUTPUT ${GENERATED_SRC}
                COMMAND "${Java_JAVA_EXECUTABLE}" -cp "${LOCAL_ANTLR_GENERATOR_JAR}" org.antlr.v4.Tool
                -visitor -Dlanguage=Cpp ${GRAMMAR_SOURCE_FILE} -o ${GENERATION_OUTPUT_DIR}
                DEPENDS ${GRAMMAR_SOURCE_FILE}
                COMMENT "Generate language for grammar ${GRAMMAR_NAME}")
    endforeach(GRAMMAR_SOURCE_FILE)
    message(STATUS "generated file: ${SOURCES}")

    add_library(${GRAMMAR_LIBRARY_NAME} ${SOURCES})
    set_property(TARGET ${GRAMMAR_LIBRARY_NAME} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${GRAMMAR_LIBRARY_NAME} PROPERTY CXX_EXTENSIONS ON)
    set_property(TARGET ${GRAMMAR_LIBRARY_NAME} PROPERTY CXX_STANDARD_REQUIRED OFF)
    target_include_directories(${GRAMMAR_LIBRARY_NAME} PUBLIC ${GENERATION_OUTPUT_DIR} ${ANTLR4CPP_INCLUDE_DIRS})
    target_link_libraries(${GRAMMAR_LIBRARY_NAME} PUBLIC ${ANTLR4CPP_LIBS})
endfunction()

set(GRAMMAR_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/LCode.g4
    ${CMAKE_CURRENT_SOURCE_DIR}/LSystem.g4
)

add_library(lindenmayer-langage ${GRAMMAR_GENERATED_SOURCES})
generate_grammars(grammar-library "${GRAMMAR_SOURCE_FILES}")
target_include_directories(lindenmayer-langage PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${ANTLR4CPP_INCLUDE_DIRS}")
target_link_libraries(lindenmayer-langage PUBLIC "${ANTLR4CPP_LIBS}" grammar-library)
target_compile_definitions(lindenmayer-langage PUBLIC ANTLR4CPP_STATIC)





add_subdirectory(private)

set(LINDENMAYER_CORE_SOURCES
    src/Symbol.cpp
    include/Symbol.hpp
    src/Module.cpp
    include/Module.hpp
    src/RewriteRules.cpp
    include/RewriteRules.hpp
    src/Program.cpp
    include/Program.hpp
    src/ModuleTable.cpp
    include/ModuleTable.hpp
    src/ATurtle2D.cpp
    include/ATurtle2D.hpp
    private/LoadProgramFromLCode.hpp
    private/LoadProgramFromLCode.cpp
    include/Optional.hpp
    private/ContextHelper.cpp
    private/ContextHelper.hpp
    include/ActionFactory.hpp
    src/ActionFactory.cpp
    include/ModuleAction.hpp
    private/LCodeAST.cpp
    private/LCodeAST.hpp
    LCodeASTBuilder.cpp
    LCodeASTBuilder.hpp
    include/LCode/SymbolTable.hpp
    src/LCode/SymbolTable.cpp
        src/LCode/SymbolDefinitionPass.cpp
    include/LCode/SymbolDefinitionPass.hpp
        src/LCode/ScopeTree.cpp
    include/LCode/ScopeTree.hpp src/LCode/AbstractSyntaxTreeNode.cpp include/LCode/AbstractSyntaxTreeNode.hpp include/LCode/StorageType.hpp src/LCode/GatherConstantExpressionPass.cpp include/LCode/GatherConstantExpressionPass.hpp include/LCode/CompilationContext.hpp src/LCode/CompilationContext.cpp src/LCode/AbstractSyntaxTreeAlgorithms.cpp include/LCode/AbstractSyntaxTreeAlgorithms.hpp include/LCode/AbstractSyntaxTreeBuilder.cpp include/LCode/AbstractSyntaxTreeBuilder.hpp)

if (ENABLE_MSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
endif()

if (ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -fno-omit-frame-pointer")
endif()

if (ENABLE_SANITIZERS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

add_library(LindenmayerHeart ${LINDENMAYER_CORE_SOURCES})

target_link_libraries(LindenmayerHeart PUBLIC antlr4_static lindenmayer-langage)
target_include_directories(LindenmayerHeart PUBLIC "${CMAKE_SOURCE_DIR}/externals/optional")
target_include_directories(LindenmayerHeart PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private
		PUBLIC "${PROJECT_SOURCE_DIR}/externals/optional")

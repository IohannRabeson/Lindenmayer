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
        src/LCode/AbstractSyntaxTreeNode.cpp include/LCode/AbstractSyntaxTreeNode.hpp include/LCode/StorageType.hpp src/LCode/AbstractSyntaxTreeAlgorithms.cpp include/LCode/AbstractSyntaxTreeAlgorithms.hpp src/LCode/AbstractSyntaxTreeBuilder.cpp include/LCode/AbstractSyntaxTreeBuilder.hpp include/Symbol.hpp src/Symbol.cpp src/LCode/SymbolTable.cpp include/LCode/SymbolTable.hpp include/LCode/ScopeTree.hpp src/LCode/LCodeScopeTreeBuilder.cpp include/LCode/LCodeScopeTreeBuilder.hpp include/LCode/ScopeTreeAlgorithms.hpp src/LCode/LCodeProgram.cpp include/LCode/LCodeProgram.hpp src/LCode/ParseError.cpp include/LCode/ParseError.hpp include/LCode/StringFormat.hpp src/StringFormat.cpp include/LCode/LCodeScopeTree.hpp include/LCode/ParseErrorListener.cpp include/LCode/ParseErrorListener.hpp)

add_library(LindenmayerHeart ${LINDENMAYER_CORE_SOURCES})
toolkit_target_version(LindenmayerHeart "${TRAVIS_BUILD_NUMBER}")
toolkit_enable_cxx17(LindenmayerHeart)
target_link_libraries(LindenmayerHeart PUBLIC antlr4_static lindenmayer-langage)
target_include_directories(LindenmayerHeart PUBLIC "${CMAKE_SOURCE_DIR}/externals/optional")
target_include_directories(LindenmayerHeart PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/private
		PUBLIC "${PROJECT_SOURCE_DIR}/externals/optional")

# Prevent .ts files to be deleted when the project is cleaned?
set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM 1)

file(GLOB_RECURSE TRANSLATION_CPP_SOURCE_FILES
     "${PROJECT_SOURCE_DIR}/app/*.hpp" "${PROJECT_SOURCE_DIR}/app/*.cpp"
     "${PROJECT_SOURCE_DIR}/core/*.hpp" "${PROJECT_SOURCE_DIR}/core/*.cpp")

set(TRANSLATION_FILES
    MidiBoy_en.ts
    MidiBoy_fr.ts
)

qt5_create_translation(TRANSLATION_MESSAGES ${TRANSLATION_CPP_SOURCE_FILES} ${TRANSLATION_FILES})

foreach(TRANSLATION_FILE ${TRANSLATION_FILES})
list(APPEND TS_FILES_TEMP "${CMAKE_CURRENT_SOURCE_DIR}/${TRANSLATION_FILE}")
endforeach()
set(TS_FILES "${TS_FILES_TEMP}" PARENT_SCOPE)

add_custom_target(translations_update DEPENDS "${TRANSLATION_FILES}" COMMENT "Updating translation files...")
set_source_files_properties("${TRANSLATION_FILES}" PROPERTIES GENERATED TRUE)

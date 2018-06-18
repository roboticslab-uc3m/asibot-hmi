# Find Doxygen.
find_package(Doxygen)

if(DOXYGEN_FOUND)
  # Add user option and hide from GUIs.
  option(ASIBOT_HMI_DOXYGEN_HTML "Generate doxygen HTML output" ON)
  mark_as_advanced(ASIBOT_HMI_DOXYGEN_HTML)

  if(ASIBOT_HMI_DOXYGEN_HTML)
    # Specify dox file name.
    set(DOX_FILE Doxyfile)

    # Create "dox" target.
    add_custom_target(dox SOURCES "${CMAKE_SOURCE_DIR}/doc/${DOX_FILE}")

    # Add build rule for "dox" target.
    add_custom_command(TARGET dox
                       COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/doc/${DOX_FILE}"
                       COMMENT "Generating doxygen documentation")
  endif()

  # Register features.
  add_feature_info(ASIBOT_HMI_docs ASIBOT_HMI_DOXYGEN_HTML "ASIBOT_HMI documentation generator.")
endif()

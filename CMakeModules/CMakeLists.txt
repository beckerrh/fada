FILE(GLOB CMAKEFILES "*.cmake")
INSTALL(FILES ${CMAKEFILES} DESTINATION cmake/Modules)

CONFIGURE_FILE(
  ${CMAKE_CURRENT_SOURCE_DIR}/DefineProjectCmake.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/DefineProjectCmake.cmake @ONLY
  )
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/DefineProjectCmake.cmake DESTINATION cmake/Modules)

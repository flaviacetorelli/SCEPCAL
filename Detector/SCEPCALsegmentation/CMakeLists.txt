project(SCEPCALsegmentation)

file(GLOB sources
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB headers
  ${PROJECT_SOURCE_DIR}/include/*.h
)

add_library(SCEPCALsegmentation SHARED ${sources} ${headers})

target_include_directories(SCEPCALsegmentation PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

set_target_properties(SCEPCALsegmentation PROPERTIES PUBLIC_HEADER "${headers}")

target_link_libraries(
  SCEPCALsegmentation
  DD4hep::DDCore
)

dd4hep_generate_rootmap(SCEPCALsegmentation)

install(TARGETS SCEPCALsegmentation EXPORT DetectorTargets
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev
)

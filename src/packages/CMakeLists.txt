# src/packaging/CMakeLists.txt:jeff

if(NOT DEFINED nomlib_INSTALL_CMAKEDIR)
  set(nomlib_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/nomlib"
    CACHE STRING "Path to installing CMake import files")
endif(NOT DEFINED nomlib_INSTALL_CMAKEDIR)

if(NOT DEFINED nomlib_INSTALL_PKGCONFIGDIR)
  set(nomlib_INSTALL_PKGCONFIGDIR "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    CACHE STRING "Path to installing pkg-config helper")
endif(NOT DEFINED nomlib_INSTALL_PKGCONFIGDIR)

if(NOM_INSTALL_CMAKE_PACKAGE)
  include(CMakePackageConfigHelpers)

  set(INCLUDE_INSTALL_DIR "include/")

  # generate a cmake package version file for nomlib
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nomlibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    # One of: (AnyNewerVersion|SameMajorVersion|ExactVersion)
    COMPATIBILITY SameMajorVersion
  )

  # generate a config file from a template we create
  configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/templates/nomlibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nomlibConfig.cmake"
    INSTALL_DESTINATION "${nomlib_INSTALL_CMAKEDIR}"
    #COMPONENT Development
  )

  if(BUILD_SHARED_LIBS)
    set(type shared)
  else()
    set(type static)
  endif(BUILD_SHARED_LIBS)

  # !! Distribute nomlib cmake import file for using the engine externally
  install(EXPORT libtarget
    FILE "nomlib-${type}-targets.cmake"
    #NAMESPACE nom::
    DESTINATION "${nomlib_INSTALL_CMAKEDIR}"
    COMPONENT Development
  )

  # nomlib's tailored cmake package for importing into foreign cmake build
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nomlibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nomlibConfigVersion.cmake"
    DESTINATION "${nomlib_INSTALL_CMAKEDIR}"
    COMPONENT Development
  )
endif(NOM_INSTALL_CMAKE_PACKAGE)

if(NOM_INSTALL_PKGCONFIG_SUPPORT)
  # nomlib.pc.in -> nomlib.pc
  include(PkgConfig)

  # Install nomlib's pkg-config module so others can find us
  install(FILES
    "${CMAKE_BINARY_DIR}/nomlib.pc"
    DESTINATION "${nomlib_INSTALL_PKGCONFIGDIR}"
    COMPONENT Development
  )
endif(NOM_INSTALL_PKGCONFIG_SUPPORT)

# Install nomlib's icon files
install(  FILES
          "${PROJECT_SOURCE_DIR}/Resources/nomlib.icns"
          "${PROJECT_SOURCE_DIR}/Resources/nomlib.ico"
          "${PROJECT_SOURCE_DIR}/Resources/nomlib.png"
          DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
          OPTIONAL
          COMPONENT Core
)


# Install software license & general project information
install(  FILES
          ${PROJECT_SOURCE_DIR}/README.md
          ${PROJECT_SOURCE_DIR}/LICENSE.md
          DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
          COMPONENT Core
        )

# Install hardware definitions for input devices (game controllers)
if(NOM_INSTALL_GCDB)
  install(FILES
    "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
    COMPONENT Core
  )
endif()

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/nomlib.desktop"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
  OPTIONAL
  COMPONENT Core
)

install(DIRECTORY
  "${PROJECT_SOURCE_DIR}/licenses"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
  COMPONENT Core
  PATTERN ".*" EXCLUDE
)

# Install the generated documentation files
if(NOM_BUILD_DOCS)
  install(  DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
            DESTINATION "${CMAKE_INSTALL_DOCDIR}/${CMAKE_PROJECT_VERSION}"
            OPTIONAL
            COMPONENT Documentation
            PATTERN ".*" EXCLUDE # .DS_Store dotfiles
  )
endif(NOM_BUILD_DOCS)

# If enabled, other projects on your local host will be able to find nomlib
# without requiring the `install` target to be executed.
if(NOM_INSTALL_CMAKE_REGISTRY)
  # Export library targets for the build tree
  export(EXPORT libtarget
    FILE "${CMAKE_CURRENT_BINARY_DIR}/nomlib-${type}-targets.cmake"
    #Namespace nom::
  )

  # Register the build directory in the CMake user package registry
  export(PACKAGE nomlib)
endif()


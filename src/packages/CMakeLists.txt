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
    #COMPATIBILITY SameMinorVersion
    COMPATIBILITY ExactVersion
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
    "${PROJECT_SOURCE_DIR}/cmake/packaging/nomlibConfig.cmake"
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
          "${NOMLIB_RESOURCES_DIR}/nomlib.icns"
          "${NOMLIB_RESOURCES_DIR}/icon.png"
          DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
          OPTIONAL
          COMPONENT Core
)


# Install software license & general project information
install(  FILES
          ${PROJECT_SOURCE_DIR}/README.md
          ${PROJECT_SOURCE_DIR}/LICENSE.md
          # Install hardware definitions for input devices (game controllers)
          "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
          DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
          COMPONENT Core
        )

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

if(APPLE AND USE_OLD_OSX_CODE)
  # Bundle libraries we depend on in the appropriate Framework bundles

  if( NOM_BUILD_CORE_UNIT )
    install(  DIRECTORY ${SDL2_LIBRARY}
              DESTINATION "${PROJECT_NAME}-core.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_CORE_UNIT )

  if( NOM_BUILD_SYSTEM_UNIT )
    install(  DIRECTORY ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY}
              DESTINATION "${PROJECT_NAME}-system.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_SYSTEM_UNIT )

  if( NOM_BUILD_AUDIO_UNIT )
    install(  DIRECTORY ${LIBSNDFILE_LIBRARY}
              DESTINATION "${PROJECT_NAME}-audio.framework/Frameworks"
              PATTERN ".*" EXCLUDE
    )
  endif( NOM_BUILD_AUDIO_UNIT )

  if( NOM_BUILD_GUI_UNIT )
    # dylibs
    install(  FILES ${LIBROCKET_CORE_LIBRARY} ${LIBROCKET_CONTROLS_LIBRARY}
              ${LIBROCKET_DEBUGGER_LIBRARY}
              DESTINATION "${PROJECT_NAME}-gui.framework/Libraries"
    )
  endif( NOM_BUILD_GUI_UNIT )

  # nomlib's icon
  install(  FILES
            "${NOMLIB_RESOURCES_DIR}/nomlib.icns"
            DESTINATION "${PROJECT_NAME}.framework/Resources"
            OPTIONAL
  )

  # Install nomlib's CMake find module; this should end up in a spot that is
  # automatically searched by find_package.
  install(  DIRECTORY
            "${NOMLIB_RESOURCES_DIR}/CMake"
            DESTINATION "${PROJECT_NAME}.framework/Resources"
            PATTERN ".*" EXCLUDE
  )

  # Install nomlib's pkg-config module so third-party software can find us
  install(  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
            DESTINATION "${PROJECT_NAME}.framework/Resources")

  # Install software license & general project information
  install(  FILES
            ${PROJECT_SOURCE_DIR}/README.md
            ${PROJECT_SOURCE_DIR}/LICENSE.md
            DESTINATION "${PROJECT_NAME}.framework/Resources"
  )

  # Install the generated documentation files
  # if( NOM_BUILD_DOCS )
  #   install(  DIRECTORY ${PROJECT_BINARY_DIR}/docs/html
  #             DESTINATION "${PROJECT_NAME}.framework/Resources/Documentation"
  #             PATTERN ".*" EXCLUDE
  #   )
  # endif( NOM_BUILD_DOCS )

  # Install hardware definitions for input devices (game controllers)
  install(  DIRECTORY "${NOMLIB_SHARED_SUPPORT_DIR}/InputDevices"
            DESTINATION "${PROJECT_NAME}.framework/Resources/SharedSupport"
            OPTIONAL
            PATTERN ".*" EXCLUDE )
endif(APPLE AND USE_OLD_OSX_CODE)


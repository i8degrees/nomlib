# cmake/packaging.cmake:jeff
#
# CPack distribution (generic) setup
#
# SEE ALSO
# 1. https://cmake.org/cmake/help/latest/module/CPack.html
#

include(InstallRequiredSystemLibraries)
include("${PROJECT_SOURCE_DIR}/cmake/CPackConfig.cmake")

# Default generator type when the end-user has not explicitly chosen.
set(CPACK_GENERATOR "ZIP")


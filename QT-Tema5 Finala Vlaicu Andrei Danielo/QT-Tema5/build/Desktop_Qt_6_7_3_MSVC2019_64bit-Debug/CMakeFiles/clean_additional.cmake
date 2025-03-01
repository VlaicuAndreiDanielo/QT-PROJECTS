# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QT-Tema5_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QT-Tema5_autogen.dir\\ParseCache.txt"
  "QT-Tema5_autogen"
  )
endif()

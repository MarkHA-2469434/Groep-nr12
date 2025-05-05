# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\MonopolyQt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\MonopolyQt_autogen.dir\\ParseCache.txt"
  "MonopolyQt_autogen"
  )
endif()

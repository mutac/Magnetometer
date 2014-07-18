
set (INCLUDES
  ${CMAKE_SOURCE_DIR}
)

include_directories (${INCLUDES})

set (SOURCES
  mStd.cpp
  mString.cpp
  Variant.cpp
)

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif ()

add_library(resource ${SOURCES})

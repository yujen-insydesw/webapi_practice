cmake_minimum_required(VERSION 3.10)

# Project
project(MiniBoostRestAPI VERSION 1.0)

# C++ flag
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Pthread flag
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Include header
include_directories(MiniBoostRestAPI PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
# Source code
file(GLOB SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp
  )
# Executable
add_executable(MiniBoostRestAPI ${SOURCES})
# Additional flags
target_compile_options(MiniBoostRestAPI PRIVATE
  -Wall -Wpedantic -Wextra
  )

# Find Thread (could be pthread or win-thread)
find_package(Threads REQUIRED)
target_link_libraries(MiniBoostRestAPI Threads::Threads)

# Find Boost
find_package(Boost 1.71.0 REQUIRED COMPONENTS system)
if(Boost_VERSION VERSION_GREATER_EQUAL "1.75.0")
  # Boost Jason start from 1.75.0
  find_package(Boost 1.75.0 REQUIRED COMPONENTS system json)
  # Define BOOST_JSON
  add_definitions(-DBOOST_JSON)
endif()
target_include_directories(MiniBoostRestAPI PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(MiniBoostRestAPI ${Boost_LIBRARIES})

# Install rules
install(TARGETS MiniBoostRestAPI
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin # For Windows DLLs
)
install(DIRECTORY include/ DESTINATION include)

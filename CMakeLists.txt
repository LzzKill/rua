cmake_minimum_required(VERSION 3.26)
project(moonlisp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME})

# Module File
target_sources(${PROJECT_NAME}
  PUBLIC
  FILE_SET CXX_MODULES
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/module
  FILES
  "module/constant.ixx"
  "module/lexer.ixx"
  "module/exception.ixx"
)

# Source File
target_sources(${PROJECT_NAME}
  PRIVATE
  "source/main.cpp"
  "source/lexer.cpp"
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/module
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE 
    "/experimental:module"
    "/std:c++latest"
    "/ZI"  # Dynamic Debug
  )
  
  target_link_options(${PROJECT_NAME} PRIVATE "/INCREMENTAL")

  if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  endif()
  
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${PROJECT_NAME} PRIVATE
    "-fmodules"
    "-std=c++20"
  )
endif()
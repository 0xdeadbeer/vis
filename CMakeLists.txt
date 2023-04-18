cmake_minimum_required(VERSION 3.26.3)

include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v3.0.0
)
 
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
 
# ------------------------------------------------------------------------------
project(vis
    VERSION 1.0 
    DESCRIPTION "Vi Scheduler (vis) is a simple TUI program built for managing your schedules in a calendar-like grid."
    LANGUAGES CXX) 
 
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(vis src/main.cpp)
target_include_directories(vis PRIVATE src)
 
target_link_libraries(vis
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)

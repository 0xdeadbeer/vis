add_subdirectory(engine) 
add_subdirectory(calendar) 

add_executable(vis main.cpp)

target_link_libraries(vis engine) 
target_link_libraries(vis calendar) 

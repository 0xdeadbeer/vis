add_subdirectory(engine) 
add_subdirectory(calendar) 
add_subdirectory(base64) 

add_executable(vis main.cpp)

target_link_libraries(vis engine) 
target_link_libraries(vis calendar) 
target_link_libraries(vis base64) 

set(installable_libs engine calendar base64)
install(TARGETS ${installable_libs} DESTINATION lib)

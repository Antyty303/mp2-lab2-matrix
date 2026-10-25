file(GLOB benchmark_list RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
message("!!!!!! ${benchmark_list}")

foreach(benchmark_filename ${benchmark_list})

  get_filename_component(benchmark ${benchmark_filename} NAME_WE)

  add_executable(${benchmark} ${benchmark_filename})
  target_include_directories(${benchmark} PUBLIC "${MP2_INCLUDES}")
  target_link_libraries(${benchmark} PRIVATE benchmark::benchmark)
    
  set_target_properties(${benchmark} PROPERTIES
    OUTPUT_NAME "${benchmark}"
    PROJECT_LABEL "${benchmark}"
    RUNTIME_OUTPUT_DIRECTORY "../")
endforeach()

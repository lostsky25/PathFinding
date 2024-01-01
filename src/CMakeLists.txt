add_executable(path_finding
  Algorithms/BFS.cpp
  Algorithms/DFS.cpp

  Core/Application.cpp
  Core/EntryPoint.cpp
  Core/Layer.cpp
  Core/LayerStack.cpp
  Core/Log.cpp
  Core/MainWindow.cpp
  Core/PathFinding.cpp

  UI/ImGuiLayer.cpp
)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DPF_DEBUG")

## Copies sfml's dll
if (WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET path_finding POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:path_finding> $<TARGET_FILE_DIR:path_finding>
    COMMAND_EXPAND_LISTS
  )
endif()

target_compile_features(path_finding PRIVATE cxx_std_17)
target_include_directories(path_finding PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(path_finding
  PUBLIC
  ImGui-SFML::ImGui-SFML
  spdlog::spdlog
)

include(Install.cmake)
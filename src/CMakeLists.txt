add_executable(path_finding
  main.cpp
  AbstractGrid.cpp
  Application.cpp
  BFS.cpp
  DFS.cpp
  PathFinding.cpp
)

target_compile_features(path_finding PRIVATE cxx_std_17)

target_link_libraries(path_finding
  PUBLIC
  ImGui-SFML::ImGui-SFML
)

include(Install.cmake)
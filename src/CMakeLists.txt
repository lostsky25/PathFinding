add_executable(path-finding
  main.cpp
  AbstractGrid.cpp
  Application.cpp
  BFS.cpp
  PathFinding.cpp
)

# Yep, that's it!
target_link_libraries(path-finding
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)
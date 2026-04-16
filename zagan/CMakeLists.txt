add_library(zagan-core INTERFACE)
add_library(zagan::core ALIAS zagan-core)
target_include_directories(zagan-core INTERFACE include)
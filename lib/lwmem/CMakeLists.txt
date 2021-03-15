add_library(lwmem INTERFACE)

target_sources(lwmem INTERFACE
  src/lwmem/lwmem.c
  src/system/lwmem_sys_cmsis_os.c
)

target_include_directories(
  lwmem
  INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    src
    src/include
    src/lwmem
    src/include/lwmem
    src/include/system
)

target_link_libraries(
  lwmem
  INTERFACE WUI
)

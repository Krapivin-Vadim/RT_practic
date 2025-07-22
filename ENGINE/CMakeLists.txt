forte_add_directory_module(OPC_Client OFF "custom module")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_hcpp(Motor)
forte_add_sourcefile_hcpp(AEngine)
forte_add_sourcefile_hcpp(FooEngine)
forte_add_sourcefile_hcpp(MainEngine)
forte_add_sourcefile_hcpp(Engine_fbt)

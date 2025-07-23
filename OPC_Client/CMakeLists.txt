# This is a generated build-system-configuration file for 4diac forte
#
# To use the current directory-name as basis for the module-name use

forte_add_directory_module(OPC_Client OFF "custom module")

forte_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

forte_add_sourcefile_c(open62541.c)
forte_add_sourcefile_h(open62541.h)
forte_add_sourcefile_hcpp(OPC_Client_fbt)
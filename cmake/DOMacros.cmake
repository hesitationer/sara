macro(do_get_os_info)
  string(REGEX MATCH "Linux" OS_IS_LINUX ${CMAKE_SYSTEM_NAME})
  set(DO_LIB_INSTALL_DIR "lib")
  set(DO_INCLUDE_INSTALL_DIR
      "include/DO-${DO_MAJOR_VERSION}.${DO_MINOR_VERSION}")
endmacro(do_get_os_info)

macro(do_dissect_version)
  # Find version components
  string(REGEX REPLACE "^([0-9]+).*" "\\1"
         DO_VERSION_MAJOR "${DO_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1"
         DO_VERSION_MINOR "${DO_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1"
         DO_VERSION_PATCH ${DO_VERSION})
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1"
         DO_VERSION_CANDIDATE ${DO_VERSION})
  set(DO_SOVERSION "${DO_VERSION_MAJOR}.${DO_VERSION_MINOR}")
endmacro(do_dissect_version)




################################################################################
# Helper macros
# 
macro (do_message _msg)
  message (STATUS "[DO] ${_msg}")
endmacro (do_message _msg)


macro (do_step_message _msg)
  message ("[DO] ${_msg}")
endmacro (do_step_message _msg)


macro (do_substep_message _msg)
  message ("     ${_msg}")
endmacro (do_substep_message _msg)


macro (do_append_components _component_list _component)
  set(DO_${_component}_LIBRARIES DO_${_component})
  set(DO_${_component}_USE_FILE UseDO${_component})
  list(APPEND "${_component_list}" ${_component})
endmacro (do_append_components)


macro (do_list_files _src_files _rel_path _extension)
  file(GLOB _src_files
       RELATIVE ${_rel_path}
       FILES_MATCHING PATTERN ${_extension})
  foreach (l ${LIST})
    set(l ${PATH}/l)
    message (l)
  endforeach ()
  message (${LIST})
endmacro (do_list_files)


macro (do_append_library _library_name
                         _library_type # shared or static
                         _include_dirs
                         _hdr_files _src_files
                         _lib_dependencies)
  get_property(DO_${_library_name}_ADDED GLOBAL PROPERTY 
               _DO_${_library_name}_INCLUDED)
  if (NOT DO_${_library_name}_ADDED)
    # 1. Verbose
    #message(STATUS "[DO] Creating project 'DO${_library_name}'")
    # 2. Bookmark the project to make sure we don't try to add the library 
    #    more than once.
    set_property(GLOBAL PROPERTY _DO_${_library_name}_INCLUDED 1)
    # 3. Include third-party library directories.
    if (NOT "${_include_dirs}" STREQUAL "")
      include_directories(${_include_dirs})
    endif ()
    # 4. Create the project:
    if (NOT "${_src_files}" STREQUAL "")
      # - Case 1: the project contains 'cpp' source files
      #   Specify the source files.
      add_library(DO_${_library_name} ${LIBRARY_TYPE} ${_hdr_files} ${_src_files})
      # Link with external libraries
      #message(STATUS 
      #        "[DO] Linking project 'DO${_library_name}' with '${_lib_dependencies}'")
      target_link_libraries(DO_${_library_name} ${_lib_dependencies})
    else ()
      # - Case 2: the project is a header-only library
      #   Specify the source files.
      #add_library(DO_${_library_name} STATIC ${_hdr_files})
      #message(STATUS 
      #"[DO] No linking needed for header-only project 'DO.${_library_name}'")
      #set_target_properties(DO_${_library_name} PROPERTIES LINKER_LANGUAGE CXX)
      add_custom_target(DO_${_library_name} SOURCES ${_hdr_files})
    endif ()
  endif ()
  set_property(TARGET DO_${_library_name} PROPERTY FOLDER "DO Modules")
endmacro (do_append_library)


macro (do_cotire _target _master_header)
  # Experimental: create precompiled libraries
  set_target_properties(DO_${_target} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT
                        ${_master_header})
  cotire(DO_${_target})
  target_link_libraries(DO_${_target}_unity DO_${_target})
endmacro (do_cotire)


macro (do_unit_test _unit_test_name _srcs _additional_lib_deps)
  if (POLICY CMP0020)
    cmake_policy(SET CMP0020 OLD)
  endif (POLICY CMP0020)
  include_directories(${gtest_DIR}/include)
  add_executable(DO_${_unit_test_name}_test ${_srcs})
  target_link_libraries(DO_${_unit_test_name}_test
                        ${_additional_lib_deps}
                        gtest)
  set_target_properties(DO_${_unit_test_name}_test PROPERTIES
                        COMPILE_FLAGS -DSRCDIR=${CMAKE_CURRENT_SOURCE_DIR}
                        COMPILE_DEFINITIONS DO_STATIC)
  add_test(DO_${_unit_test_name}_test
           "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/DO_${_unit_test_name}_test")
endmacro (do_unit_test)


macro (do_set_specific_target_properties _target _additional_compile_flags)
  set_target_properties(${_target} PROPERTIES
                        VERSION ${DO_VERSION}
                        SOVERSION ${DO_SOVERSION}
                        COMPILE_DEFINITIONS ${_additional_compile_flags}
                        OUTPUT_NAME_DEBUG   ${_target}-${DO_VERSION}-d
                        OUTPUT_NAME_RELEASE ${_target}-${DO_VERSION})
endmacro (do_set_specific_target_properties)


macro (do_add_msvc_precompiled_header _pch _src_var)
  if (MSVC)
    get_filename_component(_pch_basename ${_pch} NAME_WE)
    set(_pch_binary "${CMAKE_CURRENT_BINARY_DIR}/${_pch_basename}.pch")
    set(_srcs ${${_src_var}})

    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    set(_pch_src ${CMAKE_CURRENT_BINARY_DIR}/${_pch_basename}.cpp)
    file(WRITE ${_pch_src} "// Precompiled header unity generated by CMake\n")
    file(APPEND ${_pch_src} "#include \"${_pch}\"\n")

    set_source_files_properties(${_pch_src} PROPERTIES
                                COMPILE_FLAGS "/Yc\"${_pch}\" /Fp\"${_pch_binary}\""
                                OBJECT_OUTPUTS "${_pch_binary}")
    set_source_files_properties(${_srcs} PROPERTIES
                                COMPILE_FLAGS "/Yu\"${_pch}\" /FI\"${_pch_binary}\" /Fp\"${_pch_binary}\""
                                OBJECT_DEPENDS "${_pch_binary}")
    list(APPEND ${_src_var} ${_pch_src})
  endif ()
endmacro (do_add_msvc_precompiled_header)

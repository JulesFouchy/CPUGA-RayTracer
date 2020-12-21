cmake_minimum_required(VERSION 3.9)

project(c3ga CXX)
set(CMAKE_BUILD_TYPE Release)

# include eigen
# find_package(Eigen3 REQUIRED)
# if(${EIGEN3_FOUND})
#     message(STATUS "lib EIGEN3 found")
#     message(STATUS "  version " ${EIGEN3_VERSION_STRING})
#     message(STATUS "  include " ${EIGEN3_INCLUDE_DIR})
# else()
include_directories("../eigen-3.3.9") # manually specify the include location
# endif()

# call the CMakeLists.txt to make the documentation (Doxygen)
# > 'make html' to generate the documentation
find_package(Doxygen OPTIONAL_COMPONENTS QUIET)
if(DOXYGEN_FOUND)
    add_subdirectory(doc)
else()
    message(WARNING "Doxygen not found, skip dococumentation")
endif()


# files to compile
set(source_files src/c3ga/Mvec.cpp)
file(GLOB_RECURSE header_files src/c3ga/*.hpp)

# display info
message(STATUS "  sources")
foreach(src_file ${source_files})
    message("          " ${src_file})
endforeach()
message(STATUS "  headers")
foreach(src_file ${header_files})
    message("          " ${src_file})
endforeach()



# generate a library
if (MSVC)
	add_library(c3ga STATIC ${source_files} ${header_files})
else()
	add_library(c3ga SHARED ${source_files} ${header_files})
endif()

# compilation flags
if (MSVC)   
    target_compile_features(c3ga PRIVATE cxx_std_14) 
    #target_compile_options(c3ga PRIVATE -Wall -O2) 
else()
    target_compile_features(c3ga PRIVATE cxx_std_14) 
    target_compile_options(c3ga PRIVATE -Wall -O2 -Wno-return-local-addr)
endif()
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftime-report ") # comilation profiling:  -Q -ftime-report
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-elide-constructors") # force the compilor to use our move 'copy constructor'

# include directory path
include_directories(src)
include_directories(${EIGEN3_INCLUDE_DIR})
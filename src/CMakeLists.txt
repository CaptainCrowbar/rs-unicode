cmake_minimum_required(VERSION 3.30)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# The version number here is the canonical one for the project. Run
# bump-version to increment the patch number and copy the new version to
# version.hpp.

project(rs-unicode
    VERSION 0.1.4
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
    add_compile_options(/EHsc /Gy /MP /O2 /sdl /utf-8 /W4 /WX)
else()
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    add_compile_options(-fdiagnostics-color=always -finput-charset=UTF-8 -march=native -O2 -Wall -Wextra -Wpedantic -Werror)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-Wsuggest-override -Wsuggest-destructor-override)
    endif()
endif()

if(WIN32)
    add_compile_definitions(NOMINMAX=1 UNICODE=1 _UNICODE=1 WINVER=0xa00 _WIN32_WINNT=0xa00)
else()
    add_compile_definitions(_REENTRANT=1 _XOPEN_SOURCE=700)
    if(APPLE)
        add_compile_definitions(_DARWIN_C_SOURCE=1)
    endif()
endif()

find_package(Threads REQUIRED)
include_directories(.)
set(library rs-unicode)
set(unittest test-rs-unicode)

add_library(${library} STATIC

    rs-unicode/encoding.cpp
    rs-unicode/legacy.cpp
    rs-unicode/regex.cpp
    rs-unicode/string.cpp
    rs-unicode/ucd-canonical-combining-class.cpp
    rs-unicode/ucd-canonical-composition-mapping.cpp
    rs-unicode/ucd-canonical-decomposition-mapping.cpp
    rs-unicode/ucd-case-folding.cpp
    rs-unicode/ucd-default-ignorable.cpp
    rs-unicode/ucd-east-asian-width.cpp
    rs-unicode/ucd-extended-pictographic.cpp
    rs-unicode/ucd-full-composition-exclusion.cpp
    rs-unicode/ucd-general-category.cpp
    rs-unicode/ucd-grapheme-cluster-break.cpp
    rs-unicode/ucd-indic-conjunct-break.cpp
    rs-unicode/ucd-line-break.cpp
    rs-unicode/ucd-pattern-syntax.cpp
    rs-unicode/ucd-pattern-white-space.cpp
    rs-unicode/ucd-simple-lowercase-mapping.cpp
    rs-unicode/ucd-simple-titlecase-mapping.cpp
    rs-unicode/ucd-simple-uppercase-mapping.cpp
    rs-unicode/ucd-special-casing.cpp
    rs-unicode/ucd-white-space.cpp
    rs-unicode/ucd-xid-continue.cpp
    rs-unicode/ucd-xid-start.cpp

)

add_executable(${unittest}

    test/character-properties-test.cpp
    test/encoding-conversion-test.cpp
    test/encoding-general-test.cpp
    test/encoding-iterators-test.cpp
    test/enum-test.cpp
    test/format-test.cpp
    test/legacy-test.cpp
    test/property-map-test.cpp
    test/regex-test.cpp
    test/string-casing-test.cpp
    test/string-iterators-test.cpp
    test/string-literals-test.cpp
    test/string-manipulation-test.cpp
    test/string-properties-test.cpp
    test/ucd-grapheme-break-test.cpp
    test/ucd-normalization-test.cpp
    test/unit-test.cpp
    test/version-test.cpp

)

target_link_libraries(${unittest}
    PRIVATE ${library}
    PRIVATE iconv
    PRIVATE pcre2-8
    PRIVATE Threads::Threads
)

install(FILES ${library}.hpp DESTINATION include)
install(DIRECTORY ${library} DESTINATION include PATTERN "*.cpp" EXCLUDE)
install(TARGETS ${library} LIBRARY DESTINATION lib)

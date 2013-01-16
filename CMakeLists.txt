# package
include(CheckTypeSize)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckLibraryExists)

set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	"${CMAKE_CURRENT_SOURCE_DIR}")


cmake_minimum_required(VERSION 2.6)
project(minikonoha)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")

SET(CFLAGS "-Wsign-compare -Wformat")
SET(CFLAGS "${CFLAGS} -Wall -Wcomment -Wunused-macros")
SET(CMAKE_C_FLAGS   "${CFLAGS} -Wno-long-long")
SET(CMAKE_CXX_FLAGS "${CFLAGS}")

if(MSVC)
	SET(CMAKE_C_FLAGS_RELEASE      "/D NDEBUG /W3 /Wall /TP /MT /Ox /Ob2 /GL")
	SET(CMAKE_C_FLAGS_DEBUG        "/D_DEBUG /W3 /Wall /TP /MTd /Zi /Ob0 /Od /RTC1")
	SET(CMAKE_C_FLAGS_COMPRESSED   "/D NDEBUG /W3 /Wall /TP /MT /Ox /GL")
else(MSVC)
	SET(CMAKE_C_FLAGS_RELEASE      "${CMAKE_C_FLAGS} -O3 -g")
	SET(CMAKE_C_FLAGS_DEBUG        "${CMAKE_C_FLAGS} -O0 -g3")
	SET(CMAKE_C_FLAGS_COMPRESSED   "${CMAKE_C_FLAGS} -Os -g0")
endif(MSVC)
SET(CMAKE_CXX_FLAGS_RELEASE    "${CMAKE_CXX_FLAGS} -O3 -g")
SET(CMAKE_CXX_FLAGS_DEBUG      "${CMAKE_CXX_FLAGS} -O0 -g3")
SET(CMAKE_CXX_FLAGS_COMPRESSED "${CMAKE_CXX_FLAGS} -Os -g0")

set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
set(SO_VERSION      ${VERSION_MAJOR}.${VERSION_MINOR})
set(PACKAGE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
if(${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")
	set(INCLUDE_DIRS ${INCLUDE_DIRS} /usr/pkg/include)
endif(${CMAKE_SYSTEM_NAME} MATCHES "NetBSD")

find_package(Konoha)
if(MINIKONOHA_FOUND)
	include_directories()
	set(INCLUDE_DIRS ${INCLUDE_DIRS} ${MINIKONOHA_INCLUDE_DIR})
endif()

macro(POP _STR _DELIM _RESULT)
	string(REPLACE ${_DELIM} ";" _tmp ${_STR})
	list(REVERSE _tmp)
	list(GET _tmp 0 ${_RESULT})
endmacro()

include_directories(${INCLUDE_DIRS})

set(PARENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PARENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
macro(add_test_with_property test_name)
	add_test(${test_name} minikonoha ${ARGN})
	set_tests_properties(${test_name} PROPERTIES TIMEOUT 5)
	set_tests_properties(${test_name} PROPERTIES ENVIRONMENT
		"KONOHA_HOME=${PARENT_BINARY_DIR}")
endmacro(add_test_with_property test_name)

macro(add_konoha_test test_name test_file)
	if(test_name MATCHES "Stress")
		if(TEST_STRESS)
			add_test_with_property(${test_name} -MOutputTest ${test_file})
			set_tests_properties(${test_name} PROPERTIES TIMEOUT 10)
		endif(TEST_STRESS)
	else(test_name MATCHES "Stress")
		if(NOT test_name MATCHES "^Shell")
			add_test_with_property(${test_name} -MOutputTest -DBUFSIZ=1024 -DPASSWD=XXX ${test_file})
		endif(NOT test_name MATCHES "^Shell")
	endif(test_name MATCHES "Stress")
endmacro(add_konoha_test test_name test_file)

macro(SET_CXX_PROPERTY_IF_MSVC TARGET)
	if(MSVC)
		foreach(arg ${ARGN})
			set_source_files_properties(${arg} PROPERTIES LANGUAGE CXX )
		endforeach(arg ${ARGN})
		set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX )
	endif(MSVC)
endmacro(SET_CXX_PROPERTY_IF_MSVC TARGET)

macro(PKGNAME _STR _RESULT)
	set(_tmp)
	POP(${_STR} "/" _tmp)
	POP(${_tmp} "." ${_RESULT})
endmacro()

macro(add_konoha_package _PKGNAME)
	if(NOT __NOT_BUILD)
		project(${_PKGNAME})
		message(STATUS "Compiling ${_PKGNAME} package")
		file(GLOB_RECURSE test_files ${CMAKE_CURRENT_SOURCE_DIR}/test/*.k)
		foreach(f ${test_files})
			string(REPLACE // / f ${f})
			string(REPLACE ${PARENT_SOURCE_DIR}/ "" test_name ${f})
			string(REPLACE /test "" test_name ${test_name})
			add_konoha_test(${test_name} ${f})
		endforeach(f ${test_files})
		set(CMAKE_BUILD_TYPE ${KONOHA_BUILD_TYPE})
		PKGNAME(${_PKGNAME} _PACKAGE_NAME)
		set(LIB  "${_PACKAGE_NAME}_glue")
		set(_PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
		set(_PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${_PACKAGE_VERSION})
		set(_KONOHA_PACKAGE_DIR lib/minikonoha/${SO_VERSION}/package/${KONOHA_VERSION}/${PROJECT_NAME})
		set(CMAKE_C_FLAGS_${uppercase_CMAKE_BUILD_TYPE}
			"${CMAKE_C_FLAGS_${uppercase_CMAKE_BUILD_TYPE}} ${PACKAGE_C_FLAGS}")
		set(CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}
			"${CMAKE_CXX_FLAGS_${uppercase_CMAKE_BUILD_TYPE}} ${PACKAGE_CXX_FLAGS}")
		set(_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
				${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_BINARY_DIR}
				${KONOHA_INCLUDE_DIRS}
				${_PACKAGE_INCLUDE_DIR})
		include_directories(${_INCLUDE_DIRS})
		list(LENGTH PACKAGE_SOURCE_CODE LEN)
		if(${LEN} GREATER 0)
			add_library(${LIB} SHARED ${PACKAGE_SOURCE_CODE})
			SET_CXX_PROPERTY_IF_MSVC(${LIB} ${PACKAGE_SOURCE_CODE})
			set_target_properties(${LIB} PROPERTIES PREFIX "")
			if(PACKAGE_EXTRA_LIBRARY)
				target_link_libraries(${LIB} ${PACKAGE_EXTRA_LIBRARY})
			endif(PACKAGE_EXTRA_LIBRARY)
			install(TARGETS ${LIB} DESTINATION ${_KONOHA_PACKAGE_DIR})
		endif(${LEN} GREATER 0)
		if(PACKAGE_SCRIPT_CODE)
			install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_SCRIPT_CODE}
				DESTINATION ${_KONOHA_PACKAGE_DIR})
			foreach(scriptcode ${PACKAGE_SCRIPT_CODE})
				add_custom_command(OUTPUT
					${CMAKE_CURRENT_BINARY_DIR}/${scriptcode}
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${CMAKE_CURRENT_SOURCE_DIR}/${scriptcode}
					${CMAKE_CURRENT_BINARY_DIR}/${scriptcode})
			endforeach(scriptcode ${PACKAGE_SCRIPT_CODE})
			add_custom_target(copy_${_PKGNAME}_scriptcode ALL DEPENDS
				${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_SCRIPT_CODE})
		endif(PACKAGE_SCRIPT_CODE)
		# for copy test files
		if(PACKAGE_TEST_FILE)
			foreach(testfile ${PACKAGE_TEST_FILE})
				add_custom_command(OUTPUT
					${CMAKE_CURRENT_BINARY_DIR}/${testfile}
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${CMAKE_CURRENT_SOURCE_DIR}/test/${testfile}
					${CMAKE_CURRENT_BINARY_DIR}/${testfile})
			endforeach(testfile ${PACKAGE_TEST_FILE})
			add_custom_target(copy_${_PKGNAME}_testfile ALL DEPENDS
				${PACKAGE_TEST_FILE})
		endif(PACKAGE_TEST_FILE)
		add_custom_command(OUTPUT ${PARENT_BINARY_DIR}/package/${_PKGNAME}
			COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_CURRENT_BINARY_DIR}
			${PARENT_BINARY_DIR}/package/${_PKGNAME})
		add_custom_target(create_symlink_${_PKGNAME} ALL DEPENDS
			${PARENT_BINARY_DIR}/package/${_PKGNAME})
	else(NOT __NOT_BUILD)
		foreach(_PKGLIB ${__NOT_FOUND})
			show_package_warning(${_PKGNAME} ${_PKGLIB})
		endforeach(_PKGLIB ${__NOT_FOUND})
	endif(NOT __NOT_BUILD)
endmacro(add_konoha_package)

file(GLOB PACKAGES ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(dir ${PACKAGES})
	if(IS_DIRECTORY ${dir} AND EXISTS ${dir}/CMakeLists.txt)
		add_subdirectory(${dir})
	endif()
endforeach()

macro(create_dir DIR_NAME)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DIR_NAME}
		COMMAND ${CMAKE_COMMAND} -E make_directory
		${CMAKE_CURRENT_BINARY_DIR}/${DIR_NAME})
	add_custom_target(make_${DIR_NAME}_dir ALL DEPENDS
		${CMAKE_CURRENT_BINARY_DIR}/${DIR_NAME})
endmacro(create_dir DIR_NAME)

create_dir(package)

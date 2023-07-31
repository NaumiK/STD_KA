cmake_minimum_required(VERSION 3.24)
project(STD_PRJ)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
message(STATUS "Collected libs: ${LIBLIST}")
foreach(LIB ${LIBLIST})
    target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB})
    target_include_directories(${LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach(LIB ${LIBLIST})

add_subdirectory(apps)
message(STATUS "Collected apps: ${APPLIST}")

foreach(APP ${APPLIST})
    target_include_directories(${APP} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

    target_include_directories(${APP} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/${APP}")

    target_link_libraries(${APP} ${LIBLIST})
endforeach(APP ${APPLIST})

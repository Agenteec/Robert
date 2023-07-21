# CMakeList.txt: проект CMake для Robert; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  # Поиск CXX
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH})
endif()
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Robert")

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

find_package(box2d CONFIG REQUIRED)

# Добавление путей к заголовочным файлам SFML
include_directories(${SFML_INCLUDE_DIR})
include_directories(${Box2D_INCLUDE_DIR})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#include_directories("/usr/include")
endif()
# Добавьте источник в исполняемый файл этого проекта.
add_executable (Robert "Robert.cpp" "Robert.h")

target_link_libraries(Robert PRIVATE sfml-system sfml-network sfml-graphics sfml-window box2d::box2d)



# Укажите директорию с файлами для копирования
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/Resources)

# Укажите директорию, в которую нужно скопировать файлы
set(DESTINATION_DIR ${CMAKE_BINARY_DIR})

# Копируйте файлы
file(COPY ${SOURCE_DIR} DESTINATION ${DESTINATION_DIR})

#target_link_libraries(Robert PRIVATE )
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Robert PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

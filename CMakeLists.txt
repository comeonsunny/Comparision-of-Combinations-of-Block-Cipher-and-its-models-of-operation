cmake_minimum_required(VERSION 3.10)

project(CMP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/opt/homebrew/Cellar/libtomcrypt/1.18.2/include/)
link_directories(/opt/homebrew/Cellar/libtomcrypt/1.18.2/lib)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/log)

add_executable(CMP
	main.cc
	Cipher.cc
)
target_link_libraries(CMP -ltomcrypt)
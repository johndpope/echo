﻿MESSAGE( STATUS "Configuring module: 7zip ...")

# Set Module Name
SET(MODULE_NAME zlib)

# 包含目录
INCLUDE_DIRECTORIES( ${ECHO_SRC_PATH})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR})

# 链接库目录
LINK_DIRECTORIES(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# 包含文件
SET(ZLIB_HEADER_PATH ${ECHO_ROOT_PATH}/thirdparty/zlib)
SET(ZLIB_SOURTH_PATH ${ECHO_ROOT_PATH}/thirdparty/zlib)

#IF(ECHO_BUILD_PLATFORM_HTML5)

SET(HEADER_FILES
	${ZLIB_HEADER_PATH}/zconf.h
	${ZLIB_HEADER_PATH}/zlib.h
	${ZLIB_HEADER_PATH}/crc32.h
	${ZLIB_HEADER_PATH}/deflate.h
	${ZLIB_HEADER_PATH}/gzguts.h
	${ZLIB_HEADER_PATH}/inffast.h
	${ZLIB_HEADER_PATH}/inffixed.h
	${ZLIB_HEADER_PATH}/inflate.h
	${ZLIB_HEADER_PATH}/inftrees.h
	${ZLIB_HEADER_PATH}/trees.h
	${ZLIB_HEADER_PATH}/zutil.h
)
		
SET(SOURCE_FILES
	${ZLIB_SOURTH_PATH}/adler32.c
	${ZLIB_SOURTH_PATH}/compress.c
	${ZLIB_SOURTH_PATH}/crc32.c
	${ZLIB_SOURTH_PATH}/deflate.c
	${ZLIB_SOURTH_PATH}/gzclose.c
	${ZLIB_SOURTH_PATH}/gzlib.c
	${ZLIB_SOURTH_PATH}/gzread.c
	${ZLIB_SOURTH_PATH}/gzwrite.c
	${ZLIB_SOURTH_PATH}/inflate.c
	${ZLIB_SOURTH_PATH}/infback.c
	${ZLIB_SOURTH_PATH}/inftrees.c
	${ZLIB_SOURTH_PATH}/inffast.c
	${ZLIB_SOURTH_PATH}/trees.c
	${ZLIB_SOURTH_PATH}/uncompr.c
	${ZLIB_SOURTH_PATH}/zutil.c
)

SET(ALL_FILES ${HEADER_FILES} ${SOURCE_FILES})

# 分组
GROUP_FILES(ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR})

IF(ECHO_UNICODE)
	ADD_DEFINITIONS("-DUNICODE -D_UNICODE")
ENDIF()

# 生成库
ADD_LIBRARY(${MODULE_NAME} ${ALL_FILES} CMakeLists.txt)

# 设置目录
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "thirdparty")

# ??????
MESSAGE(STATUS "Configure success!")

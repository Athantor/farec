
FIND_PACKAGE(Qt4 4.5.0 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(CMakeCommons.txt)

include_directories( . ${CMAKE_CURRENT_BINARY_DIR} libfarec)

#################3

SET(farec_SRCS
  farec
  FarecApp
  farecmainwin
)

STRING(TOUPPER ${CMAKE_BUILD_TYPE} MYCBT)
IF( ${MYCBT} STREQUAL DEBUG )
	add_definitions("-DDEBUG_KRZYS")
	
	SET(farec_SRCS	${farec_SRCS} farecmainwin_tests )
ENDIF()

SET(farec_UIS
	farecmainwin.ui
)

SET(farec_MOCS_H
	farecmainwin.hh
)

QT4_WRAP_CPP(farec_MOCS_SRCS ${farec_MOCS_H})


QT4_WRAP_UI(farec_UIS_H ${farec_UIS})

ADD_SUBDIRECTORY(libfarec)

ADD_EXECUTABLE(farec ${farec_SRCS} ${farec_UIS_H} ${farec_MOCS_SRCS})

TARGET_LINK_LIBRARIES(farec ${QT_LIBRARIES} fr)
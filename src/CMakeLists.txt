
FIND_PACKAGE(Qt4 4.5.0 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(CMakeCommons.txt)

include_directories( . ${CMAKE_CURRENT_BINARY_DIR} libfarec)

#################3

SET(farec_ui_SRCS
  ui/farecmainwin
  ui/GradientDisplay
  ui/Graph
  ui/DbConnect
  ui/PersonAdder
  ui/Persons
)

SET(farec_SRCS
  farec
  FarecApp
)

IF(NOT DEFINED ${CMAKE_BUILD_TYPE})
	STRING(TOUPPER ${CMAKE_BUILD_TYPE} MYCBT)
ELSE()
	SET(MYCBT "RELEASE")
ENDIF()

IF( ${MYCBT} STREQUAL DEBUG )
	add_definitions("-DDEBUG_KRZYS")
	
	SET(farec_SRCS ${farec_SRCS} ui/farecmainwin_tests )
ENDIF()

SET(farec_UIS
	ui/farecmainwin.ui
	ui/GradientDisplay.ui
	ui/DbConnect.ui
	ui/PersonAdder.ui
	ui/Persons.ui
)

SET(farec_MOCS_H
	ui/farecmainwin.hh
	ui/GradientDisplay.hh
	ui/Graph.hh
	ui/DbConnect.hh
	ui/PersonAdder.hh
	ui/Persons.hh
)

SET(farec_RC
	ui/icons.qrc
)

QT4_ADD_RESOURCES(farec_RC_F ${farec_RC})

QT4_WRAP_CPP(farec_MOCS_SRCS ${farec_MOCS_H})


QT4_WRAP_UI(farec_UIS_H ${farec_UIS})

ADD_SUBDIRECTORY(libfarec)

ADD_EXECUTABLE(farec ${farec_ui_SRCS} ${farec_MOCS_SRCS} ${farec_SRCS} ${farec_UIS_H} ${farec_RC_F})

TARGET_LINK_LIBRARIES(farec ${QT_LIBRARIES} fr)
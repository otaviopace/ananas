set(SOURCES
	${SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/fesetexceptflag.c
	${CMAKE_CURRENT_SOURCE_DIR}/feholdexcept.c
	${CMAKE_CURRENT_SOURCE_DIR}/fenv.c
	${CMAKE_CURRENT_SOURCE_DIR}/feupdateenv.c
	${CMAKE_CURRENT_SOURCE_DIR}/__flt_rounds.c
	${CMAKE_CURRENT_SOURCE_DIR}/fesetround.c
	PARENT_SCOPE
)

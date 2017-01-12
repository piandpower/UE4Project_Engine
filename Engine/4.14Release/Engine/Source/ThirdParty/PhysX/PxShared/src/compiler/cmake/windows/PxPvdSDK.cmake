#
# Build PxPvdSDK
#

SET(GW_DEPS_ROOT $ENV{GW_DEPS_ROOT})

SET(PXSHARED_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../../../../src)

SET(LL_SOURCE_DIR ${PXSHARED_SOURCE_DIR}/pvd)

FIND_PACKAGE(nvToolsExt REQUIRED)

SET(PXPVDSDK_LIBTYPE SHARED)

SET(PXPVDSDK_PLATFORM_FILES
	${PXSHARED_SOURCE_DIR}/compiler/resource_${LIBPATH_SUFFIX}/PxPvdSDK.rc
)

SET(PXPVDSDK_PLATFORM_INCLUDES
	${NVTOOLSEXT_INCLUDE_DIRS}
)

# Use generator expressions to set config specific preprocessor definitions
SET(PXPVDSDK_COMPILE_DEFS
	# Common to all configurations
	${PXSHARED_WINDOWS_COMPILE_DEFS};PX_PVDSDK_DLL=1;PX_FOUNDATION_DLL=1;

	$<$<CONFIG:debug>:${PXSHARED_WINDOWS_DEBUG_COMPILE_DEFS};>
	$<$<CONFIG:checked>:${PXSHARED_WINDOWS_CHECKED_COMPILE_DEFS};>
	$<$<CONFIG:profile>:${PXSHARED_WINDOWS_PROFILE_COMPILE_DEFS};>
	$<$<CONFIG:release>:${PXSHARED_WINDOWS_RELEASE_COMPILE_DEFS};>
)
	
# include PxPvdSDK common
INCLUDE(../common/PxPvdSDK.cmake)

# Add linked libraries
TARGET_LINK_LIBRARIES(PxPvdSDK PUBLIC ${NVTOOLSEXT_LIBRARIES} PxFoundation)



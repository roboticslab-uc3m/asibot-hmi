include(GitInfo) # YCM

# Define current version.
set(ASIBOT_HMI_VERSION_MAJOR 0)
set(ASIBOT_HMI_VERSION_MINOR 1)
set(ASIBOT_HMI_VERSION_PATCH 0)

set(ASIBOT_HMI_VERSION
    ${ASIBOT_HMI_VERSION_MAJOR}.${ASIBOT_HMI_VERSION_MINOR}.${ASIBOT_HMI_VERSION_PATCH})

set(ASIBOT_HMI_VERSION_SHORT ${ASIBOT_HMI_VERSION})

# Retrieve current revision from Git working tree.
git_wt_info(SOURCE_DIR "${CMAKE_SOURCE_DIR}"
            PREFIX ASIBOT_HMI)

if(DEFINED ASIBOT_HMI_GIT_WT_HASH)
    if(ASIBOT_HMI_GIT_WT_TAG_REVISION GREATER 0)
        set(ASIBOT_HMI_VERSION_REVISION ${ASIBOT_HMI_GIT_WT_TAG_REVISION})
        string(REPLACE "-" "" _date ${ASIBOT_HMI_GIT_WT_AUTHOR_DATE})
        set(ASIBOT_HMI_VERSION_SOURCE
            "${_date}.${ASIBOT_HMI_GIT_WT_DATE_REVISION}+git${ASIBOT_HMI_GIT_WT_HASH_SHORT}")
    endif()

    if(ASIBOT_HMI_GIT_WT_DIRTY)
        set(ASIBOT_HMI_VERSION_DIRTY "dirty")
    endif()
endif()

if(DEFINED ASIBOT_HMI_VERSION_SOURCE)
    if(NOT "${ASIBOT_HMI_GIT_WT_TAG}" STREQUAL "v${ASIBOT_HMI_VERSION_SHORT}")
        set(ASIBOT_HMI_VERSION
            "${ASIBOT_HMI_VERSION_SHORT}+${ASIBOT_HMI_VERSION_SOURCE}")
    else()
        set(ASIBOT_HMI_VERSION
           "${ASIBOT_HMI_VERSION_SHORT}+${ASIBOT_HMI_VERSION_REVISION}-${ASIBOT_HMI_VERSION_SOURCE}")
    endif()
elseif(NOT "${ASIBOT_HMI_GIT_WT_TAG}" STREQUAL "v${ASIBOT_HMI_VERSION_SHORT}")
    set(ASIBOT_HMI_VERSION "${ASIBOT_HMI_VERSION_SHORT}~dev")
else()
    set(ASIBOT_HMI_VERSION "${ASIBOT_HMI_VERSION_SHORT}")
endif()

if(DEFINED ASIBOT_HMI_VERSION_DIRTY)
    set(ASIBOT_HMI_VERSION "${ASIBOT_HMI_VERSION}+${ASIBOT_HMI_VERSION_DIRTY}")
endif()

# Print version.
message(STATUS "ASIBOT_HMI version: ${ASIBOT_HMI_VERSION_SHORT} (${ASIBOT_HMI_VERSION})")

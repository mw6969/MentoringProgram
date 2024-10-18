find_path(CRYPTOPP_INCLUDE_DIR cryptopp/cryptlib.h)
find_library(CRYPTOPP_LIBRARY NAMES cryptopp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CryptoPP DEFAULT_MSG CRYPTOPP_INCLUDE_DIR CRYPTOPP_LIBRARY)

if(CRYPTOPP_FOUND)
    set(CRYPTOPP_INCLUDE_DIRS ${CRYPTOPP_INCLUDE_DIR})
    set(CRYPTOPP_LIBRARIES ${CRYPTOPP_LIBRARY})
    mark_as_advanced(CRYPTOPP_INCLUDE_DIR CRYPTOPP_LIBRARY)
endif()
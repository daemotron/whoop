include(CheckIncludeFiles)

check_include_files(openssl/ssl.h HAVE_OPENSSL_H)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

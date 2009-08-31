include(CheckIncludeFiles)
include(CheckSymbolExists)

check_include_files(syslog.h HAVE_SYSLOG_H)
check_include_files(openssl/ssl.h HAVE_OPENSSL_H)

check_symbol_exists(LOG_EMERG   "syslog.h" HAVE_LOG_EMERG_S)
check_symbol_exists(LOG_ALERT   "syslog.h" HAVE_LOG_ALERT_S)
check_symbol_exists(LOG_CRIT    "syslog.h" HAVE_LOG_CRIT_S)
check_symbol_exists(LOG_ERR     "syslog.h" HAVE_LOG_ERR_S)
check_symbol_exists(LOG_WARNING "syslog.h" HAVE_LOG_WARNING_S)
check_symbol_exists(LOG_NOTICE  "syslog.h" HAVE_LOG_NOTICE_S)
check_symbol_exists(LOG_INFO    "syslog.h" HAVE_LOG_INFO_S)
check_symbol_exists(LOG_DEBUG   "syslog.h" HAVE_LOG_DEBUG_S)

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

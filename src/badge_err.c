
#include "badge_err.h"

#ifdef BADGEROS_KERNEL
#include "log.h"
#include "panic.h"
#else
#include <stdio.h>
#include <stdlib.h>
#endif

// Names for badge_eloc_t.
char const *badge_eloc_name[_badge_eloc_num] = {
    "ELOC_UNKNOWN",
    "ELOC_GPIO",
    "ELOC_I2C",
    "ELOC_UART",
    "ELOC_SPI",
    "ELOC_DISP",
    "ELOC_IP",
    "ELOC_WIFI",
    "ELOC_BT",
    "ELOC_RADIO",
    "ELOC_EXTMEM",
    "ELOC_INTMEM",
    "ELOC_THREADS",
    "ELOC_FILESYSTEM",
    "ELOC_BLKDEV",
    "ELOC_PROCESS",
};

// Names for badge_ecause_t.
char const *badge_ecause_name[_badge_ecause_num] = {
    "ECAUSE_OK",        "ECAUSE_UNKNOWN",   "ECAUSE_PARAM",      "ECAUSE_RANGE",       "ECAUSE_TOOLONG",
    "ECAUSE_TOOSHORT",  "ECAUSE_NOMEM",     "ECAUSE_PERM",       "ECAUSE_TIMEOUT",     "ECAUSE_UNEXPECTED",
    "ECAUSE_NOENT",     "ECAUSE_FORMAT",    "ECAUSE_RESET",      "ECAUSE_REFUSED",     "ECAUSE_INUSE",
    "ECAUSE_NOTCONFIG", "ECAUSE_UNAVAIL",   "ECAUSE_NOROUTE",    "ECAUSE_BREAK",       "ECAUSE_UNSUPPORTED",
    "ECAUSE_NOTACK",    "ECAUSE_CANCELLED", "ECAUSE_ILLEGAL",    "ECAUSE_READONLY",    "ECAUSE_NOTFOUND",
    "ECAUSE_IS_DIR",    "ECAUSE_IS_FILE",   "ECAUSE_EXISTS",     "ECAUSE_NOSPACE",     "ECAUSE_NOTEMPTY",
    "ECAUSE_STATE",     "ECAUSE_IOERR",     "ECAUSE_WOULDBLOCK", "ECAUSE_PIPE_CLOSED", "ECAUSE_UNSEEKABLE",
    "ECAUSE_BAD_FD",    "ECAUSE_CROSSDEV",
};



// Get the name of a badge_eloc_t.
char const *badge_eloc_get_name(badge_eloc_t eloc) {
    return (eloc < 0 || eloc >= _badge_eloc_num) ? "unknown location" : badge_eloc_name[eloc];
}

// Get the name of a badge_ecause_t.
char const *badge_ecause_get_name(badge_ecause_t ecause) {
    return (ecause < 0 || ecause >= _badge_ecause_num) ? "unknown cause" : badge_ecause_name[ecause];
}



void badge_err_log_impl(badge_err_t ec, bool is_err, char const *file, int line) {
#ifdef BADGEROS_KERNEL
    logkf(
        is_err ? LOG_ERROR : LOG_WARN,
        "%{cs}:%{d}: %{cs} error: %{cs}",
        file,
        line,
        badge_eloc_get_name(ec.location),
        badge_ecause_get_name(ec.cause)
    );
#else
    if (is_err) {
        fputs("\033[31m", stdout);
    } else {
        fputs("\033[33m", stdout);
    }
    printf("%s:%d: %s error: %s", file, line, badge_eloc_get_name(ec.location), badge_ecause_get_name(ec.cause));
#endif
}

void badge_err_assert_failed(badge_err_t ec, char const *file, int line) {
#ifdef BADGEROS_KERNEL
    logkf(
        LOG_FATAL,
        "%{cs}:%{d}: %{cs} error: %{cs}",
        file,
        line,
        badge_eloc_get_name(ec.location),
        badge_ecause_get_name(ec.cause)
    );
    panic_abort();
#else
    fputs("\033[31m", stdout);
    printf("%s:%d: %s error: %s", file, line, badge_eloc_get_name(ec.location), badge_ecause_get_name(ec.cause));
    abort();
#endif
}

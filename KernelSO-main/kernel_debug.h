#ifndef INCLUDE_KERNEL_DEBUG_H
#define INCLUDE_KERNEL_DEBUG_H

#include "serial.h"

#ifndef KERNEL_DEBUG
#define KERNEL_DEBUG 0
#endif

#if KERNEL_DEBUG
#define KDEBUG_LOG(message) serial_write(message)
#else
#define KDEBUG_LOG(message) do { (void)(message); } while (0)
#endif

#endif /* INCLUDE_KERNEL_DEBUG_H */
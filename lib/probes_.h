/*
 * Copyright (c) 2014-2016 DeNA Co., Ltd., Kazuho Oku, Fastly, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef h2o__probes_h
#define h2o__probes_h

/* This file is placed under lib, and must only be included from the source files of the h2o / libh2o, because H2O_USE_DTRACE is a
 * symbol available only during the build phase of h2o.  That's fine, because only h2o / libh2o has the sole right to define probes
 * belonging to the h2o namespace.
 */
#if H2O_USE_DTRACE

#include "picotls.h"
#include "h2o-probes.h"

#define H2O_CONN_IS_PROBED(label, conn) (PTLS_UNLIKELY(H2O_H2O_##label##_ENABLED()) && h2o_conn_is_traced(conn))

#define H2O_PROBE_CONN0(label, conn)                                                                                               \
    do {                                                                                                                           \
        h2o_conn_t *_conn = (conn);                                                                                                \
        if (H2O_CONN_IS_PROBED(label, _conn)) {                                                                                    \
            H2O_H2O_##label(_conn->id);                                                                                            \
        }                                                                                                                          \
    } while (0)

#define H2O_PROBE_CONN(label, conn, ...)                                                                                           \
    do {                                                                                                                           \
        h2o_conn_t *_conn = (conn);                                                                                                \
        if (H2O_CONN_IS_PROBED(label, _conn)) {                                                                                    \
            H2O_H2O_##label(_conn->id, __VA_ARGS__);                                                                               \
        }                                                                                                                          \
    } while (0)

#define H2O_PROBE(label, ...)                                                                                                      \
    do {                                                                                                                           \
        if (PTLS_UNLIKELY(H2O_H2O_##label##_ENABLED())) {                                                                          \
            H2O_H2O_##label(__VA_ARGS__);                                                                                          \
        }                                                                                                                          \
    } while (0)

#define H2O_PROBE_HEXDUMP(s, l)                                                                                                    \
    ({                                                                                                                             \
        size_t _l = (l);                                                                                                           \
        ptls_hexdump(alloca(_l * 2 + 1), (s), _l);                                                                                 \
    })
#else

#define H2O_CONN_IS_PROBED(label, conn) (0)
#define H2O_PROBE_CONN(label, conn, ...)
#define H2O_PROBE(label, ...)
#define H2O_PROBE_HEXDUMP(s, l)

#endif
#endif
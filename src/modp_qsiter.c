/**
 * \file
 * <pre>
 * modp_qs.c query string key-value pair iterator
 * https://github.com/client9/stringencoders
 *
 * Copyright &copy; 2012-2016  Nick Galbreath
 * All rights reserved.
 * Released under MIT license. See LICENSE for details.
 * </PRE>
 */

#include "modp_qsiter.h"

void qsiter_reset(struct qsiter_t* qsi, const char* s, size_t len)
{
    qsi->s = s;
    qsi->len = len;
    qsi->pos = 0;

    qsi->key = NULL;
    qsi->keylen = 0;
    qsi->val = NULL;
    qsi->vallen = 0;
}

int qsiter_next(struct qsiter_t* qsi)
{
    const char* eq;
    const char* charstart;
    const char* ends;

    if (qsi->pos >= qsi->len) {
        qsi->key = NULL;
        qsi->keylen = 0;
        qsi->val = NULL;
        qsi->vallen = 0;
        return 0;
    }

    charstart = qsi->s + qsi->pos;
    ends = (const char*)memchr(charstart, '&', qsi->len - qsi->pos);

    if (ends == NULL) {
        eq = (const char*)memchr(charstart, '=', qsi->len - qsi->pos);
        if (eq == NULL) {
            qsi->key = charstart;
            qsi->keylen = (size_t)(qsi->len - qsi->pos);
            qsi->val = NULL;
            qsi->vallen = (size_t)0;
        } else {
            qsi->key = charstart;
            qsi->keylen = (size_t)(eq - charstart);
            qsi->val = eq + 1;
            qsi->vallen = (size_t)((qsi->s + qsi->len) - qsi->val);
        }
        qsi->pos = qsi->len;
        return 1;
    } else {
        /* &&foo=bar */
        eq = (const char*)memchr(charstart, '=', (size_t)(ends - charstart));
        if (eq == NULL) {
            qsi->key = charstart;
            qsi->keylen = (size_t)(ends - charstart);
            qsi->val = NULL;
            qsi->vallen = (size_t)0;
        } else {
            qsi->key = charstart;
            qsi->keylen = (size_t)(eq - charstart);
            qsi->val = eq + 1;
            qsi->vallen = (size_t)(ends - eq - 1);
        }
        qsi->pos = (size_t)((ends - qsi->s) + 1);
        return 1;
    }
}

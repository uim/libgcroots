
/*
 * Copyright 1988, 1989 Hans-J. Boehm, Alan J. Demers
 * Copyright (c) 1991-1995 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 2000 by Hewlett-Packard Company.  All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/*
 * ChangeLog
 *
 * 2006-12-23 YAMAMOTO Kengo <yamaken AT bp.iij4u.or.jp>
 *            - Imported from mark.c of gc7.0alpha7
 *            - Remove all functions but GC_noop1()
 */

# include "private/gcroots_priv.h"
# include "private/gc_priv.h"

/* Single argument version, robust against whole program analysis. */
void GC_noop1(word x)
{
    static volatile word sink;

    sink = x;
}

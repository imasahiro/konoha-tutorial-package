/****************************************************************************
 * Copyright (c) 2012, the Konoha project authors. All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************************************/

/* ************************************************************************ */

#include <minikonoha/minikonoha.h>
#include <minikonoha/sugar.h>
#include <minikonoha/klib.h>
#include <minikonoha/import/methoddecl.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* Tutorial4 */


// --------------------------------------------------------------------------
static kbool_t Tutorial4_PackupNameSpace(KonohaContext *kctx, kNameSpace *ns, int option, KTraceInfo *trace)
{
	{ /* Func[int, int] */
		kparamtype_t p = {KType_int, 0};
		KClass *FuncClass = KLIB KClass_Generics(kctx,
				KClass_Func, /* this class */
				KType_int,   /* return type */
				2,           /* param size */
				&p           /* param list */
				);
	}

	{ /* Func[void, Object, String] */
		kparamtype_t p[] = {{KType_Object}, {KType_String}};
		KClass *FuncClass = KLIB KClass_Generics(kctx, KClass_Func, KType_void, 2, p);
	}

	{ /* Array[int] */
		KClass *FuncClass = KLIB KClass_Generics(kctx,
				KClass_Array, /* this class */
				KType_void,   /* return type */
				1,           /* param size */
				&p           /* param list */
				);
	}

	{ /* Array[int] */
		kparamtype_t p = {KType_int, 0};
		KClass *ArrayClass = KClass_p0(kctx,
				KClass_Array, /* this class */
				KType_int,    /* param type */
				);
	}

	return true;
}

static kbool_t Tutorial4_ExportNameSpace(KonohaContext *kctx, kNameSpace *ns, kNameSpace *exportNS, int option, KTraceInfo *trace)
{
	return true;
}

KDEFINE_PACKAGE *Tutorial4_Init(void)
{
	static KDEFINE_PACKAGE d = {0};
	KSetPackageName(d, "Tutorial4", "0.0");
	d.PackupNameSpace = Tutorial4_PackupNameSpace;
	d.ExportNameSpace = Tutorial4_ExportNameSpace;
	return &d;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

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
/* Tutorial5 */
static KMETHOD System_hello_world(KonohaContext *kctx, KonohaStack *sfp)
{
	int n = sfp[1].intValue;
	kMethod *mtd = NULL;
	BEGIN_UnusedStack(lsfp);
	KUnsafeFieldSet(lsfp[K_CALLDELTA+0].asObject, K_NULL);
	KUnsafeFieldSet(lsfp[K_CALLDELTA+1].intValue, n);
	{
		KStackSetMethodAll(lsfp,
				KLIB Knull(kctx, KClass_Int), /* Default Object of ReturnType */
				0/*UL*/, /* line number */
				mtd, /* call method */
				1 /* argc */);
		KStackCall(lsfp);
	}
	END_UnusedStack();
	int ret = lsfp[0].intValue;
	KReturnUnboxValue(ret);
}

// --------------------------------------------------------------------------
static kbool_t Tutorial5_PackupNameSpace(KonohaContext *kctx, kNameSpace *ns, int option, KTraceInfo *trace)
{
	KDEFINE_METHOD MethodData[] = {
		_Public|_Im, _F(System_hello_world), KType_Int, KType_System, KMethodName_("hello_world"), 1, KType_Int, KFieldName_("n"),
		DEND,
	};
	KLIB kNameSpace_LoadMethodData(kctx, ns, MethodData, trace);
	return true;
}

static kbool_t Tutorial5_ExportNameSpace(KonohaContext *kctx, kNameSpace *ns, kNameSpace *exportNS, int option, KTraceInfo *trace)
{
	return true;
}

KDEFINE_PACKAGE *Tutorial5_Init(void)
{
	static KDEFINE_PACKAGE d = {0};
	KSetPackageName(d, "Tutorial5", "0.0");
	d.PackupNameSpace = Tutorial5_PackupNameSpace;
	d.ExportNameSpace = Tutorial5_ExportNameSpace;
	return &d;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

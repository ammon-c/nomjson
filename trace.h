//-----------------------------------------------------------------------------
// trace.h
// Routine to output formatted debug text to console.
//
// Define preprocessor symbol TRACE to enable trace output; otherwise the
// trace statements aren't compiled into the code.
//
// (C) Copyright 1997-2019 Ammon R. Campbell.
//
// I wrote this code for use in my own educational and experimental
// programs, but you may also freely use it in yours as long as you
// abide by the following terms and conditions:
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above
//     copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials
//     provided with the distribution.
//   * The name(s) of the author(s) and contributors (if any) may not
//     be used to endorse or promote products derived from this
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.  IN OTHER WORDS, USE AT YOUR OWN RISK, NOT OURS.  
//-----------------------------------------------------------------------------

#pragma once
#pragma warning(disable:4505)
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#if defined (TRACE) && defined(_DEBUG)
# define trace dbg_trace
#else
  // Trick the optimizer into removing it entirely.
# define trace 1 ? (void)0 : dbg_trace
#endif

#ifdef TRACE
// cppcheck-suppress unusedFunction
static void dbg_trace(_Printf_format_string_ const char *pszFmt, ...)
{
   std::vector<char> buffer(8192, 0);
   va_list vv;

   va_start(vv, pszFmt);
   vsprintf_s(&buffer[0], buffer.size(), pszFmt, vv);
   va_end(vv);
   printf("%s", &buffer[0]);
   fflush(stdout);
}
#else
// cppcheck-suppress unusedFunction
static void dbg_trace(const char *pszFmt, ...)
{
   (void)pszFmt;
   // Do nothing.
}
#endif

// Unicode version of above.
#ifdef TRACE
// cppcheck-suppress unusedFunction
static void dbg_trace(_Printf_format_string_ const wchar_t *pszFmt, ...)
{
   std::vector<wchar_t> buffer(8192, 0);
   va_list vv;

   va_start(vv, pszFmt);
   vswprintf_s(&buffer[0], buffer.size(), pszFmt, vv);
   va_end(vv);
   wprintf(L"%s", &buffer[0]);
   fflush(stdout);
}
#else
// cppcheck-suppress unusedFunction
static void dbg_trace(const wchar_t *pszFmt, ...)
{
   (void)pszFmt;
   // Do nothing.
}
#endif


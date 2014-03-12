/* This file is part of the 'stringi' package for R.
 * Copyright (c) 2013-2014, Marek Gagolewski and Bartek Tartanus
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "stri_stringi.h"


/**
 * Default constructor
 *
 */
StriContainerRegexPattern::StriContainerRegexPattern()
   : StriContainerUTF16()
{
   this->str = NULL;
   this->lastMatcher = NULL;
}


/**
 * Construct String Container from R character vector
 * @param rstr R character vector
 * @param nrecycle extend length [vectorization]
 * @param flags regexp flags
 */
StriContainerRegexPattern::StriContainerRegexPattern(SEXP rstr, R_len_t _nrecycle, uint32_t _flags)
   : StriContainerUTF16(rstr, _nrecycle, true)
{
   this->lastMatcher = NULL;
   this->flags = _flags;
}



/** Copy constructor
 *
 */
StriContainerRegexPattern::StriContainerRegexPattern(StriContainerRegexPattern& container)
   :    StriContainerUTF16((StriContainerUTF16&)container)
{
   this->lastMatcher = NULL;
   this->flags = container.flags;
}




StriContainerRegexPattern& StriContainerRegexPattern::operator=(StriContainerRegexPattern& container)
{
   this->~StriContainerRegexPattern();
   (StriContainerUTF16&) (*this) = (StriContainerUTF16&)container;
   this->lastMatcher = NULL;
   this->flags = container.flags;
   return *this;
}


/** Destructor
 *
 */
StriContainerRegexPattern::~StriContainerRegexPattern()
{
   if (lastMatcher) {
      delete lastMatcher;
      lastMatcher = NULL;
   }
}




/** the returned matcher shall not be deleted by the user
 *
 * it is assumed that \code{vectorize_next()} is used:
 * for \code{i >= this->n} the last matcher is returned
 *
 * @param i index
 */
RegexMatcher* StriContainerRegexPattern::getMatcher(R_len_t i)
{
   if (lastMatcher) {
      if (i >= n) {
#ifndef NDEBUG
      if ((debugMatcherIndex % n) != (i % n)) {
         throw StriException("DEBUG: vectorize_getMatcher - matcher reuse failed!");
      }
#endif
         return lastMatcher; // reuse
      }
      else {
         delete lastMatcher;
         lastMatcher = NULL;
      }
   }

   UErrorCode status = U_ZERO_ERROR;
   lastMatcher = new RegexMatcher(this->get(i), flags, status);
   if (U_FAILURE(status))
      throw StriException(status);
#ifndef NDEBUG
   debugMatcherIndex = (i % n);
#endif

   return lastMatcher;
}



uint32_t StriContainerRegexPattern::getRegexFlags(SEXP opts_regex)
{
   uint32_t flags = 0;
   if (!Rf_isVectorList(opts_regex))
      Rf_error(MSG__ARG_EXPECTED_LIST, "opts_regex"); // error() call allowed here

   R_len_t narg = LENGTH(opts_regex);

   if (narg > 0) {

      SEXP names = Rf_getAttrib(opts_regex, R_NamesSymbol);
      if (names == R_NilValue || LENGTH(names) != narg)
         Rf_error(MSG__RESOURCE_ERROR_GET); // error() call allowed here

      for (R_len_t i=0; i<narg; ++i) {
         if (STRING_ELT(names, i) == NA_STRING)
            Rf_error(MSG__RESOURCE_ERROR_GET); // error() call allowed here

         const char* curname = CHAR(STRING_ELT(names, i));

         if  (!strcmp(curname, "case_insensitive")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "case_insensitive");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_CASE_INSENSITIVE;
         } else if  (!strcmp(curname, "comments")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "comments");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_COMMENTS;
         } else if  (!strcmp(curname, "dotall")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "dotall");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_DOTALL;
         } else if  (!strcmp(curname, "literal")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "literal");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_LITERAL;
         } else if  (!strcmp(curname, "multiline")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "multiline");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_MULTILINE;
         } else if  (!strcmp(curname, "unix_lines")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "unix_lines");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_UNIX_LINES;
         } else if  (!strcmp(curname, "uword")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "uword");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_UWORD;
         } else if  (!strcmp(curname, "error_on_unknown_escapes")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_regex, i), "error_on_unknown_escapes");
            if (LOGICAL(val)[0]==TRUE) flags |= UREGEX_ERROR_ON_UNKNOWN_ESCAPES;
         } else {
            Rf_warning(MSG__INCORRECT_REGEX_OPTION, curname);
         }
      }
   }

   return flags;
}

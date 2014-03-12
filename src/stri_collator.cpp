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
 * Create & set up an ICU Collator
 *
 * WARNING: this fuction is allowed to call the error() function.
 * Use before STRI__ERROR_HANDLER_BEGIN (with other prepareargs).
 *
 * @param opts_collator named R list or NA
 * @return NULL if no collator is requested (iff opts_collator equals NA);
 * otherwise, a Collator object that should be closed with ucol_close()
 * after use
 *
 *
 * @version 0.1 (Marek Gagolewski)
 */
UCollator* stri__ucol_open(SEXP opts_collator)
{
   if (Rf_isVectorList(opts_collator)) {
      R_len_t narg = LENGTH(opts_collator);

      if (narg <= 0) { // no custom settings - Collator'll be default-as-hell
         UErrorCode err = U_ZERO_ERROR;
         UCollator* col = ucol_open(NULL, &err);
         if (U_FAILURE(err)) {
            Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here
         }
         return col;
      }

      SEXP names = Rf_getAttrib(opts_collator, R_NamesSymbol);
      if (names == R_NilValue || LENGTH(names) != narg)
         Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here

      // search for locale & create collator
      UErrorCode err = U_ZERO_ERROR;
      UCollator* col = NULL;
      for (R_len_t i=0; i<narg; ++i) {
         if (STRING_ELT(names, i) == NA_STRING)
            Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here
         const char* curname = CHAR(STRING_ELT(names, i));
         if (!strcmp(curname, "locale")) {
            const char* qloc = stri__prepare_arg_locale(VECTOR_ELT(opts_collator, i), "locale", true);
            col = ucol_open(qloc, &err);
            break;
         }
      }

      if (!col) col = ucol_open(NULL, &err); // default locale

      if (U_FAILURE(err)) {
         Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here
      }

      // other opts
      for (R_len_t i=0; i<narg; ++i) {
         if (STRING_ELT(names, i) == NA_STRING)
            Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here

         const char* curname = CHAR(STRING_ELT(names, i));
         err = U_ZERO_ERROR;

         if (!strcmp(curname, "locale")) {
            // ignore
         } else if  (!strcmp(curname, "strength")) {
            SEXP val = stri_prepare_arg_integer_1(VECTOR_ELT(opts_collator, i), "strength");
            ucol_setAttribute(col, UCOL_STRENGTH, (UColAttributeValue)(INTEGER(val)[0]-1), &err);
         } else if  (!strcmp(curname, "alternate_shifted")) {
            SEXP val = stri_prepare_arg_string_1(VECTOR_ELT(opts_collator, i), "alternate_shifted");
            ucol_setAttribute(col, UCOL_ALTERNATE_HANDLING, LOGICAL(val)[0]?UCOL_SHIFTED:UCOL_NON_IGNORABLE, &err);
         } else if  (!strcmp(curname, "french")) {
            SEXP val = stri_prepare_arg_logical_1(VECTOR_ELT(opts_collator, i), "french");
            ucol_setAttribute(col, UCOL_CASE_FIRST,
               (LOGICAL(val)[0]==NA_LOGICAL?UCOL_OFF:(LOGICAL(val)[0]?UCOL_UPPER_FIRST:UCOL_LOWER_FIRST)), &err);
         } else if  (!strcmp(curname, "uppercase_first")) {
            bool val_bool = stri__prepare_arg_logical_1_notNA(VECTOR_ELT(opts_collator, i), "uppercase_first");
            ucol_setAttribute(col, UCOL_ALTERNATE_HANDLING, val_bool?UCOL_ON:UCOL_OFF, &err);
         } else if  (!strcmp(curname, "case_level")) {
            bool val_bool = stri__prepare_arg_logical_1_notNA(VECTOR_ELT(opts_collator, i), "case_level");
            ucol_setAttribute(col, UCOL_CASE_LEVEL, val_bool?UCOL_ON:UCOL_OFF, &err);
         } else if  (!strcmp(curname, "normalization")) {
            bool val_bool = stri__prepare_arg_logical_1_notNA(VECTOR_ELT(opts_collator, i), "normalization");
            ucol_setAttribute(col, UCOL_NORMALIZATION_MODE, val_bool?UCOL_ON:UCOL_OFF, &err);
         } else if  (!strcmp(curname, "numeric")) {
            bool val_bool = stri__prepare_arg_logical_1_notNA(VECTOR_ELT(opts_collator, i), "numeric");
            ucol_setAttribute(col, UCOL_NUMERIC_COLLATION, val_bool?UCOL_ON:UCOL_OFF, &err);
         } else {
            Rf_warning(MSG__INCORRECT_COLLATOR_OPTION, curname);
         }

         if (U_FAILURE(err)) {
            Rf_error(MSG__RESOURCE_ERROR_GET); // error() allowed here
         }
      }

      return col;
   }
   else {
      // arg is not a list - is it a single NA then?
      opts_collator = stri_prepare_arg_logical_1(opts_collator, "opts_collator_not_list");
      if (LOGICAL(opts_collator)[0] != NA_LOGICAL)
         Rf_error(MSG__INCORRECT_INTERNAL_ARG); // error() allowed here
      return NULL;
   }
}

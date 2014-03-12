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
 * Replace all occurences of a character class
 *
 * @param str character vector; strings to search in
 * @param pattern character vector; charclasses to search for
 * @param replacement character vector; strings to replace with
 *
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-07)
 * @version 0.2 (Marek Gagolewski, 2013-06-15) Use StrContainerCharClass
 * @version 0.3 (Marek Gagolewski, 2013-06-16) make StriException-friendly
 */
SEXP stri_replace_all_charclass(SEXP str, SEXP pattern, SEXP replacement)
{
   str          = stri_prepare_arg_string(str, "str");
   pattern      = stri_prepare_arg_string(pattern, "pattern");
   replacement  = stri_prepare_arg_string(replacement, "replacement");
   R_len_t vectorize_length = stri__recycling_rule(true, 3, LENGTH(str), LENGTH(pattern), LENGTH(replacement));

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerUTF8 replacement_cont(replacement, vectorize_length);
   StriContainerCharClass pattern_cont(pattern, vectorize_length);

   SEXP ret;
   PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   String8 buf(0); // @TODO: calculate buf len a priori?

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      if (str_cont.isNA(i) || replacement_cont.isNA(i) || pattern_cont.isNA(i)) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      CharClass pattern_cur = pattern_cont.get(i);
      R_len_t str_cur_n     = str_cont.get(i).length();
      const char* str_cur_s = str_cont.get(i).c_str();
      R_len_t j, jlast;
      UChar32 chr;

      R_len_t sumbytes = 0;
      deque<R_len_t_x2> occurences;
      for (jlast=j=0; j<str_cur_n; ) {
         U8_NEXT(str_cur_s, j, str_cur_n, chr);
         if (pattern_cur.test(chr)) {
            occurences.push_back(R_len_t_x2(jlast, j));
            sumbytes += j-jlast;
         }
         jlast = j;
      }

      if (occurences.size() == 0) {
         SET_STRING_ELT(ret, i, str_cont.toR(i)); // no change
         continue;
      }

      R_len_t     replacement_cur_n = replacement_cont.get(i).length();
      const char* replacement_cur_s = replacement_cont.get(i).c_str();
      R_len_t buf_need = str_cur_n+(R_len_t)occurences.size()*replacement_cur_n-sumbytes;
      buf.resize(buf_need);

      jlast = 0;
      char* curbuf = buf.data();
      deque<R_len_t_x2>::iterator iter = occurences.begin();
      for (; iter != occurences.end(); ++iter) {
         R_len_t_x2 match = *iter;
         memcpy(curbuf, str_cur_s+jlast, (size_t)match.v1-jlast);
         curbuf += match.v1-jlast;
         jlast = match.v2;
         memcpy(curbuf, replacement_cur_s, (size_t)replacement_cur_n);
         curbuf += replacement_cur_n;
      }
      memcpy(curbuf, str_cur_s+jlast, (size_t)str_cur_n-jlast);
      SET_STRING_ELT(ret, i, Rf_mkCharLenCE(buf.data(), buf_need, CE_UTF8));
   }

   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}



/**
 * Replace first or last occurence of a character class [internal]
 *
 * @param str character vector; strings to search in
 * @param pattern character vector; charclasses to search for
 * @param replacement character vector; strings to replace with
 * @param first replace first (TRUE) or last (FALSE)?
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-06)
 * @version 0.2 (Marek Gagolewski, 2013-06-15) Use StrContainerCharClass
 * @version 0.3 (Marek Gagolewski, 2013-06-16) make StriException-friendly
 */
SEXP stri__replace_firstlast_charclass(SEXP str, SEXP pattern, SEXP replacement, bool first)
{
   str          = stri_prepare_arg_string(str, "str");
   pattern      = stri_prepare_arg_string(pattern, "pattern");
   replacement  = stri_prepare_arg_string(replacement, "replacement");
   R_len_t vectorize_length = stri__recycling_rule(true, 3, LENGTH(str), LENGTH(pattern), LENGTH(replacement));

   STRI__ERROR_HANDLER_BEGIN
   StriContainerUTF8 str_cont(str, vectorize_length);
   StriContainerUTF8 replacement_cont(replacement, vectorize_length);
   StriContainerCharClass pattern_cont(pattern, vectorize_length);


   SEXP ret;
   PROTECT(ret = Rf_allocVector(STRSXP, vectorize_length));

   String8 buf(0); // @TODO: consider calculating buflen a priori

   for (R_len_t i = pattern_cont.vectorize_init();
         i != pattern_cont.vectorize_end();
         i = pattern_cont.vectorize_next(i))
   {
      if (str_cont.isNA(i) || replacement_cont.isNA(i) || pattern_cont.isNA(i)) {
         SET_STRING_ELT(ret, i, NA_STRING);
         continue;
      }

      CharClass pattern_cur = pattern_cont.get(i);
      R_len_t str_cur_n     = str_cont.get(i).length();
      const char* str_cur_s = str_cont.get(i).c_str();
      R_len_t j, jlast;
      UChar32 chr;

      if (first) { // search for first
         for (jlast=j=0; j<str_cur_n; ) {
            U8_NEXT(str_cur_s, j, str_cur_n, chr); // "look ahead"
            if (pattern_cur.test(chr)) {
               break; // break at first occurence
            }
            jlast = j;
         }
      }
      else { // search for last
        for (jlast=j=str_cur_n; jlast>0; ) {
            U8_PREV(str_cur_s, 0, jlast, chr); // "look behind"
            if (pattern_cur.test(chr)) {
               break; // break at first occurence
            }
            j = jlast;
         }
      }

      // match is at jlast, and ends right before j

      if (j == jlast) { // iff not found
         SET_STRING_ELT(ret, i, str_cont.toR(i)); // no change
         continue;
      }

      R_len_t     replacement_cur_n = replacement_cont.get(i).length();
      const char* replacement_cur_s = replacement_cont.get(i).c_str();
      R_len_t buf_need = str_cur_n+replacement_cur_n-(j-jlast);
      buf.resize(buf_need);
      memcpy(buf.data(), str_cur_s, (size_t)jlast);
      memcpy(buf.data()+jlast, replacement_cur_s, (size_t)replacement_cur_n);
      memcpy(buf.data()+jlast+replacement_cur_n, str_cur_s+j, (size_t)str_cur_n-j);
      SET_STRING_ELT(ret, i, Rf_mkCharLenCE(buf.data(), buf_need, CE_UTF8));
   }

   UNPROTECT(1);
   return ret;
   STRI__ERROR_HANDLER_END(;/* nothing special to be done on error */)
}



/**
 * Replace first occurence of a character class
 *
 * @param str character vector; strings to search in
 * @param pattern character vector; charclasses to search for
 * @param replacement character vector; strings to replace with
 *
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-06)
 */
SEXP stri_replace_first_charclass(SEXP str, SEXP pattern, SEXP replacement)
{
   return stri__replace_firstlast_charclass(str, pattern, replacement, true);
}



/**
 * Replace last occurence of a character class
 *
 * @param str character vector; strings to search in
 * @param pattern character vector; charclasses to search for
 * @param replacement character vector; strings to replace with
 *
 * @return character vector
 *
 * @version 0.1 (Marek Gagolewski, 2013-06-06)
 */
SEXP stri_replace_last_charclass(SEXP str, SEXP pattern, SEXP replacement)
{
   return stri__replace_firstlast_charclass(str, pattern, replacement, false);
}

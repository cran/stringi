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


const UProperty CharClass::NA_binprop = UCHAR_INVALID_CODE;
const uint32_t  CharClass::NA_gcmask  = 0xffffffff;



// these static vars are automatically generated via an R script
const R_len_t CharClass::binprop_maxchars = 32;

const R_len_t CharClass::binprop_length = 57;

const char* CharClass::binprop_names[] = { // sorted by binprop_names_normalized
   "ALPHABETIC", "ASCII_HEX_DIGIT", "BIDI_CONTROL", "BIDI_MIRRORED", "CASED",
   "CASE_IGNORABLE", "CASE_SENSITIVE", "CHANGES_WHEN_CASEFOLDED",
   "CHANGES_WHEN_CASEMAPPED", "CHANGES_WHEN_LOWERCASED",
   "CHANGES_WHEN_NFKC_CASEFOLDED", "CHANGES_WHEN_TITLECASED",
   "CHANGES_WHEN_UPPERCASED", "DASH", "DEFAULT_IGNORABLE_CODE_POINT", "DEPRECATED",
   "DIACRITIC", "EXTENDER", "FULL_COMPOSITION_EXCLUSION", "GRAPHEME_BASE",
   "GRAPHEME_EXTEND", "GRAPHEME_LINK", "HEX_DIGIT", "HYPHEN", "ID_CONTINUE",
   "IDEOGRAPHIC", "IDS_BINARY_OPERATOR", "ID_START", "IDS_TRINARY_OPERATOR",
   "JOIN_CONTROL", "LOGICAL_ORDER_EXCEPTION", "LOWERCASE", "MATH", "NFC_INERT",
   "NFD_INERT", "NFKC_INERT", "NFKD_INERT", "NONCHARACTER_CODE_POINT",
   "PATTERN_SYNTAX", "PATTERN_WHITE_SPACE", "POSIX_ALNUM", "POSIX_BLANK",
   "POSIX_GRAPH", "POSIX_PRINT", "POSIX_XDIGIT", "QUOTATION_MARK", "RADICAL",
   "SEGMENT_STARTER", "SOFT_DOTTED", "S_TERM", "TERMINAL_PUNCTUATION",
   "UNIFIED_IDEOGRAPH", "UPPERCASE", "VARIATION_SELECTOR", "WHITE_SPACE",
   "XID_CONTINUE", "XID_START"
};

const char* CharClass::binprop_names_normalized[] = { // sorted by binprop_names_normalized
   "ALPHABETIC", "ASCIIHEXDIGIT", "BIDICONTROL", "BIDIMIRRORED", "CASED",
   "CASEIGNORABLE", "CASESENSITIVE", "CHANGESWHENCASEFOLDED",
   "CHANGESWHENCASEMAPPED", "CHANGESWHENLOWERCASED", "CHANGESWHENNFKCCASEFOLDED",
   "CHANGESWHENTITLECASED", "CHANGESWHENUPPERCASED", "DASH",
   "DEFAULTIGNORABLECODEPOINT", "DEPRECATED", "DIACRITIC", "EXTENDER",
   "FULLCOMPOSITIONEXCLUSION", "GRAPHEMEBASE", "GRAPHEMEEXTEND", "GRAPHEMELINK",
   "HEXDIGIT", "HYPHEN", "IDCONTINUE", "IDEOGRAPHIC", "IDSBINARYOPERATOR",
   "IDSTART", "IDSTRINARYOPERATOR", "JOINCONTROL", "LOGICALORDEREXCEPTION",
   "LOWERCASE", "MATH", "NFCINERT", "NFDINERT", "NFKCINERT", "NFKDINERT",
   "NONCHARACTERCODEPOINT", "PATTERNSYNTAX", "PATTERNWHITESPACE", "POSIXALNUM",
   "POSIXBLANK", "POSIXGRAPH", "POSIXPRINT", "POSIXXDIGIT", "QUOTATIONMARK",
   "RADICAL", "SEGMENTSTARTER", "SOFTDOTTED", "STERM", "TERMINALPUNCTUATION",
   "UNIFIEDIDEOGRAPH", "UPPERCASE", "VARIATIONSELECTOR", "WHITESPACE",
   "XIDCONTINUE", "XIDSTART"
};

const UProperty CharClass::binprop_code[] = { // sorted by binprop_names_normalized
   UCHAR_ALPHABETIC, UCHAR_ASCII_HEX_DIGIT, UCHAR_BIDI_CONTROL,
   UCHAR_BIDI_MIRRORED, UCHAR_CASED, UCHAR_CASE_IGNORABLE, UCHAR_CASE_SENSITIVE,
   UCHAR_CHANGES_WHEN_CASEFOLDED, UCHAR_CHANGES_WHEN_CASEMAPPED,
   UCHAR_CHANGES_WHEN_LOWERCASED, UCHAR_CHANGES_WHEN_NFKC_CASEFOLDED,
   UCHAR_CHANGES_WHEN_TITLECASED, UCHAR_CHANGES_WHEN_UPPERCASED, UCHAR_DASH,
   UCHAR_DEFAULT_IGNORABLE_CODE_POINT, UCHAR_DEPRECATED, UCHAR_DIACRITIC,
   UCHAR_EXTENDER, UCHAR_FULL_COMPOSITION_EXCLUSION, UCHAR_GRAPHEME_BASE,
   UCHAR_GRAPHEME_EXTEND, UCHAR_GRAPHEME_LINK, UCHAR_HEX_DIGIT, UCHAR_HYPHEN,
   UCHAR_ID_CONTINUE, UCHAR_IDEOGRAPHIC, UCHAR_IDS_BINARY_OPERATOR, UCHAR_ID_START,
   UCHAR_IDS_TRINARY_OPERATOR, UCHAR_JOIN_CONTROL, UCHAR_LOGICAL_ORDER_EXCEPTION,
   UCHAR_LOWERCASE, UCHAR_MATH, UCHAR_NFC_INERT, UCHAR_NFD_INERT, UCHAR_NFKC_INERT,
   UCHAR_NFKD_INERT, UCHAR_NONCHARACTER_CODE_POINT, UCHAR_PATTERN_SYNTAX,
   UCHAR_PATTERN_WHITE_SPACE, UCHAR_POSIX_ALNUM, UCHAR_POSIX_BLANK,
   UCHAR_POSIX_GRAPH, UCHAR_POSIX_PRINT, UCHAR_POSIX_XDIGIT, UCHAR_QUOTATION_MARK,
   UCHAR_RADICAL, UCHAR_SEGMENT_STARTER, UCHAR_SOFT_DOTTED, UCHAR_S_TERM,
   UCHAR_TERMINAL_PUNCTUATION, UCHAR_UNIFIED_IDEOGRAPH, UCHAR_UPPERCASE,
   UCHAR_VARIATION_SELECTOR, UCHAR_WHITE_SPACE, UCHAR_XID_CONTINUE, UCHAR_XID_START
};



/** Get desired character class code from given name
 *
 * @param charclass CHARSXP, can be NA
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-02)
 * @version 0.1-24 (Marek Gagolewski, 2014-03-11) gcmask is now uint32_t, not UCharCategory
 */
CharClass::CharClass(SEXP charclass)
{
#ifndef NDEBUG
   if (TYPEOF(charclass) != CHARSXP)
      throw StriException(MSG__INCORRECT_INTERNAL_ARG);
#endif

   binprop = NA_binprop;
   gcmask  = NA_gcmask;
   complement = false;

   if (charclass == NA_STRING)
      return; // leave (NA_binprop, NA_gcmask) == NA

   R_len_t n = LENGTH(charclass);
   const char* name = CHAR(charclass);

   if (name[0] == '^') { // if n==0, then name[0] == '\0'
      complement = true;
      name++;
      n--;
   }

   if (n == 0) {
      Rf_warning(MSG__CHARCLASS_INCORRECT);
   }
   else if (n <= 2) { // possibly it's a general category
      gcmask = CharClass::getGeneralCategoryMaskFromName(name, n);
   }
   else { // possibly it's a binary property
      binprop = CharClass::getBinaryPropertyFromName(name, n);
   }
}


/**
 * Get unicode general category mask from name
 *
 * @param name character string
 * @param n name's length
 * @return general category mask
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-02)
 * @version 0.1-24 (Marek Gagolewski, 2014-03-11) gcmask is now uint32_t, not UCharCategory
 */
uint32_t CharClass::getGeneralCategoryMaskFromName(const char* name, R_len_t n)
{
   uint32_t retmask = NA_gcmask;

   if (n >= 1 && n <= 2) {

      // compare case-insensitively
      char name1 = (U8_IS_SINGLE(name[0]) && name[0]!=0)?(char)u_toupper((UChar32)name[0]):0;
      char name2 = (U8_IS_SINGLE(name[1]) && name[1]!=0)?(char)u_toupper((UChar32)name[1]):0;

      switch(name1) {
         case 'L':
            switch (name2) {
               case 'U':  retmask = (uint32_t)U_GC_LU_MASK; break;
               case 'L':  retmask = (uint32_t)U_GC_LL_MASK; break;
               case 'T':  retmask = (uint32_t)U_GC_LT_MASK; break;
               case 'M':  retmask = (uint32_t)U_GC_LM_MASK; break;
               case 'O':  retmask = (uint32_t)U_GC_LO_MASK; break;
               case 'C':  retmask = (uint32_t)U_GC_LC_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_L_MASK;  break;
            }
            break;

         case 'Z':
            switch (name2) {
               case 'S':  retmask = (uint32_t)U_GC_ZS_MASK; break;
               case 'L':  retmask = (uint32_t)U_GC_ZL_MASK; break;
               case 'P':  retmask = (uint32_t)U_GC_ZP_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_Z_MASK;  break;
            }
            break;

         case 'N':
            switch (name2) {
               case 'D':  retmask = (uint32_t)U_GC_ND_MASK; break;
               case 'L':  retmask = (uint32_t)U_GC_NL_MASK; break;
               case 'O':  retmask = (uint32_t)U_GC_NO_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_N_MASK;  break;
            }
            break;

         case 'C':
            switch (name2) {
               case 'N':  retmask = (uint32_t)U_GC_CN_MASK; break;
               case 'C':  retmask = (uint32_t)U_GC_CC_MASK; break;
               case 'F':  retmask = (uint32_t)U_GC_CF_MASK; break;
               case 'O':  retmask = (uint32_t)U_GC_CO_MASK; break;
               case 'S':  retmask = (uint32_t)U_GC_CS_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_C_MASK;  break;
            }
            break;

         case 'M':
            switch (name2) {
               case 'N':  retmask = (uint32_t)U_GC_MN_MASK; break;
               case 'E':  retmask = (uint32_t)U_GC_ME_MASK; break;
               case 'C':  retmask = (uint32_t)U_GC_MC_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_M_MASK;  break;
            }
            break;

         case 'P':
            switch (name2) {
               case 'D':  retmask = (uint32_t)U_GC_PD_MASK; break;
               case 'S':  retmask = (uint32_t)U_GC_PS_MASK; break;
               case 'E':  retmask = (uint32_t)U_GC_PE_MASK; break;
               case 'C':  retmask = (uint32_t)U_GC_PC_MASK; break;
               case 'O':  retmask = (uint32_t)U_GC_PO_MASK; break;
               case 'I':  retmask = (uint32_t)U_GC_PI_MASK; break;
               case 'F':  retmask = (uint32_t)U_GC_PF_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_P_MASK;  break;
            }
            break;

         case 'S':
            switch (name2) {
               case 'M':  retmask = (uint32_t)U_GC_SM_MASK; break;
               case 'C':  retmask = (uint32_t)U_GC_SC_MASK; break;
               case 'K':  retmask = (uint32_t)U_GC_SK_MASK; break;
               case 'O':  retmask = (uint32_t)U_GC_SO_MASK; break;
               case '\0': retmask = (uint32_t)U_GC_S_MASK;  break;
            }
            break;
      }
   }

   if (retmask == NA_gcmask)
      Rf_warning(MSG__CHARCLASS_INCORRECT);

   return retmask;
}


/**
 * Get binary property code from name
 *
 * @param name character string
 * @param n name's length
 * @return general category mask
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-02)
 */
UProperty CharClass::getBinaryPropertyFromName(const char* name, R_len_t n)
{
   // convert name to uppercase and get rid of all non-letters
   // similar behavior: ICU's charset selection
   char name2[CharClass::binprop_maxchars+1];
   R_len_t n2 = 0; // true number of characters

   R_len_t j;
   UChar32 chr;
   for (j=0; j<n && n2<CharClass::binprop_maxchars; ) {
      U8_NEXT(name, j, n, chr);
      chr = u_toupper(chr);
      if (u_isupper(chr) && chr <= ASCII_MAXCHARCODE)
         name2[n2++] = (char)chr;
      else if (u_ispunct(chr) || u_isspace(chr))
         ; // ignore
      else
         name2[n2++] = ASCII_SUBSTITUTE;
   }
   name2[n2] = '\0';

   // -------------------------------------------------------------------

   UProperty id = UCHAR_INVALID_CODE;

   if (n2 >= 1) {
      // binprop_names is sorted; apply binary search
      R_len_t i1 = 0;
      R_len_t i2 = CharClass::binprop_length-1;
      while (i1 <= i2) {
         int im = i1+(i2-i1)/2;

#ifndef NDEBUG
         if (i1 < 0 || i2 >= CharClass::binprop_length || im < i1 || im > i2)
            throw StriException("CharClass::getBinaryPropertyFromName FAILED; %d, %d, %d", i1, i2, im);
#endif
         int cmpres = strcmp(name2, CharClass::binprop_names_normalized[im]); // no collation needed

         if (cmpres == 0) {
            id = CharClass::binprop_code[im];
            break; // found!
         }
         else if (cmpres > 0)
            i1 = im + 1;
         else
            i2 = im - 1;
      }
   }

   if (id == UCHAR_INVALID_CODE)
   Rf_warning(MSG__CHARCLASS_INCORRECT_WHICH, name);

   return id;
}



/** Test if a character falls into a given charclass
 *
 * @param c UTF-32 char code
 * @return TRUE (1) or FALSE (0)
 *
 * @version 0.1-?? (Marek Gagolewski, 2013-06-02)
 * @version 0.1-24 (Marek Gagolewski, 2014-03-11) gcmask is now uint32_t, not UCharCategory
 */
int CharClass::test(UChar32 c)
{
   if (binprop != UCHAR_INVALID_CODE) {
      int res = (u_hasBinaryProperty(c, binprop) != 0);
      if (complement) return !(res);
      else            return  (res);
   }
   else if (gcmask != NA_gcmask) {
      int res = ((U_GET_GC_MASK(c) & gcmask) != 0);
      if (complement) return !(res);
      else            return  (res);
   }
   else
      return FALSE;
}

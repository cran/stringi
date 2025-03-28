/* This file is part of the 'stringi' project.
 * Copyright (c) 2013-2025, Marek Gagolewski <https://www.gagolewski.com/>
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


#ifndef __stri_messages_h
#define __stri_messages_h


// CONVENTION: start with a lowercase letter, do not end with a dot
// separate messages with "; " like "hello" "; " "it is me you're looking for"
// or MSG__INCORRECT_NAMED_ARG "; " MSG__EXPECTED_SMALLER

// arg names and symbols should be `backticked`
// string-like objects should be 'quoted'

// ASCII only

// ensure proper capitalisation: ASCII, Unicode, UTF-8, ...


// see stri_exception.cpp for more messages



#define MSG__INCORRECT_INTERNAL_ARG \
   "incorrect argument"

#define MSG__INCORRECT_NAMED_ARG \
   "incorrect argument `%s`"

#define MSG__INTERNAL_ERROR \
   "internal error"

#define MSG__ICU_ERROR \
   "%s (%s)"

#define MSG__ICU_ERROR_WITH_CONTEXT \
   "%s (%s, context=`%s`)"

#define MSG__ICU_WARNING \
   "%s (%s)"


/// incorrect uchar class id, see stri_char_getpropertyid and stri_char_getcategoryid
#define MSG__INCORRECT_UCHAR_CLASS_ID \
   "incorrect character class identifier"

#define MSG__INCORRECT_MATCH_OPTION \
   "incorrect option for `%s`"

#define MSG__INCORRECT_COLLATOR_OPTION \
   "incorrect opts_collator setting: '%s'; ignoring"

#define MSG__INCORRECT_COLLATOR_OPTION_SPEC \
   "incorrect collator option specifier, see ?stri_opts_collator"

#define MSG__INCORRECT_BRKITER_OPTION_SPEC \
   "incorrect break iterator option specifier, see ?stri_opts_brkiter"

#define MSG__INCORRECT_FIXED_OPTION \
   "incorrect opts_fixed setting: '%s'; ignoring"

#define MSG__INCORRECT_REGEX_OPTION \
   "incorrect opts_regex setting: '%s'; ignoring"

#define MSG__INVALID_CODE_POINT \
   "invalid Unicode code point \\U%08x"

#define MSG__INVALID_CODE_POINT_FIXING \
   "invalid UTF-8 code point definition; fixing"

#define MSG__INVALID_CODE_POINT_REPLNA \
   "invalid UTF-8 code point definition; setting string to NA, see also ?stri_enc_toutf8"

#define MSG__INVALID_UTF8 \
   "invalid UTF-8 byte sequence detected; try calling stri_enc_toutf8()"

#define MSG__INVALID_ESCAPE \
   "invalid escape sequence detected; setting NA"

#define MSG__REPLACEMENT_ZERO \
   "replacement has length zero"

#define MSG_REPLACEMENT_MULTIPLE \
   "number of items to replace is not a multiple of replacement length"

#define MSG__UNCONVERTIBLE_CODE_POINT \
   "the Unicode code point \\U%08x cannot be converted to destination encoding"

#define MSG__UNCONVERTIBLE_BINARY_1 \
   "input data \\x%02x in the current source encoding could not be converted to Unicode"

#define MSG__UNCONVERTIBLE_BINARY_2 \
   "input data \\x%02x\\x%02x in the current source encoding could not be converted to Unicode"

#define MSG__UNCONVERTIBLE_BINARY_3 \
   "input data \\x%02x\\x%02x\\x%02x in the current source encoding could not be converted to Unicode"

#define MSG__UNCONVERTIBLE_BINARY_4 \
   "input data \\x%02x\\x%02x\\x%02x\\x%02x in the current source encoding could not be converted to Unicode"

#define MSG__UNCONVERTIBLE_BINARY_n \
   "some input data in the current source encoding could not be converted to Unicode"

#define MSG__WARN_LIST_COERCION \
   "argument is not an atomic vector; coercing"

/// warning when applying recycling rule to not fully recycled args
#define MSG__WARN_RECYCLING_RULE \
   "longer object length is not a multiple of shorter object length"

#define MSG__WARN_RECYCLING_RULE2 \
   "vector length not consistent with other arguments"

#define MSG__WARN_EMPTY_VECTOR \
   "vector has length zero"

#define MSG__EXPECTED_NONNEGATIVE \
   "expected a nonnegative numeric value"

#define MSG__EXPECTED_POSITIVE \
   "expected a positive numeric value"

#define MSG__EXPECTED_SMALLER \
   "value too large"

#define MSG__EXPECTED_LARGER \
   "value too small"

#define MSG__EXPECTED_ASCII \
   "expected a string that consists of ASCII characters only"

#define MSG__EXPECTED_CHAR_IN_SET \
   "expected a character in [%s]"


#define MSG__TIMEZONE_INCORRECT_ID \
   "incorrect time zone identifier"

#define MSG__LOCALE_ERROR_SET \
   "locale could not be set or selected"

#define MSG__ENC_ERROR_GETNAME \
   "character encoding name could not be fetched by the ICU converter"

#define MSG__ENC_ERROR_SET \
   "character encoding could not be set, queried, or selected"

#define MSG__ENC_ERROR_CONVERT \
   "encoding could not be converted"

#define MSG__LOCALE_INCORRECT_ID \
   "incorrect locale identifier"

#define MSG__ENC_INCORRECT_ID \
   "incorrect character encoding identifier"

#define MSG__ENC_INCORRECT_ID_WHAT \
   "incorrect character encoding identifier: '%s'"

#define MSG__ENC_NOT8BIT \
   "encoding %s is not an 8-bit encoding"

#define MSG__BYTESENC \
   "bytes encoding is not supported by this function"

#define MSG__REGEX_FAILED \
   "regex search failed"

#define MSG__REGEX_CONFIG_FAILED \
   "regex engine configuration failed"

#define MSG__REGEX_FAILED_DETAILS \
   "regex search failed: %s"

#define MSG__FIXED_CONFIG_FAILED \
   "fixed search engine configuration failed"

#define MSG__STRSEARCH_FAILED \
   "string search failed"

#define MSG__RESOURCE_ERROR_GET \
   "requested ICU resource is unavailable"

#define MSG__RESOURCE_ERROR_APPLY \
   "error while applying operation"

#define MSG__LOCATE_DIM_START \
   "start"

#define MSG__LOCATE_DIM_END \
   "end"

#define MSG__LOCATE_DIM_LENGTH \
   "length"

#define MSG__NEWLINE_FOUND \
   "newline character found in a string"

#define MSG__NOT_EQ_N_CODEPOINTS \
   "each string in `%s` should consist of exactly %d code points"

#define MSG__NOT_EQ_N_WIDTH \
   "each string in `%s` should consist of code points of total width %d"

#define MSG__CHARCLASS_INCORRECT_WHICH \
   "unknown character class '%s'; assuming NA"

#define MSG__CHARCLASS_INCORRECT \
   "unknown character class"

#define MSG__ARG_EXPECTED_NOT_NA \
   "missing values in argument `%s` is not supported"

#define MSG__ARG_EXPECTED_NOT_EMPTY \
   "argument `%s` should be a non-empty vector"

#define MSG__ARG_EXPECTED_NOT_NULL \
   "argument `%s` should not be a NULL"

#define MSG__ARG_EXPECTED_1_STRING \
   "argument `%s` should be a single character string; only the first element is used"

#define MSG__ARG_EXPECTED_1_LOGICAL \
   "argument `%s` should be a single logical value; only the first element is used"

#define MSG__ARG_EXPECTED_1_INTEGER \
   "argument `%s` should be a single integer value; only the first element is used"

#define MSG__ARG_EXPECTED_1_NUMERIC \
   "argument `%s` should be a single numeric value; only the first element is used"

#define MSG__ARG_EXPECTED_STRING \
   "argument `%s` should be a character vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_LIST \
   "argument `%s` should be a list"

#define MSG__ARG_EXPECTED_LIST_STRING \
   "argument `%s` should be a list of character vectors (or an object coercible to)"

#define MSG__ARG_EXPECTED_LIST_INTEGER \
   "argument `%s` should be a list of integer vectors or an integer vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_VECTOR \
   "argument `%s` should be a vector"

#define MSG__ARG_EXPECTED_RAW \
   "argument `%s` should be a raw vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_LOGICAL \
   "argument `%s` should be a logical vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_INTEGER \
   "argument `%s` should be an integer vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_NUMERIC \
   "argument `%s` should be a numeric vector (or an object coercible to)"

#define MSG__ARG_EXPECTED_POSIXct \
   "argument `%s` should be an object of class POSIXct (or an object coercible to)"

#define MSG__ARG_EXPECTED_STRING_NO_COERCION \
   "argument `%s` should be a character vector"

#define MSG__ARG_EXPECTED_RAW_IN_LIST_NO_COERCION \
   "all elements in `%s` should be a raw vectors"

#define MSG__ARG_EXPECTED_RAW_NO_COERCION \
   "argument `%s` should be a raw vector"

#define MSG__ARG_EXPECTED_LOGICAL_NO_COERCION \
   "argument `%s` should be a logical vector"

#define MSG__ARG_EXPECTED_INTEGER_NO_COERCION \
   "argument `%s` should be an integer vector"

#define MSG__ARG_EXPECTED_NUMERIC_NO_COERCION \
   "argument `%s` should be a numeric vector"

#define MSG__ARG_EXPECTED_MATRIX_WITH_GIVEN_COLUMNS \
   "argument `%s` should be a matrix with %d columns"

#define MSG__ARG_EXPECTED_NOT_MATRIX \
   "argument `%s` is a matrix, which is not supported in this context"

#define MSG__ARG_IGNORING \
   "ignoring argument `%s` in this context"

#define MSG__ARG_EXCLUSIVE \
   "arguments `%s` and `%s` are mutually exclusive in this context"

#define MSG__ARG_NEED_MORE \
   "too few arguments"

#define MSG__ARG_UNUSED \
   "some arguments have not been used"

#define MSG__ARG_UNUSED_1 \
   "one argument has not been used"

#define MSG__ARG_UNUSED_N \
   "%d arguments have not been used"

#define MSG__PROBLEMATIC_FORMAT_SPECIFIER_CHAR \
   "conversion specifier '%%%c' might be non-portable/problematic"

#define MSG__INVALID_FORMAT_SPECIFIER \
   "conversion specifier '%%%s' is not valid"

#define MSG__INVALID_FORMAT_SPECIFIER_SUB \
   "conversion specifier '%%%.*s' is not valid"

#define MSG__EMPTY_SEARCH_PATTERN_UNSUPPORTED \
   "empty search patterns are not supported"

#define MSG__OVERLAPPING_PATTERN_UNSUPPORTED \
   "overlapping pattern matches are not supported"

#define MSG__OVERLAPPING_OR_UNSORTED_INDEXES \
   "index ranges must be sorted and mutually disjoint"

#define MSG__MEM_ALLOC_ERROR \
   "memory allocation or access error"

#define MSG__MEM_ALLOC_ERROR_WITH_SIZE \
   "memory allocation error: failed to allocate %zu bytes"

#define MSG__BUF_SIZE_EXCEEDED \
   "internal error: required buffer size is beyond the permitted limit"

#define MSG__U_CHARSET_IS_UTF8 \
   "system ICU assumes that the default character set is always UTF-8, and hence this function has no effect"

#define MSG__CHARSXP_2147483647 \
    "Elements of character vectors (CHARSXPs) are limited to 2^31-1 bytes"

#endif

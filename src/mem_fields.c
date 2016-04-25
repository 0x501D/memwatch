/* ANSI-C code produced by gperf version 3.0.4 */
/* Command-line: gperf -m 10 -D -t -L ANSI-C -N get_mem_field mem_fields.gperf  */
/* Computed positions: -k'4,11' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "mem_fields.gperf"

#include <stdint.h>
#include <string.h>
/* Command-line options:
   -m 10 -D -t -L ANSI-C -N get_mem_field */
#line 7 "mem_fields.gperf"
struct mem_field {
    char *name;
    uint64_t value;
};

#define TOTAL_KEYWORDS 41
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 48
/* maximum key range = 43, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 19, 49, 49, 49, 49,
      20, 49, 49, 49, 49, 49, 49, 20, 49, 49,
      49, 49, 16, 15, 12, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 15, 49, 13,  2,  5,
      13, 14,  3, 49,  3,  9, 49, 12, 15,  2,
       2, 49,  8, 49, 28, 49, 17, 49, 11, 49,
      49, 49,  8, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
      49, 49, 49, 49, 49, 49
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct mem_field *
get_mem_field (register const char *str, register unsigned int len)
{
  static struct mem_field wordlist[] =
    {
#line 33 "mem_fields.gperf"
      {"Slab", 0},
#line 39 "mem_fields.gperf"
      {"Bounce", 0},
#line 16 "mem_fields.gperf"
      {"Cached", 0},
#line 15 "mem_fields.gperf"
      {"Buffers", 0},
#line 30 "mem_fields.gperf"
      {"AnonPages", 0},
#line 25 "mem_fields.gperf"
      {"Mlocked", 0},
#line 19 "mem_fields.gperf"
      {"Inactive", 0},
#line 31 "mem_fields.gperf"
      {"Mapped", 0},
#line 18 "mem_fields.gperf"
      {"Active", 0},
#line 27 "mem_fields.gperf"
      {"SwapFree", 0},
#line 26 "mem_fields.gperf"
      {"SwapTotal", 0},
#line 17 "mem_fields.gperf"
      {"SwapCached", 0},
#line 32 "mem_fields.gperf"
      {"Shmem", 0},
#line 12 "mem_fields.gperf"
      {"MemTotal", 0},
#line 21 "mem_fields.gperf"
      {"Inactive(anon)", 0},
#line 28 "mem_fields.gperf"
      {"Dirty", 0},
#line 20 "mem_fields.gperf"
      {"Active(anon)", 0},
#line 37 "mem_fields.gperf"
      {"PageTables", 0},
#line 36 "mem_fields.gperf"
      {"KernelStack", 0},
#line 29 "mem_fields.gperf"
      {"Writeback", 0},
#line 13 "mem_fields.gperf"
      {"MemFree", 0},
#line 23 "mem_fields.gperf"
      {"Inactive(file)", 0},
#line 45 "mem_fields.gperf"
      {"VmallocChunk", 0},
#line 41 "mem_fields.gperf"
      {"CommitLimit", 0},
#line 40 "mem_fields.gperf"
      {"WritebackTmp", 0},
#line 34 "mem_fields.gperf"
      {"SReclaimable", 0},
#line 42 "mem_fields.gperf"
      {"Committed_AS", 0},
#line 50 "mem_fields.gperf"
      {"Hugepagesize", 0},
#line 22 "mem_fields.gperf"
      {"Active(file)", 0},
#line 24 "mem_fields.gperf"
      {"Unevictable", 0},
#line 51 "mem_fields.gperf"
      {"DirectMap4k", 0},
#line 35 "mem_fields.gperf"
      {"SUnreclaim", 0},
#line 44 "mem_fields.gperf"
      {"VmallocUsed", 0},
#line 43 "mem_fields.gperf"
      {"VmallocTotal", 0},
#line 46 "mem_fields.gperf"
      {"HugePages_Total", 0},
#line 38 "mem_fields.gperf"
      {"NFS_Unstable", 0},
#line 49 "mem_fields.gperf"
      {"HugePages_Surp", 0},
#line 48 "mem_fields.gperf"
      {"HugePages_Rsvd", 0},
#line 52 "mem_fields.gperf"
      {"DirectMap2M", 0},
#line 14 "mem_fields.gperf"
      {"MemAvailable", 0},
#line 47 "mem_fields.gperf"
      {"HugePages_Free", 0}
    };

  static signed char lookup[] =
    {
      -1, -1, -1, -1, -1, -1,  0, -1,  1,  2,  3,  4,  5,  6,
       7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
      21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
      35, 36, 37, 38, 39, -1, 40
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}

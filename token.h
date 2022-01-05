// -----------------------------------------------------------------------
// ---- hash64.h -- FNV-1 by Noll (5,1 2009) -----------------------------
// ---- (public domain, no copyright claimed) ----------------------------
#if !defined(__FNV_H__)
#define __FNV_H__

//#include <sys/types.h>

#define FNV_VERSION "5.0.2"	/* @(#) FNV Version */


/*
 * 32 bit FNV-0 hash type
 */
typedef u_int32_t Fnv32_t;


/*
 * 32 bit FNV-0 zero initial basis
 *
 * This historic hash is not recommended.  One should use
 * the FNV-1 hash and initial basis instead.
 */
#define FNV0_32_INIT ((Fnv32_t)0)


/*
 * 32 bit FNV-1 and FNV-1a non-zero initial basis
 *
 * The FNV-1 initial basis is the FNV-0 hash of the following 32 octets:
 *
 *              chongo <Landon Curt Noll> /\../\
 *
 * NOTE: The \'s above are not back-slashing escape characters.
 * They are literal ASCII  backslash 0x5c characters.
 *
 * NOTE: The FNV-1a initial basis is the same value as FNV-1 by definition.
 */
#define FNV1_32_INIT ((Fnv32_t)0x811c9dc5)
#define FNV1_32A_INIT FNV1_32_INIT


/*
 * determine how 64 bit unsigned values are represented
 */
//#include "longlong.h"
#define HAVE_64BIT_LONG_LONG


/*
 * 64 bit FNV-0 hash
 */
#if defined(HAVE_64BIT_LONG_LONG)
typedef u_int64_t Fnv64_t;
#else /* HAVE_64BIT_LONG_LONG */
typedef struct {
    u_int32_t w32[2]; /* w32[0] is low order, w32[1] is high order word */
} Fnv64_t;
#endif /* HAVE_64BIT_LONG_LONG */


/*
 * 64 bit FNV-0 zero initial basis
 *
 * This historic hash is not recommended.  One should use
 * the FNV-1 hash and initial basis instead.
 */
#if defined(HAVE_64BIT_LONG_LONG)
#define FNV0_64_INIT ((Fnv64_t)0)
#else /* HAVE_64BIT_LONG_LONG */
extern const Fnv64_t fnv0_64_init;
#define FNV0_64_INIT (fnv0_64_init)
#endif /* HAVE_64BIT_LONG_LONG */


/*
 * 64 bit FNV-1 non-zero initial basis
 *
 * The FNV-1 initial basis is the FNV-0 hash of the following 32 octets:
 *
 *              chongo <Landon Curt Noll> /\../\
 *
 * NOTE: The \'s above are not back-slashing escape characters.
 * They are literal ASCII  backslash 0x5c characters.
 *
 * NOTE: The FNV-1a initial basis is the same value as FNV-1 by definition.
 */
#if defined(HAVE_64BIT_LONG_LONG)
#define FNV1_64_INIT ((Fnv64_t)0xcbf29ce484222325ULL)
#define FNV1A_64_INIT FNV1_64_INIT
#else /* HAVE_64BIT_LONG_LONG */
extern const fnv1_64_init;
extern const Fnv64_t fnv1a_64_init;
#define FNV1_64_INIT (fnv1_64_init)
#define FNV1A_64_INIT (fnv1a_64_init)
#endif /* HAVE_64BIT_LONG_LONG */


/*
 * hash types
 */
enum fnv_type {
    FNV_NONE = 0,	/* invalid FNV hash type */
    FNV0_32 = 1,	/* FNV-0 32 bit hash */
    FNV1_32 = 2,	/* FNV-1 32 bit hash */
    FNV1a_32 = 3,	/* FNV-1a 32 bit hash */
    FNV0_64 = 4,	/* FNV-0 64 bit hash */
    FNV1_64 = 5,	/* FNV-1 64 bit hash */
    FNV1a_64 = 6,	/* FNV-1a 64 bit hash */
};


/*
 * these test vectors are used as part o the FNV test suite
 */
struct test_vector {
    void *buf;		/* start of test vector buffer */
    int len;		/* length of test vector */
};
struct fnv0_32_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv32_t fnv0_32;		/* expected FNV-0 32 bit hash value */
};
struct fnv1_32_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv32_t fnv1_32;		/* expected FNV-1 32 bit hash value */
};
struct fnv1a_32_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv32_t fnv1a_32;		/* expected FNV-1a 32 bit hash value */
};
struct fnv0_64_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv64_t fnv0_64;		/* expected FNV-0 64 bit hash value */
};
struct fnv1_64_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv64_t fnv1_64;		/* expected FNV-1 64 bit hash value */
};
struct fnv1a_64_test_vector {
    struct test_vector *test;	/* test vector buffer to hash */
    Fnv64_t fnv1a_64;		/* expected FNV-1a 64 bit hash value */
};


/*
 * external functions
 */
/* hash_32.c */
extern Fnv32_t fnv_32_buf(void *buf, size_t len, Fnv32_t hashval);
extern Fnv32_t fnv_32_str(char *buf, Fnv32_t hashval);

/* hash_32a.c */
extern Fnv32_t fnv_32a_buf(void *buf, size_t len, Fnv32_t hashval);
extern Fnv32_t fnv_32a_str(char *buf, Fnv32_t hashval);

/* hash_64.c */
extern Fnv64_t fnv_64_buf(void *buf, size_t len, Fnv64_t hashval);
extern Fnv64_t fnv_64_str(char *buf, Fnv64_t hashval);

/* hash_64a.c */
extern Fnv64_t fnv_64a_buf(void *buf, size_t len, Fnv64_t hashval);
extern Fnv64_t fnv_64a_str(char *buf, Fnv64_t hashval);

/* test_fnv.c */
extern struct test_vector fnv_test_str[];
extern struct fnv0_32_test_vector fnv0_32_vector[];
extern struct fnv1_32_test_vector fnv1_32_vector[];
extern struct fnv1a_32_test_vector fnv1a_32_vector[];
extern struct fnv0_64_test_vector fnv0_64_vector[];
extern struct fnv1_64_test_vector fnv1_64_vector[];
extern struct fnv1a_64_test_vector fnv1a_64_vector[];
extern void unknown_hash_type(char *prog, enum fnv_type type, int code);
extern void print_fnv32(Fnv32_t hval, Fnv32_t mask, int verbose, char *arg);
extern void print_fnv64(Fnv64_t hval, Fnv64_t mask, int verbose, char *arg);


#endif /* __FNV_H__ */



// -----------------------------------------------------------------------
// ---- hash64.c -- FNV-1 by Noll (5,1 2009) -----------------------------

/*
 * FNV-0 defines the initial basis to be zero
 */
#if !defined(HAVE_64BIT_LONG_LONG)
const Fnv64_t fnv0_64_init = { 0UL, 0UL };
#endif /* ! HAVE_64BIT_LONG_LONG */


/*
 * FNV-1 defines the initial basis to be non-zero
 */
#if !defined(HAVE_64BIT_LONG_LONG)
const Fnv64_t fnv1_64_init = { 0x84222325UL, 0xcbf29ce4UL };
#endif /* ! HAVE_64BIT_LONG_LONG */


/*
 * 64 bit magic FNV-0 and FNV-1 prime
 */
#if defined(HAVE_64BIT_LONG_LONG)
#define FNV_64_PRIME ((Fnv64_t)0x100000001b3ULL)
#else /* HAVE_64BIT_LONG_LONG */
#define FNV_64_PRIME_LOW ((unsigned long)0x1b3)	/* lower bits of FNV prime */
#define FNV_64_PRIME_SHIFT (8)		/* top FNV prime shift above 2^32 */
#endif /* HAVE_64BIT_LONG_LONG */


/*
 * fnv_64_buf - perform a 64 bit Fowler/Noll/Vo hash on a buffer
 *
 * input:
 *	buf	- start of buffer to hash
 *	len	- length of buffer in octets
 *	hval	- previous hash value or 0 if first call
 *
 * returns:
 *	64 bit hash as a static hash type
 *
 * NOTE: To use the 64 bit FNV-0 historic hash, use FNV0_64_INIT as the hval
 *	 argument on the first call to either fnv_64_buf() or fnv_64_str().
 *
 * NOTE: To use the recommended 64 bit FNV-1 hash, use FNV1_64_INIT as the hval
 *	 argument on the first call to either fnv_64_buf() or fnv_64_str().
 */
Fnv64_t
fnv_64_buf(void *buf, size_t len, Fnv64_t hval)
{
    unsigned char *bp = (unsigned char *)buf;	/* start of buffer */
    unsigned char *be = bp + len;		/* beyond end of buffer */

#if defined(HAVE_64BIT_LONG_LONG)

    /*
     * FNV-1 hash each octet of the buffer
     */
    while (bp < be) {

	/* multiply by the 64 bit FNV magic prime mod 2^64 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
	hval *= FNV_64_PRIME;
#else /* NO_FNV_GCC_OPTIMIZATION */
	hval += (hval << 1) + (hval << 4) + (hval << 5) +
		(hval << 7) + (hval << 8) + (hval << 40);
#endif /* NO_FNV_GCC_OPTIMIZATION */

	/* xor the bottom with the current octet */
	hval ^= (Fnv64_t)*bp++;
    }

#else /* HAVE_64BIT_LONG_LONG */

    unsigned long val[4];			/* hash value in base 2^16 */
    unsigned long tmp[4];			/* tmp 64 bit value */

    /*
     * Convert Fnv64_t hval into a base 2^16 array
     */
    val[0] = hval.w32[0];
    val[1] = (val[0] >> 16);
    val[0] &= 0xffff;
    val[2] = hval.w32[1];
    val[3] = (val[2] >> 16);
    val[2] &= 0xffff;

    /*
     * FNV-1 hash each octet of the buffer
     */
    while (bp < be) {

	/*
	 * multiply by the 64 bit FNV magic prime mod 2^64
	 *
	 * Using 0x100000001b3 we have the following digits base 2^16:
	 *
	 *	0x0	0x100	0x0	0x1b3
	 *
	 * which is the same as:
	 *
	 *	0x0	1<<FNV_64_PRIME_SHIFT	0x0	FNV_64_PRIME_LOW
	 */
	/* multiply by the lowest order digit base 2^16 */
	tmp[0] = val[0] * FNV_64_PRIME_LOW;
	tmp[1] = val[1] * FNV_64_PRIME_LOW;
	tmp[2] = val[2] * FNV_64_PRIME_LOW;
	tmp[3] = val[3] * FNV_64_PRIME_LOW;
	/* multiply by the other non-zero digit */
	tmp[2] += val[0] << FNV_64_PRIME_SHIFT;	/* tmp[2] += val[0] * 0x100 */
	tmp[3] += val[1] << FNV_64_PRIME_SHIFT;	/* tmp[3] += val[1] * 0x100 */
	/* propagate carries */
	tmp[1] += (tmp[0] >> 16);
	val[0] = tmp[0] & 0xffff;
	tmp[2] += (tmp[1] >> 16);
	val[1] = tmp[1] & 0xffff;
	val[3] = tmp[3] + (tmp[2] >> 16);
	val[2] = tmp[2] & 0xffff;
	/*
	 * Doing a val[3] &= 0xffff; is not really needed since it simply
	 * removes multiples of 2^64.  We can discard these excess bits
	 * outside of the loop when we convert to Fnv64_t.
	 */

	/* xor the bottom with the current octet */
	val[0] ^= (unsigned long)*bp++;
    }

    /*
     * Convert base 2^16 array back into an Fnv64_t
     */
    hval.w32[1] = ((val[3]<<16) | val[2]);
    hval.w32[0] = ((val[1]<<16) | val[0]);

#endif /* HAVE_64BIT_LONG_LONG */

    /* return our new hash value */
    return hval;
}


/*
 * fnv_64_str - perform a 64 bit Fowler/Noll/Vo hash on a buffer
 *
 * input:
 *	buf	- start of buffer to hash
 *	hval	- previous hash value or 0 if first call
 *
 * returns:
 *	64 bit hash as a static hash type
 *
 * NOTE: To use the 64 bit FNV-0 historic hash, use FNV0_64_INIT as the hval
 *	 argument on the first call to either fnv_64_buf() or fnv_64_str().
 *
 * NOTE: To use the recommended 64 bit FNV-1 hash, use FNV1_64_INIT as the hval
 *	 argument on the first call to either fnv_64_buf() or fnv_64_str().
 */
Fnv64_t
fnv_64_str(char *str, Fnv64_t hval)
{
    unsigned char *s = (unsigned char *)str;	/* unsigned string */

#if defined(HAVE_64BIT_LONG_LONG)

    /*
     * FNV-1 hash each octet of the string
     */
    while (*s) {

	/* multiply by the 64 bit FNV magic prime mod 2^64 */
#if defined(NO_FNV_GCC_OPTIMIZATION)
	hval *= FNV_64_PRIME;
#else /* NO_FNV_GCC_OPTIMIZATION */
	hval += (hval << 1) + (hval << 4) + (hval << 5) +
		(hval << 7) + (hval << 8) + (hval << 40);
#endif /* NO_FNV_GCC_OPTIMIZATION */

	/* xor the bottom with the current octet */
	hval ^= (Fnv64_t)*s++;
    }

#else /* !HAVE_64BIT_LONG_LONG */

    unsigned long val[4];	/* hash value in base 2^16 */
    unsigned long tmp[4];	/* tmp 64 bit value */

    /*
     * Convert Fnv64_t hval into a base 2^16 array
     */
    val[0] = hval.w32[0];
    val[1] = (val[0] >> 16);
    val[0] &= 0xffff;
    val[2] = hval.w32[1];
    val[3] = (val[2] >> 16);
    val[2] &= 0xffff;

    /*
     * FNV-1 hash each octet of the string
     */
    while (*s) {

	/*
	 * multiply by the 64 bit FNV magic prime mod 2^64
	 *
	 * Using 1099511628211, we have the following digits base 2^16:
	 *
	 *	0x0	0x100	0x0	0x1b3
	 *
	 * which is the same as:
	 *
	 *	0x0	1<<FNV_64_PRIME_SHIFT	0x0	FNV_64_PRIME_LOW
	 */
	/* multiply by the lowest order digit base 2^16 */
	tmp[0] = val[0] * FNV_64_PRIME_LOW;
	tmp[1] = val[1] * FNV_64_PRIME_LOW;
	tmp[2] = val[2] * FNV_64_PRIME_LOW;
	tmp[3] = val[3] * FNV_64_PRIME_LOW;
	/* multiply by the other non-zero digit */
	tmp[2] += val[0] << FNV_64_PRIME_SHIFT;	/* tmp[2] += val[0] * 0x100 */
	tmp[3] += val[1] << FNV_64_PRIME_SHIFT;	/* tmp[3] += val[1] * 0x100 */
	/* propagate carries */
	tmp[1] += (tmp[0] >> 16);
	val[0] = tmp[0] & 0xffff;
	tmp[2] += (tmp[1] >> 16);
	val[1] = tmp[1] & 0xffff;
	val[3] = tmp[3] + (tmp[2] >> 16);
	val[2] = tmp[2] & 0xffff;
	/*
	 * Doing a val[3] &= 0xffff; is not really needed since it simply
	 * removes multiples of 2^64.  We can discard these excess bits
	 * outside of the loop when we convert to Fnv64_t.
	 */

	/* xor the bottom with the current octet */
	val[0] ^= (unsigned long)(*s++);
    }

    /*
     * Convert base 2^16 array back into an Fnv64_t
     */
    hval.w32[1] = ((val[3]<<16) | val[2]);
    hval.w32[0] = ((val[1]<<16) | val[0]);

#endif /* !HAVE_64BIT_LONG_LONG */

    /* return our new hash value */
    return hval;
}


// -----------------------------------------------------------------------
// ---- token.h -- uses hash64.c and hash64.h above ----------------------
// note that timestr requires time.h and sys/time.h

#define DNSHASHINITSIZE		((u_int64_t)14695981039346656037ULL)
#define DNSHASHTRUESIZE		((u_int64_t)218340105584896ULL)
#define DNSLABELSIZE		8
#define DNSVALUESIZE		64

// label hashes are 8-byte, 62 bit-combinations per byte
// labelout should be exactly 9 characters wide, with [8] = '\0'
CH dnstoken(CH *valuein, CH *tokenout) {
  CH *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  IN valuelen = 0; WI (valuein[valuelen] NQ '\0') { valuelen++; }
  u_int64_t hashtotal = fnv_64_buf(valuein, valuelen, DNSHASHINITSIZE);
  hashtotal %= DNSHASHTRUESIZE;
  CH hashlabel[DNSLABELSIZE WITHNUL];
  hashlabel[DNSLABELSIZE] = '\0';
  hashlabel[0] = hashlabel[1] = hashlabel[2] = hashlabel[3] = '@';
  hashlabel[4] = hashlabel[5] = hashlabel[6] = hashlabel[7] = '@';
  IN hashindex = DNSLABELSIZE, hashdiff = 0;
  WI (--hashindex >= 0) {
    hashdiff = (int)(hashtotal % 62);
    hashlabel[hashindex] = charset[hashdiff];
    tokenout[hashindex] = charset[hashdiff];
    hashtotal /= 62;
  }
  tokenout[DNSLABELSIZE] = '\0';
  RT (hashtotal == 0) ? 'l' : '-';
}

CH dnsrotate(CH *tokenin, CH *host, SCH dir, CH *tokenout) {
  CH *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  IN charsetlen = 62;
  IN nibcount = 0;
  IN tix = 0; // token index
  IN hix = -1; // host index
  WI (INC hix GQ 0) {
    IF (host[hix] EQ '.' OR host[hix] EQNUL) {
      IN csi = 0; // charset index
      CH setchar = charset[csi];
      WI (setchar NQNUL AND setchar NQ tokenin[tix])
        { setchar = charset[INC csi]; }
      IF (dir EQ -1) {
        csi = (csi + charsetlen - (nibcount % charsetlen)) % charsetlen;
      } EL { csi = (csi + (nibcount % charsetlen)) % charsetlen; }
      tokenout[tix] = charset[csi]; 
      tokenout[tix + 1] = '\0';
      nibcount = 0;
      IF (INC tix GT DNSLABELSIZE) { RT '!'; }
    } EF INRANGE(host[hix], A0, A9) {
      nibcount = (nibcount * 10) + (host[hix] - A0);
    } EL { tokenout[0] = '!'; tokenout[1] = NUL; RT '!'; }
    IF (host[hix] EQNUL) { BK; }
  }
  RT Y;
}

VD timestr(CH *timebuf, IN buflen) {
  struct timeval tmnow;
  struct tm *tmloc;
  gettimeofday(&tmnow, NULL);
  tmloc = localtime(&tmnow.tv_sec);
  strftime(timebuf, buflen, "%Y.%m.%d.%H.%M.%S", tmloc);
  IN tlen = -1; WI (timebuf[INC tlen] NQNUL);
  sprintf(&timebuf[tlen], ".%06d", (int)tmnow.tv_usec);
  timebuf[tlen + 7] = timebuf[tlen + 6];
  timebuf[tlen + 6] = timebuf[tlen + 5];
  timebuf[tlen + 5] = timebuf[tlen + 4];
  timebuf[tlen + 4] = '.';
  timebuf[tlen + 8] = '\0';
}
// ^ map function in/out history (in, out <- timebuf, in/out)

/*
 * synths.c
 *
 * SCCS ID:  @(#)synths.c 1.2 96/05/19
 */

#include "f2c.h"

extern int synths_(integer *voice, integer *pitch, real *rms, real *rc, real *speech, integer *k);
extern int initsynths_(void);
/* comlen contrl_ 12 */
/*:ref: pitsyn_ 14 12 4 4 4 6 6 4 4 4 6 6 4 6 */
/*:ref: irc2pc_ 14 5 6 6 4 6 6 */
/*:ref: bsynz_ 14 7 6 4 4 6 6 6 6 */
/*:ref: deemp_ 14 2 6 4 */
/*:ref: initpitsyn_ 14 0 */
/*:ref: initbsynz_ 14 0 */
/*:ref: initdeemp_ 14 0 */

/*  -- translated by f2c (version 19951025).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/


/* Common Block Declarations */

extern struct {
    integer order, lframe;
    logical corrp;
} contrl_;

#define contrl_1 contrl_

/* Table of constant values */

static real c_b2 = .7f;

/* ***************************************************************** */

/* 	SYNTHS Version 54 */

/* $Log: synths.c,v $
/* Revision 1.2  2001/01/25 23:45:49  jpoehlmann
/* Version 1.7c. Identical with files on the ftp Server ftp.franken.de.
/* (+ 1 patch in cli.c, wich is on the server too)
/* Not compiled now
/* */
/* Revision 1.5  1996/08/01  19:31:58  dm */
/* Normalized array indexing in initialization code */

/* Revision 1.5  1996/03/26  19:31:58  jaf */
/* Commented out trace statements. */

/* Revision 1.4  1996/03/25  19:41:01  jaf */
/* Changed so that MAXFRM samples are always returned in the output array */
/* SPEECH. */

/* This required delaying the returned samples by MAXFRM sample times, */
/* and remembering any "left over" samples returned by PITSYN from one */
/* call of SYNTHS to the next. */

/* Changed size of SPEECH from 2*MAXFRM to MAXFRM.  Removed local */
/* variable SOUT.  Added local state variables BUF and BUFLEN. */

/* Revision 1.3  1996/03/25  19:20:10  jaf */
/* Added comments about the range of possible return values for argument */
/* K, and increased the size of the arrays filled in by PITSYN from 11 to */
/* 16, as has been already done inside of PITSYN. */

/* Revision 1.2  1996/03/22  00:18:18  jaf */
/* Added comments explaining meanings of input and output parameters, and */
/* indicating which array indices can be read or written. */

/* Added entry INITSYNTHS, which does nothing except call the */
/* corresponding initialization entries for subroutines PITSYN, BSYNZ, */
/* and DEEMP. */

/* Revision 1.1  1996/02/07 14:49:44  jaf */
/* Initial revision */


/* ***************************************************************** */

/* The note below is from the distributed version of the LPC10 coder. */
/* The version of the code below has been modified so that SYNTHS always */
/* has a constant frame length output of MAXFRM. */

/* Also, BSYNZ and DEEMP need not be modified to work on variable */
/* positions within an array.  It is only necessary to pass the first */
/* index desired as the array argument.  What actually gets passed is the */
/* address of that array position, which the subroutine treats as the */
/* first index of the array. */

/* This technique is used in subroutine ANALYS when calling PREEMP, so it */
/* appears that multiple people wrote different parts of this LPC10 code, */
/* and that they didn't necessarily have equivalent knowledge of Fortran */
/* (not surprising). */

/*  NOTE: There is excessive buffering here, BSYNZ and DEEMP should be */
/*        changed to operate on variable positions within SOUT.  Also, */
/*        the output length parameter is bogus, and PITSYN should be */
/*        rewritten to allow a constant frame length output. */

/* Input: */
/*  VOICE  - Half frame voicing decisions */
/*           Indices 1 through 2 read. */
/* Input/Output: */
/*  PITCH  - Pitch */
/*           PITCH is restricted to range 20 to 156, inclusive, */
/*           before calling subroutine PITSYN, and then PITSYN */
/*           can modify it further under some conditions. */
/*  RMS    - Energy */
/*           Only use is for debugging, and passed to PITSYN. */
/*           See comments there for how it can be modified. */
/*  RC     - Reflection coefficients */
/*           Indices 1 through ORDER restricted to range -.99 to .99, */
/*           before calling subroutine PITSYN, and then PITSYN */
/*           can modify it further under some conditions. */
/* Output: */
/*  SPEECH - Synthesized speech samples. */
/*           Indices 1 through the final value of K are written. */
/*  K      - Number of samples placed into array SPEECH. */
/*           This is always MAXFRM. */

/*< 	SUBROUTINE SYNTHS(VOICE, PITCH, RMS, RC, SPEECH, K) >*/
/* Subroutine */ int synths_0_(int n__, integer *voice, integer *pitch, real *
	rms, real *rc, real *speech, integer *k)
{
    /* Initialized data */

    static real buf[360] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,
	    0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };
    static integer buflen = 180;

    /* System generated locals */
    integer i__1;
    real r__1, r__2;

    /* Local variables */
    real rmsi[16];
    integer nout, ivuv[16], i__, j;
    extern /* Subroutine */ int deemp_(real *, integer *);
    real ratio;
    integer ipiti[16];
    extern /* Subroutine */ int initdeemp_(void), bsynz_(real *, integer *, 
	    integer *, real *, real *, real *, real *), irc2pc_(real *, real *
	    , integer *, real *, real *);
    real g2pass;
    extern /* Subroutine */ int initbsynz_(void);
    real pc[10];
    extern /* Subroutine */ int pitsyn_(integer *, integer *, integer *, real 
	    *, real *, integer *, integer *, integer *, real *, real *, 
	    integer *, real *), initpitsyn_(void);
    real rci[160]	/* was [10][16] */;

/*< 	INCLUDE 'config.fh' >*/
/*< 	INCLUDE 'contrl.fh' >*/
/* $Log: synths.c,v $
/* Revision 1.2  2001/01/25 23:45:49  jpoehlmann
/* Version 1.7c. Identical with files on the ftp Server ftp.franken.de.
/* (+ 1 patch in cli.c, wich is on the server too)
/* Not compiled now
/* */
/* Revision 1.3  1996/03/29  22:03:47  jaf */
/* Removed definitions for any constants that were no longer used. */

/* Revision 1.2  1996/03/26  19:34:33  jaf */
/* Added comments indicating which constants are not needed in an */
/* application that uses the LPC-10 coder. */

/* Revision 1.1  1996/02/07  14:43:51  jaf */
/* Initial revision */

/*   LPC Configuration parameters: */
/* Frame size, Prediction order, Pitch period */
/*< 	parameter (MAXFRM = 180, MAXORD = 10, MAXPIT = 156) >*/
/*< 	INTEGER VOICE(2), PITCH >*/
/*       Arguments */
/* $Log: synths.c,v $
/* Revision 1.2  2001/01/25 23:45:49  jpoehlmann
/* Version 1.7c. Identical with files on the ftp Server ftp.franken.de.
/* (+ 1 patch in cli.c, wich is on the server too)
/* Not compiled now
/* */
/* Revision 1.3  1996/03/29  22:05:55  jaf */
/* Commented out the common block variables that are not needed by the */
/* embedded version. */

/* Revision 1.2  1996/03/26  19:34:50  jaf */
/* Added comments indicating which constants are not needed in an */
/* application that uses the LPC-10 coder. */

/* Revision 1.1  1996/02/07  14:44:09  jaf */
/* Initial revision */

/*   LPC Processing control variables: */

/* *** Read-only: initialized in setup */

/*  Files for Speech, Parameter, and Bitstream Input & Output, */
/*    and message and debug outputs. */

/* Here are the only files which use these variables: */

/* lpcsim.f setup.f trans.f error.f vqsetup.f */

/* Many files which use fdebug are not listed, since it is only used in */
/* those other files conditionally, to print trace statements. */
/* 	integer fsi, fso, fpi, fpo, fbi, fbo, pbin, fmsg, fdebug */
/*  LPC order, Frame size, Quantization rate, Bits per frame, */
/*    Error correction */
/* Subroutine SETUP is the only place where order is assigned a value, */
/* and that value is 10.  It could increase efficiency 1% or so to */
/* declare order as a constant (i.e., a Fortran PARAMETER) instead of as 
*/
/* a variable in a COMMON block, since it is used in many places in the */
/* core of the coding and decoding routines.  Actually, I take that back. 
*/
/* At least when compiling with f2c, the upper bound of DO loops is */
/* stored in a local variable before the DO loop begins, and then that is 
*/
/* compared against on each iteration. */
/* Similarly for lframe, which is given a value of MAXFRM in SETUP. */
/* Similarly for quant, which is given a value of 2400 in SETUP.  quant */
/* is used in only a few places, and never in the core coding and */
/* decoding routines, so it could be eliminated entirely. */
/* nbits is similar to quant, and is given a value of 54 in SETUP. */
/* corrp is given a value of .TRUE. in SETUP, and is only used in the */
/* subroutines ENCODE and DECODE.  It doesn't affect the speed of the */
/* coder significantly whether it is .TRUE. or .FALSE., or whether it is 
*/
/* a constant or a variable, since it is only examined once per frame. */
/* Leaving it as a variable that is set to .TRUE.  seems like a good */
/* idea, since it does enable some error-correction capability for */
/* unvoiced frames, with no change in the coding rate, and no noticeable 
*/
/* quality difference in the decoded speech. */
/* 	integer quant, nbits */
/*< 	logical corrp >*/
/* *** Read/write: variables for debugging, not needed for LPC algorithm 
*/

/*  Current frame, Unstable frames, Output clip count, Max onset buffer, 
*/
/*    Debug listing detail level, Line count on listing page */

/* nframe is not needed for an embedded LPC10 at all. */
/* nunsfm is initialized to 0 in SETUP, and incremented in subroutine */
/* ERROR, which is only called from RCCHK.  When LPC10 is embedded into */
/* an application, I would recommend removing the call to ERROR in RCCHK, 
*/
/* and remove ERROR and nunsfm completely. */
/* iclip is initialized to 0 in SETUP, and incremented in entry SWRITE in 
*/
/* sread.f.  When LPC10 is embedded into an application, one might want */
/* to cause it to be incremented in a routine that takes the output of */
/* SYNTHS and sends it to an audio device.  It could be optionally */
/* displayed, for those that might want to know what it is. */
/* maxosp is never initialized to 0 in SETUP, although it probably should 
*/
/* be, and it is updated in subroutine ANALYS.  I doubt that its value */
/* would be of much interest to an application in which LPC10 is */
/* embedded. */
/* listl and lincnt are not needed for an embedded LPC10 at all. */
/* 	integer nframe, nunsfm, iclip, maxosp, listl, lincnt */
/* 	common /contrl/ fsi, fso, fpi, fpo, fbi, fbo, pbin, fmsg, fdebug */
/*< 	common /contrl/ order, lframe >*/
/* 	common /contrl/ quant, nbits */
/*< 	common /contrl/ corrp >*/
/* 	common /contrl/ nframe, nunsfm, iclip, maxosp, listl, lincnt */
/*< 	REAL RMS, RC(ORDER), SPEECH(MAXFRM) >*/
/*< 	INTEGER K >*/
/*       Parameters/constants */
/*< 	REAL GPRIME >*/
/*< 	PARAMETER (GPRIME = .7) >*/
/*< 	INTEGER TWOFRM >*/
/*< 	PARAMETER (TWOFRM = 2*MAXFRM) >*/
/*       Local variables that need not be saved */
/*< 	INTEGER I, J, NOUT >*/
/*< 	REAL RATIO, G2PASS >*/
/*< 	REAL PC(MAXORD) >*/
/*< 	INTEGER IPITI(16), IVUV(16) >*/
/*< 	REAL RCI(MAXORD,16), RMSI(16) >*/
/*       Local state */
/*       BUF is a buffer of speech samples that would have been returned 
*/
/*       by the older version of SYNTHS, but the newer version doesn't, */
/*       so that the newer version can always return MAXFRM samples on */
/*       every call.  This has the effect of delaying the return of */
/*       samples for one additional frame time. */

/*       Indices 1 through BUFLEN contain samples that are left over from 
*/
/*       the last call to SYNTHS.  Given the way that PITSYN works, */
/*       BUFLEN should always be in the range MAXFRM-MAXPIT+1 through */
/*       MAXFRM, inclusive, after a call to SYNTHS is complete. */

/*       On the first call to SYNTHS (or the first call after */
/*       reinitializing with the entry INITSYNTHS), BUFLEN is MAXFRM, and 
*/
/*       a frame of silence is always returned. */
/*< 	REAL BUF(TWOFRM) >*/
/*< 	INTEGER BUFLEN >*/
/*< 	SAVE BUF, BUFLEN >*/
/*< 	DATA BUF /TWOFRM*0./, BUFLEN /MAXFRM/ >*/
    /* Parameter adjustments */
    if (voice) {
	--voice;
	}
    if (rc) {
	--rc;
	}
    if (speech) {
	--speech;
	}

    /* Function Body */
    switch(n__) {
	case 1: goto L_initsynths;
	}

/* 	IF (LISTL.GE.3) THEN */
/* 	   WRITE(FDEBUG,400) NFRAME, VOICE, PITCH, RMS, RC */
/* 400	   FORMAT(1X/' SYNTHESIS DATA -- FRAME',I6, */
/*     1    T32,2I3,I6,1X,F5.0,10F8.3/) */
/* 	   IF (LISTL.GE.4) WRITE(FDEBUG,410) */
/* 410	   FORMAT(' EPOCH  G2PASS  RATIO PSCALE') */
/* 	END IF */
/*< 	pitch = max(min(pitch,156),20) >*/
/* Computing MAX */
    i__1 = min(*pitch,156);
    *pitch = max(i__1,20);
/*< 	do i = 1, order >*/
    i__1 = contrl_1.order;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*< 	    rc(i) = max(min(rc(i),.99),-.99) >*/
/* Computing MAX */
/* Computing MIN */
	r__2 = rc[i__];
	r__1 = (real) dmin(r__2,.99f);
	rc[i__] = (real) dmax(r__1,-.99f);
/*< 	end do >*/
    }
/*< 	C >*/
    pitsyn_(&contrl_1.order, &voice[1], pitch, rms, &rc[1], &contrl_1.lframe, 
	    ivuv, ipiti, rmsi, rci, &nout, &ratio);
/*< 	IF (NOUT.GT.0) THEN >*/
    if (nout > 0) {
/*< 	   DO J = 1,NOUT >*/
	i__1 = nout;
	for (j = 1; j <= i__1; ++j) {

/*             Add synthesized speech for pitch period J to the en
d of */
/*             BUF. */

/* 	      IF (LISTL.GE.3) THEN */
/* 	         IF (LISTL.EQ.3) THEN */
/* 	            WRITE(FDEBUG,420) J, NOUT, IVUV(J), IPITI(J), RMSI
(J), */
/*     1             (RCI(I,J),I=1,ORDER) */
/* 420	            FORMAT(1X,'PITSYN EPOCH ',I2,' OF ',I2,T32,I4,I
8,1X, */
/*     1             F5.0,T50,10F8.3) */
/* 	         ELSE */
/* 	            WRITE(FDEBUG,422) J, NOUT, IVUV(J), IPITI(J), RMSI
(J), */
/*     1             (RCI(I,J),I=1,ORDER) */
/* 422	            FORMAT(1X,I2,'/',I2,T32,I4,I8,1X, */
/*     1             F5.0,T50,10F8.3) */
/* 	         END IF */
/* 	      END IF */
/*< 	      CALL IRC2PC(RCI(1,J), PC, ORDER, GPRIME, G2PASS) >*/
	    irc2pc_(&rci[j * 10 - 10], pc, &contrl_1.order, &c_b2, &g2pass);
/* 	      IF (LISTL.GE.4) WRITE(FDEBUG,430) G2PASS, RATIO, 1.0, PC
 */
/* 430	      FORMAT(T7,3F7.3,T50,10F8.1) */
/*< 	  >*/
	    bsynz_(pc, &ipiti[j - 1], &ivuv[j - 1], &buf[buflen], &rmsi[j - 1]
		    , &ratio, &g2pass);
/*< 	      CALL DEEMP(BUF(BUFLEN+1), IPITI(J)) >*/
	    deemp_(&buf[buflen], &ipiti[j - 1]);
/*< 	      BUFLEN = BUFLEN + IPITI(J) >*/
	    buflen += ipiti[j - 1];
/*< 	   END DO >*/
	}

/*          Copy first MAXFRM samples from BUF to output array SPEECH 
*/
/*          (scaling them), and then remove them from the beginning of
 */
/*          BUF. */

/*< 	   DO I = 1,MAXFRM >*/
	for (i__ = 1; i__ <= 180; ++i__) {
/*< 	      SPEECH(I) = BUF(I) / 4096. >*/
	    speech[i__] = buf[i__ - 1] / 4096.f;
/*< 	   END DO >*/
	}
/*< 	   K = MAXFRM >*/
	*k = 180;
/*< 	   BUFLEN = BUFLEN - MAXFRM >*/
	buflen += -180;
/*< 	   DO I = 1,BUFLEN >*/
	i__1 = buflen;
	for (i__ = 1; i__ <= i__1; ++i__) {
/*< 	      BUF(I) = BUF(MAXFRM+I) >*/
	    buf[i__ - 1] = buf[i__ + 179];
/*< 	   END DO >*/
	}
/*< 	END IF >*/
    }
/*< 	RETURN >*/
    return 0;
/*< 	ENTRY INITSYNTHS () >*/

L_initsynths:
/*< 	DO I = 1,TWOFRM >*/
    for (i__ = 0; i__ < 360; ++i__) {
/*< 	   BUF(I) = 0. >*/
	buf[i__] = 0.f;
/*< 	END DO >*/
    }
/*< 	BUFLEN = MAXFRM >*/
    buflen = 180;
/* Initialize local state inside of the following subroutines. */
/*< 	CALL INITPITSYN () >*/
    initpitsyn_();
/*< 	CALL INITBSYNZ () >*/
    initbsynz_();
/*< 	CALL INITDEEMP () >*/
    initdeemp_();
/*< 	RETURN >*/
    return 0;
/*< 	END >*/
} /* synths_ */

/* Subroutine */ int synths_(integer *voice, integer *pitch, real *rms, real *
	rc, real *speech, integer *k)
{
    return synths_0_(0, voice, pitch, rms, rc, speech, k);
    }

/* Subroutine */ int initsynths_(void)
{
    return synths_0_(1, (integer *)0, (integer *)0, (real *)0, (real *)0, (
	    real *)0, (integer *)0);
    }


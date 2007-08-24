/* ABSTRA.f -- translated by f2c (version 19970805).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#ifdef __cplusplus
extern "C" {
#endif

/* OTB patches: replace "f2c.h" by "otb_6S_f2c.h" */
/*#include "f2c.h"*/
#include "otb_6S_f2c.h"

/* Common Block Declarations */

Extern struct {
    real z__[34], p[34], t[34], wh[34], wo[34];
} sixs_atm__;

#define sixs_atm__1 sixs_atm__

Extern struct {
    real zpl[34], ppl[34], tpl[34], whpl[34], wopl[34];
} sixs_planesim__;

#define sixs_planesim__1 sixs_planesim__

/*<    >*/
/* Subroutine */ int abstra_(integer *idatm, real *wl, real *xmus, real *xmuv,
	 real *uw, real *uo3, real *uwus, real *uo3us, integer *idatmp, real *
	uwpl, real *uo3pl, real *uwusp, real *uo3usp, real *dtwava, real *
	dtozon, real *dtdica, real *dtoxyg, real *dtniox, real *dtmeth, real *
	dtmoca, real *utwava, real *utozon, real *utdica, real *utoxyg, real *
	utniox, real *utmeth, real *utmoca, real *ttwava, real *ttozon, real *
	ttdica, real *ttoxyg, real *ttniox, real *ttmeth, real *ttmoca)
{
    /* Initialized data */

    static integer ivli[6] = { 2500,5060,7620,10180,12740,15300 };
    static real co3[102] = { (float).0045,(float).008,(float).0107,(float)
	    .011,(float).0127,(float).0171,(float).02,(float).0245,(float)
	    .0307,(float).0384,(float).0478,(float).0567,(float).0654,(float)
	    .0762,(float).0915,(float).1,(float).109,(float).12,(float).128,(
	    float).112,(float).111,(float).116,(float).119,(float).113,(float)
	    .103,(float).0924,(float).0828,(float).0757,(float).0707,(float)
	    .0658,(float).0556,(float).0477,(float).0406,(float).0387,(float)
	    .0382,(float).0294,(float).0209,(float).018,(float).0191,(float)
	    .0166,(float).0117,(float).0077,(float).0061,(float).0085,(float)
	    .0061,(float).0037,(float).0032,(float).0031,(float).00255,(float)
	    .00198,(float).0014,(float)8.25e-4,(float)2.5e-4,(float)0.,(float)
	    0.,(float)0.,(float)5.65e-4,(float).00204,(float).00735,(float)
	    .0203,(float).0498,(float).118,(float).246,(float).518,(float)
	    1.02,(float)1.95,(float)3.79,(float)6.65,(float)12.4,(float)22.,(
	    float)36.7,(float)59.5,(float)85.,(float)126.,(float)168.,(float)
	    206.,(float)242.,(float)271.,(float)291.,(float)302.,(float)303.,(
	    float)294.,(float)277.,(float)254.,(float)226.,(float)196.,(float)
	    168.,(float)144.,(float)117.,(float)97.5,(float)76.5,(float)60.4,(
	    float)46.2,(float)34.6,(float)25.2,(float)20.,(float)15.7,(float)
	    12.,(float)10.,(float)8.8,(float)8.3,(float)8.6 };
    static real cch2o[15] = { (float)0.,(float).19,(float).15,(float).12,(
	    float).1,(float).09,(float).1,(float).12,(float).15,(float).17,(
	    float).2,(float).24,(float).28,(float).33,(float)0. };

    /* Builtin functions */
    double exp(doublereal), sqrt(doublereal);

    /* Local variables */
    real a[8], g;
    integer i__, k, n;
    real u, v, y, p0, r2[34], r3[34], t0;
    integer id, nh;
    real ds, te, ud, xd;
    integer iv;
    real rm[34], xh, tp[34], up, xi, tt, ut, uu, tn, ds2, te2, air, phi, rat[
	    10], upd, udp;
    integer inu;
    real psi, uud, upl, udt, tnu[30]	/* was [10][3] */, upt, utt, uut, uuu,
	     ah2o, ako3, r2pl[34], r3pl[34], rmo2, rmo3;
    extern /* Subroutine */ int print_error__(char *, ftnlen);
    real accu, rmco, updp, rmpl[34], udtp, updt, uppl;
    extern /* Subroutine */ int dica2_(real *, integer *), dica3_(real *, 
	    integer *), dica1_(real *, integer *);
    real uupl, uptt;
    extern /* Subroutine */ int moca1_(real *, integer *), moca2_(real *, 
	    integer *), moca3_(real *, integer *), moca5_(real *, integer *), 
	    moca6_(real *, integer *), moca4_(real *, integer *);
    real rmch4;
    extern /* Subroutine */ int meth1_(real *, integer *), meth2_(real *, 
	    integer *), meth3_(real *, integer *), meth4_(real *, integer *), 
	    meth5_(real *, integer *), meth6_(real *, integer *);
    real roco2;
    extern /* Subroutine */ int wava6_(real *, integer *), wava5_(real *, 
	    integer *), wava4_(real *, integer *), wava3_(real *, integer *), 
	    wava2_(real *, integer *), wava1_(real *, integer *);
    real rmn2o;
    extern /* Subroutine */ int niox1_(real *, integer *), niox2_(real *, 
	    integer *);
    real test1, test2, test3;
    extern /* Subroutine */ int niox6_(real *, integer *), niox5_(real *, 
	    integer *), niox4_(real *, integer *), niox3_(real *, integer *), 
	    ozon1_(real *, integer *), oxyg3_(real *, integer *), oxyg4_(real 
	    *, integer *), oxyg5_(real *, integer *), oxyg6_(real *, integer *
	    );
    integer idgaz;
    real roair, atest, ratpl[10], updtp;
    doublereal ptest, ptest1;
    real dtcont, ttcont, utcont;

/*     transmittance calculation for ozone, water vapor, */
/*     carbon dioxyde and oxygen. */
/*     downward absorption water vapor dtwava */
/*     downward absorption ozone       dtozon */
/*     downward absorption carbon diox dtdica */
/*     downward absorption oxygen      dtoxyg */
/*     downward absorption nitrous oxi dtniox */
/*     downward absorption methane     dtmeth */
/*     downward absorption carbon mono dtmoca */
/*     upward absorption water vapor   utwava */
/*     upward absorption ozone         utozon */
/*     upward absorption carbon diox   utdica */
/*     upward absorption oxygen        utoxyg */
/*     upward   absorption nitrous oxi utniox */
/*     upward   absorption methane     utmeth */
/*     upward   absorption carbon mono utmoca */
/*     total(on the two paths ) absorption water vapor ttwava */
/*     total(on the two paths ) absorption ozone       ttozon */
/*     total(on the two paths ) absorption carbon diox ttdica */
/*     total(on the two paths ) absorption oxygen      ttoxyg */
/*     total    absorption nitrous oxi ttniox */
/*     total    absorption methane     ttmeth */
/*     total    absorption carbon mono ttmoca */
/*<       common /sixs_atm/ z(34),p(34),t(34),wh(34),wo(34) >*/
/*<       common /sixs_planesim/ zpl(34),ppl(34),tpl(34),whpl(34),wopl(34) >*/
/*<       real z,p,t,wh,wo >*/
/*<       real zpl,ppl,tpl,whpl,wopl >*/
/*<       integer iv,ivli(6),idatm,idatmp,i,id,idgaz,inu,k,n,nh >*/
/*<    >*/
/*<       real rmpl(34),r2pl(34),r3pl(34),ratpl(10) >*/
/*<       real cch2o(15) >*/
/*<       real wl,xmus,xmuv,uw,uo3,uwus,uo3us,uwpl,uo3pl,uwusp,uo3usp >*/
/*<       real accu,ah2o,xh,dtcont,utcont,ttcont >*/
/*<       double precision ptest1,ptest >*/
/*<       real dtoxyg,dtniox,dtmeth,dtmoca,dtwava,dtozon,dtdica >*/
/*<       real utniox,utdica,utoxyg,utwava,utozon >*/
/*<       real utmeth,utmoca,ttwava,ttozon,ttdica,ttoxyg,ttniox,ttmeth >*/
/*<       real ttmoca >*/
/*<       real p0,g,t0,ds,te,roair,air,roco2,rmo2,rmo3,rmn2o,rmch4 >*/
/*<       real rmco,v,te2,phi,psi,uu,u,up,uud,uut,uuu >*/
/*<       real ud,ut,upd,upt,udp,updp,udtp,updtp >*/
/*<       real ds2,uupl,upl,uppl >*/
/*<       real xi,xd,ako3,test1,test2,test3,udt,atest >*/
/*<       real updt,tt,y,utt,uptt,tn >*/

/*<       data(ivli(i),i=1,6)/2500,5060,7620,10180,12740,15300/ >*/
/*<    >*/
/*<    >*/
/*<       accu=1.E-10 >*/
    accu = (float)1e-10;
/*<       dtwava=1. >*/
    *dtwava = (float)1.;
/*<       utwava=1. >*/
    *utwava = (float)1.;
/*<       ttwava=1. >*/
    *ttwava = (float)1.;
/*<       dtcont=1. >*/
    dtcont = (float)1.;
/*<       utcont=1. >*/
    utcont = (float)1.;
/*<       ttcont=1. >*/
    ttcont = (float)1.;
/*<       dtozon=1. >*/
    *dtozon = (float)1.;
/*<       utozon=1. >*/
    *utozon = (float)1.;
/*<       ttozon=1. >*/
    *ttozon = (float)1.;
/*<       dtdica=1. >*/
    *dtdica = (float)1.;
/*<       utdica=1. >*/
    *utdica = (float)1.;
/*<       ttdica=1. >*/
    *ttdica = (float)1.;
/*<       dtoxyg=1. >*/
    *dtoxyg = (float)1.;
/*<       utoxyg=1. >*/
    *utoxyg = (float)1.;
/*<       ttoxyg=1. >*/
    *ttoxyg = (float)1.;
/*<       dtniox=1. >*/
    *dtniox = (float)1.;
/*<       utniox=1. >*/
    *utniox = (float)1.;
/*<       ttniox=1. >*/
    *ttniox = (float)1.;
/*<       dtmeth=1. >*/
    *dtmeth = (float)1.;
/*<       utmeth=1. >*/
    *utmeth = (float)1.;
/*<       ttmeth=1. >*/
    *ttmeth = (float)1.;
/*<       dtmoca=1. >*/
    *dtmoca = (float)1.;
/*<       utmoca=1. >*/
    *utmoca = (float)1.;
/*<       ttmoca=1. >*/
    *ttmoca = (float)1.;
/*<       do 201 i=1,10 >*/
    for (i__ = 1; i__ <= 10; ++i__) {
/* orig do 201 i=1,7 */
/*<       rat(i)=1. >*/
	rat[i__ - 1] = (float)1.;
/*<       tnu(i,1)=1. >*/
	tnu[i__ - 1] = (float)1.;
/*<       tnu(i,2)=1. >*/
	tnu[i__ + 9] = (float)1.;
/*<       tnu(i,3)=1. >*/
	tnu[i__ + 19] = (float)1.;
/*<   201 continue >*/
/* L201: */
    }
/*<       if (idatm.eq.0) return >*/
    if (*idatm == 0) {
	return 0;
    }
/*<       if((xmus.eq.0.).or.(xmuv.eq.0.)) goto 95 >*/
    if (*xmus == (float)0. || *xmuv == (float)0.) {
	goto L95;
    }
/*     constants determination */
/*<       p0=1013.25 >*/
    p0 = (float)1013.25;
/*<       g=98.1 >*/
    g = (float)98.1;
/*<       t0=250. >*/
    t0 = (float)250.;
/*     volumic mass in kilogrammes per m3 */
/*<       ds=0. >*/
    ds = (float)0.;
/*<       te=0. >*/
    te = (float)0.;
/*<       roair=0. >*/
    roair = (float)0.;
/*<       air=0.028964/0.0224 >*/
    air = (float)1.2930357142857143;
/*<       roco2=0.044/0.0224 >*/
    roco2 = (float)1.9642857142857142;
/*<       rmo2=0.032/0.0224 >*/
    rmo2 = (float)1.4285714285714286;
/*<       rmo3=0.048/0.0224 >*/
    rmo3 = (float)2.1428571428571428;
/*<       rmn2o=0.044/0.0224 >*/
    rmn2o = (float)1.9642857142857142;
/*<       rmch4=0.016/0.0224 >*/
    rmch4 = (float).7142857142857143;
/*<       rmco =0.028/0.0224 >*/
    rmco = (float)1.25;
/*<       uwus=1.424 >*/
    *uwus = (float)1.424;
/*<       uo3us=.344 >*/
    *uo3us = (float).344;
/*<       if(idatm.eq.8) goto 80 >*/
    if (*idatm == 8) {
	goto L80;
    }
/*<       goto 90 >*/
    goto L90;
/*<    80 rat(1)=uw/uwus >*/
L80:
    rat[0] = *uw / *uwus;
/*<       rat(2)=1. >*/
    rat[1] = (float)1.;
/*<       rat(3)=1. >*/
    rat[2] = (float)1.;
/*<       rat(4)=uo3/uo3us >*/
    rat[3] = *uo3 / *uo3us;
/*<       rat(5)=1. >*/
    rat[4] = (float)1.;
/*<       rat(6)=1. >*/
    rat[5] = (float)1.;
/*<       rat(7)=1. >*/
    rat[6] = (float)1.;
/*<       rat(8)=uw/uwus >*/
    rat[7] = *uw / *uwus;
/*<       rat(9)=uw/uwus >*/
    rat[8] = *uw / *uwus;
/*<       rat(10)=uw/uwus >*/
    rat[9] = *uw / *uwus;
/*<    90 v=1.0e+04/wl >*/
L90:
    v = (float)1e4 / *wl;
/*<       iv=v/5. >*/
    iv = v / (float)5.;
/*<       iv=iv*5 >*/
    iv *= 5;
/*<       id=((iv-2500)/10)/256+1 >*/
    id = (iv - 2500) / 10 / 256 + 1;
/*<       do 40 idgaz=1,7 >*/
    for (idgaz = 1; idgaz <= 7; ++idgaz) {


/*<       if (id.le.6) inu=(iv-ivli(id))/10+1 >*/
	if (id <= 6) {
	    inu = (iv - ivli[id - 1]) / 10 + 1;
	}
/*<       goto(101,102,103,104,105,106),id >*/
	switch (id) {
	    case 1:  goto L101;
	    case 2:  goto L102;
	    case 3:  goto L103;
	    case 4:  goto L104;
	    case 5:  goto L105;
	    case 6:  goto L106;
	}
/*<       goto 270 >*/
	goto L270;
/*<   106 if(idgaz.eq.1) call wava6(a,inu) >*/
L106:
	if (idgaz == 1) {
	    wava6_(a, &inu);
	}
/*<       if(idgaz.eq.2) goto 270 >*/
	if (idgaz == 2) {
	    goto L270;
	}
/*<       if(idgaz.eq.3) call oxyg6(a,inu) >*/
	if (idgaz == 3) {
	    oxyg6_(a, &inu);
	}
/*<       if(idgaz.eq.4) goto 270 >*/
	if (idgaz == 4) {
	    goto L270;
	}
/*<       if(idgaz.eq.5) call niox6(a,inu) >*/
	if (idgaz == 5) {
	    niox6_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth6(a,inu) >*/
	if (idgaz == 6) {
	    meth6_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca6(a,inu) >*/
	if (idgaz == 7) {
	    moca6_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   105 if(idgaz.eq.1) call wava5(a,inu) >*/
L105:
	if (idgaz == 1) {
	    wava5_(a, &inu);
	}
/*<       if(idgaz.eq.2) goto 270 >*/
	if (idgaz == 2) {
	    goto L270;
	}
/*<       if(idgaz.eq.3) call oxyg5(a,inu) >*/
	if (idgaz == 3) {
	    oxyg5_(a, &inu);
	}
/*<       if(idgaz.eq.4) goto 270 >*/
	if (idgaz == 4) {
	    goto L270;
	}
/*<       if(idgaz.eq.5) call niox5(a,inu) >*/
	if (idgaz == 5) {
	    niox5_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth5(a,inu) >*/
	if (idgaz == 6) {
	    meth5_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca5(a,inu) >*/
	if (idgaz == 7) {
	    moca5_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   104 if(idgaz.eq.1) call wava4(a,inu) >*/
L104:
	if (idgaz == 1) {
	    wava4_(a, &inu);
	}
/*<       if(idgaz.eq.2) goto 270 >*/
	if (idgaz == 2) {
	    goto L270;
	}
/*<       if(idgaz.eq.3) call oxyg4(a,inu) >*/
	if (idgaz == 3) {
	    oxyg4_(a, &inu);
	}
/*<       if(idgaz.eq.4) goto 270 >*/
	if (idgaz == 4) {
	    goto L270;
	}
/*<       if(idgaz.eq.5) call niox4(a,inu) >*/
	if (idgaz == 5) {
	    niox4_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth4(a,inu) >*/
	if (idgaz == 6) {
	    meth4_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca4(a,inu) >*/
	if (idgaz == 7) {
	    moca4_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   103 if(idgaz.eq.1) call wava3(a,inu) >*/
L103:
	if (idgaz == 1) {
	    wava3_(a, &inu);
	}
/*<       if(idgaz.eq.2) call dica3(a,inu) >*/
	if (idgaz == 2) {
	    dica3_(a, &inu);
	}
/*<       if(idgaz.eq.3) call oxyg3(a,inu) >*/
	if (idgaz == 3) {
	    oxyg3_(a, &inu);
	}
/*<       if(idgaz.eq.4) goto 270 >*/
	if (idgaz == 4) {
	    goto L270;
	}
/*<       if(idgaz.eq.5) call niox3(a,inu) >*/
	if (idgaz == 5) {
	    niox3_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth3(a,inu) >*/
	if (idgaz == 6) {
	    meth3_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca3(a,inu) >*/
	if (idgaz == 7) {
	    moca3_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   102 if(idgaz.eq.1) call wava2(a,inu) >*/
L102:
	if (idgaz == 1) {
	    wava2_(a, &inu);
	}
/*<       if(idgaz.eq.2) call dica2(a,inu) >*/
	if (idgaz == 2) {
	    dica2_(a, &inu);
	}
/*<       if(idgaz.eq.3) goto 270 >*/
	if (idgaz == 3) {
	    goto L270;
	}
/*<       if(idgaz.eq.4) goto 270 >*/
	if (idgaz == 4) {
	    goto L270;
	}
/*<       if(idgaz.eq.5) call niox2(a,inu) >*/
	if (idgaz == 5) {
	    niox2_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth2(a,inu) >*/
	if (idgaz == 6) {
	    meth2_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca2(a,inu) >*/
	if (idgaz == 7) {
	    moca2_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   101 if(idgaz.eq.1) call wava1(a,inu) >*/
L101:
	if (idgaz == 1) {
	    wava1_(a, &inu);
	}
/*<       if(idgaz.eq.2) call dica1(a,inu) >*/
	if (idgaz == 2) {
	    dica1_(a, &inu);
	}
/*<       if(idgaz.eq.3) goto 270 >*/
	if (idgaz == 3) {
	    goto L270;
	}
/*<       if(idgaz.eq.4) call ozon1(a,inu) >*/
	if (idgaz == 4) {
	    ozon1_(a, &inu);
	}
/*<       if(idgaz.eq.5) call niox1(a,inu) >*/
	if (idgaz == 5) {
	    niox1_(a, &inu);
	}
/*<       if(idgaz.eq.6) call meth1(a,inu) >*/
	if (idgaz == 6) {
	    meth1_(a, &inu);
	}
/*<       if(idgaz.eq.7) call moca1(a,inu) >*/
	if (idgaz == 7) {
	    moca1_(a, &inu);
	}
/*<       goto 271 >*/
	goto L271;
/*<   270 do 200 i=1,8 >*/
L270:
	for (i__ = 1; i__ <= 8; ++i__) {
/*<       a(i)=0. >*/
	    a[i__ - 1] = (float)0.;
/*<   200 continue >*/
/* L200: */
	}
/*<   271 continue >*/
L271:
/*     mixing ratio calculation for each gaseous constituents */
/*<       do k=1,33  >*/
	for (k = 1; k <= 33; ++k) {
/*<       roair=air*273.16*p(k)/(1013.25*t(k)) >*/
	    roair = air * (float)273.16 * sixs_atm__1.p[k - 1] / (
		    sixs_atm__1.t[k - 1] * (float)1013.25);
/*<       tp(k)=(t(k)+t(k+1))/2. >*/
	    tp[k - 1] = (sixs_atm__1.t[k - 1] + sixs_atm__1.t[k]) / (float)2.;
/*<       te=tp(k)-t0 >*/
	    te = tp[k - 1] - t0;
/*<       te2=te*te >*/
	    te2 = te * te;
/*<       phi=exp(a(3)*te+a(4)*te2) >*/
	    phi = exp(a[2] * te + a[3] * te2);
/*<       psi=exp(a(5)*te+a(6)*te2) >*/
	    psi = exp(a[4] * te + a[5] * te2);
/*<       if(idgaz.eq.1) rm(k)=wh(k)/(roair*1000.) >*/
	    if (idgaz == 1) {
		rm[k - 1] = sixs_atm__1.wh[k - 1] / (roair * (float)1e3);
	    }
/*<       if(idgaz.eq.2) rm(k)=3.3e-04*roco2/air >*/
	    if (idgaz == 2) {
		rm[k - 1] = roco2 * (float)3.3e-4 / air;
	    }
/*<       if(idgaz.eq.3) rm(k)=0.20947*rmo2/air >*/
	    if (idgaz == 3) {
		rm[k - 1] = rmo2 * (float).20947 / air;
	    }
/*<       if(idgaz.eq.4) rm(k)=wo(k)/(roair*1000.) >*/
	    if (idgaz == 4) {
		rm[k - 1] = sixs_atm__1.wo[k - 1] / (roair * (float)1e3);
	    }
/*<       if(idgaz.eq.5) rm(k)=310.e-09*rmn2o/air >*/
	    if (idgaz == 5) {
		rm[k - 1] = rmn2o * (float)3.1e-7 / air;
	    }
/*<       if(idgaz.eq.6) rm(k)=1.72e-06*rmch4/air >*/
	    if (idgaz == 6) {
		rm[k - 1] = rmch4 * (float)1.72e-6 / air;
	    }
/*<       if(idgaz.eq.7) rm(k)=1.00e-09*rmco /air >*/
	    if (idgaz == 7) {
		rm[k - 1] = rmco * (float)1e-9 / air;
	    }
/*<       r2(k)=rm(k)*phi >*/
	    r2[k - 1] = rm[k - 1] * phi;
/*<       r3(k)=rm(k)*psi >*/
	    r3[k - 1] = rm[k - 1] * psi;
/*<       enddo >*/
	}

/*<       uu=0. >*/
	uu = (float)0.;
/*<       u=0. >*/
	u = (float)0.;
/*<       up=0. >*/
	up = (float)0.;
/*<       uud=0. >*/
	uud = (float)0.;
/*<       uut=0. >*/
	uut = (float)0.;
/*<       uuu=0. >*/
	uuu = (float)0.;
/*<       ud=0. >*/
	ud = (float)0.;
/*<       ut=0. >*/
	ut = (float)0.;
/*<       upd=0. >*/
	upd = (float)0.;
/*<       upt=0. >*/
	upt = (float)0.;
/*<       udp=0. >*/
	udp = (float)0.;
/*<       updp=0. >*/
	updp = (float)0.;
/*<       udtp=0. >*/
	udtp = (float)0.;
/*<       updtp=0. >*/
	updtp = (float)0.;
/*     pressure scale integration sigma=p/p0 */
/*<          do 50 k=2,33                                   >*/
	for (k = 2; k <= 33; ++k) {
/*<          ds=(p(k-1)-p(k))/p(1)                         >*/
	    ds = (sixs_atm__1.p[k - 2] - sixs_atm__1.p[k - 1]) / 
		    sixs_atm__1.p[0];
/*<          ds2=(p(k-1)*p(k-1)-p(k)*p(k))/(2.*p(1)*p0)   >*/
	    ds2 = (sixs_atm__1.p[k - 2] * sixs_atm__1.p[k - 2] - 
		    sixs_atm__1.p[k - 1] * sixs_atm__1.p[k - 1]) / (
		    sixs_atm__1.p[0] * (float)2. * p0);
/*<          uu=uu+((rm(k)+rm(k-1))/2.)*ds*rat(idgaz)    >*/
	    uu += (rm[k - 1] + rm[k - 2]) / (float)2. * ds * rat[idgaz - 1];
/*<          u =u +((r2(k)+r2(k-1))/2.)*ds*rat(idgaz)   >*/
	    u += (r2[k - 1] + r2[k - 2]) / (float)2. * ds * rat[idgaz - 1];
/*<          up=up+((r3(k)+r3(k-1))/2.)*ds2*rat(idgaz) >*/
	    up += (r3[k - 1] + r3[k - 2]) / (float)2. * ds2 * rat[idgaz - 1];
/*<    50    continue                                 >*/
/* L50: */
	}
/*<          uu=uu*p(1)*100./g                       >*/
	uu = uu * sixs_atm__1.p[0] * (float)100. / g;
/*<          u=u*p(1)*100./g                        >*/
	u = u * sixs_atm__1.p[0] * (float)100. / g;
/*<          up=up*p(1)*100./g                     >*/
	up = up * sixs_atm__1.p[0] * (float)100. / g;
/*<          if(idgaz.eq.4) uu=1000.*uu/rmo3     >*/
	if (idgaz == 4) {
	    uu = uu * (float)1e3 / rmo3;
	}
/*<          if(idgaz.eq.2) uu=1000.*uu/roco2   >*/
	if (idgaz == 2) {
	    uu = uu * (float)1e3 / roco2;
	}
/*<          if(idgaz.eq.5) uu=1000.*uu/rmn2o >*/
	if (idgaz == 5) {
	    uu = uu * (float)1e3 / rmn2o;
	}
/*<          if(idgaz.eq.6) uu=1000.*uu/rmch4 >*/
	if (idgaz == 6) {
	    uu = uu * (float)1e3 / rmch4;
	}
/*<          if(idgaz.eq.7) uu=1000.*uu/rmco >*/
	if (idgaz == 7) {
	    uu = uu * (float)1e3 / rmco;
	}
/*     mixing ratio for plane calculation for each gaseous constituent
s */
/*<       if ((idatmp.eq.0).or.(idatmp.eq.4)) then >*/
	if (*idatmp == 0 || *idatmp == 4) {
/*<           uupl=uu >*/
	    uupl = uu;
/*<           upl=u >*/
	    upl = u;
/*<           uppl=up >*/
	    uppl = up;
/*<           else >*/
	} else {
/*<           do k=1,33 >*/
	    for (k = 1; k <= 33; ++k) {
/*<           roair=air*273.16*ppl(k)/(1013.25*tpl(k)) >*/
		roair = air * (float)273.16 * sixs_planesim__1.ppl[k - 1] / (
			sixs_planesim__1.tpl[k - 1] * (float)1013.25);
/*<           tp(k)=(tpl(k)+tpl(k+1))/2. >*/
		tp[k - 1] = (sixs_planesim__1.tpl[k - 1] + 
			sixs_planesim__1.tpl[k]) / (float)2.;
/*<           te=tp(k)-t0 >*/
		te = tp[k - 1] - t0;
/*<           te2=te*te >*/
		te2 = te * te;
/*<           phi=exp(a(3)*te+a(4)*te2) >*/
		phi = exp(a[2] * te + a[3] * te2);
/*<           psi=exp(a(5)*te+a(6)*te2) >*/
		psi = exp(a[4] * te + a[5] * te2);
/*<           if(idgaz.eq.1) rmpl(k)=whpl(k)/(roair*1000.) >*/
		if (idgaz == 1) {
		    rmpl[k - 1] = sixs_planesim__1.whpl[k - 1] / (roair * (
			    float)1e3);
		}
/*<           if(idgaz.eq.2) rmpl(k)=3.3e-04*roco2/air >*/
		if (idgaz == 2) {
		    rmpl[k - 1] = roco2 * (float)3.3e-4 / air;
		}
/*<           if(idgaz.eq.3) rmpl(k)=0.20947*rmo2/air >*/
		if (idgaz == 3) {
		    rmpl[k - 1] = rmo2 * (float).20947 / air;
		}
/*<           if(idgaz.eq.4) rmpl(k)=wopl(k)/(roair*1000.) >*/
		if (idgaz == 4) {
		    rmpl[k - 1] = sixs_planesim__1.wopl[k - 1] / (roair * (
			    float)1e3);
		}
/*<           if(idgaz.eq.5) rmpl(k)=310.e-09*rmn2o/air >*/
		if (idgaz == 5) {
		    rmpl[k - 1] = rmn2o * (float)3.1e-7 / air;
		}
/*<           if(idgaz.eq.6) rmpl(k)=1.72e-06*rmch4/air >*/
		if (idgaz == 6) {
		    rmpl[k - 1] = rmch4 * (float)1.72e-6 / air;
		}
/*<           if(idgaz.eq.7) rmpl(k)=1.00e-09*rmco /air >*/
		if (idgaz == 7) {
		    rmpl[k - 1] = rmco * (float)1e-9 / air;
		}
/*<           r2pl(k)=rmpl(k)*phi >*/
		r2pl[k - 1] = rmpl[k - 1] * phi;
/*<           r3pl(k)=rmpl(k)*psi >*/
		r3pl[k - 1] = rmpl[k - 1] * psi;
/*<           enddo >*/
	    }

/*<           uupl=0. >*/
	    uupl = (float)0.;
/*<           upl=0. >*/
	    upl = (float)0.;
/*<           uppl=0. >*/
	    uppl = (float)0.;
/* update ratio plane */
/*<           do i=1,10 >*/
	    for (i__ = 1; i__ <= 10; ++i__) {
/*<           ratpl(i)=1. >*/
		ratpl[i__ - 1] = (float)1.;
/*<           enddo >*/
	    }
/*<           if (idatmp.eq.8) then >*/
	    if (*idatmp == 8) {
/*<           ratpl(1)=uwpl/uwusp >*/
		ratpl[0] = *uwpl / *uwusp;
/*<           ratpl(2)=1. >*/
		ratpl[1] = (float)1.;
/*<           ratpl(3)=1. >*/
		ratpl[2] = (float)1.;
/*<           ratpl(4)=uo3pl/uo3usp >*/
		ratpl[3] = *uo3pl / *uo3usp;
/*<           ratpl(5)=1. >*/
		ratpl[4] = (float)1.;
/*<           ratpl(6)=1. >*/
		ratpl[5] = (float)1.;
/*<           ratpl(7)=1. >*/
		ratpl[6] = (float)1.;
/*<           ratpl(8)=uwpl/uwusp >*/
		ratpl[7] = *uwpl / *uwusp;
/*<           ratpl(9)=uwpl/uwusp >*/
		ratpl[8] = *uwpl / *uwusp;
/*<           ratpl(10)=uwpl/uwusp >*/
		ratpl[9] = *uwpl / *uwusp;
/*<           endif >*/
	    }
/*     pressure scale integration sigma=p/p0 */
/* * all gases */
/*<          do k=2,33 >*/
	    for (k = 2; k <= 33; ++k) {
/*<          ds=(ppl(k-1)-ppl(k))/ppl(1) >*/
		ds = (sixs_planesim__1.ppl[k - 2] - sixs_planesim__1.ppl[k - 
			1]) / sixs_planesim__1.ppl[0];
/*<          ds2=(ppl(k-1)*ppl(k-1)-ppl(k)*ppl(k))/(2.*ppl(1)*p0) >*/
		ds2 = (sixs_planesim__1.ppl[k - 2] * sixs_planesim__1.ppl[k - 
			2] - sixs_planesim__1.ppl[k - 1] * 
			sixs_planesim__1.ppl[k - 1]) / (sixs_planesim__1.ppl[
			0] * (float)2. * p0);
/*<          uupl=uupl+((rmpl(k)+rmpl(k-1))/2.)*ds*ratpl(idgaz) >*/
		uupl += (rmpl[k - 1] + rmpl[k - 2]) / (float)2. * ds * ratpl[
			idgaz - 1];
/*<          upl =upl +((r2pl(k)+r2pl(k-1))/2.)*ds*ratpl(idgaz) >*/
		upl += (r2pl[k - 1] + r2pl[k - 2]) / (float)2. * ds * ratpl[
			idgaz - 1];
/*<          uppl=uppl+((r3pl(k)+r3pl(k-1))/2.)*ds2*ratpl(idgaz) >*/
		uppl += (r3pl[k - 1] + r3pl[k - 2]) / (float)2. * ds2 * ratpl[
			idgaz - 1];
/*<          enddo >*/
	    }
/*<          uupl=uupl*ppl(1)*100./g >*/
	    uupl = uupl * sixs_planesim__1.ppl[0] * (float)100. / g;
/*<          upl=upl*ppl(1)*100./g >*/
	    upl = upl * sixs_planesim__1.ppl[0] * (float)100. / g;
/*<          uppl=uppl*ppl(1)*100./g >*/
	    uppl = uppl * sixs_planesim__1.ppl[0] * (float)100. / g;
/*<          if(idgaz.eq.4) uupl=1000*uupl/rmo3 >*/
	    if (idgaz == 4) {
		uupl = uupl * 1000 / rmo3;
	    }
/*<          if(idgaz.eq.2) uupl=1000*uupl/roco2 >*/
	    if (idgaz == 2) {
		uupl = uupl * 1000 / roco2;
	    }
/*<          if(idgaz.eq.5) uupl=1000*uupl/rmn2o >*/
	    if (idgaz == 5) {
		uupl = uupl * 1000 / rmn2o;
	    }
/*<          if(idgaz.eq.6) uupl=1000*uupl/rmch4 >*/
	    if (idgaz == 6) {
		uupl = uupl * 1000 / rmch4;
	    }
/*<          if(idgaz.eq.7) uupl=1000*uupl/rmco >*/
	    if (idgaz == 7) {
		uupl = uupl * 1000 / rmco;
	    }
/* endif test idatm =0,4 */
/*<       endif >*/
	}
/*     downward path */
/*<       uud=uu/xmus >*/
	uud = uu / *xmus;
/*     upward path */
/*<       uuu=uupl/xmuv >*/
	uuu = uupl / *xmuv;
/*     total(down+up) path */
/*<       uut=uu/xmus+uupl/xmuv >*/
	uut = uu / *xmus + uupl / *xmuv;
/*<       if(idgaz.eq.1) goto 146 >*/
	if (idgaz == 1) {
	    goto L146;
	}
/*<       if(idgaz.eq.2.and.iv.gt.9620) goto 147 >*/
	if (idgaz == 2 && iv > 9620) {
	    goto L147;
	}
/*<       if(idgaz.eq.3.and.iv.gt.15920) goto 147 >*/
	if (idgaz == 3 && iv > 15920) {
	    goto L147;
	}
/*<       if(idgaz.eq.4) goto 146 >*/
	if (idgaz == 4) {
	    goto L146;
	}
/*<       goto 145 >*/
	goto L145;
/*<   146 xi=0. >*/
L146:
	xi = (float)0.;
/*<       n=0 >*/
	n = 0;
/*<       xd=0. >*/
	xd = (float)0.;
/*<       if(iv.lt.2350.or.iv.gt.3000) goto 148 >*/
	if (iv < 2350 || iv > 3000) {
	    goto L148;
	}
/*<       xi=(v-2350.)/50.+1. >*/
	xi = (v - (float)2350.) / (float)50. + (float)1.;
/*<       NH=XI+1.001 >*/
	nh = xi + (float)1.001;
/*<       XH=XI-FLOAT(NH) >*/
	xh = xi - (real) nh;
/*<       AH2O=CCH2O(NH)+XH*(CCH2O(NH)-CCH2O(NH-1)) >*/
	ah2o = cch2o[nh - 1] + xh * (cch2o[nh - 1] - cch2o[nh - 2]);
/*<       DTCONT=EXP(-AH2O*UUD) >*/
	dtcont = exp(-ah2o * uud);
/*<       UTCONT=EXP(-AH2O*UUU) >*/
	utcont = exp(-ah2o * uuu);
/*<       TTCONT=EXP(-AH2O*UUT) >*/
	ttcont = exp(-ah2o * uut);
/*<   148 if (idgaz.eq.1) goto 145 >*/
L148:
	if (idgaz == 1) {
	    goto L145;
	}
/*<       if(iv.lt.13000) goto 145 >*/
	if (iv < 13000) {
	    goto L145;
	}
/*<       if(iv.le.23400) goto 130 >*/
	if (iv <= 23400) {
	    goto L130;
	}
/*<       if(iv.ge.27500) goto 135 >*/
	if (iv >= 27500) {
	    goto L135;
	}
/*<       tnu(4,1)=1. >*/
	tnu[3] = (float)1.;
/*<       tnu(4,2)=1. >*/
	tnu[13] = (float)1.;
/*<       tnu(4,3)=1. >*/
	tnu[23] = (float)1.;
/*<       goto 45 >*/
	goto L45;
/*<   130 xi=(v-13000.)/200.+1. >*/
L130:
	xi = (v - (float)1.3e4) / (float)200. + (float)1.;
/*<       goto 140 >*/
	goto L140;
/*<   135 xi=(v-27500.)/500.+57. >*/
L135:
	xi = (v - (float)27500.) / (float)500. + (float)57.;
/*<   140 n=xi+1.001 >*/
L140:
	n = xi + (float)1.001;
/*<       xd=xi-float(n) >*/
	xd = xi - (real) n;
/*<       ako3=co3(n)+xd*(co3(n)-co3(n-1)) >*/
	ako3 = co3[n - 1] + xd * (co3[n - 1] - co3[n - 2]);
/*<       test1=ako3*uud >*/
	test1 = ako3 * uud;
/*<       test2=ako3*uuu >*/
	test2 = ako3 * uuu;
/*<       test3=ako3*uut >*/
	test3 = ako3 * uut;
/*     exponential overflow test */
/*<       if(test1.gt.86.0) test1=86.0 >*/
	if (test1 > (float)86.) {
	    test1 = (float)86.;
	}
/*<       if(test2.gt.86.0) test2=86.0 >*/
	if (test2 > (float)86.) {
	    test2 = (float)86.;
	}
/*<       if(test3.gt.86.0) test3=86.0 >*/
	if (test3 > (float)86.) {
	    test3 = (float)86.;
	}
/*<       tnu(4,1)=exp(-test1) >*/
	tnu[3] = exp(-test1);
/*<       tnu(4,2)=exp(-test2) >*/
	tnu[13] = exp(-test2);
/*<       tnu(4,3)=exp(-test3) >*/
	tnu[23] = exp(-test3);
/*<       goto 40 >*/
	goto L40;
/*<   145 continue >*/
L145:
/*<       if(idgaz.eq.4.and.iv.gt.3020) goto 147 >*/
	if (idgaz == 4 && iv > 3020) {
	    goto L147;
	}

/*     downward path */

/*<       ud=u/xmus >*/
	ud = u / *xmus;
/*<       upd=up/xmus >*/
	upd = up / *xmus;
/*<       udt=ud >*/
	udt = ud;
/*<       if(ud.eq.0.and.upd.eq.0.) udt=1. >*/
	if (ud == (float)0. && upd == (float)0.) {
	    udt = (float)1.;
	}
/*<       tn=a(2)*upd/(2*udt) >*/
	tn = a[1] * upd / (udt * 2);
/*<       atest=a(2) >*/
	atest = a[1];
/*<       if (a(2).eq.0.and.a(1).eq.0.) atest=1. >*/
	if (a[1] == (float)0. && a[0] == (float)0.) {
	    atest = (float)1.;
	}
/*<       updt=upd >*/
	updt = upd;
/*<       if(ud.eq.0.and.upd.eq.0.) updt=1. >*/
	if (ud == (float)0. && upd == (float)0.) {
	    updt = (float)1.;
	}
/*<       tt=1+4*(a(1)/atest)*((ud*ud)/updt) >*/
	tt = a[0] / atest * 4 * (ud * ud / updt) + 1;
/*<       y=-tn*(sqrt(tt)-1) >*/
	y = -tn * (sqrt(tt) - 1);
/*<       if(idgaz.eq.1) y=-a(1)*ud/sqrt(1+(a(1)/atest)*(ud*ud/updt)) >*/
	if (idgaz == 1) {
	    y = -a[0] * ud / sqrt(a[0] / atest * (ud * ud / updt) + 1);
	}
/*<       tnu(idgaz,1)=exp(y) >*/
	tnu[idgaz - 1] = exp(y);

/*     upward path modified to take account for plane content */

/*<       udp=upl/xmuv >*/
	udp = upl / *xmuv;
/*<       updp=uppl/xmuv >*/
	updp = uppl / *xmuv;
/*<       udtp=udp >*/
	udtp = udp;
/*<       if(udp.eq.0.and.updp.eq.0.) udtp=1. >*/
	if (udp == (float)0. && updp == (float)0.) {
	    udtp = (float)1.;
	}
/*<       tn=a(2)*updp/(2*udtp) >*/
	tn = a[1] * updp / (udtp * 2);
/*<       atest=a(2) >*/
	atest = a[1];
/*<       if (a(2).eq.0.and.a(1).eq.0.) atest=1. >*/
	if (a[1] == (float)0. && a[0] == (float)0.) {
	    atest = (float)1.;
	}
/*<       updtp=updp >*/
	updtp = updp;
/*<       if(udp.eq.0.and.updp.eq.0.) updtp=1. >*/
	if (udp == (float)0. && updp == (float)0.) {
	    updtp = (float)1.;
	}
/*<       tt=1+4*(a(1)/atest)*((udp*udp)/updtp) >*/
	tt = a[0] / atest * 4 * (udp * udp / updtp) + 1;
/*<       y=-tn*(sqrt(tt)-1) >*/
	y = -tn * (sqrt(tt) - 1);
/*<       if(idgaz.eq.1) y=-a(1)*udp/sqrt(1+(a(1)/atest)*(udp*udp/updtp)) >*/
	if (idgaz == 1) {
	    y = -a[0] * udp / sqrt(a[0] / atest * (udp * udp / updtp) + 1);
	}
/*<       tnu(idgaz,2)=exp(y) >*/
	tnu[idgaz + 9] = exp(y);

/*     total(down+up) path modified on the way up */

/*<       ut=u/xmus+upl/xmuv >*/
	ut = u / *xmus + upl / *xmuv;
/*<       upt=up/xmus+uppl/xmuv >*/
	upt = up / *xmus + uppl / *xmuv;
/*<       utt=ut >*/
	utt = ut;
/*<       if(ut.eq.0.and.upt.eq.0.) utt=1. >*/
	if (ut == (float)0. && upt == (float)0.) {
	    utt = (float)1.;
	}
/*<       tn=a(2)*upt/(2*utt) >*/
	tn = a[1] * upt / (utt * 2);
/*<       uptt=upt >*/
	uptt = upt;
/*<       if(ut.eq.0.and.upt.eq.0.) uptt=1. >*/
	if (ut == (float)0. && upt == (float)0.) {
	    uptt = (float)1.;
	}
/*<       tt=1+4*(a(1)/atest)*((ut*ut)/uptt) >*/
	tt = a[0] / atest * 4 * (ut * ut / uptt) + 1;
/*<       y=-tn*(sqrt(tt)-1) >*/
	y = -tn * (sqrt(tt) - 1);
/*<       if(idgaz.eq.1) y=-a(1)*ut/sqrt(1+(a(1)/atest)*(ut*ut/uptt)) >*/
	if (idgaz == 1) {
	    y = -a[0] * ut / sqrt(a[0] / atest * (ut * ut / uptt) + 1);
	}
/*<       tnu(idgaz,3)=exp(y) >*/
	tnu[idgaz + 19] = exp(y);
/*<       goto 40 >*/
	goto L40;
/*<   147 tnu(idgaz,1)=1. >*/
L147:
	tnu[idgaz - 1] = (float)1.;
/*<       tnu(idgaz,2)=1. >*/
	tnu[idgaz + 9] = (float)1.;
/*<       tnu(idgaz,3)=1. >*/
	tnu[idgaz + 19] = (float)1.;
/*<    40 continue >*/
L40:
	;
    }

/*<    45 ptest1=tnu(1,1)*dtcont >*/
L45:
    ptest1 = tnu[0] * dtcont;
/*<       ptest=ptest1 >*/
    ptest = ptest1;
/*<       if (ptest.gt.accu) then >*/
    if (ptest > accu) {
/*<          dtwava=ptest >*/
	*dtwava = ptest;
/*<          else >*/
    } else {
/*<          dtwava=0. >*/
	*dtwava = (float)0.;
/*<          endif >*/
    }
/*<       ptest1=tnu(1,2)*utcont >*/
    ptest1 = tnu[10] * utcont;
/*<       ptest=ptest1 >*/
    ptest = ptest1;
/*<       if (ptest.gt.accu) then >*/
    if (ptest > accu) {
/*<          utwava=ptest >*/
	*utwava = ptest;
/*<          else >*/
    } else {
/*<          utwava=0. >*/
	*utwava = (float)0.;
/*<          endif >*/
    }
/*<       ptest1=tnu(1,3)*ttcont >*/
    ptest1 = tnu[20] * ttcont;
/*<       ptest=ptest1 >*/
    ptest = ptest1;
/*<       if (ptest.gt.accu) then >*/
    if (ptest > accu) {
/*<          ttwava=ptest >*/
	*ttwava = ptest;
/*<          else >*/
    } else {
/*<          ttwava=0. >*/
	*ttwava = (float)0.;
/*<          endif >*/
    }
/*<       dtdica=tnu(2,1) >*/
    *dtdica = tnu[1];
/*<       utdica=tnu(2,2) >*/
    *utdica = tnu[11];
/*<       ttdica=tnu(2,3) >*/
    *ttdica = tnu[21];
/*<       dtoxyg=tnu(3,1) >*/
    *dtoxyg = tnu[2];
/*<       utoxyg=tnu(3,2) >*/
    *utoxyg = tnu[12];
/*<       ttoxyg=tnu(3,3) >*/
    *ttoxyg = tnu[22];
/*<       dtozon=tnu(4,1) >*/
    *dtozon = tnu[3];
/*<       utozon=tnu(4,2) >*/
    *utozon = tnu[13];
/*<       ttozon=tnu(4,3) >*/
    *ttozon = tnu[23];
/*<       dtniox=tnu(5,1) >*/
    *dtniox = tnu[4];
/*<       utniox=tnu(5,2) >*/
    *utniox = tnu[14];
/*<       ttniox=tnu(5,3) >*/
    *ttniox = tnu[24];
/*<       dtmeth=tnu(6,1) >*/
    *dtmeth = tnu[5];
/*<       utmeth=tnu(6,2) >*/
    *utmeth = tnu[15];
/*<       ttmeth=tnu(6,3) >*/
    *ttmeth = tnu[25];
/*<       dtmoca=tnu(7,1) >*/
    *dtmoca = tnu[6];
/*<       utmoca=tnu(7,2) >*/
    *utmoca = tnu[16];
/*<       ttmoca=tnu(7,3) >*/
    *ttmoca = tnu[26];
/*<       if (idatmp.eq.0) then >*/
    if (*idatmp == 0) {
/*<          ttwava=dtwava >*/
	*ttwava = *dtwava;
/*<          utwava=1. >*/
	*utwava = (float)1.;
/*<          ttdica=dtdica >*/
	*ttdica = *dtdica;
/*<          utdica=1. >*/
	*utdica = (float)1.;
/*<          ttoxyg=dtoxyg >*/
	*ttoxyg = *dtoxyg;
/*<          utoxyg=1. >*/
	*utoxyg = (float)1.;
/*<          ttozon=dtozon >*/
	*ttozon = *dtozon;
/*<          utozon=1. >*/
	*utozon = (float)1.;
/*<          ttniox=dtniox >*/
	*ttniox = *dtniox;
/*<          utniox=1. >*/
	*utniox = (float)1.;
/*<          ttmeth=dtmeth >*/
	*ttmeth = *dtmeth;
/*<          utmeth=1. >*/
	*utmeth = (float)1.;
/*<          ttmoca=dtmoca >*/
	*ttmoca = *dtmoca;
/*<          utmoca=1. >*/
	*utmoca = (float)1.;
/*<          endif >*/
    }
/*<       return >*/
    return 0;
/*<    >*/
L95:
    print_error__("Error on zenithal angle ( near 90 deg )", 39L);
/*<       return >*/
    return 0;
/*<       end >*/
} /* abstra_ */

#ifdef __cplusplus
	}
#endif

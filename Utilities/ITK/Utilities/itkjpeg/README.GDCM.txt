This directory contains a subset of the Independent JPEG Group's (IJG)
libjpeg version 6b.  We only include enough of distribution to build
libjpeg.  We do not include the standard executables that come with
libjpeg (cjpeg, djpeg, etc.). Furthermore, the standard libjpeg build
process is replaced with a CMake build process.

We'd like to thank the IJG for distributing a public JPEG IO library.

We'd like to also thank the dcmtk crew for releasing their toolkit under such a nice license that allow us to patch ijg properly to handle all sort of jpeg compression found in the DICOM implementation. The copyright can be found in COPYRIGHT.dcmtk.

Modifications
-------------

1.
jconfig.h is usually generated by the build process. For this distribution,
we ship a version of jconfig.h.in to be used across several platforms. It's 
purpose is also to allow generation of both 8bits and 12bits lib.



2.
jdhuff.c was modified to support special Philips MRI image:

Index: jdhuff.c
===================================================================
RCS file: /cvs/public/gdcm/src/jpeg/libijg8/jdhuff.c,v
retrieving revision 1.1
diff -u -3 -p -r1.1 jdhuff.c
--- jdhuff.c    24 Jun 2003 10:09:48 -0000      1.1
+++ jdhuff.c    7 Oct 2004 02:29:43 -0000
@@ -259,8 +259,11 @@ jpeg_make_d_derived_tbl (j_decompress_pt
   if (isDC) {
     for (i = 0; i < numsymbols; i++) {
       int sym = htbl->huffval[i];
-      if (sym < 0 || sym > 15)
-       ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
+      if (sym < 0 || sym > 15) {
+         // Now, we can read Philips MRI Images
+         htbl->huffval[i]=15;
+//     ERREXIT(cinfo, JERR_BAD_HUFF_TABLE);
+      }
     }
   }
 }


3.
jmorecfg.h was modified so that we can modify it automatically at build time to
either generate a 8bits jpeg lib or 12bits jpeg library:

Index: jmorecfg.h
===================================================================
RCS file: /cvs/public/gdcm/src/jpeg/libijg8/jmorecfg.h,v
retrieving revision 1.4
diff -u -3 -p -r1.4 jmorecfg.h
--- jmorecfg.h  31 Oct 2003 14:59:50 -0000      1.4
+++ jmorecfg.h  7 Oct 2004 02:30:40 -0000
@@ -19,7 +19,8 @@
  * We do not support run-time selection of data precision, sorry.
  */

-#define BITS_IN_JSAMPLE  8     /* use 8 or 12 */
+//#define BITS_IN_JSAMPLE  8   /* use 8 or 12 */
+#define BITS_IN_JSAMPLE @GDCM_BITS_IN_JSAMPLE@

 /*
  * Maximum number of components (color channels) allowed in JPEG image.




4.
To further allow us to have to different copy of the 8, 12 and 16 bits jpeg
library we had to mangle the name. Fur this purpose two new file were added to
the library: gdcm_mangle_8bits.h, gdcm_mangle_12bits.h and gdcm_mangle_16bits.h. 
Those file were generated using:

        nm libgdcmijpeg8.a | grep " [R|T] " | colrm 1 11 | sort


5.
In order to read lossless images, we had to apply the ls-patch to jpeg-6b. So I started from scratch:

tar xvfz /tmp/jpegsrc.v6b.tar.gz                        (1)
patch < /tmp/ljpeg-6b.patch                             (2)
patch -p0 < arithmetic-without-arith-option-full.patch  (3)
patch -p0 < warnings10-14.patch                         (4)
patch -p0 < previous-gdcm.patch                         (5)
-----------------------------------------------------------
= The subdir src/jpeg/libijg

(now I also need to apply the redhat patch for cplusplus lib) 

(1) http://www.ijg.org/files/jpegsrc.v6b.tar.gz
(2) http://www.oceana.com/ftp/ljpeg/ljpeg-patch.v6b.tar.gz
(3), (4) and (5) http://www.creatis.insa-lyon.fr/~malaterre/jpeg/

(3) Was made with a carefull inspection of dcmtk code (see convert.sh at the same spot as the patch)
(4) Is basically just compilation of the lib with -W -Wall -Werror
(5) This contains some configuration copy/paste from VTK, and the 'well known' 12 bits Philips MRI DICOM patch



NOTE for later:
(shoud we disable getenv just as dcmtk ? there is a ijg flag for that NO_ENV or something similar

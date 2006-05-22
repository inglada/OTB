/*
 * $Id: ogr_perl.i,v 1.4 2005/09/21 19:04:12 kruland Exp $
 *
 * perl specific code for ogr bindings.
 */

/*
 * $Log: ogr_perl.i,v $
 * Revision 1.4  2005/09/21 19:04:12  kruland
 * Need to %include cpl_exceptions.i
 *
 * Revision 1.3  2005/09/21 18:00:05  kruland
 * Turn on UseExceptions in ogr init code.
 *
 * Revision 1.2  2005/09/13 17:36:28  kruland
 * Whoops!  import typemaps_perl.i.
 *
 * Revision 1.1  2005/09/13 16:08:45  kruland
 * Added perl specific modifications for gdal and ogr.
 *
 *
 */

%init %{

  UseExceptions();
  if ( OGRGetDriverCount() == 0 ) {
    OGRRegisterAll();
  }
  
%}

%include cpl_exceptions.i

%rename (GetDriverCount) OGRGetDriverCount;
%rename (GetOpenDSCount) OGRGetOpenDSCount;
%rename (SetGenerate_DB2_V72_BYTE_ORDER) OGRSetGenerate_DB2_V72_BYTE_ORDER;
%rename (RegisterAll) OGRRegisterAll();

%import typemaps_perl.i

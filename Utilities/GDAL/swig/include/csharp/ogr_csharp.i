/*
 * $Id: ogr_csharp.i,v 1.2 2005/09/06 01:51:04 kruland Exp $
 */

/*
 * $Log: ogr_csharp.i,v $
 * Revision 1.2  2005/09/06 01:51:04  kruland
 * Removed GetDriverByName, GetDriver, Open, OpenShared because they are defined
 * in ogr now.
 *
 * Revision 1.1  2005/09/02 16:19:23  kruland
 * Major reorganization to accomodate multiple language bindings.
 * Each language binding can define renames and supplemental code without
 * having to have a lot of conditionals in the main interface definition files.
 *
 */

%rename (GetFieldType) GetType;
%rename (GetDriverCount) OGRGetDriverCount;
%rename (GetOpenDSCount) OGRGetOpenDSCount;
%rename (SetGenerate_DB2_V72_BYTE_ORDER) OGRSetGenerate_DB2_V72_BYTE_ORDER;
%rename (RegisterAll) OGRRegisterAll();

%include typemaps_csharp.i

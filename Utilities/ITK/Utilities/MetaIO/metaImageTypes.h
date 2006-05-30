#ifndef METAIMAGETYPES_H
#define METAIMAGETYPES_H

typedef enum { MET_MOD_CT, MET_MOD_MR, MET_MOD_NM, MET_MOD_US, MET_MOD_OTHER,
       MET_MOD_UNKNOWN } MET_ImageModalityEnumType;

#define MET_NUM_IMAGE_MODALITY_TYPES 6

const char MET_ImageModalityTypeName[MET_NUM_IMAGE_MODALITY_TYPES][17] = {
   {'M','E','T','_','M','O','D','_','C','T','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','M','R','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','N','M','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','U','S','\0',' ',' ',' ',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','O','T','H','E','R','\0',' ',' ',' '},
   {'M','E','T','_','M','O','D','_','U','N','K','N','O','W','N','\0',' '}};

#endif

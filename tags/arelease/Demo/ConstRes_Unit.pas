unit ConstRes_Unit;

interface

const

  StringID = 100;
  MainCaption             = 1  + StringID;
  OTONodeStr              = 2  + StringID;
  TUNodeStr               = 3  + StringID;
  SkladNodeStr            = 4  + StringID;
  NomenkNodeStr           = 5  + StringID;
  PointNodeStr            = 6  + StringID;
  ConstNodeStr            = 7  + StringID;
  OTONewNameStr           = 8  + StringID;
  TUNewNameStr            = 9  + StringID;
  SkladNewNameStr         = 10 + StringID;
  NomenkNewNameStr        = 11 + StringID;
  AboutCaption            = 12 + StringID;
  AboutLabelCaption       = 13 + StringID;
  AboutNameLabelCaption1  = 14 + StringID;
  AboutNameLabelCaption2  = 15 + StringID;
  AboutLabelAddresCaption = 16 + StringID;
  AboutLabelTelCaption    = 17 + StringID;
  SaveAsQuery             = 18 + StringID;
  sCloseQuery             = 19 + StringID;
  WhatIsR1                = 20 + StringID;
  WhatIsR2                = 21 + StringID;
  WhatIsE1                = 22 + StringID;
  WhatIsE2                = 23 + StringID;
  DemoMessage             = 24 + StringID;
  BuildDemoMessage        = 25 + StringID;

  ErrorID = 30;
  ErrorCreateFMSClass           = 1  + ErrorID;
  ErrorLoadFMSFile              = 2  + ErrorID;
  ErrorSaveFMSFile              = 3  + ErrorID;
  ErrorFileOpen                 = 4  + ErrorID;
  ErrorSelectTU_OTO             = 5  + ErrorID;
  ErrorPointCoor                = 6  + ErrorID;
  ErrorPointRel                 = 7  + ErrorID;
  ErrorRunRdo                   = 8  + ErrorID;
  ErrorBuildModel               = 9  + ErrorID;
  ErrorCmdParams                = 10 + ErrorID;
  ErrorDecimalSeparator         = 11 + ErrorID;
  ErrorDecimalAndListSeparators = 12 + ErrorID;
  ErrorLoadResultDLL            = 13 + ErrorID;
  ErrorLoadBitmap               = 14 + ErrorID;
  ErrorSaveBitmap               = 15 + ErrorID;
  ErrorDrawBitmap               = 16 + ErrorID;
  ErrorMapScrollBoxResize       = 17 + ErrorID;
  ErrorFunction                 = 18 + ErrorID;
  ErrorCreateCompatibleDC       = 19 + ErrorID;
  ErrorCreateCompatibleBitmap   = 20 + ErrorID;
  ErrorClipboardCopy            = 21 + ErrorID;
  ErrorClearAll                 = 22 + ErrorID;
  ErrorShowFMSObject            = 23 + ErrorID;
  ErrorSelectTreeNode           = 24 + ErrorID;
  ErrorChangeFMSObject          = 25 + ErrorID;
  ErrorInputDoubleValue         = 26 + ErrorID;
  Error_mlLowTimeMinus          = 27 + ErrorID;
  Error_mlUniform               = 28 + ErrorID;
  Error_mlNormal                = 29 + ErrorID;
  ErrorWorkTimeMinus            = 30 + ErrorID;
  ErrorRunDemo                  = 31 + ErrorID;
  ErrorPlaybackError            = 32 + ErrorID;

  ID_Model = 0;
  RC_PAT = 1 + ID_Model;
  RC_RTP = 2 + ID_Model;
  RC_RSS = 3 + ID_Model;
  RC_OPR = 4 + ID_Model;
  RC_FRM = 5 + ID_Model;
  RC_FUN = 6 + ID_Model;
  RC_SMR = 7 + ID_Model;
  RC_PMD = 8 + ID_Model;

  RC_Demka = 101;

implementation

end.

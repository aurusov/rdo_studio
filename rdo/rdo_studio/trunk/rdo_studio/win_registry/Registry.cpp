/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
* 24-Jan-00 | Created
*  6-Feb-01 | [2.0.0.1223] REQ #278: close key
*****************************************************************************/
#include "stdafx.h"
#include "registry.h"
#include "shlwapi.h"
#include "resource.h"

/****************************************************************************
*                                   makeKey
* Inputs:
*	HKEY root: Root of key
*       const CString & path: Path of the form
*			a\b\c\d...
*	HKEY * key: Result of opening key
* Result: LONG
*       The result of ::RegOpenKey
* Effect: 
*       If the path cannot be opened, tries to back off creating the key
*	one level at a time
****************************************************************************/

static LONG makeKey(HKEY root, const CString & path, HKEY * key)
    {
     LONG result = ::RegOpenKey(root, path, key);
     if(result == ERROR_SUCCESS)
	return result;

     // We have a path of the form a\b\c\d  
     // But apparently a/b/c doesn't exist

     int i = path.ReverseFind(_T('\\'));
     if(i == -1)
	return result;  // well, we lose

     CString s;
     s = path.Left(i);

     HKEY newkey;
     result = makeKey(root, s, &newkey);
     if(result != ERROR_SUCCESS)
	return result;

     // OK, we now have created a\b\c
     CString v;
     v = path.Right(path.GetLength() - i - 1);
     DWORD disp;

     result = ::RegCreateKeyEx(newkey, 
     				v,
				0, NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				NULL,
				key,
				&disp);
     ::RegCloseKey(newkey); // no longer needed                     // REQ #278
     return result;
    }

/****************************************************************************
*                                  makePath
* Inputs:
*       CString & path: Existing path
*	const CString & var: Value or subpath
*	CString & name: Place to put name
* Result: void
*       
* Effect: 
*       Takes a path of the form
*		\this\that
*	and a variable name 'var' of the form
*		whatever\substring
*	and updates 'path' so that it is
*		\this\that\whatever
*	and updates 'name' so that it is
*		substring
****************************************************************************/

static void makePath(CString & path, const CString & var, CString & name)
    {
     // locate the rightmost \ of the 'var'.  If there isn't one,
     // we simply copy var to name...

     int i = var.ReverseFind(_T('\\'));
     if(i == -1)
        { /* no path */
	 name = var;
	 return;
	} /* no path */

     // append the prefix of the var to the path, leaving only the name
     if(path[path.GetLength() - 1] != _T('\\'))
	path += _T("\\");
     path += var.Left(i);
     name = var.Right(var.GetLength() - i - 1);
    }

/****************************************************************************
*                              GetRegistryString
* Inputs:
*	HKEY root: HKEY_CURRENT_USER, etc.
*       const CString &var: Name of variable
*	CString &value: place to put value
* Result: BOOL
*       TRUE if registry key found, &val is updated
*	FALSE if registry key not found, &val is not modified
* Effect: 
*       Retrieves the key based on 
*	root\IDS_PROGRAM_ROOT\var
* Notes:
*	This presumes the value is a text string (SZ_TEXT)
****************************************************************************/

BOOL GetRegistryString(HKEY root, const CString &var, CString & val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	{ /* not found */
	 ::SetLastError(result);
	 return FALSE;
	} /* not found */
     TCHAR buffer[256];
     DWORD len = sizeof(buffer)/sizeof(TCHAR);
     DWORD type;

     result = ::RegQueryValueEx(key, name, 0, &type, (LPBYTE)buffer, &len);
     ::RegCloseKey(key);

     if(result != ERROR_SUCCESS)
	{ /* error */
	 ::SetLastError(result);
	 return FALSE;
	} /* error */

     if(type != REG_SZ)
	{ /* wrong type */
	 ::SetLastError(ERROR_INVALID_DATA);
	 return FALSE;
	} /* wrong type */

     val = buffer;

     return TRUE;
    }

/****************************************************************************
*                              GetRegistryString
* Inputs:
*       HKEY root: Root key
*	const CString & path: Path value
*	const CString & var: Variable name
*	CString & val: Place to put value
* Result: BOOL
*       TRUE if successful
*	FALSE if error (::GetLastError gives error result)
* Effect: 
*       Reads the Registry value from the specified path/var
****************************************************************************/

BOOL GetRegistryString(HKEY root, const CString & path, const CString & var, CString & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    s += var;
    return GetRegistryString(root, s, val);
   } // GetRegistryString

/****************************************************************************
*                              GetRegistryString
* Inputs:
*       HKEY root: Root key
*	const CString & path: Path value
*	UINT var: Variable name, as string ID
*	CString & val: Place to put value
* Result: BOOL
*       TRUE if successful
*	FALSE if error (::GetLastError gives error result)
* Effect: 
*       Reads the Registry value from the specified path/var
****************************************************************************/

BOOL GetRegistryString(HKEY root, const CString & path, UINT var, CString & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    CString t;
    t.LoadString(var);
    s += t;
    return GetRegistryString(root, s, val);
   } // GetRegistryString

/****************************************************************************
*			      DeleteRegistryValue
* Inputs:
*       HKEY root: Root of path
*	const CString &var: Name of variable
* Result: BOOL
*       TRUE if item was deleted, or did not exist
*	FALSE if item was not deleted
* Effect: 
*       Deletes the variable named
****************************************************************************/

BOOL DeleteRegistryValue(HKEY root, const CString & var)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	return FALSE; // couldn't find it

     ::RegDeleteValue(key, name);
     ::RegCloseKey(key);
     return TRUE; // TODO: error detection
    }

/****************************************************************************
*                              DeleteRegistryKey
* Inputs:
*       HKEY root: Root of path
*	const CString & key: Key name to delete
* Result: BOOL
*       TRUE if successful, or the key was not found (postcondition: key 
*                                          will not exist!)
*	FALSE if error
* Effect: 
*       Deletes the key and all its subkeys
****************************************************************************/

BOOL DeleteRegistryKey(HKEY root, const CString & keyname)
   {
    CString path;
    path.LoadString(IDS_PROGRAM_ROOT);

    CString name;
    makePath(path, keyname, name);

    HKEY key;
    LONG result = ::RegOpenKey(root, path, &key);
    if(result == ERROR_FILE_NOT_FOUND)
       return TRUE; // couldn't find it, so it is deleted already
    if(result != ERROR_SUCCESS)
       return FALSE; 

    SHDeleteKey(key, name); // delete key and all subkeys
    return TRUE; // TODO: error detection
   } // DeleteRegistryKey

/****************************************************************************
*                               GetRegistryInt
* Inputs:
*	HKEY  root: root of path
*       const CString &var: Name of variable
*	DWORD &val: Place to put value
* Result: BOOL
*       TRUE if registry key found, &val is updated
*	FALSE if registry key not found, &val is not modified
* Effect: 
*       Retrieves the key based on 
*	root\IDS_PROGRAM_ROOT\var
* Notes:
*	This presumes the value is a 32-bit value
****************************************************************************/

BOOL GetRegistryInt(HKEY root, const CString &var, DWORD & val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	{ /* failed */
	 ::SetLastError(result);
	 return FALSE;
	} /* failed */

     DWORD buffer;
     DWORD len =  sizeof(buffer);
     DWORD type;

     result = ::RegQueryValueEx(key, name, 0, &type, (LPBYTE)&buffer, &len);
     ::RegCloseKey(key);

     if(result != ERROR_SUCCESS)
	{ /* error */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* error */

     if(type != REG_DWORD)
	{ /* wrong type */
	 ::SetLastError(ERROR_INVALID_DATA);
	 return FALSE;
	} /* wrong type */

     val = buffer;

     return TRUE;
    }

/****************************************************************************
*                               GetRegistryInt
* Inputs:
*	HKEY  root: root of path
*       const CString &path: path to variable
*	const CString &var: rest of path to variable (may be in path form)
*	DWORD &val: Place to put value
* Result: BOOL
*       TRUE if successful, value is changed
*	FALSE in unsuccessful, value is unchanged
****************************************************************************/

BOOL GetRegistryInt(HKEY root, const CString & path, const CString & var, DWORD & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    s += var;
    return GetRegistryInt(root, s, val);
   } // GetRegistryInt

/****************************************************************************
*                               GetRegistryInt
* Inputs:
*	HKEY  root: root of path
*       const CString &path: path to variable
*	UINT var: rest of path to variable expressed as a stringtable ID
*	DWORD &val: Place to put value
* Result: BOOL
*       TRUE if successful, value is changed
*	FALSE in unsuccessful, value is unchanged
****************************************************************************/

BOOL GetRegistryInt(HKEY root, const CString & path, UINT var, DWORD & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    CString t;
    t.LoadString(var);
    s += t;
    return GetRegistryInt(root, s, val);
   } // GetRegistryInt

/****************************************************************************
*                               GetRegistryInt64
* Inputs:
*	HKEY  root: root of path
*	const CString &var: rest of path to variable (may be in path form)
*	ULONGLONG &val: Place to put value
* Result: BOOL
*       TRUE if successful, value is changed
*	FALSE in unsuccessful, value is unchanged
****************************************************************************/

BOOL GetRegistryInt64(HKEY root, const CString & var, ULONGLONG & val)
   {
    CString path;
    path.LoadString(IDS_PROGRAM_ROOT);

    CString name;
    makePath(path, var, name);

    HKEY key;
    LONG result = ::RegOpenKey(root, path, &key);
    if(result != ERROR_SUCCESS)
       return FALSE;

    ULONGLONG buffer;
    DWORD len =  sizeof(buffer);
    DWORD type;

    result = ::RegQueryValueEx(key, name, 0, &type, (LPBYTE)&buffer, &len);
    ::RegCloseKey(key);

    if(result != ERROR_SUCCESS)
       { /* save error */
	::SetLastError((DWORD)result);
	return FALSE;
       } /* save error */

    if(type != REG_BINARY)
       { /* wrong type */
	::SetLastError(ERROR_INVALID_DATA);
	return FALSE;
       } /* wrong type */

    if(len != sizeof(ULONGLONG))
       { /* wrong length */
	::SetLastError(ERROR_INVALID_DATA);
	return FALSE;
       } /* wrong length */

    val = buffer;

    return TRUE;
   } // GetRegistryInt64

/****************************************************************************
*                            GetRegistryDWordArray
* Inputs:
*	HKEY  root: root of path
*       const CString &var: Name of variable
* Result: CDWordArray
*       The array of data
*	NULL if there is an error
* Effect: 
*       Allocates a DWORD array.  
* Notes:
*	The caller is responsible for deleting the result
****************************************************************************/

CDWordArray * GetRegistryDWordArray(HKEY root, const CString &var)
    {
     CString path;
     DWORD len;
     DWORD type;
     
     path.LoadString(IDS_PROGRAM_ROOT);
     CString name;
     makePath(path, var, name);
     
     HKEY key;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	{ /* failed */
	 ::SetLastError(result);
	 return NULL;
	} /* failed */

     result = ::RegQueryValueEx(key, name, 0, &type, NULL, &len);

     if(result != ERROR_SUCCESS)
        { /* failed */
	 ::SetLastError((DWORD)result);
	 ::RegCloseKey(key);
	 return NULL;
	} /* failed */

     CDWordArray * data = new CDWordArray;
     DWORD count = len / sizeof(DWORD);

     data->SetSize((int)count); // preallocate the array data

     result = ::RegQueryValueEx(key, name, 0, &type, (LPBYTE)&(*data)[0], &len);

     if(result != ERROR_SUCCESS)
        { /* failed */
	 ::RegCloseKey(key);
	 ::SetLastError((DWORD)result);
	 delete data;
	 return NULL;
	} /* failed */

     ::RegCloseKey(key);

     if(type != REG_BINARY)
	{ /* wrong type */
	 ::SetLastError(ERROR_INVALID_DATA);
	 delete data;
	 return NULL;
	} /* wrong type */

     return data;
    }

/****************************************************************************
*                            SetRegistryDWordArray
* Inputs:
*	HKEY  root: root of path
*       const CString &var: Name of variable
*	const CDWordArray & data: Data to write
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Writes the data for the key
****************************************************************************/

BOOL SetRegistryDWordArray(HKEY root, const CString & var, CDWordArray & data)
    {
     CString path;
     
     path.LoadString(IDS_PROGRAM_ROOT);
     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = makeKey(root,
     			        path,
			        &key);
     if(result != ERROR_SUCCESS)
	{ /* save it */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save it */
     
     result = ::RegSetValueEx(key, name, 0, REG_BINARY, (LPBYTE)&(data[0]), 
     				data.GetSize() * sizeof(DWORD));
     ::RegCloseKey(key);
     if(result != ERROR_SUCCESS)
	::SetLastError((DWORD)result);
     
     return result == ERROR_SUCCESS;

    }

/****************************************************************************
*                              SetRegistryString
* Inputs:
*	HKEY root: root of search
*       const CString & var: Name of variable
*	CString & val: Value to write
* Result: BOOL
*       TRUE if registry key set
*	FALSE if registry key not set
* Effect: 
*       Retrieves the key based on 
*	root\IDS_PROGRAM_ROOT\var
* Notes:
*	This presumes the value is a string
****************************************************************************/

BOOL SetRegistryString(HKEY root, const CString & var, const CString & val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);
     CString name;
     makePath(path, var, name);

     HKEY key;
     DWORD disp;
     LONG result = ::RegCreateKeyEx(root,
     				    path,
				    0, NULL, 
				    REG_OPTION_NON_VOLATILE,
				    KEY_ALL_ACCESS,
				    NULL,
				    &key,
				    &disp);
     if(result != ERROR_SUCCESS)
	{ /* save result */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save result */

     result = ::RegSetValueEx(key, name, 0, REG_SZ, (LPBYTE)(LPCTSTR)val, lstrlen(val) + 1);
     ::RegCloseKey(key);

     if(result != ERROR_SUCCESS)
	::SetLastError((DWORD)result);

     return result == ERROR_SUCCESS;
    }

/****************************************************************************
*                              SetRegistryString
* Inputs:
*       HKEY root: Root key
*	const CString & path: Path to variable
*	const CString & var: Variable name
*	const CString & val: Value to set
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Writes the Cstring value to the Registry
****************************************************************************/

BOOL SetRegistryString(HKEY root, const CString & path, const CString & var, const CString & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    s += var;
    return SetRegistryString(root, s, val);
   } // SetRegistryString

/****************************************************************************
*                              SetRegistryString
* Inputs:
*       HKEY root: Root key
*	const CString & path: Path to variable
*	const CString & var: Variable name
*	UINT val: Value to set, expressed as string table ID
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Writes the Cstring value to the Registry
****************************************************************************/

BOOL SetRegistryString(HKEY root, const CString & path, UINT var, const CString & val)
   {
    CString s;
    s = path;
    s += _T("\\");
    CString t;
    t.LoadString(var);
    s += t;
    return SetRegistryString(root, s, val);
   } // SetRegistryString

/****************************************************************************
*                              SetRegistryInt
*	HKEY root : root of search
*       const CString var: Name of variable, including possibly path info
*	DWORD val: Value to set
* Result: BOOL
*       TRUE if registry key set
*	FALSE if registry key not set
* Effect: 
*       Retrieves the key based on 
*	root\IDS_PROGRAM_ROOT\var
* Notes:
*	This presumes the value is a string
****************************************************************************/

BOOL SetRegistryInt(HKEY root, const CString & var, DWORD val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);
     CString name;

     makePath(path, var, name);

     HKEY key;
     DWORD disp;
     LONG result = ::RegCreateKeyEx(root,
     			            path,
				    0, NULL, 
				    REG_OPTION_NON_VOLATILE,
				    KEY_ALL_ACCESS,
				    NULL,
				    &key,
				    &disp);
     if(result != ERROR_SUCCESS)
	{ /* save error */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save error */

     result = ::RegSetValueEx(key, name, 0, REG_DWORD, (LPBYTE)&val, sizeof(DWORD));
     ::RegCloseKey(key);

     if(result != ERROR_SUCCESS)
	::SetLastError((DWORD)result);

     return result == ERROR_SUCCESS;
    }

/****************************************************************************
*                              SetRegistryInt64
*	HKEY root : root of search
*       const CString var: Name of variable, including possibly path info
*	ULONGLONG val: Value to set
* Result: BOOL
*       TRUE if registry key set
*	FALSE if registry key not set
* Effect: 
*       Retrieves the key based on 
*	root\IDS_PROGRAM_ROOT\var
* Notes:
*	This presumes the value is a string
****************************************************************************/

BOOL SetRegistryInt64(HKEY root, const CString & var, ULONGLONG val)
   {
    CString path;
    path.LoadString(IDS_PROGRAM_ROOT);
    CString name;

    makePath(path, var, name);

    HKEY key;
    DWORD disp;
    LONG result = ::RegCreateKeyEx(root,
				   path,
				   0, NULL, 
				   REG_OPTION_NON_VOLATILE,
				   KEY_ALL_ACCESS,
				   NULL,
				   &key,
				   &disp);
    if(result != ERROR_SUCCESS)
       { /* save error result */
	::SetLastError((DWORD)result);
	return FALSE;
       } /* save error result */

    result = ::RegSetValueEx(key, name, 0, REG_BINARY, (LPBYTE)&val, sizeof(ULONGLONG));
    ::RegCloseKey(key);

    if(result != ERROR_SUCCESS)
       ::SetLastError((DWORD)result);

    return result == ERROR_SUCCESS;
   }

/****************************************************************************
*                               SetRegistryInt
* Inputs:
*       HKEY root:
*	const CString & path: initial path to the variable
*	const CString & var: rest of the path to the variable
*	DWORD val: value to write
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Writes the data to the Registry
****************************************************************************/

BOOL SetRegistryInt(HKEY root, const CString & path, const CString & var, DWORD val)
   {
    CString s = path;
    s += _T("\\");
    s += var;
    return SetRegistryInt(root, s, val);
   } // SetRegistryInt

/****************************************************************************
*                               SetRegistryInt
* Inputs:
*       HKEY root:
*	const CString & path: initial path to the variable
*	UINT var: rest of the path to the variable, as string ID
*	DWORD val: value to write
* Result: BOOL
*       TRUE if successful
*	FALSE if error
* Effect: 
*       Writes the data to the Registry
****************************************************************************/

BOOL SetRegistryInt(HKEY root, const CString & path, UINT var, DWORD val)
   {
    CString s = path;
    s += _T("\\");
    CString t;
    t.LoadString(var);
    s += t;
    return SetRegistryInt(root, s, val);
   } // SetRegistryInt

/****************************************************************************
*                               SetRegistryGUID
* Inputs:
*       HKEY root:
*	const CString &var: Name to store it under
*	const GUID & val: Value to store
* Result: BOOL
*       TRUE if successful, FALSE if error
* Effect: 
*       Retrieves the key based on
*	root\IDS_PROGRAM_ROOT\var
****************************************************************************/

BOOL SetRegistryGUID(HKEY root, const CString & var, const GUID & val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);
     CString name;

     makePath(path, var, name);
     HKEY key;
     DWORD disp;
     LONG result = ::RegCreateKeyEx(root,
				    path,
				    0, NULL,
				    REG_OPTION_NON_VOLATILE,
				    KEY_ALL_ACCESS,
				    NULL,
				    &key,
				    &disp);
     if(result != ERROR_SUCCESS)
	{ /* save result */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save result */

     result = ::RegSetValueEx(key, name, 0, REG_BINARY, (LPBYTE)&val, sizeof(GUID));
     ::RegCloseKey(key);

     if(result != ERROR_SUCCESS)
	::SetLastError((DWORD)result);

     return result == ERROR_SUCCESS;
    } // SetRegistryGUID

/****************************************************************************
*                               GetRegistryGUID
* Inputs:
*       HKEY root: root of key
*	const CString & Var: Name to store it under
*	GUID & val: Place to store result
* Result: BOOL
*       TRUE if successful, FALSE if error
* Effect: 
*       Retrieves the key based on
*	root\IDS_PROGRAM_ROOT\var
****************************************************************************/

BOOL GetRegistryGUID(HKEY root, const CString & var, GUID & val)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	{ /* save result */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save result */

     DWORD type;
     DWORD len = sizeof(GUID);
     result = ::RegQueryValueEx(key, name, 0, &type, (LPBYTE)&val, &len);

     if(result != ERROR_SUCCESS)
	{ /* save it */
	 ::SetLastError((DWORD)result);
	 return FALSE;
	} /* save it */

     if(type != REG_BINARY)
	{ /* wrong type */
	 ::SetLastError(ERROR_INVALID_DATA);
	 return FALSE;
	} /* wrong type */

     if(len != sizeof(GUID))
	{ /* wrong length */
	 ::SetLastError(ERROR_INVALID_DATA);
	 return FALSE;
	} /* wrong length */

     return TRUE;
    } // GetRegistryGUID

/****************************************************************************
*                              EnumRegistryKeys
* Inputs:
*       HKEY root: Root of search
*	const CString & group: Name of group key
* Result: CStringArray *
*       Array of key names, NULL if no keys found
* Effect: 
*       Enumerates the keys
****************************************************************************/

CStringArray * EnumRegistryKeys(HKEY root, const CString & group)
    {
     CString path;

     CStringArray * keys;
     TCHAR itemName[512];

     path.LoadString(IDS_PROGRAM_ROOT);

     path += _T("\\");
     path += group;


     HKEY key;

     LONG result = makeKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	return NULL;

     keys = new CStringArray;
     DWORD i = 0;
     while(TRUE) //lint -e716
        { /* loop */
	 result = ::RegEnumKey(key, i, itemName, sizeof(itemName)/sizeof(TCHAR));
	 if(result != ERROR_SUCCESS)
	    break;
         // we have a valid key name
	 keys->SetAtGrow(i, itemName);
	 i++;
	} /* loop */

     ::RegCloseKey(key);
     return keys;
    }

/****************************************************************************
*                              EnumRegistryValues
* Inputs:
*       HKEY root: Root of search
*	const CString & group: Name of value group key
* Result: CStringArray *
*       Array of value names, NULL if no keys found
* Effect: 
*       Enumerates the keys
* Notes:
*	The caller is responsible for freeing the array
****************************************************************************/

CStringArray * EnumRegistryValues(HKEY root, const CString & group)
    {
     CString path;

     CStringArray * keys;
     TCHAR itemName[512];

     path.LoadString(IDS_PROGRAM_ROOT);

     path += _T("\\");
     path += group;


     HKEY key;

     LONG result = makeKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	return NULL;

     keys = new CStringArray;
     DWORD i = 0;
     while(TRUE)
        { /* loop */
	 DWORD length = sizeof(itemName)/sizeof(TCHAR);
	 result = ::RegEnumValue(key, // key selection
	 			      i,   // which key
				      itemName, // place to put value name
				      &length,  // in: length of buffer
				      	        // out: length of name
				      NULL, 	// reserved, NULL
				      NULL, 	// place to put type
				      NULL, 	// place to put value
				      NULL);	// place to put value length
	 if(result != ERROR_SUCCESS)
	    break;
         // we have a valid key name
	 keys->SetAtGrow(i, itemName);
	 i++;
	} /* loop */

     ::RegCloseKey(key);
     return keys;
    }

/****************************************************************************
*                               SetRegistryFont
* Inputs:
*	HKEY root: Root to write
*	const CString & var: Name of property
*       const LOGFONT * f: Logical font, or NULL to delete the font subkey
* Result: BOOL
*       TRUE if successful, FALSE if error
* Effect: 
*       Writes logical font information to the Registry
****************************************************************************/

BOOL SetRegistryFont(HKEY root, const CString & var, const LOGFONT * f)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, var, name);

     HKEY key;
     LONG result = makeKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	return FALSE;
     
     HKEY rkey;
     DWORD disp;
     if(f == NULL)
	{ /* delete key */
	 result = SHDeleteKey(key, name); // deletes key and all subkeys
	 ASSERT(result == ERROR_SUCCESS); // better error reporting later
	 ::RegCloseKey(key);
	 return TRUE;
	} /* delete key */

     result = ::RegCreateKeyEx(key,
			       name,
			       0, NULL, 
			       REG_OPTION_NON_VOLATILE,
			       KEY_ALL_ACCESS,
			       NULL,
			       &rkey,
			       &disp);
     if(result != ERROR_SUCCESS)
	return FALSE;

     // Note that we save only a subset of the LOGFONT information
     // Note also that we use constant names from the LOGFONT structure
     // so we don't load them from the resource strings

#define RegSetFontValue(field) ::RegSetValueEx(rkey, _T(#field), 0, \
				 sizeof(f->field) == sizeof(DWORD) ? REG_DWORD : REG_BINARY, \
				 (LPBYTE)&f->field, sizeof(f->field))

     VERIFY((result = RegSetFontValue(lfHeight))    == ERROR_SUCCESS);
     VERIFY((result = RegSetFontValue(lfWeight))    == ERROR_SUCCESS);
     VERIFY((result = RegSetFontValue(lfItalic))    == ERROR_SUCCESS);
     VERIFY((result = RegSetFontValue(lfUnderline)) == ERROR_SUCCESS);
     VERIFY((result = RegSetFontValue(lfStrikeOut)) == ERROR_SUCCESS);
     result = ::RegSetValueEx(rkey, _T("lfFaceName"), 0, REG_SZ, (LPBYTE)f->lfFaceName, lstrlen(f->lfFaceName) + 1);
     
     ::RegCloseKey(rkey);
     ::RegCloseKey(key);
     return TRUE; // assume success
    } // SetRegistryFont

/****************************************************************************
*                               GetRegistryFont
* Inputs:
*       HKEY root: Root key
*	const CString & keyname: Key name
*	LPLOGFONT f: Logfont structure to be loaded
* Result: BOOL
*       TRUE if successful
*	FALSE if erro
* Effect: 
*       Loads the font information. In addition, sets the other LOGFONT
*	values
****************************************************************************/

BOOL GetRegistryFont(HKEY root, const CString & keyname, LPLOGFONT f)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, keyname, name);

     HKEY key;
     DWORD type;
     LONG result = ::RegOpenKey(root, path, &key);
     if(result != ERROR_SUCCESS)
	return FALSE;

     HKEY rkey;
     result = ::RegOpenKeyEx(key,
			     name,
			     0, // reserved
			     KEY_READ,
			     &rkey);
     if(result != ERROR_SUCCESS)
	return FALSE;

     ::ZeroMemory(f, sizeof(LOGFONT));

     BOOL ok = FALSE;
     DWORD datalen;
     //----------------
     // lfHeight
     //----------------
     datalen = sizeof(f->lfHeight);
     result = RegQueryValueEx(rkey, _T("lfHeight"),
			      NULL, // reserved,
			      &type, //type buffer
			      (LPBYTE)&f->lfHeight,
			      &datalen);
     if(result == ERROR_SUCCESS && f->lfHeight != 0)
	ok = TRUE;
     else
	::SetLastError(result != ERROR_SUCCESS ? (DWORD)result : ERROR_INVALID_DATA);

     if(ok)
	{ /* check type */
	 if(type != REG_DWORD)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */
     //----------------
     // lfWeight
     //----------------
     datalen = sizeof(f->lfWeight);
     result = RegQueryValueEx(rkey, _T("lfWeight"),
			      NULL,  // reserved
			      &type, // type buffer
			      (LPBYTE)&f->lfWeight,
			      &datalen);
     if(result == ERROR_SUCCESS && f->lfWeight > 0)
	ok = TRUE;
     else
	::SetLastError(result != ERROR_SUCCESS ? (DWORD)result : ERROR_INVALID_DATA);

     if(ok)
	{ /* check type */
	 if(type != REG_DWORD)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */

     //----------------
     // lfItalic
     //----------------
     datalen = sizeof(f->lfItalic);
     result = RegQueryValueEx(rkey, _T("lfItalic"),
			      NULL,
			      &type,
			      (LPBYTE)&f->lfItalic,
			      &datalen);
     if(result == ERROR_SUCCESS)
	ok = TRUE;
     else
	::SetLastError((DWORD)result);

     if(ok)
	{ /* check type */
	 if(type != REG_DWORD)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */
     //----------------
     // lfUnderline
     //----------------
     datalen = sizeof(f->lfUnderline);
     result = RegQueryValueEx(rkey, _T("lfUnderline"),
			      NULL,
			      &type,
			      (LPBYTE)&f->lfUnderline,
			      &datalen);
     if(result == ERROR_SUCCESS)
	ok = TRUE;
     else
	::SetLastError((DWORD)result);

     if(ok)
	{ /* check type */
	 if(type != REG_DWORD)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */

     //----------------
     // lfStrikeout
     //----------------
     datalen = sizeof(f->lfStrikeOut);
     result = RegQueryValueEx(rkey, _T("lfStrikeOut"),
			      NULL,
			      &type,
			      (LPBYTE)&f->lfStrikeOut,
			      &datalen);
     if(result == ERROR_SUCCESS)
	ok = TRUE;
     else
	::SetLastError((DWORD)result);

     if(ok)
	{ /* check type */
	 if(type != REG_DWORD)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */

     //----------------
     // lfFaceName
     //----------------
     datalen = sizeof(f->lfFaceName);
     result = RegQueryValueEx(rkey, _T("lfFaceName"),
			      NULL,
			      &type,
			      (LPBYTE)f->lfFaceName,
			      &datalen);
     if(result == ERROR_SUCCESS)
	ok = TRUE;
     else
	::SetLastError((DWORD)result);

     if(ok)
	{ /* check type */
	 if(type != REG_SZ)
	    { /* failed type */
	     ::SetLastError(ERROR_INVALID_DATA);
	    } /* failed type */	     
	} /* check type */

     ::RegCloseKey(rkey);
     ::RegCloseKey(key);
     
     return ok;
    } // GetRegistryFont

/****************************************************************************
*                               GetRegistryKey
* Inputs:
*       HKEY root: Root of key
*	const CString & name: Name of key to open
*	HKEY &key: Place to put open key
* Result: BOOL
*       TRUE if key successfully found, key value is valid
*	FALSE if key not found key value undefined
* Effect: 
*       Opens a registry key based on the standard path. It is the
*	responsibility of the caller to close this key.
*	If the key does not exist, the path to it is created
* Notes:
*	This is used when it is necessary to iterate through subkeys
****************************************************************************/

BOOL GetRegistryKey(HKEY root, const CString & keyname, HKEY & key)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, keyname, name);
     path += _T("\\");
     path += name;
     LONG result = makeKey(root, path, &key);
    
     return result == ERROR_SUCCESS;
    } // GetRegistryKey

/****************************************************************************
*                               FindRegistryKey
* Inputs:
*       HKEY root: Root of key
*	const CString & name: Name of key to open
*	HKEY &key: Place to put open key
* Result: BOOL
*       TRUE if the path exists; key is the open key to that path
*	FALSE if the path does not exist; key is unmodified
* Effect: 
*       Unlike GetRegistryKey, this does not attempt to create the key
****************************************************************************/

BOOL FindRegistryKey(HKEY root, const CString & keyname, HKEY & key)
    {
     CString path;
     path.LoadString(IDS_PROGRAM_ROOT);

     CString name;
     makePath(path, keyname, name);

     path += _T("\\");
     path += name;

     LONG result = ::RegOpenKey(root, path, &key);
     return (result == ERROR_SUCCESS);
    } // FindRegistryKey

/****************************************************************************
*                         GetRegistryWindowPlacement
* Inputs:
*       HKEY root:
*	const CString & var: base key
*	WINDOWPLACEMENT * wp: Window placement object
* Result: BOOL
*       TRUE if successful; wp is modified, and the length field is also set
*	FALSE if unsuccessful. wp is unchanged
* Effect: 
*       Reads the WindowPlacement from
*		root\path\var\WindowPlacement\...
****************************************************************************/

#ifdef IDS_KEY_PLACEMENT_FLAGS
BOOL GetRegistryWindowPlacement(HKEY root, const CString & var, WINDOWPLACEMENT * wp)
    {
     ASSERT(wp != NULL);
     BOOL ok = TRUE;
     DWORD val;
     //================
     // length
     // flags
     //================
     if(!GetRegistryInt(root, var, IDS_KEY_PLACEMENT_FLAGS, val))
	return FALSE; // no data

     wp->length = sizeof(WINDOWPLACEMENT);
     wp->flags = val;

     //================
     // showCmd
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_SHOWCMD, val);
     wp->showCmd = val;
		
     //================
     // ptMinPosition.x
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_MINPOSITION_X, val);
     wp->ptMinPosition.x = val;
     
     //================
     // ptMinMposition.y
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_MINPOSITION_Y, val);
     wp->ptMinPosition.y = val;

     //================
     // ptMaxPosition.x
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_MAXPOSITION_X, val);
     wp->ptMaxPosition.x = val;

     //================
     // ptMaxMposition.y
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_MAXPOSITION_Y, val);
     wp->ptMaxPosition.y = val;

     //================
     // rcNormalPosition.left
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_NORMAL_LEFT, val);
     wp->rcNormalPosition.left = val;

     //================
     // rcNormalPosition.top
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_NORMAL_TOP, val);
     wp->rcNormalPosition.top = val;

     //================
     // rcNormalPosition.right
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_NORMAL_RIGHT, val);
     wp->rcNormalPosition.right = val;
     
     //================
     // rcNormalPosition.bottom
     //================
     ok &= GetRegistryInt(root, var, IDS_KEY_PLACEMENT_NORMAL_BOTTOM, val);
     wp->rcNormalPosition.bottom = val;

     return ok;
    } // GetRegistryWindowPlacement
#endif

/****************************************************************************
*                         SetRegistryWindowPlacement
* Inputs:
*       HKEY root:
*	const CString & var: base key
*	const WINDOWPLACEMENT * wp: Window placement object
* Result: BOOL
*       TRUE if successful; wp is modified, and the length field is also set
*	FALSE if unsuccessful. wp is unchanged
* Effect: 
*       Writes the WindowPlacement from
*		root\path\var\WindowPlacement\...
****************************************************************************/

#ifdef IDS_KEY_PLACEMENT_FLAGS
BOOL SetRegistryWindowPlacement(HKEY root, const CString & var, const WINDOWPLACEMENT * wp)
    {
     ASSERT(wp != NULL);
     BOOL ok = TRUE;
     CString s;
     s.LoadString(IDS_KEY_PLACEMENT_FLAGS);
     //================
     // length
     // flags
     //================
     ok &= SetRegistryInt(root, var, s, wp->flags);

     //================
     // showCmd
     //================
     s.LoadString(IDS_KEY_PLACEMENT_SHOWCMD);
     ok &= SetRegistryInt(root, var, s, wp->showCmd);

     //================
     // ptMinPosition.x
     //================
     s.LoadString(IDS_KEY_PLACEMENT_MINPOSITION_X);
     ok &= SetRegistryInt(root, var, s, wp->ptMinPosition.x);

     //================
     // ptMinMposition.y
     //================
     s.LoadString(IDS_KEY_PLACEMENT_MINPOSITION_Y);
     ok &= SetRegistryInt(root, var, s, wp->ptMinPosition.y);

     //================
     // ptMaxPosition.x
     //================
     s.LoadString(IDS_KEY_PLACEMENT_MAXPOSITION_X);
     ok &= SetRegistryInt(root, var, s, wp->ptMaxPosition.x);

     //================
     // ptMaxMposition.y
     //================
     s.LoadString(IDS_KEY_PLACEMENT_MAXPOSITION_Y);
     ok &= SetRegistryInt(root, var, s, wp->ptMaxPosition.y);

     //================
     // rcNormalPosition.left
     //================
     s.LoadString(IDS_KEY_PLACEMENT_NORMAL_LEFT);
     ok &= SetRegistryInt(root, var, s, wp->rcNormalPosition.left);

     //================
     // rcNormalPosition.top
     //================
     s.LoadString(IDS_KEY_PLACEMENT_NORMAL_TOP);
     ok &= SetRegistryInt(root, var, s, wp->rcNormalPosition.top);

     //================
     // rcNormalPosition.right
     //================
     s.LoadString(IDS_KEY_PLACEMENT_NORMAL_RIGHT);
     ok &= SetRegistryInt(root, var, s, wp->rcNormalPosition.right);

     //================
     // rcNormalPosition.bottom
     //================
     s.LoadString(IDS_KEY_PLACEMENT_NORMAL_BOTTOM);
     ok &= SetRegistryInt(root, var, s, wp->rcNormalPosition.bottom);

     return TRUE;
    } // SetRegistryWindowPlacement
#endif

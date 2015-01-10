#include "XUtilsFile.h"
#include "../base/macros.h"
#include "../base/macros.h"
#include "../base/global.h"
#include "../utils/XUtilsFormatter.h"
#include "../utils/XUtilsFormatter.h"

//#define ACCESS _access
#define MKDIR(a) _mkdir((a))

#include <boost/unordered_map.hpp>
#include <windows.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <sstream>

#define X_MAX_PATH 512

boost::unordered_map<std::string, std::string> XUtilsFile::_fullPathCache;
boost::unordered_map<std::string, std::string> XUtilsFile::_filenameLookupDict;
std::vector<std::string> XUtilsFile::_searchPathArray;

// The root path of resources, the character encoding is UTF-8.
// UTF-8 is the only encoding supported by cocos2d-x API.
static std::string s_resourcePath = "";

std::string XUtilsFile::_defaultResRootPath = "";

#ifdef WIN32

BOOL WStringToString(const std::wstring &wstr, std::string &str)
{
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');

	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);

	if (nResult == 0)
	{
		return FALSE;
	}

	return TRUE;
}

#endif

// D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
static inline std::string convertPathFormatToUnixStyle(const std::string& path)
{
	std::string ret = path;
	int len = ret.length();
	for (int i = 0; i < len; ++i)
	{
		if (ret[i] == '\\')
		{
			ret[i] = '/';
		}
	}
	return ret;
}

static inline std::string _checkPath(std::string path)
{
	std::string filePath = "";
	if(!XUtilsFile::isAbsolutePath(path))
	{
		filePath = s_resourcePath+path;
	}
	else
	{
		filePath = path;
	}
	return filePath;
}

std::string XUtilsFile::getAbsolutePath(std::string path)
{
	if(isAbsolutePath(path))
	{
		return path;
	}
	else
	{
		return _checkPath(path);
	}
}

void XUtilsFile::init()
{
	if (0 == s_resourcePath.length())
	{
		//WCHAR utf16Path[X_MAX_PATH] = { 0 };
		//GetCurrentDirectoryW(sizeof(utf16Path)-1, utf16Path);

		//char utf8Path[X_MAX_PATH] = { 0 };
		//int nNum = WideCharToMultiByte(CP_UTF8, 0, utf16Path, -1, utf8Path, sizeof(utf8Path), NULL, NULL);

		s_resourcePath = XUtilsFile::getWorkPath();
	}
	_defaultResRootPath = s_resourcePath;
	_searchPathArray.push_back(_defaultResRootPath);
}

/************************************************************************/
/* 查看文件是否存在                                                     */
/************************************************************************/
bool XUtilsFile::isFileExistInternal(const std::string& strFilePath)
{
	if (0 == strFilePath.length())
	{
		return false;
	}

	std::string strPath = strFilePath;
	if (!XUtilsFile::isAbsolutePath(strPath))
	{
		std::string defaultResRootPath = "";
		//WCHAR utf16Path[X_MAX_PATH] = { 0 };
		//GetCurrentDirectory(sizeof(utf16Path)-1, utf16Path);
		//char utf8Path[X_MAX_PATH] = { 0 };
		//int nNum = WideCharToMultiByte(CP_UTF8, 0, utf16Path, -1, utf8Path, sizeof(utf8Path), NULL, NULL);
		std::string workPath = getWorkPath();
		defaultResRootPath = workPath;//convertPathFormatToUnixStyle(workPath);
		// Not absolute path, add the default root path at the beginning.
		strPath.insert(0, defaultResRootPath);
	}

	WCHAR utf16Buf[X_MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_UTF8, 0, strPath.c_str(), -1, utf16Buf, sizeof(utf16Buf) / sizeof(utf16Buf[0]));

	DWORD attr = GetFileAttributesW(utf16Buf);

	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
		return false;   //  not a file
	return true;
}

bool XUtilsFile::isDirectoryExistInternal(const std::string& dirPath)
{
	
	unsigned long fAttrib = GetFileAttributes(XUtilsFormatter::MB2WC(dirPath.c_str()).c_str());
	if (fAttrib != INVALID_FILE_ATTRIBUTES &&
		(fAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	return false;

}

std::string XUtilsFile::getNewFilename(const std::string &filename)
{
	std::string newFileName;

	// in Lookup Filename dictionary ?
	boost::unordered_map<std::string, std::string>::iterator iter = _filenameLookupDict.find(filename);

	if (iter == _filenameLookupDict.end())
	{
		newFileName = filename;
	}
	else
	{
		newFileName = iter->second;
	}
	return newFileName;
}


std::string XUtilsFile::getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename)
{
	// get directory+filename, safely adding '/' as necessary 
	std::string ret = directory;
	if (directory.size() && directory[directory.size() - 1] != '\\'){
		ret += '\\';
	}
	ret += filename;

	// if the file doesn't exist, return an empty string
	if (!isFileExistInternal(ret)) {
		ret = "";
	}
	return ret;
}

std::string XUtilsFile::getPathForFilename(const std::string& filename, const std::string& searchPath)
{
	std::string file = filename;
	std::string file_path = "";
	size_t pos = filename.find_last_of("/");
	if (pos != std::string::npos)
	{
		file_path = filename.substr(0, pos + 1);
		file = filename.substr(pos + 1);
	}

	// searchPath + file_path + resourceDirectory
	std::string path = searchPath;
	path += file_path;

	path = getFullPathForDirectoryAndFilename(path, file);

	//CCLOG("getPathForFilename, fullPath = %s", path.c_str());
	return path;
}

std::string XUtilsFile::fullPathForFilename(const std::string &filename)
{
	if (filename.empty())
	{
		return "";
	}

	if (isAbsolutePath(filename))
	{
		return filename;
	}

	// Already Cached ?
	boost::unordered_map<std::string, std::string>::iterator cacheIter = _fullPathCache.find(filename);
	if (cacheIter != _fullPathCache.end())
	{
		return cacheIter->second;
	}

	// Get the new file name.
	const std::string newFilename(getNewFilename(filename));

	std::string fullpath;

	for (std::vector<std::string>::iterator searchIt = _searchPathArray.begin(); searchIt != _searchPathArray.end(); ++searchIt)
	{
		fullpath = getPathForFilename(newFilename, *searchIt);

		if (fullpath.length() > 0)
		{
			// Using the filename passed in as key.
			_fullPathCache.insert(std::make_pair(filename, fullpath));
			return fullpath;
		}
	}

	//if (isPopupNotify()){
	//	CCLOG("cocos2d: fullPathForFilename: No file found at %s. Possible missing file.", filename.c_str());
	//}

	// FIXME: Should it return nullptr ? or an empty string ?
	// The file wasn't found, return the file name passed in.
	return filename;
}

bool XUtilsFile::isAbsolutePath(const std::string& path)
{
	if (path.length() > 2 && (path[0]=='\\'))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XUtilsFile::isFileExist(const std::string& filename)
{
	return isFileExistInternal(filename);
}

bool XUtilsFile::renameFile(const std::string &oldname, const std::string &name, const std::string &path)
{
	std::string oldPath = _checkPath(path + oldname);
	std::string newPath = _checkPath(path + name);

	if(XUtilsFile::isFileExist(newPath))
	{
		XUtilsFile::deleteFile(newPath);
	}

	// Rename a file
	int errorCode = MoveFile( (XUtilsFormatter::UT2WC(oldPath.c_str())).c_str(),  (XUtilsFormatter::UT2WC(newPath.c_str())).c_str());

	if (0 == errorCode)
	{
		XLOGP("Fail to rename file %s to %s !Error code is %d", oldPath.c_str(), newPath.c_str(), errorCode);
		int err = GetLastError();
		return false;
	}
	else 
	{
		return true;
	}
	return false;
}

bool XUtilsFile::deleteFile(const std::string &path)
{
	std::string filePath = _checkPath(path);
	if (!filePath.empty())
	{
		std::wstring wStr = XUtilsFormatter::UT2WC(filePath.c_str());
		if (DeleteFile(wStr.c_str()))
		{
			return true;
		}
		else 
		{
			int err = GetLastError();
			return false;
		}
	}
	return false;
}

bool XUtilsFile::deleteDirectory(const std::string& strPath)
{
	TSTRING strDirPath = UT2WC_STR(strPath);  
#ifdef _WIN32_WCE  
    if(strDirPath[0] != TEXT('\\'))  
    {  
        return false;  
    }  
#endif  
    if(strDirPath[strDirPath.size() - 1] == TEXT('\\'))  
    {  
        strDirPath += TEXT("*.*");  
    }  
    else  
    {  
        strDirPath += TEXT("\\*.*");  
    }  
  
    WIN32_FIND_DATA fd;  
    HANDLE hdFind;  
    hdFind = FindFirstFile(strDirPath.c_str(),&fd);  
    if(hdFind != INVALID_HANDLE_VALUE)  
    {  
        do{  
            //At winXP system the "." means current directory, the ".."means parent directory.  
            if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))  
            {         
                //It is directory     
				TSTRING strNextDir = UT2WC_STR(strPath);  
                if(strNextDir[strNextDir.size() -1] != TEXT('\\'))  
                    strNextDir += TEXT("\\");  
  
                strNextDir += fd.cFileName;   
                deleteDirectory(WC2UT_STR(strNextDir));  
                RemoveDirectory(strNextDir.c_str());  
            }  
            else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  
            {     
                //It is file   
                TSTRING strPathFile = UT2WC_STR(strPath);  
                if(strPathFile[strPathFile.size() - 1] !=TEXT( '\\'))  
                    strPathFile += TEXT("\\");  
  
                strPathFile += fd.cFileName;  
                DeleteFile(strPathFile.c_str());  
            }  
        }while(FindNextFile(hdFind,&fd));  
    }  
    FindClose(hdFind);  
  
    return RemoveDirectory(UT2WC_CHAR(strPath))!=0;  
}

std::string XUtilsFile::getFileData(const std::string& filename, const char* mode)
{
    XASSERT(!filename.empty() && mode != NULL, "Invalid parameters.");
	std::string filePath = _checkPath(filename);
	std::string data = "";
	std::ifstream ifs(filePath.c_str());
	if (ifs)
	{
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		data.assign(buffer.str());
	}
	else
	{
		XLOGP("%s file can not open",filename.c_str());
	}
	return data;
}

size_t XUtilsFile::getFileSize(std::string filename)
{
	FILE * pFile;
	size_t size;

	pFile = fopen(filename.c_str(), "rb");
	if (pFile == NULL)
		XLOG("Error opening file");
	else
	{
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		fclose(pFile);
		return size;
	}
	return 0;
}

bool XUtilsFile::writeFileData(const std::string& fileName,const std::string& data)
{
	std::string filePath = _checkPath(fileName);
	std::ofstream of(filePath.c_str());
	if (of.good())
	{
		of << data;
		of.close();
		return true;
	}
	else
	{
		XASSERT(0,filePath+" error in XUtilsFile::writeFileData");
	}
	return false;
}

std::vector<std::string> XUtilsFile::getFilesInDir(const std::string dir)
{
	
	std::vector<std::string> allFilePath;

	WIN32_FIND_DATA findFileData;
	
	std::string rootDir = "";
	rootDir.assign(dir);
	rootDir = _checkPath(rootDir);
	std::string findStr = rootDir;
	HANDLE hFind = FindFirstFile(XUtilsFormatter::UT2WC(findStr.c_str()).c_str(), &findFileData);
	if (rootDir.find_last_of("*") != std::string::npos)
	{
		rootDir = rootDir.substr(0, rootDir.size() - 1);
	}
	if (rootDir.find_last_of("\\") != std::string::npos)
	{
		rootDir = rootDir.substr(0, rootDir.size() - 1);
	}
	std::string str = "";
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			str = XUtilsFormatter::WC2UT(findFileData.cFileName);
			if (FILE_ATTRIBUTE_DIRECTORY & findFileData.dwFileAttributes)
			{
				if (str.compare("..") == 0 || str.compare(".") == 0)
				{
					continue;
				}
				else
				{
					std::vector<std::string> bunchV = XUtilsFile::getFilesInDir(rootDir + "\\" + str + "\\*");
					allFilePath.insert(allFilePath.end(), bunchV.begin(), bunchV.end());
				}
			}
			else
			{
				allFilePath.push_back(rootDir + "\\" + str);
			}
			
			
		}while (::FindNextFile(hFind, &findFileData));
		::FindClose(hFind);
	}
	return allFilePath;
}

std::string XUtilsFile::formatPath(const std::string& path)
{
	std::string returnPath = convertPathFormatToUnixStyle(path);
#ifdef WIN32
	
	if (path.find_first_of(":") != 1 && path.find_first_of(".\/") != 0 && path.find_first_of("file:") != 0)
	{
		returnPath.insert(0, ".\/");
	}

#endif
	return returnPath;
}

void XUtilsFile::checkDirAndCreate(std::string path)
{
	std::string formatPath = convertPathFormatToUnixStyle(path);
	char *fileName = const_cast<char*>(formatPath.c_str()), *tag;
	HANDLE hFile;
	hFile = CreateFile (TEXT("\\ONE.TXT"),      // Open One.txt
		GENERIC_READ,           // Open for reading
		0,                      // Do not share
		NULL,                   // No security
		OPEN_EXISTING,          // Existing file only
		FILE_ATTRIBUTE_NORMAL,  // Normal file
		NULL);                  // No template file
	for (tag = fileName; *tag; tag++)
	{
		if (*tag == '\/')
		{
			char buf[1000], path[1000];
			strcpy(buf, fileName);
			buf[strlen(fileName) - strlen(tag) + 1] = NULL;
			strcpy(path, buf);
			XUtilsFile::checkFolderExistOrNot(path);
		}
	}
}
BOOL XUtilsFile::checkFolderExistOrNot(std::string strFolderPath)
{
	std::wstring str = XUtilsFormatter::UT2WC(strFolderPath.c_str());
	DWORD FileAttributeValue = ::GetFileAttributes(str.c_str());
	if(FileAttributeValue == 0xFFFFFFFF)
	{
		CreateDirectory(str.c_str(),NULL);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

std::string XUtilsFile::getRelativePath(std::string fileName)
{
	return getWorkPath()+fileName;
}

std::string XUtilsFile::getWorkPath()
{
	std::string returnPath = "";
	TCHAR szPath[X_MAX_PATH];
	int nSize = X_MAX_PATH;
	GetModuleFileName( NULL, szPath, nSize );
	TCHAR *p = wcsrchr(szPath, '\\');
	*p = '\\';
	++p;
	*p = '\0';
	returnPath = XUtilsFormatter::WC2UT(szPath);


	return returnPath;
}

bool XUtilsFile::checkDirectoryExist(const std::string& strPath)  
{  
	BOOL bReturn = FALSE;  
	if(strPath.size() >= MAX_PATH)  
	{  
		return FALSE;  
	}  
	WIN32_FIND_DATA fd;  
	HANDLE hdFind = FindFirstFile(XUtilsFormatter::UT2WC(strPath).c_str(),&fd);  
	if(hdFind != INVALID_HANDLE_VALUE)  
	{  
		if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)  
		{  
			bReturn = TRUE;  
		}  
	}  
	FindClose(hdFind);  
	return bReturn;  
}  

bool XUtilsFile::copyDirectory(std::string& srcPath, std::string& destPath)
{
	std::string strDirPath = srcPath;  
    BOOL bResult = TRUE;  
#ifdef _WIN32_WCE  
    if(strDirPath[0] != '\\')  
    {  
        return FALSE;  
    }  
#endif  
    if(strDirPath[strDirPath.size() - 1] == '\\')  
    {  
        strDirPath += "*.*";  
    }  
    else  
    {  
        strDirPath += "\\*.*";  
    }  
  
    WIN32_FIND_DATA fd;  
    HANDLE hdFind;  
	hdFind = FindFirstFile(XUtilsFormatter::UT2WC(strDirPath).c_str(),&fd);  
    if(hdFind != INVALID_HANDLE_VALUE)  
    {  
        do{  
            if(!checkDirectoryExist(destPath))  
            {  
                CreateDirectory(UT2WC_CHAR(destPath),NULL);  
  
            }  
  
            //At winXP system the "." means current directory, the ".."means parent directory.  
            if(fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != TEXT('.'))  
            {         
                //It is directory     
                std::wstring strSourceNextDir = UT2WC_STR(srcPath);  
                std::wstring strDestinationNextDir = UT2WC_STR(destPath);  
  
                if(strSourceNextDir[strSourceNextDir.size() -1] != TEXT('\\'))  
                {  
                    strSourceNextDir += TEXT("\\");  
                }  
  
                if(strDestinationNextDir[strDestinationNextDir.size() -1] != TEXT('\\'))  
                {  
                    strDestinationNextDir += TEXT("\\");  
                }  
  
                strSourceNextDir += fd.cFileName;  
                strDestinationNextDir += fd.cFileName;  
  
                CreateDirectory(strSourceNextDir.c_str(),NULL);  
  
                copyDirectory(WC2UT_STR(strSourceNextDir.c_str()),WC2UT_STR(strDestinationNextDir.c_str()));
            }  
            else if(fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)  
            {     
                //It is file   
                std::wstring strSourceFilePath = UT2WC_STR(srcPath);  
                std::wstring strDestinationFilePath = UT2WC_STR(destPath);  
  
                if(strSourceFilePath[strSourceFilePath.size() - 1] != TEXT('\\'))  
                {  
                    strSourceFilePath += TEXT("\\");  
                }  
  
                if(strDestinationFilePath[strDestinationFilePath.size() - 1] != TEXT('\\'))  
                {  
                    strDestinationFilePath += TEXT("\\");  
                }  
  
                strSourceFilePath += fd.cFileName;  
                strDestinationFilePath += fd.cFileName;  
  
                //The file is read-only ,cancle the attributes of read-only  
                if(GetFileAttributes(strSourceFilePath.c_str()) & FILE_ATTRIBUTE_READONLY)   
                    SetFileAttributes(strSourceFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);   
  
                if(GetFileAttributes(strDestinationFilePath.c_str()) &FILE_ATTRIBUTE_READONLY)   
                    SetFileAttributes(strDestinationFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);  
  
				if(CopyFile(strSourceFilePath.c_str(),strDestinationFilePath.c_str(),FALSE)==FALSE)  
                {     
#ifdef _WIN32_WCE  
                    ASSERT(FALSE);  
#else   
                    std::string strInfo;  
                    strInfo = TEXT("Copy File/" ");  
                    strInfo += strSourceFilePath;  
                    strInfo += TEXT("/"  Failed !");  
                    OutputDebugString(strInfo.c_str());  
#endif  
                    return FALSE;  
                }  
            }  
        }while(FindNextFile(hdFind,&fd));  
    }  
    else  
    {  
        bResult = FALSE;  
    }  
    FindClose(hdFind);  
    return bResult;  
}

bool XUtilsFile::moveFile(std::string& srcPath, std::string& destPath)
{
	XUtilsFile::copyDirectory(srcPath, destPath);
	XUtilsFile::deleteDirectory(srcPath);
	return true;
}

XUtilsFile::XUtilsFile()
{
}

XUtilsFile::~XUtilsFile()
{
}

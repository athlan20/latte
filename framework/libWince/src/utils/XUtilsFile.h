#ifndef __XUtilsFile_H__
#define __XUtilsFile_H__

#include "../base/macros.h"
#include <boost/function.hpp>
#include <boost/unordered_map.hpp>
#include <string>
#include <vector>


class XUtilsFile
{
public:
	X_DLL XUtilsFile();
	X_DLL ~XUtilsFile();

public:
	/**
	 * 初始化
	 * @param
	 * @return
	 */
	X_DLL static void init();
	/**
	*  Checks whether a file exists without considering search paths and resolution orders.
	*  @param The file (with absolute path) to look up for
	*  @return Returns true if the file found at the given absolute path, otherwise returns false
	*/
	X_DLL static bool isFileExistInternal(const std::string& filename);

	/**
	*  Checks whether the path is an absolute path.
	*
	*  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
	*        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
	*
	*  @param strPath The path that needs to be checked.
	*  @return true if it's an absolute path, otherwise it will return false.
	*/
	X_DLL static bool isAbsolutePath(const std::string& path);

	/**
	*  Checks whether a file exists.
	*
	*  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
	*  @param strFilePath The path of the file, it could be a relative or absolute path.
	*  @return true if the file exists, otherwise it will return false.
	*/
	X_DLL static bool isFileExist(const std::string& filename);

	X_DLL static std::string fullPathForFilename(const std::string &filename);

	/**
	*  Gets the new filename from the filename lookup dictionary.
	*  It is possible to have a override names.
	*  @param filename The original filename.
	*  @return The new filename after searching in the filename lookup dictionary.
	*          If the original filename wasn't in the dictionary, it will return the original filename.
	*/
	X_DLL static std::string XUtilsFile::getNewFilename(const std::string &filename);

	/**
	*  Gets full path for filename, resolution directory and search path.
	*
	*  @param filename The file name.
	*  @param resolutionDirectory The resolution directory.
	*  @param searchPath The search path.
	*  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
	*/
	X_DLL static std::string getPathForFilename(const std::string& filename, const std::string& searchPath);

	/**
	*  Gets full path for the directory and the filename.
	*
	*  @note Only iOS and Mac need to override this method since they are using
	*        `[[NSBundle mainBundle] pathForResource: ofType: inDirectory:]` to make a full path.
	*        Other platforms will use the default implementation of this method.
	*  @param strDirectory The directory contains the file we are looking for.
	*  @param strFilename  The name of the file.
	*  @return The full path of the file, if the file can't be found, it will return an empty string.
	*/
	X_DLL static std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename);

	/**
	*  Gets resource file data
	*
	*  @param[in]  filename The resource file name which contains the path.
	*  @param[in]  pszMode The read mode of the file.
	*/
	X_DLL static std::string getFileData(const std::string& filename, const char* mode = "r");
	X_DLL static size_t getFileSize(std::string filename);
	/**
	 * 判断是否是存在的目录
	 * @param
	 * @return
	 */
	X_DLL static bool isDirectoryExistInternal(const std::string& dirPath);
	
	/**
	 * 递归式访问文件夹里的所有文件
	 * @param
	 * @return
	 */
	X_DLL static std::vector<std::string> getFilesInDir(const std::string dir="*");

	/**
	 * 
	 * @param
	 * @return
	 */
	X_DLL static bool writeFileData(const std::string& fileName, const std::string& data);

	X_DLL static bool renameFile(const std::string &oldname, const std::string &name, const std::string &path = "");

	/**
	* 格式化路径字符串，使之变为合法路径表示方法
	*
	*/
	X_DLL static std::string formatPath(const std::string& path);
	
	/**
	 * 获取绝对路径
	 * @param
	 * @return
	 */
	X_DLL static std::string getAbsolutePath(std::string path);

	//检查路径目录是否存在，没有的话，就一级级创建好目录
	X_DLL static void checkDirAndCreate(std::string path);
	X_DLL static BOOL checkFolderExistOrNot(std::string path);
	/**
	 * 删除文件
	 * @param
	 * @return
	 */
	X_DLL static bool deleteFile(const std::string &path);
	X_DLL static bool deleteDirectory(const std::string &path);

	X_DLL static std::string getRelativePath(std::string fileName);
	X_DLL static std::string getWorkPath();
	X_DLL bool static XUtilsFile::checkDirectoryExist(const std::string& strPath);
	X_DLL bool static XUtilsFile::copyDirectory(std::string& srcPath, std::string& destPath);
	X_DLL bool static XUtilsFile::moveFile(std::string& srcPath, std::string& destPath);
private:
	/**
	*  The full path cache. When a file is found, it will be added into this cache.
	*  This variable is used for improving the performance of file search.
	*/
	static boost::unordered_map<std::string, std::string> _fullPathCache;
	static boost::unordered_map<std::string, std::string> _filenameLookupDict;

	/**
	*  The default root path of resources.
	*  If the default root path of resources needs to be changed, do it in the `init` method of FileUtils's subclass.
	*  For instance:
	*  On Android, the default root path of resources will be assigned with "assets/" in FileUtilsAndroid::init().
	*  Similarly on Blackberry, we assign "app/native/Resources/" to this variable in FileUtilsBlackberry::init().
	*/
	static std::string _defaultResRootPath;

	/**
	* The vector contains search paths.
	* The lower index of the element in this vector, the higher priority for this search path.
	*/
	static std::vector<std::string> _searchPathArray;
};
#endif    // __XUtilsFile_H__

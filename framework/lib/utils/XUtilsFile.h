#ifndef __XUtilsFile_H__
#define __XUtilsFile_H__

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class XUtilsFile
{
public:
	XUtilsFile();
	~XUtilsFile();

public:
	/**
	 * 初始化
	 * @param
	 * @return
	 */
	static void init();
	/**
	*  Checks whether a file exists without considering search paths and resolution orders.
	*  @param The file (with absolute path) to look up for
	*  @return Returns true if the file found at the given absolute path, otherwise returns false
	*/
	static bool isFileExistInternal(const std::string& filename);

	/**
	*  Checks whether the path is an absolute path.
	*
	*  @note On Android, if the parameter passed in is relative to "assets/", this method will treat it as an absolute path.
	*        Also on Blackberry, path starts with "app/native/Resources/" is treated as an absolute path.
	*
	*  @param strPath The path that needs to be checked.
	*  @return true if it's an absolute path, otherwise it will return false.
	*/
	static bool isAbsolutePath(const std::string& path);

	/**
	*  Checks whether a file exists.
	*
	*  @note If a relative path was passed in, it will be inserted a default root path at the beginning.
	*  @param strFilePath The path of the file, it could be a relative or absolute path.
	*  @return true if the file exists, otherwise it will return false.
	*/
	static bool isFileExist(const std::string& filename);

	static std::string fullPathForFilename(const std::string &filename);

	/**
	*  Returns the fullpath for a given filename.
	*  This is an alternative for fullPathForFilename, there are two main differences:
	*  First, it returns empty string instead of the original filename when no file found for the given name.
	*  Secondly, it's a const function.
	*  @param filename The file name to look up for
	*  @return The full path for the file, if not found, the return value will be an empty string
	*/
	static std::string searchFullPathForFilename(const std::string& filename);

	/**
	*  Gets the new filename from the filename lookup dictionary.
	*  It is possible to have a override names.
	*  @param filename The original filename.
	*  @return The new filename after searching in the filename lookup dictionary.
	*          If the original filename wasn't in the dictionary, it will return the original filename.
	*/
	static std::string XUtilsFile::getNewFilename(const std::string &filename);

	/**
	*  Gets full path for filename, resolution directory and search path.
	*
	*  @param filename The file name.
	*  @param resolutionDirectory The resolution directory.
	*  @param searchPath The search path.
	*  @return The full path of the file. It will return an empty string if the full path of the file doesn't exist.
	*/
	static std::string getPathForFilename(const std::string& filename, const std::string& searchPath);

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
	static std::string getFullPathForDirectoryAndFilename(const std::string& directory, const std::string& filename);

	/**
	*  Gets resource file data
	*
	*  @param[in]  filename The resource file name which contains the path.
	*  @param[in]  pszMode The read mode of the file.
	*/
	static std::string getFileData(const std::string& filename, const char* mode = "r");

	/**
	 * 判断是否是存在的目录
	 * @param
	 * @return
	 */
	static bool isDirectoryExistInternal(const std::string& dirPath);
	
	/**
	 * 递归式访问文件夹里的所有文件
	 * @param
	 * @return
	 */
	static std::vector<std::string> getFilesInDir(const std::string dir);

	/**
	 * 
	 * @param
	 * @return
	 */
	static bool writeFileData(const std::string& fileName, const std::string& data);

	static bool renameFile(const std::string &oldname, const std::string &name, const std::string &path=".\/");

	/**
	* 格式化路径字符串，使之变为合法路径表示方法
	*
	*/
	static std::string formatPath(const std::string& path);

	//检查路径目录是否存在，没有的话，就一级级创建好目录
	static void checkDirAndCreate(std::string path);

	/**
	 * 删除文件
	 * @param
	 * @return
	 */
	static bool deleteFile(const std::string &path);
private:
	/**
	*  The full path cache. When a file is found, it will be added into this cache.
	*  This variable is used for improving the performance of file search.
	*/
	static std::unordered_map<std::string, std::string> _fullPathCache;
	static std::unordered_map<std::string, std::string> _filenameLookupDict;

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

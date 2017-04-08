/***********************************************************************
https://github.com/MartinJK/IVNetwork/blob/master/Documents/Development_ReverseEngineering/IV-Classes.txt

See: rage::fiDevice
***********************************************************************/

#ifndef FILE_DEVICE_H
#define FILE_DEVICE_H

#include "../../EngineDefinitions.h"
#include "../Templates/Noncopyable.h"

typedef void* FHANDLE;

namespace BEngine
{
	enum class SeekMode
	{
		FSEEK_SET, //Seek to an absolute position.
		FSEEK_CUR, //Seek relative to the current position.
		FSEEK_END  //Seek relative the end of the file.
	};

	enum class BFileResult
	{
		FILE_OK,
		FILE_COULD_NOT_OPEN,
		FILE_COULD_NOT_CLOSE,
		FILE_ERROR, 
		FILE_IS_OPEN,
	};

	enum class BFileMode
	{
		FILE_RB, //Read Binary
		//FILE_WB, //Write Binary
		//FILE_RWB, //Read and Write Binary
	};

	//-----------------------------------------------------------------------
	//BFile 
	//
	//Class for safer reading and writing files. 
	//-----------------------------------------------------------------------

	class CBFile : public CNoncopyable
	{
	public:
		CBFile();
		CBFile(const char* fileName, BFileMode mode);
		virtual ~CBFile();

		BFileResult open(const char* fileName, BFileMode mode);
		BFileResult close();

		BFileResult seek(int offset, SeekMode mode);
		BFileResult read(void* buffer, size_t elementSize, uint32 elementCount);
		//void write(void* pBuffer, size_t elementSize, uint32 elementCount);

		bool isOpen() const { return m_fileIsOpen; }
		uint64 getFileSize() const { return m_fileSize; }
		uint64 getFileSize(FHANDLE);
	protected:
		bool m_fileIsOpen;
		uint64 m_fileSize;
		FHANDLE m_fileHandle;
	};


	/*
	class CFileDevice
	{
	public:
		CFileDevice() = default;
		virtual ~CFileDevice() = default;

		virtual FHANDLE open(const char* fileName, const char* mode);
		virtual void close(FHANDLE);

		virtual void seek(FHANDLE, int offset, int origin);
		virtual void read(FHANDLE, void* pBuffer, size_t bufferSize);

		uint64 getFileSize(FHANDLE);
	};
	*/
}

#endif //FILE_DEVICE_H


/*
class fiDevice  {
public:
fiDevice () {}
virtual ~fiDevice () {}

virtual HANDLE  open (char * pszName, bool bReadonly) = 0;                              // m4
virtual HANDLE  openBulk (char * pszName, DWORD * ) { return INVALID_HANDLE_VALUE; }    // m8
virtual HANDLE  create (char * pszName) { return INVALID_HANDLE_VALUE; }                // mC
virtual HANDLE  _m10 (char * pszName) = 0;
virtual int     read (HANDLE hFile, void * pBuffer, int nCount) = 0;                    // m14
virtual int     readBulk (HANDLE hFile, int nOffset, int nOffsetHight, void * buf, int nLength) { return -1; }  // _m18
virtual int     writeBulk (HANDLE hFile, int nOffset, int nOffsetHigh, void * buf, int nLength) { return -1; }
virtual int     write (HANDLE hFile, void * pBuffer, int nCount) = 0;
virtual int     seek (HANDLE hFile, int nPos, int nWhence) = 0; // _m24
virtual int     close (HANDLE hFile) = 0;
virtual int     closeBulk  (HANDLE hFile) { return -1; }    // _m2C
virtual int     filelength (HANDLE hFile);      // _m30
virtual int     _m34 (HANDLE) { return 0; }
virtual bool    unlink (char * pszName) { return false; }
virtual bool    rename (char * pszOldName, char * pszNewName) { return false; }
virtual bool    mkdir (char * pszName) { return false; }
virtual bool    rmdir (char * pszName) { return false; }
virtual __int64 filelengthLong (char * pszName);    // get length of the contained file
virtual __int64 getFileTime (char * pszName) = 0;                                               // m4C
virtual bool    setFileTime (char * pszName, int nFileTime, int nFileTimeHigh) = 0;             // m50
virtual HANDLE  findFirst (char * pszName, void * pResults) { return INVALID_HANDLE_VALUE; }    // m54
virtual bool    findNext (HANDLE hFind, void * pResults) { return false; }                      // m58
virtual int     findClose (HANDLE hFind) { return -1; }                                         // m5C
virtual bool    truncate (HANDLE hFile) { return false; }                                       // m60
virtual DWORD   getFileAttrs (HANDLE hFile) { return 0xFFFFFFFF; }                              // m64
virtual bool    setFileAttrs (HANDLE hFile, DWORD dwAttrs) { return false; }                    // m68
virtual int     getResourceVersion (char * pszName, DWORD _a4, DWORD * dwFlags);                // m6C
virtual HANDLE  getContainerHandle() { return INVALID_HANDLE_VALUE; }
virtual __int64 _m74 (char * pszName) { return 0LL; };
virtual void    _m78 () {}
virtual int     _m7C (char * pszName) { return 0; };

};
*/
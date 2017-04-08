#include "FileDevice.h"

#include <iostream>

namespace BEngine
{
	CBFile::CBFile() : m_fileSize(0) , m_fileIsOpen(false)
	{	
		m_fileHandle = nullptr;
	}

	CBFile::CBFile(const char* fileName, BFileMode mode)
	{
		open(fileName, mode);
	}

	CBFile::~CBFile()
	{
		if (m_fileIsOpen)
		{
			this->close();
		}
	}

	BFileResult CBFile::open(const char* fileName, BFileMode mode)
	{
		if (!m_fileIsOpen)
		{	
			const char* cMode = "";
			switch (mode)
			{
			case BEngine::BFileMode::FILE_RB:
				cMode = "rb";
				break;
			default:
				cMode = "rb";
				break;
			}

			FILE* inFile = nullptr;
			auto err = fopen_s(&inFile, fileName, cMode);
			if (err == 0)
			{
				m_fileHandle = inFile;
				m_fileSize = getFileSize(inFile);
				m_fileIsOpen = true;
				return BFileResult::FILE_OK;
			}
			else
			{
				return BFileResult::FILE_COULD_NOT_OPEN;
			}
		}
		return BFileResult::FILE_IS_OPEN;
	}

	BFileResult CBFile::close()
	{
		auto err = fclose((FILE*)m_fileHandle);
		if (err == 0)
		{
			m_fileIsOpen = false;
			return BFileResult::FILE_OK;
		}
		else
		{
			return BFileResult::FILE_COULD_NOT_CLOSE;
		}
	}

	BFileResult CBFile::seek(int offset, SeekMode mode)
	{
		if (m_fileIsOpen && (FILE*)m_fileHandle != nullptr)
		{
			switch (mode)
			{
			case BEngine::SeekMode::FSEEK_SET:
				fseek((FILE*)m_fileHandle, offset, SEEK_SET);
				break;
			case BEngine::SeekMode::FSEEK_CUR:
				fseek((FILE*)m_fileHandle, offset, SEEK_CUR);
				break;
			case BEngine::SeekMode::FSEEK_END:
				fseek((FILE*)m_fileHandle, offset, SEEK_END);
				break;
			default:
				break;
			}
			return BFileResult::FILE_OK;
		}	
		return BFileResult::FILE_ERROR;
	}

	//http://en.cppreference.com/w/cpp/io/c/fread
	BFileResult CBFile::read(void* buffer, size_t elementSize, uint32 elementCount)
	{
		if (m_fileIsOpen && (FILE*)m_fileHandle != nullptr)
		{
			fread(buffer, elementSize, elementCount, (FILE*)m_fileHandle);
			return BFileResult::FILE_OK;
		}
		return BFileResult::FILE_ERROR;
	}

	uint64 CBFile::getFileSize(FHANDLE hFile)
	{
		FILE* file = (FILE*)hFile;
		if (file != nullptr)
		{
			fseek(file, 0, SEEK_END);
			uint64 size = ftell(file);
			fseek(file, 0, SEEK_SET);
			return size;
		}
	}

	/*
	FHANDLE CFileDevice::open(const char* fileName, const char* mode)
	{
		FILE* file;
		fopen_s(&file, fileName, mode);
		return file;
	}

	void CFileDevice::close(FHANDLE hFile)
	{
		fclose(hFile);
	}

	void CFileDevice::seek(FHANDLE hFile, int offset, int origin)
	{
		fseek(hFile, offset, origin);
	}

	void CFileDevice::read(FHANDLE hFile, void* pBuffer, size_t bufferSize)
	{
		fread(pBuffer, 1, bufferSize, hFile);
	}

	uint64 CFileDevice::getFileSize(FHANDLE hFile)
	{
		fseek(hFile, 0, SEEK_END);
		uint32 size = ftell(hFile);
		fseek(hFile, 0, SEEK_SET);
		return size;
	}
	*/
}
#include "NuoStrings.h"

#include <Windows.h>
#include <cassert>


#include "NuoFile.h"


NuoFile::NuoFile(const std::string& path)
	: _path(path), _file(0)
{
}


void NuoFile::Write(void* buffer, size_t size)
{
	FILE* file = FileHandle(false);
	if (!file)
		return;

	fwrite(buffer, size, 1, file);
}


void NuoFile::Seek(size_t pos)
{
	fseek(_file, (LONG)pos, SEEK_SET);
}


size_t NuoFile::Size()
{
	std::wstring wpath = StringToUTF16(_path);
	struct _stat fileState;
	_wstat(wpath.c_str(), &fileState);

	return fileState.st_size;
}


size_t NuoFile::Position()
{
	return ftell(_file);
}


PNuoReadStream NuoFile::ReadStream()
{
	if (!_stream)
	{
		FILE* file = FileHandle(true);
		if (!file)
			return 0;

		std::wstring wpath = StringToUTF16(_path);
		struct _stat fileState;
		_wstat(wpath.c_str(), &fileState);

		IStream* iStream = 0;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileState.st_size);

		assert(hGlobal);
		void* pGlobal = GlobalLock(hGlobal);

		assert(pGlobal);
		fread(pGlobal, fileState.st_size, 1, file);
		GlobalUnlock(hGlobal);

		HRESULT result = CreateStreamOnHGlobal(hGlobal, true, &iStream);
		if (result != S_OK)
		{
			GlobalFree(hGlobal);
			return 0;
		}

		_stream.reset(new NuoReadStream(iStream));
	}

	return _stream;
}

void NuoFile::ReadTo(std::vector<char>& content)
{
	FILE* file = FileHandle(true);
	if (!file)
		return;

	size_t fileSize = Size();
	content.resize(fileSize + 1);
	content[fileSize] = '\0';

	fread(content.data(), 1, fileSize, file);
}

void NuoFile::SaveStream(const PNuoWriteStream& stream, long size)
{
	HGLOBAL hGlobal = stream->GlobalBuffer();
	void* buffer = GlobalLock(hGlobal);

	if (!buffer)
		return;

	if (!size)
	{
		STATSTG state;
		stream->Stream()->Stat(&state, 0);
		size = (long)(state.cbSize.QuadPart);
	}

	FILE* file = FileHandle(false);
	if (!file)
	{
		GlobalUnlock(hGlobal);
		return;
	}

	fwrite(buffer, size, 1, file);
	fclose(file);
	_file = 0;

	GlobalUnlock(hGlobal);
}


FILE* NuoFile::FileHandle(bool read)
{
	if (!_file)
	{
		std::wstring wpath = StringToUTF16(_path);

		if (read)
			_wfopen_s(&_file, wpath.c_str(), L"rb");
		else
			_wfopen_s(&_file, wpath.c_str(), L"wb");
	}

	return _file;
}


NuoFile::~NuoFile()
{
	if (_file)
		fclose(_file);
}


NuoReadStream::NuoReadStream(const Microsoft::WRL::ComPtr<IStream>& iStream)
	: _iStream(iStream)
{
}


Microsoft::WRL::ComPtr<IStream>& NuoReadStream::Stream()
{
	return _iStream;
}


NuoReadStream::~NuoReadStream()
{
}


NuoWriteStream::NuoWriteStream()
	: _iStream(0), _hGlobal(0)
{
}


Microsoft::WRL::ComPtr<IStream>&  NuoWriteStream::Stream()
{
	if (!_iStream)
	{
		HRESULT r = CreateStreamOnHGlobal(0, TRUE, &_iStream);
		assert(r == S_OK);
	}

	if (!_hGlobal)
		GlobalFree(_hGlobal);

	return _iStream;
}

HGLOBAL NuoWriteStream::GlobalBuffer()
{
	if (!_hGlobal)
	{
		HRESULT r = GetHGlobalFromStream(_iStream.Get(), &_hGlobal);
		assert(r == S_OK);
	}

	return _hGlobal;
}

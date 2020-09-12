#include "NuoStrings.h"

#include <Windows.h>



#include "NuoFile.h"


NuoFile::NuoFile(const std::string& path)
	: _path(path), _file(0)
{
}


PNuoStream NuoFile::Stream()
{
	if (!_stream)
	{
		FILE* file = FileHandle();
		if (!file)
			return 0;

		std::wstring wpath = StringToUTF16(_path);
		struct _stat fileState;
		_wstat(wpath.c_str(), &fileState);

		IStream* iStream = 0;
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileState.st_size);
		void* pGlobal = GlobalLock(hGlobal);
		fread(pGlobal, fileState.st_size, 1, file);
		GlobalUnlock(hGlobal);

		HRESULT result = CreateStreamOnHGlobal(hGlobal, true, &iStream);
		if (result)
		{
			GlobalFree(hGlobal);
			return 0;
		}

		_stream.reset(new NuoStream(iStream));
	}

	return _stream;
}


FILE* NuoFile::FileHandle()
{
	if (!_file)
	{
		std::wstring wpath = StringToUTF16(_path);
		_wfopen_s(&_file, wpath.c_str(), L"rb");
	}

	return _file;
}


NuoFile::~NuoFile()
{
	if (_file)
		fclose(_file);
}


NuoStream::NuoStream(IStream* iStream)
	: _iStream(iStream)
{
}


NuoStream::operator IStream* ()
{
	return _iStream;
}


NuoStream::~NuoStream()
{
	if (_iStream)
		_iStream->Release();
}

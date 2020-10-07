#pragma once

#include <string>
#include <memory>


class NuoReadStream;
typedef std::shared_ptr<NuoReadStream> PNuoReadStream;

class NuoWriteStream;
typedef std::shared_ptr<NuoWriteStream> PNuoWriteStream;


class NuoFile : public std::enable_shared_from_this<NuoFile>
{
	PNuoReadStream _stream;
	FILE* _file;
	std::string _path;

public:
	NuoFile(const std::string& path);
	~NuoFile();

	void Write(void* buffer, size_t size);
	void Seek(size_t pos);
	size_t Position();

	PNuoReadStream ReadStream();
	void SaveStream(const PNuoWriteStream& stream, long size);
	FILE* FileHandle(bool read);
};


class NuoReadStream : public std::enable_shared_from_this<NuoReadStream>
{
	IStream* _iStream;
	NuoReadStream(IStream* iStream);

public:

	~NuoReadStream();
	operator IStream* ();

	friend NuoFile;
};


class NuoWriteStream : public std::enable_shared_from_this<NuoWriteStream>
{
	IStream* _iStream;
	HGLOBAL _hGlobal;
	HGLOBAL GlobalBuffer();
	
public:

	NuoWriteStream();
	operator IStream* ();

	friend NuoFile;
};


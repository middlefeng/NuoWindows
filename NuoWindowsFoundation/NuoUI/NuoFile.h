#pragma once

#include <string>
#include <memory>
#include <vector>
#include <wrl.h>


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
	size_t Size();
	size_t Position();
	void ReadTo(std::vector<char>& content);

	PNuoReadStream ReadStream();
	void SaveStream(const PNuoWriteStream& stream, long size);
	FILE* FileHandle(bool read);
};


class NuoReadStream : public std::enable_shared_from_this<NuoReadStream>
{
	Microsoft::WRL::ComPtr<IStream> _iStream;
	NuoReadStream(const Microsoft::WRL::ComPtr<IStream>& iStream);

public:

	~NuoReadStream();
	Microsoft::WRL::ComPtr<IStream>& Stream();

	friend NuoFile;
};


class NuoWriteStream : public std::enable_shared_from_this<NuoWriteStream>
{
	Microsoft::WRL::ComPtr<IStream> _iStream;
	HGLOBAL _hGlobal;
	HGLOBAL GlobalBuffer();
	
public:

	NuoWriteStream();
	Microsoft::WRL::ComPtr<IStream>& Stream();

	friend NuoFile;
};


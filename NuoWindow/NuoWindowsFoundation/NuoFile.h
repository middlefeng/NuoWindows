#pragma once

#include <string>
#include <memory>


class NuoStream;
typedef std::shared_ptr<NuoStream> PNuoStream;


class NuoFile : public std::enable_shared_from_this<NuoFile>
{
	PNuoStream _stream;
	FILE* _file;
	std::string _path;

public:
	NuoFile(const std::string& path);
	~NuoFile();

	PNuoStream Stream();
	FILE* FileHandle();
};


class NuoStream : public std::enable_shared_from_this<NuoStream>
{
	IStream* _iStream;
	NuoStream(IStream* iStream);

public:

	~NuoStream();
	operator IStream* ();

	friend NuoFile;
};

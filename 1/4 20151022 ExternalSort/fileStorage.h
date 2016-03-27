#ifndef FILE_STORAGE
#define FILE_STORAGE

#include "serialize.h"

static unsigned int file_id = 0;
template<class T>
class FileStorage
{
public:
	explicit FileStorage(const std::string &path = "")
	{
		std::stringstream ss;
		ss << path << file_id++ << ".dat";
		filePath_ = ss.str();
	}
	
	~FileStorage()
	{
		assert(std::remove(filePath_.c_str()) == 0);
	}
	
	void startWrite()
	{
		allObjectsNumber_ = 0;
		stream_.open(filePath_.c_str(), std::ios_base::binary | std::ios_base::out);
	}
	
	void write(const T &object)
	{
		writeObjectToStream(object, stream_);
		++allObjectsNumber_;
	}
	
	void startRead()
	{
		readObjectsNumber_ = 0;
		stream_.open(filePath_.c_str(), std::ios_base::binary | std::ios_base::in);
	}
	
	bool hasNext() const
	{
		return readObjectsNumber_ < allObjectsNumber_;
	}
	
	T read()
	{
		if (bufferQueue_.empty())
		{
			unsigned int bufferSize = static_cast<unsigned int>(sqrt(allObjectsNumber_));
			for (size_t i = 0; i <= bufferSize && hasNext(); ++i)
			{
				T object;
				readObjectFromStream(object, stream_);
				bufferQueue_.push(object);
			}
		}
		
		assert(!bufferQueue_.empty());
		T object = bufferQueue_.front();
		bufferQueue_.pop();
		++readObjectsNumber_;
		return object;
	}
	
	void stop()
	{
		stream_.close();
		stream_.clear();
	}
private:
	std::fstream stream_;
	std::string filePath_;
	std::queue<T> bufferQueue_;
	unsigned int allObjectsNumber_;
	unsigned int readObjectsNumber_;
};

#endif
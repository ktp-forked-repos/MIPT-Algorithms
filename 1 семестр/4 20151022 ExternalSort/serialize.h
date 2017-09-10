#ifndef SERIALIZE
#define SERIALIZE

template<class T>
void writeObjectToStream(const T &object, std::fstream &stream)
{
	stream.write(reinterpret_cast<const char*>(&object), sizeof(T));
}

template<>
void writeObjectToStream(const std::string &object, std::fstream &stream)
{
	writeObjectToStream(object.length(), stream);
	stream.write(object.c_str(), object.length());
}

// Нужно чтобы сериализация работала как для vector<pair<...>>, так и для pair<vector<...>, ...>
template<class T, class V>
void writeObjectToStream(const std::pair<T, V> &object, std::fstream &stream);

template<class T>
void writeObjectToStream(const std::vector<T> &object, std::fstream &stream)
{
	writeObjectToStream(object.size(), stream);
	for (T element : object)
		writeObjectToStream(element, stream);
}

template<class T, class V>
void writeObjectToStream(const std::pair<T, V> &object, std::fstream &stream)
{
	writeObjectToStream(object.first, stream);
	writeObjectToStream(object.second, stream);
}

template<class T>
void readObjectFromStream(T &object, std::fstream &stream)
{
	stream.read(reinterpret_cast<char*>(&object), sizeof(T));
}

template<>
void readObjectFromStream(std::string &object, std::fstream &stream)
{
	size_t size;
	readObjectFromStream(size, stream);
	char *buffer = new char[size];
	stream.read(buffer, size);
	object.assign(buffer, size);
	delete[] buffer;
}

template<class T, class V>
void readObjectFromStream(std::pair<T, V> &object, std::fstream &stream);

template<class T>
void readObjectFromStream(std::vector<T> &object, std::fstream &stream)
{
	size_t size;
	readObjectFromStream(size, stream);
	object.resize(size);
	for (T &element : object)
		readObjectFromStream(element, stream);
}

template<class T, class V>
void readObjectFromStream(std::pair<T, V> &object, std::fstream &stream)
{
	readObjectFromStream(object.first, stream);
	readObjectFromStream(object.second, stream);
}

#endif
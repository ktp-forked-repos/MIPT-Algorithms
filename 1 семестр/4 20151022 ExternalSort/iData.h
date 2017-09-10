#ifndef I_DATA
#define I_DATA

template<class T>
class IDataInput
{
public:
	virtual T get() = 0;
	virtual bool hasNext() const = 0;
	virtual ~IDataInput() {};
};

template<class T>
class IDataOutput
{
public:
	virtual void put(const T &object) = 0;
	virtual void finish() = 0;
	virtual ~IDataOutput() {};
};

#endif
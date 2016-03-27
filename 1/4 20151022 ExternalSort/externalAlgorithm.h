#ifndef EXTERNAL_ALGORITHM
#define EXTERNAL_ALGORITHM

#include "fileStorage.h"
#include "iData.h"

template<class T>
class ExternalAlgorithm
{
public:
	ExternalAlgorithm(IDataInput<T>* dataInput, IDataOutput<T>* dataOutput, int maxNumber, const std::string &tempDirectory = "")
		: dataInput_(dataInput)
		, dataOutput_(dataOutput)
		, tempDirectory_(tempDirectory)
		, maxNumber_(maxNumber)
	{}
	
	void execute()
	{
		getDataAndSave();
		merge();
	}
	
	virtual ~ExternalAlgorithm()
	{
		for (FileStorage<T>* fileStorage : this->fileStorages_)
			delete fileStorage;
	}

protected:
	void getDataAndSave()
	{
		allNumber_ = 0;
		for (int iFileStorage = 0; dataInput_->hasNext(); ++iFileStorage)
		{
			std::vector<T> partData;
			for (unsigned int i = 0; i < maxNumber_ && dataInput_->hasNext(); ++i)
				partData.push_back(dataInput_->get());
			processPartData(partData);
			
			FileStorage<T>* currentFileStorage = new FileStorage<T>(tempDirectory_);
			currentFileStorage->startWrite();
			for (T object : partData)
				currentFileStorage->write(object);
			currentFileStorage->stop();
			fileStorages_.push_back(currentFileStorage);
			
			allNumber_ += partData.size();
		}
	}
	
	virtual void processPartData(std::vector<T> &partData) = 0;
	
	virtual void merge() = 0;
	
	IDataInput<T>* dataInput_;
	IDataOutput<T>* dataOutput_;
	std::string tempDirectory_;
	unsigned int maxNumber_;
	unsigned int allNumber_;
	
	std::vector<FileStorage<T>*> fileStorages_;
};

#endif
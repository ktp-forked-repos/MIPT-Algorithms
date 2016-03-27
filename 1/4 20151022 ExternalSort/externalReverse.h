#ifndef EXTERNAL_REVERSE
#define EXTERNAL_REVERSE

#include "externalAlgorithm.h"

template<class T>
class ExternalReverse : public ExternalAlgorithm<T>
{
public:
	ExternalReverse(IDataInput<T>* DataInput, IDataOutput<T>* DataOutput, int maxNumber, const std::string &tempDirectory = "")
		: ExternalAlgorithm<T>(DataInput, DataOutput, maxNumber, tempDirectory) {}
	
protected:
	virtual void merge()
	{
		this->getDataAndSave();
		for (int iFileStorage = this->fileStorages_.size() - 1; iFileStorage >= 0; --iFileStorage)
		{
			this->fileStorages_[iFileStorage]->startRead();
			while (this->fileStorages_[iFileStorage]->hasNext())
				this->dataOutput_->put(this->fileStorages_[iFileStorage]->read());
			this->fileStorages_[iFileStorage]->stop();
		}
		this->dataOutput_->finish();
	}
	
	virtual void processPartData(std::vector<T> &partData)
	{
		std::reverse(partData.begin(), partData.end());
	}
};

#endif
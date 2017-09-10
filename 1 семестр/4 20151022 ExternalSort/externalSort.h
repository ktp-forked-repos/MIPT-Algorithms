#ifndef EXTERNAL_SORT
#define EXTERNAL_SORT

#include "externalAlgorithm.h"

template<class T>
class ExternalSort : public ExternalAlgorithm<T>
{
public:
	ExternalSort(IDataInput<T>* DataInput, IDataOutput<T>* DataOutput, int maxNumber, const std::string &tempDirectory = "")
		: ExternalAlgorithm<T>(DataInput, DataOutput, maxNumber, tempDirectory) {}
	
protected:
	virtual void merge()
	{
		std::priority_queue<std::pair<T, size_t>, std::vector<std::pair<T, size_t>>, std::greater<std::pair<T, size_t>>> queue;
		for (size_t iFileStorage = 0; iFileStorage < this->fileStorages_.size(); ++iFileStorage)
		{
			this->fileStorages_[iFileStorage]->startRead();
			queue.push({this->fileStorages_[iFileStorage]->read(), iFileStorage});
		}
		
		while (!queue.empty())
		{
			std::pair<T, int> p = queue.top();
			queue.pop();
			this->dataOutput_->put(p.first);
			
			size_t iFileStorage = p.second;
			if (this->fileStorages_[iFileStorage]->hasNext())
				queue.push({this->fileStorages_[iFileStorage]->read(), iFileStorage});
		}
		
		for (size_t iFileStorage = 0; iFileStorage < this->fileStorages_.size(); ++iFileStorage)
			this->fileStorages_[iFileStorage]->stop();
		
		this->dataOutput_->finish();
	}
	
	virtual void processPartData(std::vector<T> &partData)
	{
		std::sort(partData.begin(), partData.end());
	}
};

#endif
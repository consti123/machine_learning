#pragma once

#include "Classifier.h"
#include "BayesClassifier.h"
#include "DensityTreeClassifier.h"

class CopulaClassifier : public Classifier
{
public:
	CopulaClassifier();

	void train(const image_data_t & train_data, const label_data_t & train_label);
	
	label_data_t predict(const image_data_t & test_data);

	image_data_t generate(const size_t N, const short label);

	void set_maximal_depth(const size_t max_depth);
	size_t get_maximal_depth() const;
	
	void   set_nearest_neighbors(const size_t num_nbrs);
	size_t get_nearest_neighbors() const;
	
	void set_record_split(const bool enable);
	bool get_record_split();

	void set_split(const DensityTreeClassifier::split_t split);
	DensityTreeClassifier::split_t get_split() const;
	
	void set_fixed_bins(const bool enable, const size_t n);
	size_t get_fixed_bins() const;
	
	image_data_t get_copula(const image_data_t & data);

private:
	bool mTrained;
	size_t mNum_instances;
	size_t mNum_classes;
	size_t mNum_dimensions;
	std::vector<double>	mPriors;
// BayesClassifier
	BayesClassifier	mBayes;
// DensityTreeClassifier
	DensityTreeClassifier mDensityTree;
};

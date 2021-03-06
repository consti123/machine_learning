#include <algorithm>
#include <exception>
#include <iostream>  // std::left
#include <iomanip>   // std::setw std::setprecision

#include <boost/numeric/ublas/io.hpp>

#include "common.h"
	
using namespace boost::numeric::ublas;

bool compare_first(const pair_t & l, const pair_t & r)
{
	return l.first < r.first;
}

bool compare_deref(const double* l, const double* r  )
{
	return *(l) < *(r);
}


// tested in test/test_sortindex
vector<double> get_sorted_indices(const matrix_column<matrix<double> const> & data )
{
	std::vector<pair_t> to_sort;
	for(size_t i = 0; i < data.size(); i++)
	{
		double val = data(i);
		pair_t pair(val,i);
		to_sort.push_back(pair);
	}
	std::sort(to_sort.begin(), to_sort.end(), compare_first);
	vector<double> returnval( to_sort.size() );
	for(size_t i = 0; i < to_sort.size(); i++)
	{
		returnval(i) = to_sort[i].second;
	}
	return returnval;
}

vector<double> get_sorted_indices(const vector<double> & data )
{
	std::vector<pair_t> to_sort;
	for(size_t i = 0; i < data.size(); i++)
	{
		double val = data(i);
		pair_t pair(val,i);
		to_sort.push_back(pair);
	}
	std::sort(to_sort.begin(), to_sort.end(), compare_first);
	vector<double> returnval( to_sort.size() );
	for(size_t i = 0; i < to_sort.size(); i++)
	{
		returnval(i) = to_sort[i].second;
	}
	return returnval;
}

vector<double> get_ranked_indices(
		const matrix_column<matrix<double> const> & data )
{
	const size_t len = data.size();
	vector<double> returnval(len);
	std::copy( data.begin(), data.end(), returnval.begin() );
// make array of double pointers pointing to data 
	std::vector<double*> pointer_array(len);
   	for( size_t i = 0; i < len; i++)
	{
		pointer_array[i] = &returnval(i);
	}	
// sort the pointers according to the value they point at
	std::sort(pointer_array.begin(), pointer_array.end(), compare_deref);
   	for( size_t i = 0; i < len; i++)
	{
		*pointer_array[i] = i;
	}
	return returnval;
}

vector<double> get_ranked_indices(
		const vector<double>& data )
{
	size_t len = data.size();
	vector<double> returnval(len);
	std::copy( data.begin(), data.end(), returnval.begin() );
// make array of double pointers pointing to data 
	std::vector<double*> pointer_array(len);
   	for( size_t i = 0; i < len; i++)
	{
		pointer_array[i] = &returnval(i);
	}	
// sort the pointers according to the value they point at
	std::sort(pointer_array.begin(), pointer_array.end(), compare_deref);
   	for( size_t i = 0; i < len; i++)
	{
		*pointer_array[i] = i;
	}
	return returnval;
}

image_data_t read_mnist_data(const std::string & fname, size_t buff_size)
{
// open filestream to read in the data
	std::fstream fs;
	fs.open(fname, std::fstream::in);
	if(!fs)
	{
		std::string err = "Filestream not openend correctly for file " + fname;
		throw std::runtime_error(err);
	}
// determine number of lines and number of values per line
	size_t num_lines = std::count(std::istreambuf_iterator<char>(fs),
				std::istreambuf_iterator<char>(),
				'\n');
	std::cout << "Reading " << fname << ": Number of instances: " << num_lines << std::endl;
	fs.close();
	fs.open(fname, std::fstream::in);
	char s[buff_size];
	fs.getline(s,buff_size);
	std::stringstream ss(s);
	size_t per_line = std::count(std::istreambuf_iterator<char>(ss),
				std::istreambuf_iterator<char>(),
				' ') + 1;
	std::cout << "Reading " << fname <<": Number of dimensions: " << per_line << std::endl;
	fs.close();
	fs.open(fname, std::fstream::in);
// initialise the data
	 image_data_t data_return(num_lines, per_line);
// read in the data
	size_t i = 0;
	while( !fs.eof() && i < num_lines )
	{
		size_t j = 0;
		char line[buff_size];
		fs.getline(line,buff_size);
		std::stringstream sss(line);
		while( !sss.eof() )
		{
			char num[16];
			sss.getline(num,16,' ');
			double val = atof(num);
			data_return(i,j) = val;
			j++;
		}
		i++;
	}
	return data_return;
}

label_data_t read_mnist_label(const std::string & fname)
{
// open filestream to read in the data
	std::fstream fs;
	fs.open(fname, std::fstream::in);
	if(!fs)
	{
		std::string err = "Filestream not openend correctly for file " + fname;
		throw std::runtime_error(err);
	}
// determine number of lines
	size_t num_lines = std::count(std::istreambuf_iterator<char>(fs),
				std::istreambuf_iterator<char>(),
				'\n');
	std::cout << "Reading " << fname << ": Number of instances: " << num_lines << std::endl;
	fs.close();
	fs.open(fname, std::fstream::in);
// init return data
	label_data_t label_return;
// read in the data
	size_t i = 0;
	while( !fs.eof() && i < num_lines )
	{
		char num[16];
		fs.getline(num,16);
		short val = atoi(num);
		label_return.push_back(val);
		i++;
	}
	return label_return;
}

void save_data(const std::string & fname, const image_data_t & data)
{
	std::fstream fs;
	fs.open(fname, std::fstream::out);
	if(!fs)
	{
		std::stringstream stream("Filestream not openend correctly for file ");
		stream << fname;
		throw std::runtime_error(stream.str());
	}
	for(size_t i = 0; i < data.size1(); i++)
	{
		for(size_t j = 0; j < data.size2(); j++)
		{
			fs << data(i,j) << " ";
		}
		fs << '\n';
	}
}

void save_label(const std::string & fname, const label_data_t & label)
{
	std::fstream fs;
	fs.open(fname, std::fstream::out);
	if(!fs)
	{
		std::stringstream stream("Filestream not openend correctly for file ");
		stream << fname;
		throw std::runtime_error(stream.str());
	}
	for(size_t i = 0; i < label.size(); i++)
	{
		fs << label[i] << '\n';
	}
}

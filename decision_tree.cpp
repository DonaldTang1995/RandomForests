#include"desion_tree.h"
#include<algorithm>
#include<assert.h>
node::node() :feature_type(-1),type(-1),value(0), left_children(NULL), right_children(NULL)
{}

node::~node()
{
	if (left_children != NULL)
	{
		delete left_children;
	}

	if (right_children != NULL)
	{
		delete right_children;
	}
}

double node::calc_entropy(int *hash_table , int total,int type_size)
{
	if (total == 0)
		return 0;
	double entropy = 0;
	for (int i = 0; i < type_size; i++)
	{
		if (hash_table[i] != 0)
		{
			double p = (double)hash_table[i] / total;
			entropy -= p*log(p);
		}
	}
	return entropy;
}

void node::split(vector<int>&feature_index, double *x, int *t, int cols)
{

	int type_size = 0, size = feature_index.size(), flag = 0;

	std::pair<double, int>*temp = new std::pair<double, int>[size];

	for (int i = 0; i < size; i++)
	{
		int index = feature_index[i];
		type_size = std::max(type_size, t[index]);
		if (t[index] != t[feature_index[0]])
			flag = 1;
	}
	type_size++;

	if (!flag)
	{
		type = t[feature_index[0]];
		
		return;
	}

	int *left_hash_table = new int[type_size], *right_hash_table = new int[type_size];
	memset(right_hash_table, 0, sizeof(int)*type_size);
	for (int i = 0; i < size; i++)
	{
		int index = feature_index[i];
		right_hash_table[t[index]]++;
	}


	double total_entropy = calc_entropy(right_hash_table, size, type_size), max_information_gain = -1, split_val;
	int split_type;
	flag = 1;

	for (int i = 0; i < cols; i++)
	{
		
		memset(left_hash_table, 0, sizeof(int)*type_size);
		memset(right_hash_table, 0, sizeof(int)*type_size);
		for (int k = 0; k < size; k++)
		{
			int index = feature_index[k];
			right_hash_table[t[index]]++;
		}
		for (int j = 0; j < size; j++)
		{
			int index = feature_index[j];
			temp[j] = std::make_pair(x[index*cols + i], index);
		}
		std::sort(temp, temp + size);
		for (int j = 0; j < size;)
		{
			j++;
			while (j < size&&temp[j].first == temp[j - 1].first)
			{
				left_hash_table[t[temp[j].second]]++;
				right_hash_table[t[temp[j].second]]--;
				j++;
			}
			if (j == size)
				continue;
			double left_entropy = calc_entropy(left_hash_table, j, type_size);
			double right_entropy = calc_entropy(right_hash_table, size - j, type_size);
			double information_gain = total_entropy - (double(j)/size*left_entropy + double(size-j)/size*right_entropy);
			assert(information_gain<1e64);

			if (information_gain>max_information_gain)
			{
				split_val = temp[j - 1].first;
				split_type = i;
				max_information_gain = information_gain;
				flag = 0;
			}
		}
	}
	if (flag)
	{
		int max_types_index = 0;
		for (int i = 0; i < type_size; i++)
		{
			if (left_hash_table[i]>left_hash_table[max_types_index])
				max_types_index = i;
		}
		type = max_types_index;
	}
	else
	{
		vector<int>left_feature_index, right_feature_index;

	for (int i = 0; i < size; i++)
	{
		int index = feature_index[i];
		if (x[index*cols + split_type] <= split_val)
		{
			left_feature_index.push_back(index);
		}
		else
		{
			right_feature_index.push_back(index);
		}
	}

	right_children = new node;
	left_children = new node;
	left_children->split(left_feature_index, x, t, cols);
	right_children->split(right_feature_index, x, t, cols);
	feature_type = split_type;
	value = split_val;
	}
	
	delete[] right_hash_table;
	delete[] left_hash_table;
	
}

decision_tree::decision_tree(): root(NULL), rows(0), cols(0)
{}

decision_tree::decision_tree(vector<int>_feature_index, int _rows, int _cols) : feature_index(_feature_index),  rows(_rows), cols(_cols), root(NULL)
{}

decision_tree::~decision_tree()
{
	delete root;
}

void decision_tree::train(double *x,int *t)
{
	cout << "Training...\n";
	root = new node;
	root->split(feature_index, x, t, cols);
}

vector<int> decision_tree::predict(double *test_x, int size)
{
	
	vector<int>result(size);
	for (int i = 0; i < size; i++)
	{
		int index = cols*i;
		node *cur_node = root;

		while (cur_node->type == -1)
		{
			if (test_x[index + cur_node->feature_type]<=cur_node->value)
			{
				cur_node = cur_node->left_children;
			}
			else
			{
				cur_node = cur_node->right_children;
			}
		}
		result[i] = cur_node->type;
	}
	return result;
}
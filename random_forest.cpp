#include"random_forest.h"
random_forest::random_forest() :x(NULL), t(NULL), trees(NULL), rows(0), cols(0), number_of_decision_trees(0), number_of_samples_in_one_tree(0)
{}

random_forest::random_forest(double *_x, int *_t, int _rows, int _cols) : x(_x), t(_t), number_of_samples_in_one_tree((int)rows/5),
trees(NULL), rows(_rows), cols(_cols), number_of_decision_trees(0)
{}

void random_forest::set_number_of_decision_trees(int number)
{
	number_of_decision_trees = number;
}

vector<int> random_forest::sampling()
{
	

	vector<int>result;

	srand((unsigned int)time(NULL));
	for (int i = 0; i < number_of_samples_in_one_tree; i++)
	{
		result.push_back(rand() % rows);
		
	}

	return result;
	
}

void random_forest::train()
{
	trees = new decision_tree[number_of_decision_trees];
	for (int i = 0; i < number_of_decision_trees; i++)
	{
		trees[i] = decision_tree(sampling(), number_of_samples_in_one_tree, cols);
		trees[i].train(x, t);
	}
}

vector<int> random_forest::predict(double *test_x, int size)
{
	vector<int>*vote=new vector<int>[size],result(size);
	for (int i = 0; i < number_of_decision_trees; i++)
	{
		vector<int>temp = trees[i].predict(test_x, size);
		for (int j = 0; j < size; j++)
		{
			vote[j].push_back(temp[j]);
		}
	}

	for (int i = 0; i < size; i++)
	{
		std::sort(vote[i].begin(), vote[i].end());
		int max_voted = (int)-1e9,type;
		for (int j = 0; j < number_of_decision_trees;)
		{
			int total = 1;
			j++;
			while (j < number_of_decision_trees&&vote[i][j] == vote[i][j - 1])
			{
				total++;
				j++;
			}
			if (total>max_voted)
			{
				max_voted = total;
				type = vote[i][j - 1];
			}
		}
		result[i] = type;
	}
	return result;
}
#include"desion_tree.h"
#include<random>
#include<algorithm>
/*@Para x features;
  @Para t targets;
  @Para rows the number of samples;
  @Para cols the number of features;*/
class random_forest
{public:
	random_forest();
	random_forest(double *_x,int *_t,int _rows,int _cols);
	void set_number_of_decision_trees(int number);
	void train();
	vector<int> sampling();
	vector<int> predict(double *test_x,int size);
private:
	double *x;
	int *t;
	int rows;
	int cols;
	int number_of_decision_trees;
	int number_of_samples_in_one_tree;
	decision_tree *trees;

};
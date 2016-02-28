#include<iostream>
#include<vector>
#include<time.h>
using std::vector;
using std::cout;
/*@Para feature_type type of the feature;
  @Para value spliting value of the feature;
  @Para left_children left children of the node;
  @Para right_children right children of the node;
  @Para type the type of target if the node is leave,else it's -1;
  */
class node
{
public:
	node();
	~node();
	void split(vector<int>&feature_index,double *x, int *t,  int cols);
	double calc_entropy(int *hash_table , int total,int type_size);
	int feature_type;
	int type;
	double value;
	node *left_children;
	node *right_children;
};

/*@Para x features;
  @Para t Target value;
  @Para root Root of the decision tree;
  @Para rows the number of samples
  @Para cols the number of features*/
class decision_tree
{
public:
	decision_tree();
	decision_tree(vector<int>_feature_index,int _rows,int _cols);
	vector<int> predict(double *test_x,int size);
	void train(double *x,int *t);
	~decision_tree();
private:
	vector<int>feature_index;
	int *t;
	int rows;
	int cols;
	node *root;
};
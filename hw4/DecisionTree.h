#include <iostream>
#include <algorithm>
#include <vector>

#define MAX_FEATURE (1024+1)
#define MAX_TRAINING_DATA (3000000)

typedef std::vector<double*>::iterator Iterator;

class Node;
class DecisionTree;

double confusion(int const&, int const&);
double confusion(int const&, int const&, int const&, int const&);

void printTreePredict(DecisionTree const&, Node*, int = 0, int = 0);
void printForestPredict(DecisionTree**, int);

class Node{
    friend class DecisionTree;
    public:
        Node() : parent(0), left(0), right(0), first(0), last(0), decision(0){}
        Node(Iterator fir, Iterator las) : parent(0), left(0), right(0), first(fir), last(las), decision(0){}
        Node(Node* p, Iterator fir, Iterator las) : parent(p), left(0), right(0), first(fir), last(las), decision(0){}
        ~Node(){
            if(!this->isRoot()){
                if(parent->left == this){ 
                    parent->left = 0; 
                }
                else parent->right = 0;
            }
        }
        //get
        Node* getParent() const{return parent;}
        Node* getLeft() const{return left;}
        Node* getRight() const{return right;}
        Iterator getFirst() const{return first;}
        Iterator getLast() const{return last;}
        int getDecision() const{return decision;}
        int getFeaturesNum() const{return featuresNum;}
        double getCriterion() const{return criterion;}
        //set
        void setParent(Node* par){parent = par;}
        void setLeft(Node* lf){left = lf;}
        void setRight(Node* rt){right = rt;}
        void setFactor(int fea, double cri){featuresNum = fea; criterion = cri;}

        bool isRoot() const{return parent == 0;}
        bool isExternal() const{return (left == 0 && right == 0);}

        static double trueOrFalseRate(Iterator, Iterator);
        static double confusion(Iterator, Iterator);
        
        double trueOrFalseRate() const;
        double confusion() const;
        double minConfusionWithFeature(int const &, double&);

        void sortFeature(int const&);

        friend std::ostream& operator<<(std::ostream&, const Node&);

    protected:
        Node* parent;
        Node* left;
        Node* right;
        Iterator first, last;

        int decision;
        int featuresNum;
        double criterion;
};
class DecisionTree{
    public:
        DecisionTree() : root(0), examples(0){}
        DecisionTree(std::vector<double*> ex, int feaTotal, double tol) : examples(ex), featuresTotal(feaTotal), tolerance(tol){root = new Node(examples.begin(), examples.end());}
        DecisionTree(Iterator fir, Iterator las, int feaTotal, double tol) : examples(fir, las), featuresTotal(feaTotal), tolerance(tol){root = new Node(examples.begin(), examples.end());}
        ~DecisionTree();
        //get
        Node* getRoot() const{return root;}

        double setBestCriterion(Node*);

        Node* makeDecisionTree(Node*);

        int tree_predict(Node*, double *);

        friend std::ostream& operator<<(std::ostream&, const DecisionTree&);

//    private:
        Node* root;
        std::vector<double*> examples;
        int featuresTotal;
        double tolerance; 
};


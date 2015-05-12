#include "DecisionTree.h"

double confusion(int const& trueTotal, int const& falseTotal){
    if(trueTotal+falseTotal == 0) std::cout << "trueTotal+falseTotal == 0" << std::endl;
    return 2.0*trueTotal*falseTotal/(trueTotal+falseTotal)/(trueTotal+falseTotal);
}
double confusionTotal(int const& trueTotala, int const& falseTotalb, int const& trueTotalc, int const& falseTotald){
    if(trueTotala+trueTotalc+falseTotalb+falseTotald == 0) std::cout << "trueTotala+trueTotalc+falseTotalb+falseTotald == 0" << std::endl;
    return (confusion(trueTotala, falseTotalb)*(trueTotala + falseTotalb) + confusion(trueTotalc, falseTotald)*(trueTotalc + falseTotald)) 
            / (trueTotala + falseTotalb + trueTotalc + falseTotald);
}

//static functions
double Node::trueOrFalseRate(Iterator it, Iterator las){
    int trueTotal = 0, falseTotal = 0;
    while(it != las){
        switch(int(**it)){
            case +1:
                ++trueTotal;
                break;
            case -1:
                ++falseTotal;
                break;
        }
        ++it;
    }
    return double(trueTotal) / double(trueTotal+falseTotal);
}
double Node::confusion(Iterator fir, Iterator las){
    double rate = trueOrFalseRate(fir, las);
    return 2*rate*(1-rate);
}
//member functions
double Node::trueOrFalseRate() const{
    Iterator it = first;
    int trueTotal = 0, falseTotal = 0;
    while(it != last){
        switch(int((*it)[0])){
            case +1:
                ++trueTotal;
                break;
            case -1:
                ++falseTotal;
                break;
        }
        ++it;
    }
    return double(trueTotal) / double(trueTotal+falseTotal);
}
double Node::confusion() const{
    double rate = trueOrFalseRate();
    return 2*rate*(1-rate);
}
/*
double DecisionTree::confusionOfBranching(int fea, double cri){
    Iterator sep = partition_point(first, last, [&fea, &cri](double* const &a){return a[fea] < cri;});
    if(first != last){
        return (confusion(first, sep)*distance(first, sep) + confusion(sep, last)*distance(sep, last)) / distance(first, last);
    }
    else{
        std::cout << "Error: divide distance(first, last) with zero value" << std::endl;
        return 0.;
    }
}

double DecisionTree::confusionOfBranching(){
    return this->confusionOfBranching(featuresNum, criterion);
}
*/
void Node::sortFeature(int const& feaNum){
    stable_sort(first, last, [&feaNum](double* const &a, double* const &b){return a[feaNum] < b[feaNum];}); 
}
double Node::minConfusionWithFeature(int const& feaNum, double& bestCriterion){
    sortFeature(feaNum);

    //construct criterions array
    double criterions[MAX_FEATURE] = {0.};
    criterions[0] = (*first)[feaNum] - 1.;
    int countDiff = 1;

    double lastFeatureValue = (*first)[feaNum];
    double featureValue; 
    for(Iterator it = first; it != last; ++it){
        featureValue = (*it)[feaNum];
        if(lastFeatureValue != featureValue){
            criterions[countDiff] = (lastFeatureValue + featureValue)/2.;
            ++countDiff;
            lastFeatureValue = featureValue;
        }
    }
    criterions[countDiff] = lastFeatureValue + 1.;
    ++countDiff;

    //Cumulatively count yes and no for every criterion
    int yesCumul[MAX_FEATURE] = {0}, noCumul[MAX_FEATURE] = {0};
    int countYes = 0, countNo = 0;
    int criterionIter = 0;
    for(Iterator it = first; it != last; ++it){
        if(int(**it) == +1){
            ++countYes;
        }
        else if(int(**it) == -1){
            ++countNo;
        }

        if((*it)[feaNum] >= criterions[criterionIter]){
            ++criterionIter;
        }
        yesCumul[criterionIter] = countYes;
        noCumul[criterionIter] = countNo;
    }

    //calculate and find smallest confusion
    int yesTotal = yesCumul[criterionIter];
    int noTotal = noCumul[criterionIter];
    bestCriterion = criterions[0];
    double minConfusion = 1.;
    double confu = 1.;
    for(int i = 0; i <= criterionIter; ++i){
        //if not a trivial divide
        if(!(yesCumul[i]+noCumul[i]==0) && !(yesCumul[i]+noCumul[i]==yesTotal+noTotal)){
            confu = confusionTotal(yesCumul[i], noCumul[i], yesTotal-yesCumul[i], noTotal-noCumul[i]); 
        }
        else continue;
        if(confu < minConfusion){
            minConfusion = confu;
            bestCriterion = criterions[i];
        }
    }
    if(minConfusion <= 0.5)
        return minConfusion;
    else
        return 2.;
}
DecisionTree::~DecisionTree(){
    Node* p = root;
    while(true){
        while(!p->isExternal()){
            if(p->left != 0){
                p = p->left;
            }
            else{
                p = p->right;
            }
        }
        if(p->isRoot()) 
            break;
        p = p->parent;
        if(p->left != 0){
            delete p->left;
        }
        else if(p->right != 0){
            delete p->right;
        }
    }
    delete p;
}
double DecisionTree::setBestCriterion(Node* p){
    double minConfusion = 1.;
    double crit;
    for(int feaNum = 1; feaNum <= featuresTotal; ++feaNum){
        //debug
        //double f = p->featuresNum;
        //p->featuresNum = feaNum;
        //std::cout << *p << std::endl;
        //p->featuresNum = f;
        //~debug
        double confu = p->minConfusionWithFeature(feaNum, crit);
        if(confu < minConfusion){
            minConfusion = confu;
            p->criterion = crit;
            p->featuresNum = feaNum;
        }
    }
    //if(minConfusion > 0.5) std::cout << "XXX" <<std::endl;
    return minConfusion;
}
Node* DecisionTree::makeDecisionTree(Node* p){
    double toConfu = setBestCriterion(p);
    double confu = p->confusion();
    //if(toConfu > 0.5 && confu >tolerance){ 
    //    std::cout << ">>>>" << *p << std::endl;
    //    setBestCriterion(p);
    //}
    if(confu <= tolerance || toConfu > 0.5){
        p->decision = (p->trueOrFalseRate() >= 0.5) ? +1 : -1;
    //    std::cout << *p << std::endl;
        return p;
    }
    else{
        p->sortFeature(p->featuresNum);
        Iterator sep = partition_point(p->first, p->last, [&p](double* const &a){return a[p->featuresNum] < p->criterion;});
        //std::cout << *p << std::endl;
        if(p->first != sep && p->last != sep){
            p->left = makeDecisionTree(new Node(p, p->first, sep));
            p->right = makeDecisionTree(new Node(p, sep, p->last));
        }
        return p;
    }
} 

int DecisionTree::tree_predict(Node* p, double *attr){
    if(p->isExternal()){
        return p->decision;
    }
    else{
        tree_predict(attr[p->featuresNum] < p->criterion ? p->left : p->right, attr);
    }
    return 0;
}
void printTab(int const& it = 0){
    for(int i = 0; i<it; ++i){
        std::cout << "    ";
    }
}

void printTreePredict(DecisionTree const& tree, Node* p, int num, int it){
    if(it == 0){
        if(num == 0){
            std::cout << "int tree_predict(double *attr){" << std::endl; 
        }
        else{
            std::cout << "int tree_predict" << num << "(double *attr){" << std::endl; 
        }
        ++it;
    }
    printTab(it);
    if(p->isExternal()){
        std::cout << "return " << p->getDecision() << ";" << std::endl;
    }
    else{
        std::cout << "if(attr[" << p->getFeaturesNum() << "] < " << p->getCriterion() << "){" << std::endl;
        printTreePredict(tree, p->getLeft(), 0, it+1);
        printTab(it);
        std::cout << "}" << std::endl;
        printTab(it);
        std::cout << "else {" << std::endl;
        printTreePredict(tree, p->getRight(), 0, it+1);

        printTab(it);
        std::cout << "}\n";
    }
    if(it == 1){
        std::cout << "}" << std::endl;
        --it;
    }
}
void printForestPredict(DecisionTree** forest, int size){
    for(int i = 0; i<size; ++i){
        printTreePredict(*forest[i], forest[i]->getRoot(), i+1);
    }
    std::cout << "int forest_predict(double *attr){" << std::endl; 
    std::cout << "    int vote = 0;" << std::endl;
    for(int i = 0; i<size; ++i){
        std::cout << "    vote += tree_predict" << i+1 << "(attr);" << std::endl;
    }
    std::cout << "    if(vote == 0)" << std::endl;
    std::cout << "        return tree_predict" << (size+1)/2 << "(attr);" << std::endl;
    std::cout << "    else" << std::endl;
    std::cout << "        return vote>0 ? +1 : -1;" << std::endl << "}" << std::endl;
}
std::ostream& operator<<(std::ostream& out, const Node& node){
    if(node.isExternal() && node.decision != 0)
        std::cout << "decision: " << node.decision << '\t'<< "confusion: " << node.confusion() << "\t" << "values: ";
    else
        std::cout << "featuresNum: " << node.featuresNum << '\t' << "criterion: " << node.criterion << "\t" 
                  << "confusion: " << node.confusion() << "\t" << "values: ";

    for(Iterator it = node.first; it != node.last; ++it){
        if(int((*it)[0]) == +1)
            std::cout << '+';
        else
            std::cout << '-';
        std::cout << (*it)[node.featuresNum] << ' ';
    }
    return out;
}

void printDecisionTree(DecisionTree const& tree, Node* p, int it = 0){
    for(int i = 0; i<it; ++i){
        std::cout << '\t';
    }
    std::cout << *p;
    if(p->isExternal()){
        std::cout << std::endl;
    }
    else{
        ++it;
        std::cout << "(\n";
        if(p->getLeft() != 0){
            printDecisionTree(tree, p->getLeft(), it);
        }
        if(p->getRight() != 0){
            printDecisionTree(tree, p->getRight(), it);
        }
        --it;
        for(int i = 0; i<it; ++i){
            std::cout << '\t';
        }
        std::cout << ")\n";
    }
}

std::ostream& operator<<(std::ostream& out, const DecisionTree& tree){
    printDecisionTree(tree, tree.root);
    return out;
}

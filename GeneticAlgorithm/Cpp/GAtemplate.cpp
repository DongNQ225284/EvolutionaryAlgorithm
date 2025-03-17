#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define rand() dis(gen)
using namespace std;

static random_device rd;
static mt19937 gen(rd());

class Problem {
private:
    
public:
    
};

vector<bool> decode(vector<float> chromosome, Problem problem) {
    // pass
}

double get_fitness(vector<float> chromosome, Problem problem) {
    vector<bool> solution = decode(chromosome, problem);
    // pass
}

class Individual {
private:
    vector<float> chromosome; 
    double fitness;
public:
    void genIndi(Problem problem) {
       // pass
    }

    void cal_fitness(Problem problem) {
        this->fitness = get_fitness(this->chromosome, problem);
    }

    size_t getLength() {
        return this->chromosome.size();
    }

    vector<float> getChromosome() {
        return this->chromosome;
    }
    void setChromosome(vector<float> chromosome) {
        this->chromosome = chromosome;
    }

    //lấy giá trị thích nghi
    double getFitness() {
        return this->fitness;
    }
    //hiển thị thông tin của cá thể
    void show() {
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (float it : chromosome) printf("%.2lf ", it); printf("}\n");
    }
};

pair<Individual, Individual> crossover(Individual parent1, Individual parent2) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    pair<Individual, Individual> off;
    // pass
    return off;
}

mutation(Individual& indi) {
    // pass
}

class Population {
private:
    vector<Individual> list_indi;
    Problem problem;
    size_t pop_size;
public:
    Population(size_t pop_size, Problem problem) {
        this->problem = problem;
        this->pop_size = pop_size;
    }
    
    void genPop() {
        for (size_t i = 0; i != pop_size; i++) {
            Individual indi;
            indi.genIndi(problem);
            indi.cal_fitness(problem);
            list_indi.push_back(indi);
        }
    }

    size_t getSize() {
        return this->pop_size;
    }

    vector<Individual> getList() {
        return this->list_indi;
    }
    void setList(vector<Individual> list) {
        this->list_indi = list;
    }
    
    void show() {
        cout << "size of populaiton: " << pop_size << endl;
        for (Individual indi : list_indi) {
            indi.show();
        }
    }
};

pair<Individual, Individual> choice(vector<Individual> list) {
    // pass
}

vector<Individual> nextGen(vector<Individual> list, size_t pop_size) {
    // pass
}

pair<vector<double>, Individual> geneticalgorithm(Problem problem, //môi trường sống
                            size_t pop_size, //số lượng cá thể trong môi trường
                            size_t max_gen, //số lượng thế hệ
                            float p_c, //tỷ lệ lai tạo
                            float p_m //tỷ lệ đột biến
                            )  
{
    // char chr;    
    uniform_real_distribution<float> dis(0.0, 1.0);
    Population pop(pop_size, problem);
    pop.genPop();
    vector<double> history;
    for (size_t i = 0; i != max_gen; i++) {
        vector<Individual> child;
        while (child.size() < pop_size) {
            pair<Individual, Individual> parent = choice(pop.getList());
            pair<Individual, Individual> off;
            if (rand() <= p_c) {
                off = crossover(parent.first, parent.second);
                off.first.cal_fitness(problem);
                off.second.cal_fitness(problem);
                child.push_back(off.first);
                child.push_back(off.second);
            }
            if (rand() <= p_m) {
                mutation(parent.first);
                mutation(parent.second);
                parent.first.cal_fitness(problem);
                parent.second.cal_fitness(problem);
                child.push_back(parent.second);
                child.push_back(parent.second);   
            }
        }
        vector<Individual> list = pop.getList();
        for (Individual indi : child) {
            list.push_back(indi);
        }
        list = nextGen(list, pop_size);
        pop.setList(list);
        history.push_back(list[0].getFitness());
    }
    vector<Individual> list = pop.getList();
    return make_pair(history, list[0]);
}


int main() {
    //setup

    //start


    //show

    return 0;
}
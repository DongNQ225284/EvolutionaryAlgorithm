#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <fstream>

#define rand() dis(gen)
using namespace std;

static random_device rd;
static mt19937 gen(rd());

class Problem {
private:
    vector<vector<double>> cost;
public:
    Problem() {
        return;
    }
    Problem(vector<vector<double>> cost) {
        this->cost = cost;
    }
    size_t getSize() {
        return this->cost.size();
    }
    vector<vector<double>> getCost() {
        return this->cost;
    }
};

vector<size_t> decode(vector<float> chromosome, Problem problem) {
    vector<size_t> indices;
    size_t N = chromosome.size();
    for (size_t i = 0; i != N; i++) {
        indices.push_back(i);
    }
    sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return chromosome[a] < chromosome[b];
    });
    vector<size_t> solution(chromosome.size());
    for (size_t i = 0; i != N; i++) {
        solution[indices[i]] = i + 1;
    }
    return solution;
}

double get_fitness(vector<float> chromosome, Problem problem) {
    vector<size_t> city = decode(chromosome, problem);
    //for (size_t val : city) cout << val << " "; cout << endl;
    vector<vector<double>> cost = problem.getCost();
    size_t N = city.size();
    double total_cost = cost[0][city[0]];
    for (size_t i = 1; i != N; i++) {
        total_cost += cost[city[i - 1]][city[i]];
    }
    total_cost += cost[city[N - 1]][0];
    return -total_cost;
}

class Individual {
private:
    vector<float> chromosome; 
    double fitness;
public:
    void genIndi(Problem problem) {
        uniform_real_distribution<float> dis(0.0, 1.0);
        size_t N = problem.getSize();
        for (size_t i = 0; i != N - 1; i++) {
            chromosome.push_back(rand());
        }
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
    double getFitness() {
        return this->fitness;
    }
    void show() {
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (float it : chromosome) printf("%.2lf ", it); printf("}\n");
    }
};

//simulated binary crossover
pair<Individual, Individual> crossover(Individual parent1, Individual parent2, double eta = 2.0) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    float r = rand();
    double beta;
    if (r <= 0.5) {
        beta = pow(2*r, 1.0/(eta + 1.0));
    } else {
        beta = pow(0.5/(1.0 - r), 1.0/(eta + 1.0));
    }
    vector<float> p1 = parent1.getChromosome();
    vector<float> p2 = parent2.getChromosome();
    vector<float> c1(p1.size());
    vector<float> c2(p2.size());
    for (size_t i = 0; i != p1.size(); i++) {
        c1[i] = float(0.5 * ((1 + beta) * p1[i] + (1 - beta) * p2[i]));
        c2[i] = float(0.5 * ((1 - beta) * p1[i] + (1 + beta) * p2[i]));
    }
    Individual off1, off2;
    off1.setChromosome(c1);
    off2.setChromosome(c2);
    return make_pair(off1, off2);
}

//polynomail mutaion
void mutation(Individual& indi, double eta = 20) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    vector<float> chr = indi.getChromosome();
    double delta;
    for (size_t i = 0; i != chr.size(); i++) {
        double mu = rand();
        if (mu <= 0.5) {
            delta = pow(2 * mu, 1.0/(eta + 1)) - 1;
            chr[i] = (float)(chr[i] + delta * chr[i]);
            chr[i] = max(min(chr[i], 1.0f), 0.0f);
        } else {
            delta = 1 - pow(2 - 2 * mu, 1.0/(eta + 1));
            chr[i] = (float)(chr[i] + delta * (1 - chr[i]));
            chr[i] = max(min(chr[i], 1.0f), 0.0f);
        }
    }
    indi.setChromosome(chr);
    return;
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

pair<Individual, Individual> choice(vector<Individual> list, size_t k = 4) {
    uniform_int_distribution<size_t> dis(0, list.size() - 1);
    vector<Individual> tour1, tour2;
    vector<bool> mark(list.size(), false);
    size_t num = k;
    while(num > 0) {
        size_t p = rand();
        if (!mark[p]) {
            tour1.push_back(list[p]);
            mark[p] = true;
            num--;
        }
    }
    mark.assign(list.size(), false);
    num = k;
    while(num > 0) {
        size_t p = rand();
        if (!mark[p]) {
            tour2.push_back(list[p]);
            mark[p] = true;
            num--;
        }
    }
    Individual x = tour1[0];
    for (Individual indi : tour1) {
        if (x.getFitness() < indi.getFitness()) {
            x = indi;
        }
    }
    Individual y = tour2[0];
    for (Individual indi : tour2) {
        if (y.getFitness() < indi.getFitness()) {
            y = indi;
        }
    }
    return make_pair(x, y);
}

vector<Individual> nextGen(vector<Individual> list, size_t pop_size) {
    sort(list.begin(), list.end(), [&](Individual a, Individual b) {
        return a.getFitness() > b.getFitness();
    });
    vector<Individual> newList;
    for (size_t i = 0; i != pop_size; i++) {
        newList.push_back(list[i]);
    }
    return newList;
}

pair<vector<double>, Individual> GA(Problem problem, //môi trường sống
                            size_t pop_size, //số lượng cá thể trong môi trường
                            size_t max_gen, //số lượng thế hệ
                            float p_c, //tỷ lệ lai tạo
                            float p_m //tỷ lệ đột biến
                            )  
{ 
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
    ifstream file("matrix.txt");

    size_t n; file >> n;
    vector<vector<double>> cost(n, vector<double>(n));
    for (size_t i = 0; i != n; i++) {
        for (size_t j = 0; j !=n ; j++) {
            file >> cost[i][j];
        }
    }

    Problem problem(cost);
    size_t pop_size = 200;
    size_t max_gen = 500;
    float Pc = 0.9;
    float Pm = 0.3;
    
    //start
    pair<vector<double>, Individual> result;
    result = GA(problem, pop_size, max_gen, Pc, Pm);

    vector<double> fitness_history = result.first;
    Individual best = result.second;

    for (size_t i = 0; i != fitness_history.size(); i++) {
        printf("Generation %d, bestfitness = %lf\n", i, fitness_history[i]);
    }
    vector<size_t> solution = decode(best.getChromosome(), problem);
    //show
    printf("Problem:\n");
    for (size_t i = 0; i != cost.size(); i++) {
        for (double val : cost[i]) printf("%.2lf\t", val); cout << endl;
    }
    cout << endl;
    printf("solution: "); for (size_t val : solution) printf("%d\t", val); cout << endl;
    printf("total_cost = %.2lf\n", -best.getFitness());
    return 0;
}
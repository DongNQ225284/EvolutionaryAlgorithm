#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

static random_device rd;
static mt19937 gen(rd());

//vấn đề
class Problem {
private:
    vector<double> profit;
    vector<double> weight;
    double capacity;
public:
    Problem() {
        capacity = 0;
    }
    Problem(vector<double> profit, vector<double> weight, double capacity) {
        if (profit.empty() || weight.empty()) {
            cout << "empty" << endl;
            return;
        }
        this->profit = profit;
        this->weight = weight;
        this->capacity = capacity;
    }
    vector<double> getprofit() {
        return this->profit;
    }
    vector<double> getWeight() {
        return this->weight;
    }
    double getCapacity() {
        return this->capacity;
    }
    size_t getLength() {
        return this->profit.size();
    }
    void show() {
        printf("profit: "); for (double it: profit) printf("\t%.2lf\t", it); cout << endl;
        printf("weight: "); for (double it: weight) printf("\t%.2lf\t", it); cout << endl;
        printf("capacity = %.2lf\n", capacity);
    }
};

class Individual {
private:
    //nhiễm sắc thể
    vector<bool> chromosome; 
    //giá trị thích nghi
    double fitness;
public:
    void init(Problem environment) {
        size_t length = environment.getLength(); 
        uniform_int_distribution<int> dis(0, 1);
        for (size_t i = 0; i != length; i++) {
            float r = dis(gen);
            chromosome.push_back(r);
        }
    }
    //tính giá trị thích nghi
    void calculateFitness(Problem environment) {
        double total_profit = 0;
        double total_weight = 0;
        size_t length = environment.getLength();
        vector<double> profit = environment.getprofit();
        vector<double> weight = environment.getWeight();
        for (size_t i = 0; i != length; i++) {
            if (chromosome[i]) {
                total_profit += profit[i];
                total_weight += weight[i];
            }
        }
        if (total_weight > environment.getCapacity()) {
            this->fitness = 0;
        } else {
            this->fitness = total_profit; 
        } 
    }

    //lấy chiều dài của nhiễn sắc thể
    size_t getLength() {
        return this->chromosome.size();
    }
    //lấy nhiễm sắc thể
    vector<bool> getChromosome() {
        return this->chromosome;
    }
    void setChromosome(vector<bool> chromosome) {
        this->chromosome = chromosome;
    }
    //lấy giá trị thích nghi
    double getFitness() {
        return this->fitness;
    }
    //hiển thị thông tin của cá thể
    void show() {
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (bool it : chromosome) printf("%d ", it); printf("}\n");
    }
};

//toán tử lai ghép, sử dụng lai ghép đồng nhất uniform crossover, xác suất hoán đổi là p_c
vector<Individual> crossover(Individual p1, Individual p2, float p_c = 0.2) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    vector<Individual> offspring;
    size_t length = p1.getLength();
    
    vector<bool> chromosome1 = p1.getChromosome();
    vector<bool> chromosome2 = p2.getChromosome();

    for (size_t i = 0; i != length; i++) {
        float r_c = dis(gen);
        if (r_c <= p_c) {
            swap(chromosome1[i], chromosome2[i]);
        }    
    }

    Individual off1, off2;
    off1.setChromosome(chromosome1);
    off2.setChromosome(chromosome2);
    
    offspring.push_back(off1);
    offspring.push_back(off2);
    return offspring;
}

//toán tử đột biến sử dụng đột biến đảo bit với xác suất đột biến mỗi bit là p_m
void mutation(Individual& member, float p_m = 0.2) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    vector<bool> chromosome = member.getChromosome();
    size_t length = member.getLength();
    for (size_t i = 0; i != length; i++) {
        float r_m = dis(gen);
        if (r_m <= p_m) {
            chromosome[i] = !chromosome[i];
        }
    }
    member.setChromosome(chromosome);
}

class Population {
private:
    //danh sách cá thể trong quần thể
    vector<Individual> list;
    //cá thể ưu tú
    vector<Individual> parent;
    //môi trường sống
    Problem environment;
public:
    Population(Problem environment) {
        this->environment = environment;
    }
    
    //khởi tạo quần thể với n cá thể
    void init(size_t n) {
        for (size_t i = 0; i != n; i++) {
            Individual member;
            member.init(environment);
            member.calculateFitness(environment);
            list.push_back(member);
        }
    }
    //lấy kích thước của quần thể
    size_t getSize() {
        return this->list.size();
    }
    //lấy danh sách các cá thể
    vector<Individual> getList() {
        return this->list;
    }
    //thêm một cá thể vào trong danh sách
    void append(Individual member) {
        this->list.push_back(member);
    }
    //lấy danh sách cha mẹ, nếu rỗng, lấy ra number_of_parent cá thể ngẫu nhiên
    vector<Individual> getParent(size_t number_of_parent = 2) {
        if (!parent.empty()) return this->parent;
        size_t N = list.size();
        uniform_int_distribution<size_t> dis(0, N - 1);
        vector<bool> mark(N, false);
        while (number_of_parent > 0) {
            size_t idx = dis(gen);
            if (!mark[idx]) {
                parent.push_back(list[idx]);
                mark[idx] = true;
                number_of_parent--;
            }
        }
        return parent;
    } 
    //chọn lọc sinh tồn, loại bỏ để còn n cá thể, sử dụng chọn lọc xếp hạng
    void selection(size_t n, size_t number_of_parent = 2) {
        uniform_real_distribution<float> dis(0.0, 1.0);
        size_t N = list.size();
        vector<size_t> rank;
        for (int i = 0; i != N; i++) {
            rank.push_back(i);
        }
        sort(rank.begin(), rank.end(), [&] (size_t a, size_t b) {
            return list[a].getFitness() < list[b].getFitness();
        });
        vector<double> S;
        S.push_back(1);
        for (size_t i = 1; i != N; i++) {
            S.push_back(S[i - 1] + i + 1);
        }
        double sum = S[N - 1];
        vector<bool> mark(N, false);
        vector<Individual> new_generation;
        parent.clear();
        while (n > 0) {
            float r = dis(gen);
            size_t idx = upper_bound(S.begin(), S.end(), r * sum) - S.begin();

            if (!mark[idx]) {
                new_generation.push_back(list[rank[idx]]);
                if (number_of_parent > 0) {
                    parent.push_back(list[rank[idx]]);
                    number_of_parent--;
                }
                mark[idx] = true;
                n--;
            }
        }
        list = new_generation;
    }
    //hiển thị thông tin về quần thể
    void show() {
        cout << "size of populaiton: " << list.size() << endl;
        for (size_t i = 0; i != list.size(); i++) {
            list[i].show();
        }
    }
};

void print(vector<Individual> list) {
    for (size_t i = 0; i != list.size(); i++) {
        list[i].show();
    }
}
//Thuật toán GA trả về cá thể tốt nhất
Individual geneticalgorithm(Problem environment, //môi trường sống
                            size_t number_of_individuals, //số lượng cá thể trong môi trường
                            size_t number_of_generation, //số lượng thế hệ
                            float p_c, //tỷ lệ lai tạo
                            float p_m //tỷ lệ đột biến
                            )  
{
    // char chr;    
    uniform_real_distribution<float> dis(0.0, 1.0);
    Individual best;
    Population population(environment);
    population.init(number_of_individuals);

    while (number_of_generation > 0) {
        vector<Individual> parent = population.getParent(2);
        // cout << "population:" << endl;
        // population.show();
        // cout << "parent:" << endl;
        // print(parent);
        // cin >> chr;
        size_t number_of_offspring = number_of_individuals;
        while (number_of_offspring > 0) {
            float r_c = dis(gen);
            if (r_c <= p_c) {
                vector<Individual> offspring = crossover(parent[0], parent[1], p_c);
                float r_m = dis(gen);
                if (r_m <= p_m) {
                    mutation(offspring[0]);
                    mutation(offspring[1]);
                }
                offspring[0].calculateFitness(environment);
                offspring[1].calculateFitness(environment);
                // cout << "offspring: " << endl;
                // print(offspring);
                // cin >> chr;
                population.append(offspring[0]);
                number_of_offspring--;

                population.append(offspring[1]);
                number_of_offspring--;
            }
        }
        // cout << "population:" << endl;
        // population.show();
        // cin >> chr;
        population.selection(number_of_individuals);
        number_of_generation--;
    }
    //chọn ra cá thể có giá trị thích nghi tốt nhất
    vector<Individual> list = population.getList();
    best = list[0];
    for (size_t i = 1; i != list.size(); i++) {
        if (list[i].getFitness() > best.getFitness()) {
            best = list[i];
        }
    } 
    return best;
}

int main() {
    vector<double> profit = {5, 7, 6, 8};
    vector<double> weight = {7, 4, 3, 4};
    double capacity = 9;

    Problem KnapsackProblem(profit, weight, capacity);
    size_t number_of_individuals = 10;
    size_t number_of_generation = 1000;
    float p_c = 0.2;
    float p_m = 0.3;

    Individual best = geneticalgorithm(KnapsackProblem, number_of_individuals, number_of_generation, p_c, p_m);
    
    cout << "Knapsack Problem:" << endl;
    KnapsackProblem.show();
    cout << endl;
    cout << "Solution:" << endl;
    best.show();
    
    return 0;
}

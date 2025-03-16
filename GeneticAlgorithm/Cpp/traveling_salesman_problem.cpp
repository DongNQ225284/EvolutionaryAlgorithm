#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
using namespace std;

static random_device rd;
static mt19937 gen(rd());

//vấn đề
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
    size_t getLength() {
        return this->cost.size();
    }
    vector<vector<double>> getCost() {
        return this->cost;
    }
    void show() {
        cout << "cost:" << endl;
        size_t N = cost.size();
        for (size_t i = 0; i != N; i++) {
            for (double it : cost[i]) printf("%.2lf\t", it); cout << endl;
        }
    }1```
};

class Individual {
private:
    //nhiễm sắc thể sử dụng mã hóa hoán vị
    vector<size_t> chromosome; 
    //giá trị thích nghi
    double fitness;
public:
    void init(Problem environment) {
        size_t length = environment.getLength(); 
        uniform_int_distribution<size_t> dis(1, length - 1);
        vector<bool> mark(length, false);

        while (length - 1 > 0) {
            size_t r = dis(gen);
            if (!mark[r]) {
                chromosome.push_back(r);
                mark[r] = true;
                length--;
            }
        }
    }
    //tính giá trị thích nghi, thiết lập fitness = - total_cost
    void calculateFitness(Problem environment) {
        vector<vector<double>> cost = environment.getCost();
        size_t N = chromosome.size();
        
        double total_cost = 0;
        total_cost += cost[0][chromosome[1]];
        for (size_t i = 1; i != N; i++) {
            total_cost += cost[chromosome[i - 1]][chromosome[i]];
        }
        total_cost += cost[chromosome[N - 1]][0];
        
        this->fitness = - total_cost;
    }

    //lấy chiều dài của nhiễn sắc thể
    size_t getLength() {
        return this->chromosome.size();
    }
    //lấy nhiễm sắc thể
    vector<size_t> getChromosome() {
        return this->chromosome;
    }
    void setChromosome(vector<size_t> chromosome) {
        this->chromosome = chromosome;
    }
    //lấy giá trị thích nghi
    double getFitness() {
        return this->fitness;
    }
    //hiển thị thông tin của cá thể
    void show() {
        printf("fitness = %.2lf\t\t", fitness);
        printf("chormosome: { "); for (size_t it : chromosome) printf(" %d ", it); printf("}\n");
    }
};

//toán tử lai ghép, sử dụng lai ghép thứ tự (order crossover)
vector<Individual> crossover(Individual p1, Individual p2) {
    vector<Individual> offspring;
    size_t length = p1.getLength();
    
    uniform_int_distribution<size_t> dis(0, length - 1);
    size_t L = dis(gen);
    size_t R;
    do {
        R = dis(gen);
    } while (R == L);

    if (L > R) swap(L, R);

    //sao chép gen của cha mẹ ở đoạn [L, R) 
    vector<size_t> temp1 = p1.getChromosome();
    vector<size_t> temp2 = p2.getChromosome();
    vector<size_t> chromosome1(length);
    vector<size_t> chromosome2(length);
    
    vector<bool> mark1(length, false);
    vector<bool> mark2(length, false);

    for (size_t i = L; i != R; i++) {
        chromosome1[i] = temp1[i];
        chromosome2[i] = temp2[i];
        mark1[temp1[i]] = true;
        mark2[temp2[i]] = true;
    }

    // printf("L = %d, R = %d\n", L, R);
    // cout << "step 1" << endl;
    // for (size_t it : chromosome1) cout << it << " "; cout << endl;
    // for (size_t it : chromosome2) cout << it << " "; cout << endl;
    
    //xếp các gen còn lại từ p2 vào chromosome1 và từ p1 vào chromosome2
    int pos;
    pos = R;
    for (size_t i = R; i % length != L; i++) {
        while (mark1[temp2[pos]]) {
            pos++;
            pos %= length;
        }
        chromosome1[i % length] = temp2[pos];
        mark1[temp2[pos]] = true;
    }
    pos = R;
    for (size_t i = R; i % length != L; i++) {
        while (mark2[temp1[pos]]) {
            pos++;
            pos %= length;
        }
        chromosome2[i % length] = temp1[pos];
        mark2[temp1[pos]] = true;
    }

    // cout << "step 2" << endl;
    // for (size_t it : chromosome1) cout << it << " "; cout << endl;
    // for (size_t it : chromosome2) cout << it << " "; cout << endl;

    Individual off1, off2;
    off1.setChromosome(chromosome1);
    off2.setChromosome(chromosome2);

    offspring.push_back(off1);
    offspring.push_back(off2);
    return offspring;
}

//toán tử đột biến sử dụng đột biến đảo ngược (Inversion mutation)
void mutation(Individual& member) {
    size_t length = member.getLength();
    vector<size_t> chromosome = member.getChromosome();
    uniform_int_distribution<size_t> dis(0, length - 1);
    size_t L = dis(gen), R = dis(gen);
    if (L > R) swap(L, R);
    swap(chromosome[L], chromosome[R]);
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
    
    //lấy danh sách cha mẹ
    vector<Individual> getParent(size_t number_of_parent = 2) {
        if (!parent.empty()) return this->parent;
        size_t N = list.size();
        uniform_int_distribution<size_t> dis(0, N - 1);
        vector<bool> mark(N, false);
        while (number_of_parent > 0) {
            size_t idx = dis(gen);
            if (!mark[idx]) {
                this->parent.push_back(list[idx]);
                mark[idx] = true;
                number_of_parent--;
            }
        }
        return this->parent;
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
    char chr;    
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
                vector<Individual> offspring = crossover(parent[0], parent[1]);
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
    ifstream file("matrix.txt");

    size_t n; file >> n;
    vector<vector<double>> cost(n, vector<double>(n));
    for (size_t i = 0; i != n; i++) {
        for (size_t j = 0; j !=n ; j++) {
            file >> cost[i][j];
        }
    }

    Problem TravelingSalesmanProblem(cost);

    size_t number_of_individuals = 8;
    size_t number_of_generation = 3000;
    float p_c = 0.9;
    float p_m = 0.3;

    Individual best = geneticalgorithm(TravelingSalesmanProblem, number_of_individuals, number_of_generation, p_c, p_m);
    TravelingSalesmanProblem.show();
    best.show();

    return 0;
}
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
    
public:
    
};

class Individual {
private:
    vector<bool> chromosome; 
    double fitness;
    Problem problem;

public:
    void init(Problem Problem) {
       
    }
    //tính giá trị thích nghi
    void calculateFitness(Problem environment) {
        
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
        printf("chormosome: { "); for (bool it : chromosome) printf("%.2lf ", it); printf("}\n");
    }
};

//toán tử lai ghép, sử dụng lai ghép đồng nhất uniform crossover, xác suất hoán đổi là p_c
vector<Individual> crossover(Individual p1, Individual p2, float p_c = 0.2) {
    uniform_real_distribution<float> dis(0.0, 1.0);
    vector<Individual> offspring;
    //
    return offspring;
}

//toán tử đột biến sử dụng đột biến đảo bit với xác suất đột biến mỗi bit là p_m
void mutation(Individual& member, float p_m = 0.2) {
    //
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
    
    //lấy danh sách cha mẹ, nếu rỗng
    vector<Individual> getParent(size_t number_of_parent = 2) {
        return this->parent;
    } 

    //chọn lọc sinh tồn, loại bỏ để còn n cá thể
    void selection(size_t n, size_t number_of_parent = 2) {
       
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
    
}
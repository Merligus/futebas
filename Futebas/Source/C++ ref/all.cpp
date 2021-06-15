#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <random>
#include "all.h"

using namespace std;
default_random_engine generator;

Objeto::OptionEnum draft_random_enum()
{
    std::uniform_int_distribution<int> enumerated_rand(0, 5);
    return static_cast<Objeto::OptionEnum>(enumerated_rand(generator) % (Objeto::OptionEnum::FazerNada+1));
}

int main(int argc, char **argv)
{
    int i = atoi(argv[1]);
    std::string name(argv[2]);
    Objeto obj((float)i/2, (float)i*1.5, i, name);
    Objeto obj2((float)i/3, (float)i*1.5, i, name + "_append");
    bool igual, menor, maior, maiorI, menorI;
    uniform_real_distribution<float> distribution_rand(0., 1.);

    igual = obj == obj2;
    menor = obj < obj2;
    maior = obj > obj2;
    maiorI = obj >= obj2;
    menorI = obj <= obj2;
    
    if (igual)
    {
        cout << "Igual";
        if (menor)
            cout << " e menor\n";
        else if (maior)
            cout << " e maior\n";
        else
            cout << "\n";
    }
    if (menorI)
        cout << "Menor igual\n";
    else if (maiorI)
        cout << "Maior igual\n";
    else
        cout << "Nada\n";

    obj.fileProcess(draft_random_enum(), "my_file.txt");
    obj.fileProcess(draft_random_enum(), "my_file.txt");
    obj.fileProcess(draft_random_enum(), "my_file.txt");
    obj.fileProcess(draft_random_enum(), "my_file.txt");
    obj.fileProcess(draft_random_enum(), "my_file.txt");
    obj.fileProcess(draft_random_enum(), "my_file.txt");
    
    printf("Random:\n\t%f\n", distribution_rand(generator));
    printf("\t%f\n", distribution_rand(generator));

    return 0;
}
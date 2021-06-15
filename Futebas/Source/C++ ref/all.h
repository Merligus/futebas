#include <iostream>
#include <string>

struct IMC
{
    float peso, altura;
    float valor;
};

class Objeto
{
private:
    int i;
    std::string s;
public:
    IMC peso_altura;
    enum OptionEnum{Ler = 0, Sobrescrever, EscreverApos, FazerNada};

    Objeto(float peso, float altura, int i, std::string s);
    ~Objeto();
    void fileProcess(OptionEnum o, const char* const &nome);
    bool operator==(Objeto& a);
    bool operator>(Objeto& a);
    bool operator>=(Objeto& a);
    bool operator<(Objeto& a);
    bool operator<=(Objeto& a);
};

Objeto::Objeto(float peso, float altura, int i, std::string si) : peso_altura{peso, altura}, i(i), s(si)
{
    this->peso_altura.valor = peso/altura;
    printf("Novo objeto: %.2f, %.2f, %.2f, %d, %s\n",
                                                        this->peso_altura.peso, 
                                                        this->peso_altura.altura, 
                                                        this->peso_altura.valor, 
                                                        this->i, 
                                                        this->s.begin());
}

Objeto::~Objeto()
{
    printf("Deletando objeto\n");
}

void Objeto::fileProcess(OptionEnum o, const char* const &nome)
{
    char buffer[100];
    FILE* f;

    switch (o)
    {
    case Ler:
        std::cout << "Objeto::fileProcess::Ler\n";
        f = fopen(nome, "r");
        if (f == NULL) perror ("Error opening file");
        else
        {
            while (!feof(f))
            {
                if (fgets(buffer , 100 , f) == NULL)
                    break;
                fputs(buffer, stdout);
            }
        }
        fclose(f);
        break;
    case EscreverApos:
        std::cout << "Objeto::fileProcess::EscreverApos\n";
        f = fopen(nome, "a");
        if (f == NULL) perror ("Error opening file");
        else
            fprintf(f, "%.2f\n%.2f\n%.2f\n%d\n%s\n", 
                                                        peso_altura.peso, 
                                                        peso_altura.altura, 
                                                        peso_altura.valor,
                                                        i,
                                                        s.begin());
        fclose(f);
        break;
    case Sobrescrever:
        std::cout << "Objeto::fileProcess::Sobrescrever\n";
        f = fopen(nome, "w");
        if (f == NULL) perror ("Error opening file");
        else
            fprintf(f, "%.2f\n%.2f\n%.2f\n%d\n%s\n", 
                                                        peso_altura.peso, 
                                                        peso_altura.altura, 
                                                        peso_altura.valor,
                                                        i,
                                                        s.begin());
        fclose(f);
        break;
    case FazerNada:
        std::cout << "Objeto::fileProcess::Nada\n";
        break;
    default:
        std::cout << "Objeto::fileProcess::Default\n";
        break;
    }
}

bool Objeto::operator==(Objeto& a)
{
    return this->peso_altura.peso == a.peso_altura.peso;
}

bool Objeto::operator>(Objeto& a)
{
    return this->peso_altura.peso > a.peso_altura.peso;
}

bool Objeto::operator>=(Objeto& a)
{
    return this->peso_altura.peso >= a.peso_altura.peso;
}

bool Objeto::operator<(Objeto& a)
{
    return this->peso_altura.peso < a.peso_altura.peso;
}

bool Objeto::operator<=(Objeto& a)
{
    return this->peso_altura.peso <= a.peso_altura.peso;
}
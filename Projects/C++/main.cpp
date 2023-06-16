#include <iostream>
#include <string>
#include <sstream>

unsigned long long fabada = 0xfabada;

/* 1) ¿Cómo agrego un "nueva línea" o enter +
   el valor de una variable numérica a una
   cadena de texto?
*/
template <typename T>
std::string agregarNum(const std::string &s, T num)
{
    std::string str{s};
    str += "\n" + std::to_string(num);
    return str;
}

/* 2) ¿Cómo puedo obtener ese valor numérico
      de la última línea de una cadena de texto?
*/
template <typename T>
T obtenerUltimoNum(const std::string &str)
{
    size_t pos = str.find_last_of("\n");
    if (pos != str.npos)
    {
        std::string sub = str.substr(pos + 1);
        std::stringstream ss(sub);
        T num;
        ss >> num;
        return num;
    }
    throw std::runtime_error("error: no hay un número al final de la cadena.");
    // o return T{}; al estilo de atoi()
    // y si esperás al C++17 std::optional.
}

int main()
{
    std::string cadena{"Hola mundo"};

    cadena = agregarNum(cadena, 4.5);

    std::cout << "cadena: " << cadena << '\n';

    double num = obtenerUltimoNum<double>(cadena);

    std::cout << "num: " << num << '\n';

    std::string cadena("Este es el texto inicial\n");
    std::stringstream s;
    s << cadena << "añadimos la fabada " << fabada << "\n\tEsto esta tabulado!";
    std::cout << s.str();
}
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

typedef const int& cint;

//данные перехода
struct smooth_trans_args {
    int st_h = 0; //высота перехода
    //int number_st = 0; //номер перехода (глобальный)
    int type_mat = 0; //тип материала/текстуры перехода
    int type_smooth = 0; //тип сглаживания
};

//данные шестиугольника
struct hex_args {
    int hex_h = 0; //высота (в 3D)
    int type_mat = 0; //тип материала/текстуры гекса
    std::array <int, 6> st = { 0,0,0,0,0,0 }; //адреса всех окружающих гекс переходов (индексы на массив переходов)
    std::array <int, 6> vertex = { 0,0,0,0,0,0 }; //адреса вершин (нужно ли хранить?)
    //hex_args() : hex_h(0), type_mat(0){}
};

class Hexbox {
public:
    int w; //ширина поля (OX)(в кол-ве гексов)
    int h; //высота(в 2D) поля (OY)(или длина...)(в кол-ве гексов)
    int count_st; //кол-во переходов
    int count_hex; //кол-во гексов
    std::vector <hex_args> hex_grid; //массив шестиугольников
    std::vector <smooth_trans_args>  st_grid; //массив переходов
    Hexbox(cint x, cint y) {
        w = x;
        h = y;
        count_hex = w * h;

        reserve_hex_grid(hex_grid, w, h);
        fe_hex_grid(w, h);
        reserve_st_grid(st_grid, w, h);
        fe_st_grid(w, h);
    };
    ~Hexbox() = default;
    hex_args GetHex(cint x, cint y); //возвращает гекс с координатами x, y
private:
    int number_ch(cint x, cint y, cint w); //номер гекса в массиве гексов(по x, y и ширине поля)(только если квадратное!)
    void reserve_hex_grid(std::vector <hex_args> h_g, cint x, cint y);
    void reserve_st_grid(std::vector <smooth_trans_args> st_g, cint x, cint y);
    void fe_hex_grid(cint x, cint y); //заполнение дефолтными значениями параметров каждого гекса
    void fe_st_grid(cint x, cint y); //заполнение дефолтными значениями параметров каждого перехода

    int number_st(cint x1, cint y1, cint x2, cint y2, cint w);
    int count_st_func(cint x, cint y); //кол-во всех переходов для сетки с данными размерами(по ширине и длине)(только если квадратное!)


};

//номер перехода в массиве переходов(по координатам двух смежных гексов и ширине поля)(только если квадратное!)
//Правила:
//1) номер первого гекса должен быть меньше второго
//2) если y1 = y2, то номер первого гекса должен быть меньше второго на единицу
//3) если пункт 2) не выполняется, то номер первого гекса должен быть меньше второго на ширину поля w или w + 1
//4) при невыполнении правил - бан (возвращает 0 
//     (может быть использовано как особый номер для индикации границ поля и его обработки(специальный переход для границы)))
inline int Hexbox::number_st(cint x1, cint y1, cint x2, cint y2, cint w)
{
    int ch1 = number_ch(x1, y1, w);
    int ch2 = number_ch(x2, y2, w);
    if (ch1 < ch2 || (ch1 + 1) == ch2 || (ch1 + w) == ch2 || (ch1 + w + 1) == ch2) {
        return (y2 - y1) * (ch1 + ch2 + 2 * y1)
            + (1 + y2 - y1) * (y1 * (3 * w) + x2);
    }
    else {
        return 0;
    }

}

inline int Hexbox::count_st_func(cint x, cint y)
{
    int c = (3 * x * y) - 2 * x - 2 * y + 1;
    return c;
}

inline hex_args Hexbox::GetHex(cint x, cint y)
{
    int n_ch = number_ch(x, y, w);
    return hex_grid[n_ch];
}

inline void Hexbox::fe_hex_grid(cint x, cint y)
{
    count_hex = x * y;
    hex_args def_ha = { 0, 0 };
    for (int i = 0; i < count_hex; i += 1) {
        hex_grid.push_back(def_ha);
    }
}
inline void Hexbox::fe_st_grid(cint x, cint y)
{
    count_st = Hexbox::count_st_func(x, y);
    smooth_trans_args def_st = { 0, 0, 0 };
    for (int i = 0; i < count_hex; i += 1) {
        st_grid.push_back(def_st);
    }
}


inline void Hexbox::reserve_hex_grid(std::vector<hex_args> h_g, cint x, cint y)
{
    //int count_hexes = x * y;
    count_hex = x * y;
    h_g.reserve(count_hex);
}

//резервирование памяти для массива перехода (по размеру поля) 
inline void Hexbox::reserve_st_grid(std::vector<smooth_trans_args> st_g, cint x, cint y)
{
    //int n_st = Hexbox::count_st_func(x, y);
    count_st = Hexbox::count_st_func(x, y);
    st_g.reserve(count_st);
}

int Hexbox::number_ch(cint x, cint y, cint w)
{
    return w * y + x;
}


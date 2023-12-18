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
    float x2d = 0;
    //int x2d = 0; //x в 2Д
    int y2d = 0; //y в 2Д
    int hex_h = 0; //высота (в 3D)
    int type_mat = 0; //тип материала/текстуры гекса 

    //std::array <int, 6> st = { 0,0,0,0,0,0 }; //адреса всех окружающих гекс переходов (индексы на массив переходов)
    //std::array <int, 6> vertex = { 0,0,0,0,0,0 }; //адреса вершин (нужно ли хранить?)
    //hex_args() : hex_h(0), type_mat(0){}
};

class Hexbox {
public:
    int w; //ширина поля (OX)(в кол-ве гексов)
    int h; //высота(в 2D) поля (OY)(или длина...)(в кол-ве гексов)
    int count_st; //кол-во переходов
    int count_hex; //кол-во гексов
    int a = 5; 
    std::vector <hex_args> hex_grid; //массив шестиугольников
    std::vector <smooth_trans_args>  st_grid; //массив переходов
    Hexbox(cint x, cint y) {
        w = x;
        h = y;
        count_hex = w * h;
        count_st = count_st_func(w, h);
        fe_hex_grid(w, h, a);
        fe_st_grid(w, h);
  
    };
    ~Hexbox() = default;

    //API
    hex_args GetHex(cint x, cint y); //возвращает гекс с координатами x, y
    smooth_trans_args GetSt(cint ch1, cint ch2); //возвращает переход по двум номерам двух гексов
    int GetSt_num(cint ch1, cint ch2);
    
    int GetHeight_st(cint ch1, cint ch2);


    int GetHeight_hex(cint x, cint y);
    int GetHeight_st1(cint x1, cint y1, cint x2, cint y2);

    float GetCoordX_hex_float(cint x, cint y);
    int GetCoordX_hex(cint x, cint y);
    int GetCoordY_hex(cint x, cint y);

    void PutHeight_hex(cint x, cint y, cint h);
    void PutHeight_st(cint ch1, cint ch2, cint h);

    //Для материалов поверхностей
    int Get_hex_near(cint x, cint y); //возращает номер ближайшего гекса отсносительно 2д точки, 
    //где а - расстояние от центра гекса до его вершин


private:
    int number_ch(cint x, cint y, cint w); //номер гекса в массиве гексов(по x, y и ширине поля)(только если квадратное!)

    void fe_hex_grid(cint x, cint y, cint a); //заполнение дефолтными значениями параметров каждого гекса
    void fe_st_grid(cint x, cint y); //заполнение дефолтными значениями параметров каждого перехода

    int number_st(cint x1, cint y1, cint x2, cint y2, cint w);
    int count_st_func(cint x, cint y); //кол-во всех переходов для сетки с данными размерами(по ширине и длине)(только если квадратное!)
};

//номер перехода в массиве переходов(по координатам двух смежных гексов и ширине поля)
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
    //if (( ch1 < ch2) || ((ch1 + 1) == ch2) || ((ch1 + w) == ch2) || ((ch1 + w + 1) == ch2) 
    if ((ch1 < ch2) && (((ch1 + 1) == ch2 && y1 == y2) || ((ch1 + w) == ch2) || 
        (((ch1 + w - 1) == ch2) && y1 % 2 == 0)))
        //&& (x1 == w && y1 == y2 )))
    {
        return (y2 - y1) * (ch1 + ch2 + 2 * y1)
            + (1 + y1 - y2) * (y1 * (3 * w - 2) + x2);
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

//API

//получает гекс по xy (в виде структуры гекса)
inline hex_args Hexbox::GetHex(cint x, cint y)
{
    int n_ch = number_ch(x, y, w);
    return hex_grid[n_ch];
}

//получает переход по xy (в виде структуры перехода)
inline smooth_trans_args Hexbox::GetSt(cint ch1, cint ch2)
{
    int x1 = ch1 % w;
    int x2 = ch2 % w;
    int y1 = ch1 / w;
    int y2 = ch2 / w;
    int n_st = number_st(x1, y1, x2, y2, w);
    return st_grid[n_st];
}

inline int Hexbox::GetSt_num(cint ch1, cint ch2)
{
    return 0;
}

inline int Hexbox::GetHeight_hex(cint x, cint y)
{
    return GetHex(x, y).hex_h;
}

inline int Hexbox::GetHeight_st(cint ch1, cint ch2)
{
    return GetSt(ch1,ch2).st_h;
}

inline int Hexbox::GetHeight_st1(cint x1, cint y1, cint x2, cint y2)
{
    return GetHeight_st(number_ch(x1,y1,w), number_ch(x2,y2,w));
}

inline float Hexbox::GetCoordX_hex_float(cint x, cint y)
{
    return GetHex(x, y).x2d;
}

inline int Hexbox::GetCoordX_hex(cint x, cint y)
{
    return GetHex(x, y).x2d;
}

inline int Hexbox::GetCoordY_hex(cint x, cint y)
{
    return GetHex(x, y).y2d;
}

inline void Hexbox::PutHeight_hex(cint x, cint y, cint h)
{
    hex_grid[number_ch(x,y,w)].hex_h = h;
}

inline void Hexbox::PutHeight_st(cint ch1, cint ch2, cint h)
{
    int x1 = ch1 % w;
    int x2 = ch2 % w;
    int y1 = ch1 / w;
    int y2 = ch2 / w;
    st_grid[number_st(x1, y1, x2, y2, w)].st_h = h;
}

//даёт номер ближайшего гекса от точки в 2д плоскости
inline int Hexbox::Get_hex_near(cint point_x, cint point_y)
{
    /*int h = a * sqrt(3) / 2;
    float xh = x / h;
    float ya = y / a;
    int ia = ya / a;*/

    std::vector <std::pair <unsigned int, int>> distance_vec = {};
    unsigned int distance = 0;
    int x_dif = 0; //разница по х
    int y_dif = 0; //зазница по у
    for (int i = 0; i < count_hex; i += 1) {
        x_dif = hex_grid[i].x2d - point_x; 
        y_dif = hex_grid[i].y2d - point_y;
        distance = sqrt(x_dif * x_dif + y_dif * y_dif);
        distance_vec.push_back(std::make_pair(distance,i));

    }
    std::sort(distance_vec.begin(), distance_vec.end());

    /*int xh1 = ( x / h ) - h;
    int ya1 = ( y / (1.5 * a) ) - a;*/

    return distance_vec[0].second;
}

//заполняет массив гексов нулями
inline void Hexbox::fe_hex_grid(cint x, cint y, cint aa)
{
    count_hex = x * y;
    float hh = aa * sqrt(3) / 2;
    hex_args def_ha = { 0, 0 };
    /*for (int i = 0; i < count_hex; i += 1) {
        hex_grid.push_back(def_ha);
    }*/
    for (int yy = 0; yy < y; yy += 1) {
        for (int xx = 0; xx < x; xx += 1) {
            def_ha.x2d = (2 * hh * xx) + ((yy % 2) * hh) + hh;
            def_ha.y2d = ((aa + hh / 2) * yy) + aa;
            hex_grid.push_back(def_ha);
        }
    }

}

//заполняет массив переходов нулями
inline void Hexbox::fe_st_grid(cint x, cint y)
{
    count_st = Hexbox::count_st_func(x, y);
    smooth_trans_args def_st = { 0, 0, 0 };
    for (int i = 0; i < count_hex; i += 1) {
        st_grid.push_back(def_st);
    }
}

int Hexbox::number_ch(cint x, cint y, cint w)
{
    return w * y + x;
}
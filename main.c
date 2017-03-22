#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MINUS 0
#define PLUS 1

//структура полинома, состоящая из коэффициента и степени
typedef struct polynom
{
    int coeff;
    int power;
} PolyItem;

int extractNumber(char a)
{
    return (a - '0');
}
//Функция, преобразующая строку в int
int parseInt(const char* str)
{
    int ret = 0;
    int length = strlen(str);
    int i;
    
    for (i = 0; i < length; ++i)
    {
        ret *= 10;//при каждой последующей цифре, умножаем все число на 10
        ret += extractNumber(str[i]);
    }
    
    return ret;
}
//функция для поиска x в строке
int findFirstOccurence(const char* where, char what)
{
    int i;
    int length = strlen(where);//определение длинны строки
    
    for (i = 0; i < length; ++i)
    {
        if (where[i] == what)
        {
            return i;//возвращаем индекс x
        }
    }
    
    return -1;//если нет х, возвращаем -1
}

//scanf считывает до пробела или переноса строки,
//так что мы можем из строки вытаскивать один кусок полинома
PolyItem parseItem(const char* str)
{
    PolyItem ret;
    char* power = strstr(str, "^");//Ищем числа-степени
    //нашли X
    int xpos = findFirstOccurence(str, 'x');
    char* coeff;
    
    if ((xpos == -1))//если нет х
    {
        ret.power = 0;//степень равна нулю
        ret.coeff = parseInt(str);//запоминаем коэффициент,
        //чтобы посчитать числитель или знаменатель без производной
        return ret;
    }
    //если степени нет, но есть X, то есть X в 1-ой степени
    if ((power == NULL))
    {
        //Извлекаем строчку с числом-коэффициентом при Х
        coeff = (char*) malloc(1024*sizeof(char));
        strncpy(coeff, str, xpos);
        ret.power = 1;
        ret.coeff = parseInt(coeff);
        free(coeff);
        return ret;
    }
    
    coeff = (char*) malloc(1024*sizeof(char));
    
    strncpy(coeff, str, xpos);
    //printf("str %s: coeff value: [%s]\n", str, coeff);
    ret.power = parseInt(++power);
    ret.coeff = parseInt(coeff);
    free(coeff);
    return ret;
}

//функция для взятия производной
PolyItem diffPoly(PolyItem item)
{
    PolyItem ret;
    //коэффициент равен произведению
    //коэффициента этого числа на степень
    ret.coeff = item.coeff*item.power;
    ret.power = item.power - 1;//отнимаем 1 от степени
    return ret;
}

//Обертка, для удобства
int countPoly(PolyItem item, int x)
{
    return item.coeff*pow(x, item.power);
}

//Функция, которая считывает, и расчитывает ответ
double diff()
{
    int chisl = 0;//переменная для значения числителя
    int znamen = 0;//переменная для значения знаменателя
    int chislproiz = 0;//переменная для значения производной числителя
    int znamenproiz = 0;//переменная для значения производной знаменателя
    double chisl2 = 0;
    double znamen2 = 0;
    double chislproiz2 = 0;
    double znamenproiz2 = 0;
    double itog = 0;//переменная для получения результата
    int x;
    //Нужен для того, чтобы переключать между сложением и вычитанием
    int cursymbol = PLUS;
    char* chislitel;//строка, для ввода и парсинга числителя
    char* znamenatel;//строка, для ввода и парсинга знаменателя
    
    printf("Введите X: ");
    scanf("%d", &x);
    
    printf("Введите числитель:\n");
    chislitel = (char*) malloc(1024*sizeof(char));//выделение памяти
    scanf("%s", chislitel);//считывание
    
    //часть программы, которая подсчитывает числитель и производную числителя
    while (chislitel[0] != '=')
    {
        //Сначала считываем - не + или - у нас.
        if (chislitel[0] == '-')
        {
            cursymbol = MINUS;
            free(chislitel);
            chislitel = (char*) malloc(1024*sizeof(char));
            scanf("%s", chislitel);
            continue;
        }
        
        if (chislitel[0] == '+')
        {
            cursymbol = PLUS;
            free(chislitel);
            chislitel = (char*) malloc(1024*sizeof(char));
            scanf("%s", chislitel);
            continue;
        }
        
        //Добавляем/вычитаем то что уже считали к результату,
        //который мы вернем
        if (cursymbol == MINUS)
        {
            chislproiz -= countPoly(diffPoly(parseItem(chislitel)), x);
            chisl -= countPoly(parseItem(chislitel), x);
            free(chislitel);
            chislitel = (char*) malloc(1024*sizeof(char));
            scanf("%s", chislitel);
            continue;
        }
        
        if (cursymbol == PLUS)
        {
            chislproiz += countPoly(diffPoly(parseItem(chislitel)), x);
            chisl += countPoly(parseItem(chislitel), x);
            free(chislitel);
            chislitel = (char*) malloc(1024*sizeof(char));
            scanf("%s", chislitel);
            continue;
        }
    }
    
    printf("Введите знаменатель:\n");
    znamenatel = (char*) malloc(1024*sizeof(char));
    scanf("%s", znamenatel);
    
    //часть программы, которая подсчитывает знаменатель
    //и производную знаменателя
    while (znamenatel[0] != '=')
    {
        //printf("[%s]\n", znamenatel);
        if (znamenatel[0] == '-')
        {
            cursymbol = MINUS;
            free(znamenatel);
            znamenatel = (char*) malloc(1024*sizeof(char));
            scanf("%s", znamenatel);
            continue;
        }
        
        if (znamenatel[0] == '+')
        {
            cursymbol = PLUS;
            free(znamenatel);
            znamenatel = (char*) malloc(1024*sizeof(char));
            scanf("%s", znamenatel);
            continue;
        }
        
        if (cursymbol == MINUS)
        {
            znamenproiz -= countPoly(diffPoly(parseItem(znamenatel)), x);
            znamen -= countPoly(parseItem(znamenatel), x);
            free(znamenatel);
            chislitel = (char*) malloc(1024*sizeof(char));
            scanf("%s", znamenatel);
            continue;
        }
        
        if (cursymbol == PLUS)
        {
            znamenproiz += countPoly(diffPoly(parseItem(znamenatel)), x);
            znamen += countPoly(parseItem(znamenatel), x);
            free(znamenatel);
            znamenatel = (char*) malloc(1024*sizeof(char));
            scanf("%s", znamenatel);
            continue;
        }
    }
    
    //printf("chisl = %d\n", chisl);
    //printf("znam = %d\n", znamen);
    
    chisl2 = (double)chisl;
    znamen2 = (double)znamen;
    chislproiz2 = (double)chislproiz;
    znamenproiz2 = (double)znamenproiz;
    itog = ((chislproiz2*znamen2) - (chisl2*znamenproiz2))/(znamen2*znamen2);
    return itog;
}

int main()
{
    double res;
    
    printf("Программа нахождения производной рациональной функции в точке\n");
    printf("Функция должна быть введена следующим образом: ");
    printf("2x^3 + 5x^7 - 8x^5 = \n");
    printf("Между символами +- и прочими членами должны быть пробелы, ");
    printf("в самих членах полинома пробелов быть не должно\n");
    printf("Если нет коэффицента, то перед x поставьте 1\n");
    printf("Программа начнет подсчет полинома при получении символа =\n");
    res = diff();
    printf("Результат вычислений: %fl\n", res);
    return 0;
}

#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>

#define int long long
using namespace std;

//Переставляем местами коэффициенты так, как на глубочайшем шаге рекурсии (обратная битовая запись)
void permutate_odds(vector<complex<double>> &odds, int q) {
    //Подсчитываем динамикой
    vector<int> rev(odds.size());
    rev[0] = 0;

    //Динамика. odds.size() - гарантированно степень 2. q - степень
    for (int i = 0; i < odds.size(); i++) {
        rev[i] = (rev[i >> 1] >> 1) ^ ((i & 1) << (q - 1));
    }
    //Меняем местами
    for (int i = 0; i < odds.size(); i++) {
        if (i < rev[i]) swap(odds[i], odds[rev[i]]);
    }
}

void FFT(vector<complex<double>> &odds, int n, int q, bool is_reverse) {
    //Переворачиваем все, как в рекурсии
    permutate_odds(odds, q);

    //Ходим по слоям рекурсии. Если представлять себе дерево, то:
    //      О
    //     / \
    //    /   \
    //   @     @
    //На первом шаге мы находимся в О, а в @ находятся однокоэффициентные многочлены

    for (int len = 1; len < n; len *= 2) {
        //Так как начинаем на предпоследнем шаге рекурсии, двойки сократились
        complex<double> w(cos(M_PI / len), sin(M_PI / len));
        if (is_reverse) w = pow(w, -1);

        //Шагаем по всем вершинам (О) этого слоя рекурсии
        for (int i = 0; i < n; i += 2 * len) {
            //Заводим корни x_1,...,x_2*len
            complex<double> x(1, 0);
            //Шагаем как бы по результатам детей одновременно:
            //           О
            //     /            \
            //    /              \
            // (1,...,len)    (1 + len,...., 2*len)
            for (int j = 0; j < len; j++) {
                //Запоминаем то, что лежит в правой вершине
                complex<double> tmp = odds[i + j + len];
                // Так как P(x) = P0(x^2) + x * P1(x^2), в odds[i + j] лежит ровно это теперь (ведь в odds[i + j + len]
                // лежит ответ для P1, так как x_j + n/2 = -xj. Но так как есть такое равенство, нужно в odds[i + j + len]
                // положить с -x, ведь в нем на данном уровне абстракции лежит как раз ответ для парного (с минусом)
                odds[i + j + len] = odds[i + j] - x * tmp;
                odds[i + j] = odds[i + j] + x * tmp;
                x *= w;
            }
        }
    }
}

vector<int> mult(vector<complex<double>> a, vector<complex<double>> b) {
    //Подсчитываем n - степень двойки и q - ее степень
    int n = 1, q = 0;
    for (; n < a.size() + b.size() - 1; n *= 2, q++);

    //Массив с ответами
    vector<complex<double>> c(n, 0);

    //Накачиваем массивы до степени n
    while (a.size() != n) {
        a.emplace_back(0);
    }
    while (b.size() != n) {
        b.emplace_back(0);
    }

    //Считаем значения в точках
    FFT(a, n, q, false);
    FFT(b, n, q, false);

    //Перемножаем результаты
    for (int i = 0; i < n; i++) {
        c[i] = a[i] * b[i];
    }

    //Обратным алгоритмом подсчитываем коэффкициенты (подробнее в приложении)
    FFT(c, n, q, true);
    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        ans[i] = round(c[i].real()) / n;
    }
    return ans;
}

//Записываем число в систему счисления
vector<complex<double>> change_sys(int a, int r) {
    vector<complex<double>> new_sys;
    while (a > 0) {
        new_sys.emplace_back(a % r);
        a /= r;
    }
    return new_sys;
}

//Нормализуем разряды
void normalize_num(vector<int> &ref, int r) {
    int n = ref.size();
    ref.push_back(0);
    for (int i = 0; i < n; i++) {
        if (ref[i] >= r) {
            ref[i + 1] += ref[i] / r;
            ref[i] %= r;
        }
    }
    while (*ref.end() > r) {
        ref.push_back(ref[ref.size() - 1] / r);
        ref[ref.size() - 2] %= r;
    }
}

int32_t main() {
    int a, b, r;
    cin >> a >> b >> r;
    vector<complex<double>> ar = change_sys(a, r);
    vector<complex<double>> br = change_sys(b, r);

    vector<int> ans = mult(ar, br);
    normalize_num(ans, r);
    int n = 0, k = 1;
    for (long long an : ans) {
        n += k * an;
        k *= r;
    }
    cout << n;
    return 0;
}
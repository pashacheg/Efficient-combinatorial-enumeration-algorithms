#include <vector>
#include <fstream>

/* === COMBINATIONS === */

static unsigned long long combCount(unsigned long long n, unsigned long long k);

template <class TYPE>
class CombGenerator
{
protected:
    TYPE n;
    TYPE k;

public:

#ifdef VERIF
    friend class Verification<TYPE>;
#endif
    CombGenerator() {}

    CombGenerator(TYPE n, TYPE k) : n(n), k(k) {}

    virtual void operator()() = 0;
};

template <class T, class TYPE>
class GenVec_U : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

public:

    GenVec_U(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* comb = new TYPE[k + 1];

        if (!comb) { return; }

        for (int i = 0; i < k; ++i) comb[i + 1] = 0;

        int sz = combCount(n, k);
        TYPE B = 0;
        TYPE K = 0, r = 0, k1;
        for (TYPE L = 1; L <= sz; )
        {
            B = (K + r) >= L;
            K *= B; r *= B; k1 = k - 1;
            for (TYPE i = B * k1 + 1; i <= k1; ++i)
            {
                comb[i] = comb[i - 1] * (i != 1);
                do {
                    comb[i]++;
                    r = combCount(n - comb[i], k - i);
                    K += r;
                } while (K < L);
                K -= r;
            }

            comb[k] = comb[k1] + L - K;
            printComb(comb, n, k, set, f, 1);
            ++L;
        }
        delete[] comb;
    }
};

template <class T, class TYPE> 
class RecLex : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    TYPE* comb;
    TYPE* arrComb;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

    void binVec_to_Comb()
    {
        int temp = n;
        for (int i = 0, l = 0; i < temp; ++i) {
            if (!comb[i]) arrComb[l++] = i;
        }

        printComb(arrComb, n, k, set, f, 0);
    }

    void combination(TYPE x, TYPE y)
    {
        if (!x && !y) binVec_to_Comb();
        else if (x >= y && y >= 0)
        {
            comb[n - x] = 0; combination(x - 1, y - 1);
            comb[n - x] = 1; combination(x - 1, y);
        }
    }

public:
    RecLex(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        comb = new TYPE[n];
        arrComb = new TYPE[k];
        if (!comb || !arrComb) { return; }

        combination(n, k);
        delete[] comb;
        delete[] arrComb;
    }
};

template <class T, class TYPE> 
class Talg : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    Talg(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n <= 0 || k <= 0) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE s = k;
        TYPE full_size = s + 2;

        TYPE* comb = new TYPE[full_size];
        if (!comb) return;


        // == T1 == //
        TYPE i = 0;
        for (i; i < s; ++i) {
            comb[i] = i;
        }
        comb[i] = n;
        comb[++i] = 0;
        i = s - 1;

        while (1)
        {
            // == T2 == //
            TYPE x;

            printComb(comb, n, k, set, f, 0);

            if (i > -1)
            {
                x = i;
                // == T6 == //
                comb[i] = x;
                i--;
            }

            // == T3 == //
            if (comb[0] + 1 < comb[1])
            {
                comb[0]++;
                continue;
            }
            i = 1;

            // == T4 == //
            for (comb[i - 1] = i - 1, x = comb[i] + 1;
                x == comb[i + 1]; )
            {
                i++;
                comb[i - 1] = i - 1;
                x = comb[i] + 1;
            }

            if (i >= s) {
                // == T5 == //
                break;
            }
            // == T6 == //
            comb[i] = x;
            i--;
        }

        if (comb) delete[] comb;
    }
};

template <class T, class TYPE> 
class NXCBN : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    TYPE* comb;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    NXCBN(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        if (n < k || n <= 1 || k <= 1) { return; }

        int full_size = k + 1;

        TYPE* comb = new TYPE[full_size];
        if (!comb) return;


        bool L = k % 2;

        // == R1 == //
        TYPE i = 0;
        for (i; i < k; ++i)
        {
            comb[i] = i;
        }
        comb[i] = n;

        while (1)
        {
            // == R2 == //
            printComb(comb, n, k, set, f, 0);
            // == R3 == //
            if (L)
            {
                if (comb[0] + 1 < comb[1])
                {
                    comb[0]++;
                    continue;
                }
                i = 1;
            }
            else
            {
                if (comb[0] > 0)
                {
                    comb[0]--;
                    continue;
                }
                i = 1;
                goto R5;
            }

        R4:		// == R4 == //
            if (comb[i] > i)
            {
                comb[i] = comb[i - 1];
                comb[i - 1] = i - 1;
                continue;
            }
            i++;

        R5:		// == R5 == //
            if (comb[i] + 1 < comb[i + 1])
            {
                comb[i - 1] = comb[i];
                comb[i]++;
                continue;
            }
            i++;

            if (i < k) goto R4;
            else break;
        }

        if (comb) delete[] comb;
    }
};

template <class T, class TYPE> 
class Twiddle : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    TYPE* comb;
    TYPE* st_comb;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

    void binVec_to_Comb()
    {
        for (int i = 0, l = 0; i < n; ++i) {
            if (comb[i]) st_comb[l++] = i;
        }

        printComb(st_comb, n, k, set, f, 0);
    }

public:
    Twiddle(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }

        st_comb = new TYPE[k];
        if (!st_comb) { return; }
    }

    void operator()()
    {
        bool done = false;
        TYPE* p = new TYPE[n + 2];
        comb = new TYPE[n];

        if (!comb || !p) { return; }

        TYPE r = n - k;

        for (TYPE i = r; i >= 1; --i) {
            p[i] = 0;
        }
        for (TYPE i = k; i >= 1; --i) {
            p[r + i] = i;
        }
        p[0] = n + 1;
        p[n + 1] = -2;

        for (TYPE i = k; i >= 1; --i) {
            comb[r + i - 1] = 1;
        }
        for (TYPE i = r; i >= 1; --i) {
            comb[i - 1] = 0;
        }

        TYPE x, y;
        TYPE i, j, l;
        while (!done)
        {
            binVec_to_Comb();
            j = 0;
        L1:
            j++;
            if (p[j] <= 0) goto L1;
            if (!p[j - 1])
            {
                for (i = j - 1; i >= 2; --i) p[i] = -1;
                p[j] = 0;
                p[1] = x = 1;
                y = j;
                //continue;
                goto L4;
            }
            p[j - 1] *= (j <= 1);

        L2:
            do {
                ++j;
            } while (p[j] > 0);
            i = l = j - 1;

        L3:
            i++;
            while (!p[i])
            {
                p[i] = -1;
                ++i;
            }

            if (p[i] == -1)
            {
                p[i] = p[l];
                x = i; y = l;
                p[l] = -1;
                //continue;
                goto L4;
            }
            if (i == p[0])
            {
                done = true;
                //continue;
                goto L4;
            }

            p[j] = p[i];
            p[i] = 0;
            x = j; y = i;
        L4:
            ;
            comb[x - 1] = 1;
            comb[y - 1] = 0;
        }

        delete[] comb;
    }

    ~Twiddle()
    {
        if (!st_comb) { return; }

        delete[] st_comb;
    }
};

template <class T, class TYPE>
class PrefShift_U : public CombGenerator<TYPE>
{
protected:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    TYPE* comb;
    TYPE* st_comb;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

    void binVec_to_Comb()
    {
        int temp = n;
        for (int i = n, l = 1; i > 0; --i) {
            if (comb[i]) st_comb[l++] = temp - i;
        }

        printComb(st_comb, n, k, set, f, 0);
    }


public:
    PrefShift_U(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }

        st_comb = new TYPE[k + 1];
        if (!st_comb) { return; }
    }

    void operator()()
    {
        comb = new TYPE[n + 1];
        if (!comb || !st_comb) { return; }

        /*=== J1 ===*/
        TYPE i;
        TYPE t;
        for (int j = 1; j <= n; ++j) { comb[j] = j > n - k; }
        t = n - k + 1;
        i = t;
        do
        {
            /*=== J2 ===*/
            binVec_to_Comb();

            /*=== J3 ===*/
            comb[i] = 0;
            i--;

            /* === J4 ===*/
            if (i >= 1 && comb[i]) { //comb[i] == 1
                comb[t] = 1;
                t--;
                continue;
            }

            /*=== J5 ===*/
            if (!i) break; // i == 0
            else {
                comb[i] = 1;
            }

            if (t < n) {
                comb[t] = 1;
                comb[n] = 0;
                i = n - 1;
                t = n;
            }
        } while (1);

        delete[] comb;
    }

    ~PrefShift_U() {
        if (!st_comb) { return; }

        delete[] st_comb;
    }
};

template <class T, class TYPE>
class PrefShift_loopless_U : public PrefShift_U<T, TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    using PrefShift_U<T, TYPE>::set;
    using PrefShift_U<T, TYPE>::f;
    using PrefShift_U<T, TYPE>::comb;
    using PrefShift_U<T, TYPE>::st_comb;
    using PrefShift_U<T, TYPE>::printComb;

    void printBin()
    {
        for (int i = 0, l = 1; i < n; ++i)
        {
            if (comb[i]) st_comb[l++] = i;
        }

        printComb(st_comb, n, k, set, f, 0);
    }

public:
    PrefShift_loopless_U(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PrefShift_U<T, TYPE>(n, k, vec, path, func) {}

    void operator()()
    {
        TYPE i = 0;

        comb = new TYPE[n];
        if (!comb) { return; }

        for (; i < n; ++i) comb[i] = i < k;

        printBin();

        comb[k] = 1;
        comb[0] = 0;

        printBin();

        TYPE x = 1, y = 0;

        while (x < n - 1)
        {
            comb[x++] = 0;
            comb[y++] = 1;

            if (!comb[x])
            {
                comb[x] = 1;
                comb[0] = 0;

                x = y > 1 ? (TYPE)1 : x;
                y = 0;
            }

            printBin();
        }
    }
};

template <class T, class TYPE>
class Walsh_U : public CombGenerator<TYPE>
{
private:
    using CombGenerator<TYPE>::n;
    using CombGenerator<TYPE>::k;

    std::vector<T> set;
    std::fstream f;
    void (*printComb)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    Walsh_U(TYPE n, TYPE k, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        CombGenerator<TYPE>(n, k), set(vec), f(std::fstream(path, std::ios::out)), printComb(func)
    {
        if (k > n) { return; }
        if (n > (int)set.size()) { return; }
        if (n < 0 || k < 0) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* comb = new TYPE[k + 1];
        if (!comb) { return; }

        for (TYPE j = 1; j <= k; ++j) comb[j] = j;

        TYPE i = k;
        TYPE R = 1, D = 0;
        TYPE x;

        printComb(comb, n, k, set, f, 1);

        while (!D)
        {
            while (1)
            {
                if (R)
                {
                    x = i == k ? (int)n : comb[i + 1] - 1;
                    if (comb[i] < x)
                    {
                        comb[i]++;
                        if (i > 1) {
                            comb[i - 1] = comb[i] - 1;
                            if (i == 2) {
                                i = 1;
                                R = 0;
                            }
                            else {
                                i -= 2;
                            }
                        }
                        printComb(comb, n, k, set, f, 1);
                        break;
                    }
                }
                else
                {
                    if (i > k) {
                        D = 1;
                        break;
                    }
                    if (comb[i] > i)
                    {
                        comb[i]--;
                        if (i > 1) {
                            comb[i - 1] = i - 1;
                            i--;
                            R = 1;
                        }
                        printComb(comb, n, k, set, f, 1);
                        break;
                    }
                }
                i++;
                R = !R;
            }
        }

        delete[] comb;
    }
};

template <class T, class TYPE>
static void printCombU(const TYPE* comb, TYPE n1, TYPE k1, const std::vector<T>& vec, std::fstream& f, bool l)
{
    int n = n1, k = k1;

    if (k > n) { return; }
    if (n > (int)vec.size()) { return; }
    if (n < 0 || k < 0) { return; }
    if (!f) { return; }

    for (int i = 0; i < k; ++i)
    {
        f << vec[comb[i + 1] - l] << ' ';
    }
    f << std::endl;
}

template <class T, class TYPE>
static void printComb(const TYPE* comb, TYPE n1, TYPE k1, const std::vector<T>& vec, std::fstream& f, bool l)
{
    int n = n1, k = k1;

    if (k > n) { return; }
    if (n > (int)vec.size()) { return; }
    if (n < 0 || k < 0) { return; }
    if (!f) { return; }

    for (int i = 0; i < k; ++i)
    {
        f << vec[comb[i] - l] << ' ';
    }
    f << std::endl;
}

/* === PERMUTATIONS === */

template <class TYPE>
class PermutGenerator
{
protected:
    TYPE n;

public:
    PermutGenerator() {}

    PermutGenerator(TYPE n) : n(n) {}

    virtual void operator()() = 0;
};

template <class T, class TYPE>
class PermRuskey_U : public PermutGenerator<TYPE>
{
private:
    using PermutGenerator<TYPE>::n;

    std::vector<T> set;
    std::fstream f;
    void (*printPerm)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    PermRuskey_U(TYPE n, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PermutGenerator<TYPE>(n), set(vec), f(std::fstream(path, std::ios::out)), printPerm(func)
    {
        if (n > (int)set.size() || n < 1) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE x;
        TYPE first;

        TYPE* perm = new TYPE[n + 1];
        if (!perm) { return; }

        for (x = 0; x <= n; x++) { perm[x] = n + 1 - x; }

        first = perm[1];
        while (1) {
            printPerm(perm, n, set, f, 1);
            first = perm[1];
            perm[1] = perm[2];
            for (x = 2; x < n && perm[x] >= perm[x + 1]; x++) { perm[x] = perm[x + 1]; }
            if (first == 1 && x == n) { perm[x] = first; break; }
            if (x < n && first < perm[x]) { perm[x] = perm[x + 1]; x++; }
            perm[x] = first;
        }

        delete[] perm;
    }
};

template <class T, class TYPE> 
class PermPrefShift : public PermutGenerator<TYPE>
{
private:
    using PermutGenerator<TYPE>::n;

    std::vector<T> set;
    std::fstream f;
    void (*printPerm)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool);

    void prefShift(TYPE* v, TYPE k)
    {
        for (TYPE i = 0; i < k - 1; ++i)
        {
            v[i] += v[i + 1];
            v[i + 1] = v[i] - v[i + 1];
            v[i] -= v[i + 1];
        }
    }

public:
    PermPrefShift(TYPE n, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PermutGenerator<TYPE>(n), set(vec), f(std::fstream(path, std::ios::out)), printPerm(func)
    {
        if (n > (int)set.size() || n < 1) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* x = new TYPE[n],
            * perm = new TYPE[n];

        if (!x || !perm) { return; }

        TYPE j, k;

        for (TYPE i = 0; i < n; ++i) x[i] = i + 1;

    G1:
        for (j = 1; j <= n; ++j) perm[j - 1] = x[j - 1];

    G2:
        printPerm(perm, n, set, f, 1);

    G3:
        k = n;

    G4:
        prefShift(perm, k);
        if (perm[k - 1] != x[k - 1]) goto G2;
    G5:
        k--;
        if (k == 1) return;
        goto G4;

        delete[] x;
        delete[] perm;
    }
};

template <class T, class TYPE>
class PermJohnTrot_U : public PermutGenerator<TYPE>
{
private:
    using PermutGenerator<TYPE>::n;

    std::vector<T> set;
    std::fstream f;
    void (*printPerm)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    PermJohnTrot_U(TYPE n, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PermutGenerator<TYPE>(n), set(vec), f(std::fstream(path, std::ios::out)), printPerm(func)
    {
        if (n > (int)set.size() || n < 1) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* perm = new TYPE[n + 1],
            * c = new TYPE[n + 1],
            * o = new TYPE[n + 1];

        if (!perm || !c || !o) { return; }

        TYPE j, s, q;

        for (TYPE i = 1; i <= n; ++i) perm[i] = i;

    E1:
        for (j = 1; j <= n; ++j) {
            c[j] = 0;
            o[j] = 1;
        }

    E2:
        printPerm(perm, n, set, f, 1);
    E3:
        j = n;
        s = 0;

    E4:
        q = c[j] + o[j];
        if (q < 0) goto E7;
        if (q == j) goto E6;

    E5:
        perm[j - c[j] + s] += perm[j - q + s];
        perm[j - q + s] = perm[j - c[j] + s] - perm[j - q + s];
        perm[j - c[j] + s] -= perm[j - q + s];

        c[j] = q;
        goto E2;

    E6:
        if (j == 1) return;
        s++;

    E7:
        o[j] = -o[j];
        j--;
        goto E4;

        delete[] perm;
        delete[] c;
        delete[] o;
    }
};

template <class T, class TYPE> 
class PermStarTrans : public PermutGenerator<TYPE>
{
private:
    using PermutGenerator<TYPE>::n;

    std::vector<T> set;
    std::fstream f;
    void (*printPerm)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool);

public:
    PermStarTrans(TYPE n, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PermutGenerator<TYPE>(n), set(vec), f(std::fstream(path, std::ios::out)), printPerm(func)
    {
        if (n > (int)set.size() || n < 1) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* perm = new TYPE[n],
            * b = new TYPE[n],
            * c = new TYPE[n];

        if (!perm || !b || !c) { return; }

        TYPE j, k, l;

        for (TYPE i = 0; i < n; ++i) perm[i] = i;

        for (TYPE j = 0; j < n; ++j) {
            b[j] = j;
            c[j] = 0;
        }

        while (1)
        {
            printPerm(perm, n, set, f, 0);

            k = 1;
            if (c[k - 1] == k) {
                do {
                    c[k - 1] = 0;
                    k = k + 1;
                } while (c[k - 1] >= k);
            }
            if (k == n) break;
            c[k - 1]++;

            TYPE sw1 = perm[0];
            perm[0] = perm[b[k]];
            perm[b[k]] = sw1;

            j = 1;
            k--;
            while (j < k) {
                TYPE sw2 = b[j];
                b[j] = b[k];
                b[k] = sw2;

                j++;
                k--;
            }
        }

        delete[] perm;
        delete[] b;
        delete[] c;
    }
};

template <class T, class TYPE>
class PermHeap : public PermutGenerator<TYPE>
{
private:
    using PermutGenerator<TYPE>::n;

    std::vector<T> set;
    std::fstream f;
    void (*printPerm)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool);

    void swap(TYPE* a, TYPE* b)
    {
        TYPE tmp = *a;
        *a = *b;
        *b = tmp;
    }

    void heap(TYPE k, TYPE* P)
    {
        if (k == 1)
        {
            printPerm(P, n, set, f, 0);
        }
        else
        {
            heap(k - 1, P);

            for (int i = 0; i < k - 1; ++i)
            {
                swap((k % 2) ?
                    &P[0] : &P[i],
                    &P[k - 1]);

                heap(k - 1, P);
            }
        }
    }

public:
    PermHeap(TYPE n, const std::vector<T>& vec, const std::string& path,
        void (*func)(const TYPE*, TYPE, const std::vector<T>&, std::fstream&, bool)) :
        PermutGenerator<TYPE>(n), set(vec), f(std::fstream(path, std::ios::out)), printPerm(func)
    {
        if (n > (int)set.size() || n < 1) { return; }
        if (!f) { return; }
    }

    void operator()()
    {
        TYPE* P = new TYPE[n];
        if (!P) { return; }

        for (int i = 0; i < n; ++i) P[i] = i;

        heap(n, P);
    }
};



template <class T, class TYPE>
void printPermU(const TYPE* perm, TYPE n1, const std::vector<T>& vec, std::fstream& f, bool l)
{
    int n = n1;

    if (n > (int)vec.size() || n < 0) { return; }
    if (!f) { return; }

    for (int i = 1; i <= n; ++i)
    {
        f << vec[perm[i] - l] << ' ';
    }
    f << '\n';
}

template <class T, class TYPE>
void printPerm(const TYPE* perm, TYPE n1, const std::vector<T>& vec, std::fstream& f, bool l)
{
    int n = n1;

    if (n > (int)vec.size() || n < 0) { return; }
    if (!f) { return; }

    for (int i = 0; i < n; ++i)
    {
        f << vec[perm[i] - l] << ' ';
    }
    f << '\n';
}

static unsigned long long combCount(unsigned long long n, unsigned long long k)
{
    if (n < k) throw(std::exception());

    unsigned long long a = 1;
    unsigned long long b = 1;

    unsigned long long dv1, dv2;

    if (k > n - k)
    {
        dv1 = k;
        dv2 = n - k;
    }
    else
    {
        dv1 = n - k;
        dv2 = k;
    }

    unsigned long long i = 1;
    for (i = dv1 + 1; i <= n; ++i) a *= i;
    for (i = 1; i <= dv2; ++i) b *= i;
    //for (i = 1; i <= n - k; ++i) c *= i;

    return a / b;
}
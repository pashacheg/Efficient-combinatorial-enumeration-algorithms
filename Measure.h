#include <ostream>
#include <Windows.h>

#include <vector>

class Wrap
{
private:
    int val;

public:
    Wrap() {}

    Wrap(int val) : val(val) {}

    Wrap(const Wrap& obj) : val(obj.val) {}

    int getVal() const { return val; }
    void setVal(int a) { val = a; }

    static size_t adding_counter;
    static size_t subtrac_counter;
    static size_t assign_counter;
    static size_t compare_counter;
    static size_t multiply_counter;

    operator int() const { return val; }

    bool operator!() const { return !val; }

    int operator+(const Wrap& obj) const
    {
        adding_counter++;
        return val + obj.val;
    }

    int operator+(int a) const
    {
        adding_counter++;
        return val + a;
    }

    friend int operator+(int a, const Wrap& obj);

    int operator+=(const Wrap& obj)
    {
        adding_counter++;
        val += obj.val;

        return val;
    }

    friend int operator+=(int& a, const Wrap& obj);

    int operator++()
    {
        adding_counter++;

        return ++val;
    }

    int operator++(int)
    {
        adding_counter++;

        return val++;
    }

    ///////////////////////////////////////////////

    int operator-(const Wrap& obj) const
    {
        subtrac_counter++;
        return val - obj.val;
    }

    int operator-(int a) const
    {
        subtrac_counter++;
        return val - a;
    }

    friend int operator-(int a, const Wrap& obj);

    int operator-=(const Wrap& obj)
    {
        subtrac_counter++;
        val -= obj.val;

        return val;
    }

    friend int operator-=(int& a, const Wrap& obj);

    int operator--()
    {
        subtrac_counter++;

        return --val;
    }

    int operator--(int)
    {
        subtrac_counter++;

        return val--;
    }

    ///////////////////////////////////////////////

    int operator=(const Wrap& obj)
    {
        assign_counter++;
        val = obj.val;

        return val;
    }

    int operator=(int a)
    {
        assign_counter++;
        val = a;

        return val;
    }

    ///////////////////////////////////////////////

    friend bool operator==(const Wrap& a, const Wrap& b);
    friend bool operator< (const Wrap& a, const Wrap& b);
    friend bool operator> (const Wrap& a, const Wrap& b);
    friend bool operator<=(const Wrap& a, const Wrap& b);
    friend bool operator>=(const Wrap& a, const Wrap& b);

    friend bool operator==(const Wrap& a, int b);
    friend bool operator< (const Wrap& a, int b);
    friend bool operator> (const Wrap& a, int b);
    friend bool operator<=(const Wrap& a, int b);
    friend bool operator>=(const Wrap& a, int b);

    friend bool operator==(int a, const Wrap& b);
    friend bool operator< (int a, const Wrap& b);
    friend bool operator> (int a, const Wrap& b);
    friend bool operator<=(int a, const Wrap& b);
    friend bool operator>=(int a, const Wrap& b);

    ///////////////////////////////////////////////

    int operator*(const Wrap& obj) const
    {
        multiply_counter++;
        return val * obj.val;
    }

    int operator*(int a) const
    {
        multiply_counter++;
        return val * a;
    }

    friend int operator*(int a, const Wrap& obj);

    int operator*=(const Wrap& obj)
    {
        multiply_counter++;
        val *= obj.val;
        return val;
    }

    int operator*=(int a)
    {
        multiply_counter++;
        val *= a;
        return val;
    }

    friend int operator*=(int& a, const Wrap& obj);
};

size_t Wrap::adding_counter = 0;
size_t Wrap::subtrac_counter = 0;
size_t Wrap::assign_counter = 0;
size_t Wrap::compare_counter = 0;
size_t Wrap::multiply_counter = 0;

void printStatsOfWrap(std::ostream& out)
{
    out << "operation adding:\t" << Wrap::adding_counter << std::endl
        << "operation subtracting:\t" << Wrap::subtrac_counter << std::endl
        << "operation assigning:\t" << Wrap::assign_counter << std::endl
        << "operation comparing:\t" << Wrap::compare_counter << std::endl;
}

void setStatsToZero()
{
    Wrap::adding_counter = 0;
    Wrap::subtrac_counter = 0;
    Wrap::assign_counter = 0;
    Wrap::compare_counter = 0;
    Wrap::multiply_counter = 0;
}

class Stopwatch
{
private:
    FILETIME cr_time, ex_time, kerTime, userTime;
    double ker_time_start, user_time_start;
    double ker_time_end, user_time_end;
    double res_ker_time;
    double res_user_time;
    double CPU_burst_time;

public:
    Stopwatch() : ker_time_start(0.0), ker_time_end(0.0), user_time_start(0.0), user_time_end(0.0),
        res_ker_time(0.0), res_user_time(0.0), CPU_burst_time(0.0) {}

    template <class T>
    void measureRunTime(T& alg)
    {
        if (GetProcessTimes(GetCurrentProcess(), &cr_time, &ex_time, &kerTime, &userTime))
        {
            ULARGE_INTEGER k, u;

            k.LowPart = kerTime.dwLowDateTime;
            k.HighPart = kerTime.dwHighDateTime;

            u.LowPart = userTime.dwLowDateTime;
            u.HighPart = userTime.dwHighDateTime;

            ker_time_start = k.QuadPart / 1e4;
            user_time_start = u.QuadPart / 1e4;
        }
        else { std::cout << "error\n"; return; }

        alg();

        if (GetProcessTimes(GetCurrentProcess(), &cr_time, &ex_time, &kerTime, &userTime)) {
            ULARGE_INTEGER k, u;

            k.LowPart = kerTime.dwLowDateTime;
            k.HighPart = kerTime.dwHighDateTime;

            u.LowPart = userTime.dwLowDateTime;
            u.HighPart = userTime.dwHighDateTime;

            ker_time_end = k.QuadPart / 1e4;
            user_time_end = u.QuadPart / 1e4;
        }
        else { std::cout << "error\n"; return; }

        res_ker_time = ker_time_end - ker_time_start;
        res_user_time = user_time_end - user_time_start;
        CPU_burst_time = res_ker_time + res_user_time;
    }

    double getSystemTime() const { return res_ker_time; }
    double getUserTime()   const { return res_user_time; }
    double getCPUburst()   const { return CPU_burst_time; }

    void printStats(std::ostream& out) const
    {
        out << "CPU burst time:\t" << CPU_burst_time << std::endl
            << "CPU system time:\t" << res_ker_time << std::endl;
    }
};


int operator+(int a, const Wrap& obj)
{
    Wrap::adding_counter++;
    return a + obj.val;
}

int operator+=(int& a, const Wrap& obj)
{
    Wrap::adding_counter++;
    a += obj.val;

    return a;
}

int operator-(int a, const Wrap& obj)
{
    Wrap::subtrac_counter++;
    return a - obj.val;
}

int operator-=(int& a, const Wrap& obj)
{
    Wrap::subtrac_counter++;
    a -= obj.val;

    return a;
}

bool operator==(const Wrap& a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a.val == b.val;
}

bool operator< (const Wrap& a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a.val < b.val;
}

bool operator> (const Wrap& a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a.val > b.val;
}

bool operator<=(const Wrap& a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a.val <= b.val;
}

bool operator>=(const Wrap& a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a.val >= b.val;
}

bool operator==(const Wrap& a, int b)
{
    Wrap::compare_counter++;
    return a.val == b;
}

bool operator< (const Wrap& a, int b)
{
    Wrap::compare_counter++;
    return a.val < b;
}

bool operator> (const Wrap& a, int b)
{
    Wrap::compare_counter++;
    return a.val > b;
}

bool operator<=(const Wrap& a, int b)
{
    Wrap::compare_counter++;
    return a.val <= b;
}

bool operator>=(const Wrap& a, int b)
{
    Wrap::compare_counter++;
    return a.val >= b;
}

bool operator==(int a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a == b.val;
}

bool operator< (int a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a < b.val;
}

bool operator> (int a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a > b.val;
}

bool operator<=(int a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a <= b.val;
}

bool operator>=(int a, const Wrap& b)
{
    Wrap::compare_counter++;
    return a >= b.val;
}

int operator*(int a, const Wrap& obj)
{
    Wrap::multiply_counter++;
    return a * obj.val;
}

int operator*=(int& a, const Wrap& obj)
{
    Wrap::multiply_counter++;
    a *= obj.val;
    return a;
}

bool operator< (const std::vector<int>& s1, const std::vector<int>& s2)
{
    std::vector<int>::const_iterator
        iter_s1 = s1.begin(),
        iter_s2 = s2.begin();
    for (; iter_s1 != s1.end() &&
        iter_s2 != s2.end() &&
        (*iter_s1) == (*iter_s2);
        ++iter_s1, ++iter_s2);

    return (*iter_s1) < (*iter_s2);
}

bool operator== (const std::vector<int>& s1, const std::vector<int>& s2)
{
    bool F = 0;
    for (std::vector<int>::const_iterator
        iter_s1 = s1.begin(),
        iter_s2 = s2.begin();
        iter_s1 != s1.end() &&
        iter_s2 != s2.end() &&
        (F = (*iter_s1) == (*iter_s2));
        ++iter_s1, ++iter_s2);

    return F;
}
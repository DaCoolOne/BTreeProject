#ifndef AVERAGE_H
#define AVERAGE_H

template <class T>
class Average
{
private:
    T sum;
    int count;
public:
    Average(): sum(0), count(0) { }
    void add(const T val) { sum += val; count++; }
    T get() const { return sum / count; }
};

#endif
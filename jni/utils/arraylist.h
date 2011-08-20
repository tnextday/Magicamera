#pragma once
#include <math.h>
#include <assert.h>

template<typename T> 
class ArrayList
{
private:
    T       *m_values;
    int     m_size;
    int     m_capacity;

public:
    bool    ordered;
protected:
    void resize(int newSize);

public:
    ArrayList(int capacity = 8, bool bOrdered = true);
    ~ArrayList(void);
    void add(const T & value);
    void insert(int index, const T& value);
    void remove(const T& value);
    void remove(int index);
    void remove(int index, int count);
    int indexOf(const T & value);
    inline void clear();
    inline int size();
    inline T & get(int index);
    inline void set(int index, const T& value);
    inline T & operator[](int index);
    inline ArrayList<T> & operator<< (const T & value);
};



template<typename T>
ArrayList<T>::ArrayList( int capacity /*= 8*/, bool bOrdered /*= true*/ )
{
    m_capacity = capacity;
    m_values = new T[capacity];
}

template<typename T>
ArrayList<T>::~ArrayList( void )
{
    SafeDelete(m_values);
}

template<typename T>
void ArrayList<T>::resize( int newSize )
{
    T *newValues = new T[newSize];
    for(int i = 0; i < m_size; i++){
        newValues[i] = m_values[i];
    }
    delete [] m_values;
    m_values = newValues;
}

template<typename T>
void ArrayList<T>::add( const T & value )
{
    if (m_size == m_capacity){
        resize(max(8, m_size*1.75));
    }
    m_values[m_size++] = value;
}


template<typename T>
void ArrayList<T>::insert( int index, const T& value )
{
    if (m_size == m_capacity){
        resize(max(8, m_size*1.75));
    }
    if (!ordered){
        m_values[m_size++] = value;
    }else{
        int i = m_size;
        while(i > index){
            m_values[i] = m_values[--i];
        }
        m_values[index] = value;
    }
}

template<typename T>
void ArrayList<T>::remove( const T& value )
{
    int index = indexOf(value);
    if (index >= 0){
        remove(index);
    }
}

template<typename T>
void ArrayList<T>::remove( int index )
{
    assert(index >= 0 && index < m_size);
    if (ordered){
        for (int i = index ; i < m_size -1; i++){
            m_values[i] = m_values[i+1];
        }
        m_size--;
    }else{
        m_values[index] = m_values[m_size--];
    }
}


template<typename T>
void ArrayList<T>::remove( int index, int count )
{
    assert(index >= 0 && index + count < m_size);
    if (ordered){
        for (int i = index; i < m_size - count; i++){
            m_values[i] = m_values[i+count];
        }
        m_size -= count;
    }else{
        for (int i = 0; i < count; i++)
            m_values[index++] = m_values[--m_size];
    }
}

template<typename T>
int ArrayList<T>::indexOf( const T & value )
{
    for (int i = 0; i < m_size; i++){
        if (m_values[i] == value){
            return i;
        }
    }
    return -1;
}

template<typename T>
void ArrayList<T>::clear()
{
    m_size = 0;
}

template<typename T>
int ArrayList<T>::size()
{
    return m_size;
}

template<typename T>
T & ArrayList<T>::get( int index )
{
    assert(index >= 0 && index < m_size);
    return m_values[index];
}

template<typename T>
void ArrayList<T>::set( int index, const T& value )
{
    assert(index >= 0 && index < m_size);
    m_values[index] = value;
}

template<typename T>
T & ArrayList<T>::operator[]( int index )
{
    return get(index);
}

template<typename T>
ArrayList<T> & ArrayList<T>::operator<<( const T & value )
{
    add(value);
    return *this;
}

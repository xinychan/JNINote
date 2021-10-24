#ifndef HELLOJNI6_MYPARCEL_H
#define HELLOJNI6_MYPARCEL_H

#include <jni.h>

class MyParcel {
private:
    int *mData = 0; // MyParcel 对象存储数据的首地址；存储类型为int，所以这里类型为 int*

    int mDataPos = 0; // MyParcel 对象存储数据的地址的指针挪动位置

    void changePos(int pos); // 指针移动位置

public:
    // 构造函数
    MyParcel();

    // 析构函数
    virtual ~MyParcel();

    void writeInt(int val);

    void setDataPosition(int pos);

    jint readInt();
};


#endif //HELLOJNI6_MYPARCEL_H

#include "MyParcel.h"
#include <malloc.h>

MyParcel::MyParcel() {
    this->mData = static_cast<int *>(malloc(1024)); // 堆区，需要手动释放
}

MyParcel::~MyParcel() {
    // 存储数据首地址不为空，则释放数据
    if (this->mData) {
        free(this->mData);
    }
    // 指针移动位置归零
    if (this->mDataPos) {
        this->mDataPos = NULL;
    }
}

void MyParcel::changePos(int pos) {
    // 每次写入/读取数据指针移动的位置
    this->mDataPos += pos;
}

void MyParcel::writeInt(int val) {
    // mData 指针移动位置后，val 赋值给新地址
    *(this->mData + this->mDataPos) = val;
    changePos(sizeof(int));
}

void MyParcel::setDataPosition(int pos) {
    // 指针指向指定位置
    this->mDataPos = pos;
}

jint MyParcel::readInt() {
    // mData 指针移动位置后，读取新地址的值
    jint result = *(this->mData + this->mDataPos);
    changePos(sizeof(int));
    return result;
}
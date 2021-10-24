package com.example.hellojni6;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * Android中Parcelable序列化
 * 使用内置的Parcelable
 */
public class Person implements Parcelable {

    public int age;
    public String name;

    protected Person(Parcel in) {
        age = in.readInt();
        name = in.readString();
    }

    /**
     * 反序列化的时候会被调用，注意，Parcel读写字段的顺序必须一致
     */
    public static final Creator<Person> CREATOR = new Creator<Person>() {
        @Override
        public Person createFromParcel(Parcel in) {
            return new Person(in);
        }

        @Override
        public Person[] newArray(int size) {
            return new Person[size];
        }
    };

    @Override
    public int describeContents() {
        return 0;
    }

    /**
     * 序列化的时候被调用，注意，Parcel读写字段的顺序必须一致；自定义哪些字段参与序列化
     */
    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(age);
        dest.writeString(name);
    }
}

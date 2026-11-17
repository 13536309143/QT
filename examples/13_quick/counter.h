#pragma once
#include <QObject>
#include <QtQml/qqmlregistration.h>
class Counter : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit Counter(QObject *parent=nullptr):QObject(parent) {}
    int count() const { return count_; }
    Q_INVOKABLE void increment() { ++count_; emit countChanged(); }
signals: void countChanged();
private: int count_=0;
};

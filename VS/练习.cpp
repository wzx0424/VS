#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// 位置类：表示动物在空间中的位置
class Position {
private:
    int x, y;
public:
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    int getX() { return x; }
    int getY() { return y; }
    void move(int dx, int dy) { x += dx; y += dy; }
    double distanceTo(const Position& other) {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }
};

// 动物基类：动物的特征
class Animal {
protected:
    string sex;      // 性别
    int age;         // 年龄
    int health;      // 生命值/血量
    Position pos;    // 位置
public:
    Animal(string sex, int age, int health, int x, int y)
        : sex(sex), age(age), health(health), pos(x, y) {
    }
    string getSex() { return sex; }
    void setSex(string sex) { this->sex = sex; }
    int getAge() { return age; }
    void setAge(int age) { this->age = age; }
    int getHealth() { return health; }
    void setHealth(int health) { this->health = health; }
    Position getPosition() { return pos; }
    virtual void print() {
        cout << "性别: " << sex << "  年龄: " << age
            << "  生命值: " << health
            << "  位置: (" << pos.getX() << ", " << pos.getY() << ")" << endl;
    }
    virtual void move() {
        health -= 5;
        pos.move(1, 0);
        cout << "动物在移动，生命值减少5点，当前生命值: " << health << endl;
    }
    virtual ~Animal() {}
};

// 狼类：继承Animal，增加饥饿度，实现捕食羊
class Wolf : public Animal {
private:
    int hunger;  // 饥饿度
public:
    Wolf(string sex, int age, int health, int x, int y)
        : Animal(sex, age, health, x, y), hunger(0) {
    }
    int getHunger() { return hunger; }
    void print() override {
        cout << " 狼 " << endl;
        Animal::print();
        cout << "饥饿度: " << hunger << endl;
    }
    void move() override {
        health -= 10;
        hunger += 5;
        pos.move(1, 0);
        cout << "狼在移动，生命值减少10点为：" << health
            << "，饥饿度增加5点为：" << hunger << endl;
    }
    // 狼吃羊：通过引用传递羊对象，改变狼和羊的状态
    // 依赖关系：Wolf 依赖 Animal（通过引用参数）
    void eatSheep(Animal& sheep) {
        // 检查羊是否已经死亡
        if (sheep.getHealth() <= 0) {
            cout << "[警告] 这只羊已经死亡，无法捕食！" << endl;
            return;
        }
        int wolfHealthBefore = getHealth();
        int sheepHealthBefore = sheep.getHealth();
        // 狼吃羊后健康值提升50点
        setHealth(getHealth() + 50);
        // 狼饥饿度清零
        hunger = 0;
        // 羊被攻击后健康值下降100点
        sheep.setHealth(sheep.getHealth() - 100);
        cout << "狼捕食了一只羊！" << endl;
        cout << "  狼：生命值 " << wolfHealthBefore << " -> " << getHealth() << " (+50)" << endl;
        cout << "  羊：生命值 " << sheepHealthBefore << " -> " << sheep.getHealth() << " (-100)" << endl;
        // 羊生命值归零则死亡
        if (sheep.getHealth() <= 0) {
            sheep.setHealth(0);
            cout << " [提示] 羊因生命值归零而死亡！" << endl;
        }
    }
};

// 羊类：继承Animal，增加逃跑功能
class Sheep : public Animal {
public:
    Sheep(string sex, int age, int health, int x, int y)
        : Animal(sex, age, health, x, y) {
    }
    void print() override {
        cout << "---------- 羊 ----------" << endl;
        Animal::print();
        cout << "------------------------" << endl;
    }
    void move() override {
        health -= 5;
        pos.move(0, 1);
        cout << "羊在移动，生命值减少5点为：" << health << endl;
    }
    // 羊逃跑：被狼追击时逃跑
    void flee() {
        health -= 3;
        pos.move(-2, 0);
        cout << "羊逃跑，生命值减少3点为：" << health << endl;
    }
};

int main() {
    // 创建狼和羊对象
    Wolf w("公", 3, 500, 0, 0);
    Sheep s1("母", 2, 300, 5, 5);
    Sheep s2("公", 1, 250, 10, 10);
    cout << " 初始状态" << endl;
    w.print(); s1.print(); s2.print();
    cout << "\n狼移动" << endl;
    w.move();
    cout << "\n羊移动" << endl;
    s1.move();
    cout << "\n狼捕食羊" << endl;
    w.eatSheep(s1);
    cout << "\n 捕食后状态" << endl;
    w.print(); s1.print(); s2.print();
    cout << "\n========== 狼再次捕食已死亡的羊 ==========" << endl;
    w.eatSheep(s1);  // 羊已死亡，应提示无法捕食
    cout << "\n========== 羊逃跑 ==========" << endl;
    s2.flee(); s2.print();
    return 0;
}

 // 引用传递使函数能直接修改被传对象状态

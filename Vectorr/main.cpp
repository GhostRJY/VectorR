#include<iostream>
#include<utility>
#include"Vectorr.h"

int main() {
   
    //проверка работы моего вектора
    try {
        VectorR<int>myVec{1,0};
        std::cout << myVec << '\n';

        myVec.insertAt(0, 5);

        std::cout << myVec << '\n';
        
        //тест вставка элементов
        std::cout << "===============================\n"
                  << "Inserting elements\n"
                  << "-------------------------------\n";
        
        myVec.pushFront(4);
        std::cout << myVec << '\n';

        myVec.insertAt(1, 2);
        std::cout << myVec << '\n';

        myVec.insertAt(3, 3);
        std::cout <<myVec << '\n';

        myVec.pushBack(3);
        std::cout << myVec << '\n';

        //Реверс массива
        std::cout << "===============================\n"
                  << "REVERSE ARRAY!\n"
                  << "-------------------------------\n";
        myVec.reverse();
        std::cout << myVec << '\n';
                
        //тест сортировка элементов по возрастанию
        std::cout << "===============================\n"
                  << "SORT(ASC) ARRAY!\n"
                  << "-------------------------------\n";
        
        myVec.sortAsc();
        std::cout << myVec << '\n';

        //тест сортировка элементов по убыванию
        std::cout << "===============================\n"
                  << "SORT(DESC) ARRAY!\n"
                  << "-------------------------------\n";

        myVec.sortDesc();
        std::cout << myVec << '\n';

        //поиск элемента по значению
        std::cout << "===============================\n"
                  << "Elements by value search and delete\n"
                  << "-------------------------------\n";

        myVec.removeByValue(3);
        std::cout << myVec << '\n';

        //подгон size к capacity
        std::cout << "===============================\n"
            << "Trim capacity to size\n"
            << "-------------------------------\n";

        myVec.trimToSize();
        std::cout << myVec << '\n';

        //проверка на равенство
        std::cout << "===============================\n"
            << "Vectors comparison \n"
            << "-------------------------------\n";

        VectorR<int> myVec2{myVec};

        if(myVec2 == myVec)
            std::cout << "Vectors is equal\n";

        VectorR<int> myVec3{3,3};

        if(myVec3 != myVec)
            std::cout << "Vectors is not equal\n";

        //удаление элементОв
        std::cout << "===============================\n"
                  << "Delete elements\n"
                  << "-------------------------------\n";
        
        myVec.popFront();
        std::cout << myVec << '\n';

        myVec.popBack();
        std::cout << myVec << '\n';

        myVec.removeAt(3);
        std::cout << myVec << '\n';

        myVec.removeAt(2);
        std::cout << myVec << '\n';

        myVec.removeAt(1);
        std::cout << myVec << '\n';

        myVec.removeAt(0);        
        std::cout << myVec << '\n';
        
        //запись элементов при помощи cin
        //std::cin >> myVec;
        //std::cout << myVec << '\n';

    } catch(const std::exception &err) {
        std::cout << err.what();
    }
    
    getchar();

    return 0;
}
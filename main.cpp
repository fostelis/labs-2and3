#include <iostream>
#include "dynamic_array.h"
#include "linked_list.h"
#include "array_sequence.h"
#include "list_sequence.h"
#include "bit_sequence.h"
#include "algorithms.h"
#include "tests.h"
#include "io_utils.h"
//для ЛР-3
#include "piecewise_function.h"
#include "mutable_piecewise_function.h"
#include "immutable_piecewise_function.h"
#include "piecewise_enumerator.h"
#include "piecewise_operators.h"
#include "piecewise_algorithms.h"

using namespace std;

void run_piecewise_tests();

void run_lab_2();
void run_lab_3();

//UI ф-ии ЛР-2
void print_menu_lab_2();
void test_dynamic_array_ui();
void test_linked_list_ui();
void test_array_sequence_ui();
void test_immutable_ui();
void test_bit_ui();
void test_operators_ui();
void test_extended_ui();
void test_algorithms_ui();
void test_crtp_demo();

int main() {
    setlocale(LC_ALL, "Russian");
    int main_choice;
    do {
        cout << "\n========================================" << endl;
        cout << "    MAIN MENU: Select Lab" << endl;
        cout << "========================================" << endl;
        cout << "1. Lab 2 (Sequence, Arrays, Lists)" << endl;
        cout << "2. Lab 3 (Piecewise Function)" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> main_choice;
        switch (main_choice) {
            case 1: run_lab_2(); break;
            case 2: run_lab_3(); break;
            case 0: cout << "Exiting program..." << endl; break;
            default: cout << "Invalid choice." << endl;
        }
    } while (main_choice != 0);
    return 0;
}

void run_lab_2() {
    int choice;
    do {
        print_menu_lab_2();
        cin >> choice;
        switch (choice) {
            case 1: test_dynamic_array_ui(); break;
            case 2: test_linked_list_ui(); break;
            case 3: test_array_sequence_ui(); break;
            case 4: test_immutable_ui(); break;
            case 5: test_bit_ui(); break;
            case 6: test_operators_ui(); break;
            case 7: test_extended_ui(); break;
            case 8: test_algorithms_ui(); break;
            case 9: run_all_tests(); break;
            case 10: test_crtp_demo(); break;
            case 0: cout << "Back to main menu..." << endl; break;
            default: cout << "Invalid option." << endl;
        }
    } while (choice != 0);
}

void print_menu_lab_2() {
    cout << "\n=== LAB 2 MENU ===" << endl;
    cout << "1. DynamicArray" << endl;
    cout << "2. LinkedList" << endl;
    cout << "3. MutableArraySequence" << endl;
    cout << "4. ImmutableArraySequence" << endl;
    cout << "5. BitSequence" << endl;
    cout << "6. Operators" << endl;
    cout << "7. Extended Map-Reduce" << endl;
    cout << "8. Algorithms" << endl;
    cout << "9. Run All Tests (Lab 2)" << endl;
    cout << "10. CRTP Demo" << endl;
    cout << "0. Back" << endl;
    cout << "Choice: ";
}

void run_lab_3() {
    int choice;
    do {
        cout << "\n=== LAB 3 MENU (Piecewise Function) ===" << endl;
        cout << "1. Run Piecewise Function Tests" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: run_piecewise_tests(); break;
            case 0: cout << "Back to main menu..." << endl; break;
            default: cout << "Invalid option." << endl;
        }
    } while (choice != 0);
}

//UI реализации ЛР-2
void test_dynamic_array_ui() {
    cout << "\n=== DynamicArray ===" << endl;
    int arr[] = {1, 2, 3, 4, 5};
    dynamic_array<int> da(arr, 5);
    cout << "Values: ";
    for (int i = 0; i < da.get_size(); ++i) cout << da.get(i) << " ";
    cout << endl;
}

void test_linked_list_ui() {
    cout << "\n=== LinkedList ===" << endl;
    linked_list<int> list;
    list.append(1); list.append(2); list.prepend(0);
    cout << "Values: ";
    for (int i = 0; i < list.get_length(); ++i) cout << list.get(i) << " ";
    cout << endl;
}

void test_array_sequence_ui() {
    cout << "\n=== MutableArraySequence ===" << endl;
    mutable_array_sequence<int>* seq = new mutable_array_sequence<int>();
    seq->append(1); seq->append(2);
    cout << "Values: " << *seq << endl;
    delete seq;
}

void test_immutable_ui() {
    cout << "\n=== ImmutableArraySequence ===" << endl;
    immutable_array_sequence<int>* seq = new immutable_array_sequence<int>();
    seq = dynamic_cast<immutable_array_sequence<int>*>(seq->append(1));
    seq = dynamic_cast<immutable_array_sequence<int>*>(seq->append(2));
    cout << "Values: " << *seq << endl;
    delete seq;
}

void test_bit_ui() {
    cout << "\n=== BitSequence ===" << endl;
    bit_sequence* bs = new bit_sequence();
    bs->append(bit(true)); bs->append(bit(false)); bs->append(bit(true));
    cout << "Bits: ";
    for (int i = 0; i < bs->get_length(); ++i)
        cout << (bs->get(i).get_value() ? "1" : "0");
    cout << endl;
    delete bs;
}

void test_operators_ui() {
    cout << "\n=== Operators ===" << endl;
    mutable_array_sequence<int>* s1 = new mutable_array_sequence<int>();
    mutable_array_sequence<int>* s2 = new mutable_array_sequence<int>();
    s1->append(1); s2->append(2);
    cout << "Equal: " << (*s1 == *s1 ? "yes" : "no") << endl;
    sequence<int>* concat = *s1 + *s2;
    cout << "Concat: " << *concat << endl;
    delete concat; delete s1; delete s2;
}

void test_extended_ui() {
    cout << "\n=== Extended Map-Reduce ===" << endl;
    mutable_array_sequence<int>* seq = new mutable_array_sequence<int>();
    for (int i = 1; i <= 5; ++i) seq->append(i);
    cout << "Original: " << *seq << endl;
    sequence<int>* skipped = seq->skip(2);
    cout << "Skip(2): " << *skipped << endl;
    delete skipped; delete seq;
}

void test_algorithms_ui() {
    cout << "\n=== Algorithms ===" << endl;
    mutable_array_sequence<int>* seq = new mutable_array_sequence<int>();
    for (int i = 1; i <= 5; ++i) seq->append(i);
    stats<int> st = get_stats(*seq);
    cout << "Stats: min=" << st.min_val << ", max=" << st.max_val << ", avg=" << st.avg_val << endl;
    delete seq;
}

void test_crtp_demo() {
    cout << "\n=== CRTP Demo ===" << endl;
    cout << "Demonstrating Curiously Recurring Template Pattern" << endl;
    mutable_array_sequence_crtp<int>* seq = new mutable_array_sequence_crtp<int>();
    seq->append(10); seq->append(20);
    cout << "Mutable CRTP: " << *seq << endl;
    immutable_array_sequence_crtp<int>* imm = new immutable_array_sequence_crtp<int>();
    imm = dynamic_cast<immutable_array_sequence_crtp<int>*>(imm->append(100));
    cout << "Immutable CRTP: " << *imm << endl;
    delete seq; delete imm;
    cout << "[INFO] CRTP allows compile-time polymorphism instead of virtual functions" << endl;
}
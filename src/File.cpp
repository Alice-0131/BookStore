#include "../include/File.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <cstring>

int memory::compare(std::pair<char[64], int> a, std::pair<char[64], int> b) {
    if (strcmp(a.first, b.first)) {
        return strcmp(a.first, b.first);
    }
    if (a.second > b.second) {
        return 1;
    } else if (a.second < b.second){
        return -1;
    } else {
        return 0;
    }
}

memory::memory(const std::string &file1_name, const std::string &file2_name):
    file1_name(file1_name), file2_name(file2_name) {
    //file1.open(file1_name, std::ios::binary | std::ios::in | std::ios::out);
    //file2.open(file2_name, std::ios::binary | std::ios::in | std::ios::out);
    file1.open(file1_name, std::ios::binary | std::ios::out);
    file2.open(file2_name, std::ios::binary | std::ios::out);
    if (!file1) {
        file1.open(file1_name, std::ios::out | std::ios::binary);
    }
    if (!file2) {
        file2.open(file2_name, std::ios::out | std::ios::binary);
    }

    headnode tmp;
    head = new node(tmp);
    rear = head;
    while(true) {
        file1.read(reinterpret_cast<char *>(&tmp), sizeof(headnode));
        if (file1.eof()) {
            break;
        }
        rear -> next = new node(tmp);
        rear = rear -> next;
    }
    file1.close();
    file2.close();
}

memory::~memory() {
    file1.open(file1_name, std::ios::binary | std::ios::out);
    node* current = head -> next;
    delete head;
    while(current != nullptr) {
        file1.write(reinterpret_cast<char *>(&current -> info), sizeof(headnode));
        node* next = current -> next;
        delete current;
        current = next;
    }
}

void memory::Insert(int value, const std::string &index) {
    file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file2) {
        std::cout <<"fail" << std::endl;
    }
    node* current = head -> next;
    std::pair<char[64], int> tmp;
    std::strncpy(tmp.first, index.c_str(), 63);
    tmp.first[63] = '\0';
    tmp.second = value;
    arr a;
    while (current != nullptr) {
        if (compare(current->info.max, tmp) >= 0 || current -> next == nullptr){ // 找到需要插入的那一页
            file2.seekg(current -> info.ptr * sizeof(arr));
            file2.read(reinterpret_cast<char *>(&a), sizeof(arr));
            int i = 0;
            for (; i < current -> info.size; ++i) {
                if (compare(a[i], tmp) >= 0) { // 找到需要插入的数组位置
                    break;
                }
            }
            if (current -> info.size < size) { // 无需分块
                for (int j = current -> info.size; j > i; --j) {
                    strncpy(a[j].first, a[j - 1].first, 63);
                    a[j].first[63] = '\0';
                    a[j].second = a[j - 1].second;
                }
                strncpy(a[i].first, tmp.first, 63);
                a[i].first[63] = '\0';
                a[i].second = tmp.second;
                file2.seekp(current->info.ptr * sizeof(arr));
                file2.write(reinterpret_cast<char *>(&a), sizeof(arr));
                strncpy(current->info.max.first, a[current->info.size].first, 63);
                current->info.max.first[63] = '\0';
                current->info.max.second = a[current->info.size].second;
                strncpy(current->info.min.first, a[0].first, 63);
                current->info.min.first[63] = '\0';
                current->info.min.second = a[0].second;
                ++current->info.size;
            } else { // 分块操作
                headnode h;
                arr a_;
                if (i == size) { // 该元素插到下一数组中
                    strncpy(a_[0].first, tmp.first, 63);
                    a_[0].first[63] = '\0';
                    a_[0].second = tmp.second;
                    file2.seekp(0, std::ios::end);
                    h.size = 1;
                    h.ptr = file2.tellp() / sizeof(arr);
                    file2.write(reinterpret_cast<char *>(&a_), sizeof(arr));
                    strncpy(h.max.first, a_[h.size - 1].first, 63);
                    h.max.first[63] = '\0';
                    h.max.second = a_[h.size - 1].second;
                    strncpy(h.min.first, a_[0].first, 63);
                    h.min.first[63] = '\0';
                    h.min.second = a_[0].second;
                    node* n = new node(h);
                    n->next = current->next;
                    current->next = n;
                } else { // 该元素插到当前数组中
                    for (int j = i; j < current->info.size; ++j) {
                        strncpy(a_[j - i].first, a[j].first, 63);
                        a_[j - i].first[63] = '\0';
                        strncpy(a[j].first, "\0", 63);
                        a[j].first[63] = '\0';
                        a_[j - i].second = a[j].second;
                        a[j].second = 0;
                    }
                    strncpy(a[i].first, tmp.first, 63);
                    a[i].first[63] = '\0';
                    a[i].second = tmp.second;
                    file2.seekp(current->info.ptr * sizeof(arr));
                    file2.write(reinterpret_cast<char *>(&a), sizeof(arr));
                    file2.seekp(0, std::ios::end);
                    h.size = current->info.size - i;
                    h.ptr = file2.tellp() / sizeof(arr);
                    file2.write(reinterpret_cast<char *>(&a_), sizeof(arr));
                    strncpy(h.max.first, a_[h.size - 1].first, 63);
                    h.max.first[63] = '\0';
                    h.max.second = a_[h.size - 1].second;
                    strncpy(h.min.first, a_[0].first, 63);
                    h.min.first[63] = '\0';
                    h.min.second = a_[0].second;
                    node* n = new node(h);
                    strncpy(current->info.max.first, tmp.first, 63);
                    current->info.size = i + 1;
                    current->info.max.first[63] = '\0';
                    current->info.max.second = tmp.second;
                    strncpy(current->info.min.first, a[0].first,63);
                    current->info.min.first[63] = '\0';
                    current->info.min.second = a[0].second;
                    current->info.size = i + 1;
                    n->next = current->next;
                    current->next = n;
                }
            }
            file2.close();
            return;
        }
        current = current -> next;
    }
    // 若只有头结点
    strncpy(a[0].first, tmp.first, 63);
    a[0].first[63] = '\0';
    a[0].second = tmp.second;
    file2.seekp(0);
    file2.write(reinterpret_cast<char *>(&a), sizeof(arr));
    headnode h;
    strncpy(h.max.first, tmp.first, 63);
    h.max.first[63] = '\0';
    h.max.second = tmp.second;
    strncpy(h.min.first, tmp.first, 63);
    h.min.first[63] = '\0';
    h.min.second = tmp.second;
    h.size = 1;
    rear = new node(h);
    head -> next = rear;
    file2.close();
}

void memory::Delete(int value, const std::string& index) {
    file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file2) {
        std::cout <<"fail" << std::endl;
    }
    node* current = head;
    std::pair<char[64], int> tmp;
    for (int i = 0; i < index.size(); ++i) {
        tmp.first[i] = index[i];
    }
    tmp.second = value;
    arr a;
    while (current->next != nullptr) {
        if (compare(current->next -> info.min, tmp) <= 0 && compare(current->next -> info.max, tmp) >= 0) { // 找到要删除的元素所在的那一页
            file2.seekg(current->next -> info.ptr * sizeof(arr));
            file2.read(reinterpret_cast<char *>(&a), sizeof(arr));
            for (int i = 0; i < current->next -> info.size; ++i) {
                if (compare(a[i], tmp) == 0) {
                    if (current->next->info.size == 1) { // 删除后为空页
                        node* t = current -> next;
                        current->next = t -> next;
                        delete t;
                    } else {
                        for (int j = i; j < current->next->info.size - 1; ++j) {
                            strncpy(a[j].first, a[j + 1].first, 63);
                            a[j].first[63] = '\0';
                            a[j].second = a[j + 1].second;
                        }
                        --current->next->info.size;
                        strncpy(a[current->next->info.size].first, "\0", 63);
                        a[current->next->info.size].first[63] = '\0';
                        a[current->next->info.size].second = 0;
                        strncpy(current->next->info.max.first, a[current->next->info.size - 1].first, 63);
                        current->next->info.max.first[63] = '\0';
                        current->next->info.max.second = a[current->next->info.size - 1].second;
                        strncpy(current->next->info.min.first, a[0].first,63);
                        current->next->info.min.first[63] = '\0';
                        current->next->info.min.second = a[0].second;
                        file2.seekp(current->next->info.ptr * sizeof(arr));
                        file2.write(reinterpret_cast<char *>(&a), sizeof(arr));
                    }
                    file2.close();
                    return;
                }
                if (compare(a[i], tmp) >= 0) { // 删除元素不存在
                    file2.close();
                    return;
                }
            }
        }
        current = current -> next;
    }
    file2.close();
}

std::vector<int> memory::Find(const std::string &index) {
    file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
    node* current = head -> next;
    char tmp[64] = {0};
    for (int i = 0; i < index.size(); ++i) {
        tmp[i] = index[i];
    }
    arr a;
    std::vector<int> array;
    while (current != nullptr) {
        if (strcmp(current->info.min.first, tmp) <= 0 && strcmp(current->info.max.first, tmp) >= 0) {
            file2.seekg(current -> info.ptr * sizeof(arr));
            file2.read(reinterpret_cast<char *>(&a), sizeof(arr));
            for (int i = 0; i < current->info.size; ++i) {
                if (strcmp(a[i].first, tmp) == 0) {
                    array.push_back(a[i].second);
                }
                if (strcmp(a[i].first, tmp) > 0) {
                    break;
                }
            }
        }
        if (strcmp(current->info.min.first, tmp) > 0) {
            break;
        }
        current = current -> next;
    }
    file2.close();
    return array;
}

std::vector<int> memory::readAll() {
    file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
    node* current = head -> next;
    arr a;
    std::vector<int> array;
    while (current != nullptr) {
        file2.seekg(current -> info.ptr * sizeof(arr));
        file2.read(reinterpret_cast<char *>(&a), sizeof(arr));
        for (int i = 0; i < current->info.size; ++i) {
            array.push_back(a[i].second);
        }
        current = current->next;
    }
    file2.close();
    return array;
}
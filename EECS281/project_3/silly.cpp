// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  silly.cpp
//  project_3
//
//  Created by Ellen Chlachidze on 10/30/22.
//

#include <stdio.h>
#include "TableEntry.h"
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include "xcode_redirect.hpp"
#include <queue>
#include <cmath>
#include <cassert>
#include <unordered_map>
#include <map>
using namespace std;

class sillytable {
private:
    uint columns_num;
    std::string name;
    std::vector<std::vector<TableEntry>> data;
    std::map<TableEntry, vector<uint>> bst_index;
    std::unordered_map<TableEntry, vector<uint>> hash_index;
    
public:
    std::vector<pair<std::string, std::string>> columns;
    bool indexed = false;
    bool which_index = false;
    uint indexed_col = 0;
    bool quiet;
    class table_entry_comp {
    private:
        uint col_to_check;
        char compare;
        vector<TableEntry> val;
    public:
        explicit table_entry_comp(sillytable input) {
            val.reserve(1);
            string col_in;
            cin >> col_in;
            col_to_check = static_cast<uint>(find_if(input.columns.begin(), input.columns.end(), [col_in] (pair<string, string> a) {return a.second == col_in;}) - input.columns.begin());
            cin >> compare;
            switch (input.columns[col_to_check].first[0]) {
                case 'd' : {
                    double temp_in;
                    cin >> temp_in;
                    val.emplace_back(TableEntry(temp_in));
                    break;
                }
                case 'i' : {
                    int temp_in;
                    cin >> temp_in;
                    val.emplace_back(TableEntry(temp_in));
                    break;
                }
                case 'b' : {
                    string temp_in;
                    cin >> temp_in;
                    bool real_temp_in;
                    if (temp_in == "true") {
                        real_temp_in = true;
                    }
                    else {
                        real_temp_in = false;
                    }
                    val.emplace_back(TableEntry(real_temp_in));
                    break;
                }
                case 's' : {
                    string temp_in;
                    cin >> temp_in;
                    val.emplace_back(TableEntry(temp_in));
                    break;
                }
            }
        }
        uint get_col() {
            return col_to_check;
        }
        char get_comp() {
            return compare;
        }
        TableEntry get_val() {
            return val.front();
        }
        bool operator() (uint row_num, sillytable* mom) {
            switch (compare) {
                case '=' : {
                    return mom->data[row_num][col_to_check] == val.front();
                }
                case '>' : {
                    return mom->data[row_num][col_to_check] > val.front();
                }
                case '<' : {
                    return mom->data[row_num][col_to_check] < val.front();
                }
                default : {
                    return false;
                }
            }
        }
    };
    sillytable(string name, uint cols, bool quiet_in) : columns_num(cols), name(name), quiet(quiet_in) {
        for (uint i = 0; i < columns_num; i++) {
            columns.resize(static_cast<uint>(columns.size()) + 1);
            string type_in;
            cin >> type_in;
            columns[i].first =  type_in;
        }
        for (uint i = 0; i < columns_num; i++) {
            string col_name_in;
            cin >> col_name_in;
            columns[i].second =  col_name_in;
        }
        cout << "New table " << name << " with column(s) ";
        for (uint i = 0; i < columns_num; i++) {
            cout << columns[i].second << " ";
        }
        cout << "created\n";
    }
    
    void print_entry(uint row, uint col) {
            if (columns[col].first == "bool") {
                if (data[row][col] == TableEntry(true)) {
                    std::cout << "true ";
                }
                else {
                    std::cout << "false ";
                }
            }
            else {
                std::cout << data[row][col] << " ";
            }
        
    }
    
    void print_rows(int num_cols) {
        if (quiet) {
            int num_printed = 0;
            vector<uint> cols_print;
            for (int i = 0; i < num_cols; i++) {
                string cols_print_in;
                cin >> cols_print_in;
                auto it = find_if(columns.begin(), columns.end(), [cols_print_in] (pair<string, string> a) {return a.second == cols_print_in;});
                if (it == columns.end()) {
                    std::cout << "Error during PRINT: " << cols_print_in << " does not name a column in " << name << "\n";
                    string trash;
                    getline(cin, trash);
                    return;
                }
                else { cols_print.push_back(static_cast<uint>(it - columns.begin()));
                }
            }
            string trash;
            cin >> trash;
            if (trash[0] != 'A') {
                table_entry_comp comp{*this};
                if (!indexed) {
                    uint i = 0;
                    num_printed += static_cast<int>(count_if(data.begin(), data.end(), [comp, i, this](vector<TableEntry> input) mutable {
                        i++;
                        return comp(i-1, this);
                        if (input.size() == 0) {
                            return false;
                        }
                    }));
                }
                else {
                    if (!which_index && indexed_col == comp.get_col()) {
                        //bst index!
                        if (comp.get_comp() == '>') {
                            auto it = bst_index.upper_bound(TableEntry(comp.get_val()));
                            while (it != bst_index.end()) {
                                num_printed += static_cast<int>(it->second.size());
                                it++;
                            }
                        }
                        if (comp.get_comp() == '<') {
                            
                            auto it = bst_index.lower_bound(comp.get_val());
                            auto start = bst_index.begin();
                            while (start != it) {
                                num_printed += static_cast<int>(start->second.size());
                                start++;
                            }
                        }
                        if (comp.get_comp() == '=') {
                            auto it_start = bst_index.equal_range(comp.get_val()).first;
                            auto it_end = bst_index.equal_range(comp.get_val()).second;
                            while (it_start != it_end) {
                                num_printed += static_cast<int>(it_start->second.size());
                                it_start++;
                            }
                        }
                    }
                    else if (which_index && comp.get_comp() == '='  && indexed_col == comp.get_col()) {
                            auto start = hash_index.equal_range(comp.get_val()).first;
                            auto end = hash_index.equal_range(comp.get_val()).second;
                            while (start != end) {
                                num_printed += static_cast<uint>(start->second.size());
                                start++;
                            }
                        }
                    else {
                        uint i = 0;
                        num_printed += static_cast<int>(count_if(data.begin(), data.end(), [comp, i, this](vector<TableEntry> input) mutable {
                            i++;
                            return comp(i-1, this);
                            if (input.size() == 0) {
                                return false;
                            }
                        }));
                    }
                }
            }
            else if ((indexed && !which_index)) {
                num_printed += static_cast<uint>(data.size());
                
            }
            else {
                num_printed += static_cast<int>(data.size());
            }
                std::cout << "Printed " << num_printed << " matching rows from " << name << "\n";
        }
        else {
            int num_printed = 0;
            vector<uint> cols_print;
            for (int i = 0; i < num_cols; i++) {
                string cols_print_in;
                cin >> cols_print_in;
                auto it = find_if(columns.begin(), columns.end(), [cols_print_in] (pair<string, string> a) {return a.second == cols_print_in;});
                if (it == columns.end()) {
                    std::cout << "Error during PRINT: " << cols_print_in << " does not name a column in " << name << "\n";
                    string trash;
                    getline(cin, trash);
                    return;
                }
                else { cols_print.push_back(static_cast<uint>(it - columns.begin()));
                }
            }
            string trash;
            cin >> trash;
      
                for (uint k = 0; k < static_cast<uint>(cols_print.size()); k++) {
                    std::cout << columns[cols_print[k]].second << " ";
                }
                std::cout << "\n";
          
            
            
            
            if (trash[0] != 'A') {
                table_entry_comp comp{*this};
                if (!indexed) {
                    for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                        if (comp(i, this)) {
                            num_printed++;
                            for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                print_entry(i, cols_print[j]);
                            }
                         
                                std::cout << "\n";
                            
                        }
                    }
                }
                else {
                    if (!which_index && indexed_col == comp.get_col()) {
                        //bst index!
                        if (comp.get_comp() == '>') {
                            auto it = bst_index.upper_bound(TableEntry(comp.get_val()));
                            while (it != bst_index.end()) {
                                for (uint k = 0; k < static_cast<uint>(it->second.size()); k++) {
                                    num_printed++;
                                    for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                   
                                            print_entry(it->second[k], cols_print[j]);
                                        
                                    }
                          
                                        std::cout << "\n";
                                
                                }
                                it++;
                            }
                        }
                        if (comp.get_comp() == '<') {
                            
                            auto it = bst_index.lower_bound(comp.get_val());
                            auto start = bst_index.begin();
                            while (start != it) {
                                for (uint k = 0; k < static_cast<uint>(start->second.size()); k++) {
                                    num_printed++;
                                    for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                        print_entry(start->second[k], cols_print[j]);
                                    }
                                
                                        std::cout << "\n";
                                    
                                }
                                start++;
                            }
                        }
                        if (comp.get_comp() == '=') {
                            auto it_start = bst_index.equal_range(comp.get_val()).first;
                            auto it_end = bst_index.equal_range(comp.get_val()).second;
                            while (it_start != it_end) {
                                for (uint k = 0; k < static_cast<uint>(it_start->second.size()); k++) {
                                    num_printed++;
                                    for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                        print_entry(it_start->second[k], cols_print[j]);
                                    }
                          
                                        std::cout << "\n";
                                    
                                }
                                it_start++;
                            }
                        }
                    }
                    else if (which_index && comp.get_comp() == '='  && indexed_col == comp.get_col()) {
                        auto start = hash_index.equal_range(comp.get_val()).first;
                        auto end = hash_index.equal_range(comp.get_val()).second;
                        while (start != end) {
                            for (uint k = 0; k < static_cast<uint>(start->second.size()); k++) {
                                num_printed++;
                                for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                    print_entry(start->second[k], cols_print[j]);
                                }
                          
                                    std::cout << "\n";
                                
                            }
                            start++;
                        }
                    }
                    else {
                        for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                            if (comp(i, this)) {
                                num_printed++;
                                for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                                    print_entry(i, cols_print[j]);
                                }
                     
                                    std::cout << "\n";
                                
                            }
                        }
                    }
                }
            }
            else if ((indexed && !which_index)) {
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    num_printed++;
                    for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                        print_entry(i, cols_print[j]);
                    }
          
                        std::cout << "\n";
                    
                }
                
            }
            else {
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    num_printed++;
                    for (uint j = 0; j < static_cast<uint>(cols_print.size()); j++) {
                        print_entry(i, cols_print[j]);
                    }
          
                        std::cout << "\n";
                    
                }
            }
            std::cout << "Printed " << num_printed << " matching rows from " << name << "\n";
        }
    }
    
    void delete_rows() {
        string trash;
        cin >> trash;
        table_entry_comp comp{*this};
        uint count = 0;
        int deleted_amount = 0;
        auto it = remove_if(data.begin(), data.end(), [&](vector<TableEntry> input) mutable {
                        TableEntry trash = input[0];
                        count++;
            return comp(count - 1, this);});
        auto r = std::distance(it, data.end());
        data.erase(it, data.end());
        deleted_amount = static_cast<int>(r);
//        int deleted_amount = static_cast<int>(erase_if(data, [&](vector<TableEntry> input) mutable {
//            TableEntry trash = input[0];
//            count++;
//            return comp(count - 1, *this);}));
        std::cout << "Deleted " << deleted_amount << " rows from " << name << "\n";
        if (indexed) {
            if (which_index) {
                hash_index.clear();
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    vector<uint> insert_vec;
                    insert_vec.push_back(i);
                    if (hash_index.find(data[i][indexed_col]) == hash_index.end()) {
                        hash_index.insert(make_pair(data[i][indexed_col], insert_vec));
                    }
                    else {
                        hash_index.find(data[i][indexed_col])->second.push_back(i);
                    }
                }
            }
            else {
                bst_index.clear();
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    vector<uint> insert_vec;
                    insert_vec.push_back(i);
                    if (bst_index.find(data[i][indexed_col]) == bst_index.end()) {
                        bst_index.insert(make_pair(data[i][indexed_col], insert_vec));
                    }
                    else {
                        bst_index.find(data[i][indexed_col])->second.push_back(i);
                    }
                }
            }
        }
    }
    
    void join_tables(sillytable& other) {
        if (quiet) {
            int num_printed = 0;
            string first_col_in, second_col_in, trash;
            cin >> first_col_in >> trash >> second_col_in;
            uint first_col = static_cast<uint>(find_if(columns.begin(), columns.end(), [first_col_in] (pair<string, string> a) {return a.second == first_col_in;}) - columns.begin());
            uint second_col = static_cast<uint>(find_if(other.columns.begin(), other.columns.end(), [second_col_in] (pair<string, string> a) {return a.second == second_col_in;}) - other.columns.begin());
            uint num_print;
            cin >> trash >> trash >> num_print;
            vector<tuple<string, uint, uint>> to_print;
            for (uint i = 0; i < num_print; i++) {
                string name_in;
                uint table_in;
                cin >> name_in >> table_in;
                uint col_in = 0;
                if (table_in == 1) {
                    col_in = static_cast<uint>(find_if(columns.begin(), columns.end(), [name_in] (pair<string, string> a)
                                                          {return a.second == name_in;}) - columns.begin());
                }
                else {
                    col_in = static_cast<uint>(find_if(other.columns.begin(), other.columns.end(), [name_in] (pair<string, string> a)
                                                          {return a.second == name_in;}) - other.columns.begin());
                }
                to_print.emplace_back(make_tuple(name_in, table_in, col_in));
            }
            if (other.indexed && other.indexed_col == second_col) {
                if (other.which_index) {
                    for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                        auto it = other.hash_index.find(data[i][first_col]);
                        if (it != other.hash_index.end()) {
                        auto itt = it->second.begin();
                        while (itt != it->second.end()) {
                            num_printed++;
                            itt++;
                        }
                        
                    }
                    }
                }
                else {
                    for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                        auto it = other.bst_index.find(data[i][first_col]);
                        if (it != other.bst_index.end()) {
                            auto itt = it->second.begin();
                            while (itt != it->second.end()) {
                                num_printed++;
                                itt++;
                            }
                        }
                    }
                }
                
            }
            else {
                std::unordered_map<TableEntry, deque<uint>> temp_hash;
                for (uint i = 0; i < static_cast<uint>(other.data.size()); i++) {
                    if (temp_hash.find(other.data[i][second_col]) != temp_hash.end()) {
                        temp_hash.find(other.data[i][second_col])->second.push_back(i);
                    }
                    else {
                        deque<uint> insert_deque;
                        insert_deque.push_back(i);
                        temp_hash.insert(make_pair(other.data[i][second_col], insert_deque));
                    }
                }
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    auto it = temp_hash.find(data[i][first_col]);
                    if (it != temp_hash.end()) {
                        auto itt = it->second.begin();
                        while (itt != it->second.end()) {
                            num_printed++;
                            itt++;
                        }
                    }
                }
                
            }
            std::cout << "Printed " << num_printed << " rows from joining " << name << " to " << other.name << "\n";
            return;
        }
        int num_printed = 0;
        string first_col_in, second_col_in, trash;
        cin >> first_col_in >> trash >> second_col_in;
        uint first_col = static_cast<uint>(find_if(columns.begin(), columns.end(), [first_col_in] (pair<string, string> a) {return a.second == first_col_in;}) - columns.begin());
        uint second_col = static_cast<uint>(find_if(other.columns.begin(), other.columns.end(), [second_col_in] (pair<string, string> a) {return a.second == second_col_in;}) - other.columns.begin());
        uint num_print;
        cin >> trash >> trash >> num_print;
        vector<tuple<string, uint, uint>> to_print;
        for (uint i = 0; i < num_print; i++) {
            string name_in;
            uint table_in;
            cin >> name_in >> table_in;
            uint col_in = 0;
            if (table_in == 1) {
                col_in = static_cast<uint>(find_if(columns.begin(), columns.end(), [name_in] (pair<string, string> a)
                                                      {return a.second == name_in;}) - columns.begin());
            }
            else {
                col_in = static_cast<uint>(find_if(other.columns.begin(), other.columns.end(), [name_in] (pair<string, string> a)
                                                      {return a.second == name_in;}) - other.columns.begin());
            }
            to_print.emplace_back(make_tuple(name_in, table_in, col_in));
        }
            for (uint a = 0; a < static_cast<uint>(to_print.size()); a++) {
                std::cout << get<0>(to_print[a]) << " ";
            }
        std::cout << "\n";
        if (other.indexed && other.indexed_col == second_col) {
            if (other.which_index) {
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    auto it = other.hash_index.find(data[i][first_col]);
                    if (it != other.hash_index.end()) {
                    auto itt = it->second.begin();
                    while (itt != it->second.end()) {
                        num_printed++;
                        for (uint k = 0; k < static_cast<uint>(to_print.size()); k++) {
                            if (get<1>(to_print[k]) == 1) {
                                print_entry(i, get<2>(to_print[k]));
                            }
                            else {
                                other.print_entry(*itt, get<2>(to_print[k]));
                            }
                        }
                        itt++;
                        std::cout << "\n";
                        
                    }
                    
                }
                }
            }
            else {
                for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                    auto it = other.bst_index.find(data[i][first_col]);
                    if (it != other.bst_index.end()) {
                        auto itt = it->second.begin();
                        while (itt != it->second.end()) {
                            num_printed++;
                            for (uint k = 0; k < static_cast<uint>(to_print.size()); k++) {
                                
                                    if (get<1>(to_print[k]) == 1) {
                                        print_entry(i, get<2>(to_print[k]));
                                    }
                                    else {
                                        other.print_entry(*itt, get<2>(to_print[k]));
                                    }
                            
                            }
                            itt++;
                            std::cout << "\n";
                            
                        }
                    }
                }
            }
            
        }
        else {
            std::unordered_map<TableEntry, deque<uint>> temp_hash;
            for (uint i = 0; i < static_cast<uint>(other.data.size()); i++) {
                if (temp_hash.find(other.data[i][second_col]) != temp_hash.end()) {
                    temp_hash.find(other.data[i][second_col])->second.push_back(i);
                }
                else {
                    deque<uint> insert_deque;
                    insert_deque.push_back(i);
                    temp_hash.insert(make_pair(other.data[i][second_col], insert_deque));
                }
            }
            for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                auto it = temp_hash.find(data[i][first_col]);
                if (it != temp_hash.end()) {
                    auto itt = it->second.begin();
                    while (itt != it->second.end()) {
                        num_printed++;
                        for (uint k = 0; k < static_cast<uint>(to_print.size()); k++) {
                                if (get<1>(to_print[k]) == 1) {
                                    print_entry(i, get<2>(to_print[k]));
                                }
                                else {
                                    other.print_entry(*itt, get<2>(to_print[k]));
                                }
                        }
                        itt++;
                        std::cout << "\n";
                    }
                }
            }
            
        }
        std::cout << "Printed " << num_printed << " rows from joining " << name << " to " << other.name << "\n";
    }
    
    void generate_index(string index) {
        indexed = true;
        string trash, col_in;
        cin >> trash >> trash >> col_in;
        auto it = find_if(columns.begin(), columns.end(), [col_in] (pair<string, string> a) {return a.second == col_in;});
        if (it == columns.end()) {
            std::cout << "Error during GENERATE: " << col_in << " does not name a column in " << name << "\n";
            getline(cin, trash);
            return;
        }
        uint index_col = static_cast<uint>(it - columns.begin());
        indexed_col = index_col;
        if (indexed) {
            if (index[0] == 'h') {
                hash_index.clear();
            }
            else {
                bst_index.clear();
            }
        }
        if (index[0] == 'h') {
            which_index = true;
            for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                auto it = hash_index.find(data[i][index_col]);
                if (it != hash_index.end()) {
                    it->second.push_back(i);
                }
                else {
                    vector<uint> insert_vec;
                    insert_vec.push_back(i);
                    hash_index.insert(make_pair(data[i][index_col], insert_vec));
                }
            }
            std::cout << "Created hash index for table " << name << " on column " << col_in << "\n";
        }
        else {
            which_index = false;
            for (uint i = 0; i < static_cast<uint>(data.size()); i++) {
                if (bst_index.find(data[i][index_col]) != bst_index.end()) {
                    bst_index.find(data[i][index_col])->second.push_back(i);
                }
                else {
                    vector<uint> insert_vec;
                    insert_vec.push_back(i);
                    bst_index.insert(make_pair(data[i][index_col], insert_vec));
                }
            }
            std::cout << "Created bst index for table " << name << " on column " << col_in << "\n";
        }
    }
    
    void insert_rows(uint num_rows) {
        data.reserve(num_rows);
//        for (size_t i = pre_rows; i < pre_rows+num_rows; i++) {
//            data[i].reserve(columns_num);
//        }
        for (uint i = 0; i < num_rows; i++) {
            vector<TableEntry> row;
            for (uint j = 0; j < columns_num; j++) {
                if (columns[j].first == "double") {
                    double table_in;
                    cin >> table_in;
                    if (indexed && indexed_col == j) {
                        if (which_index) {
                            if (hash_index.find(TableEntry(table_in)) == hash_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                hash_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                hash_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                        else {
                            if (bst_index.find(TableEntry(table_in)) == bst_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                bst_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                bst_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                    }
                    row.emplace_back(TableEntry(table_in));
                }
                if (columns[j].first == "int") {
                    int table_in;
                    cin >> table_in;
                    if (indexed && indexed_col == j) {
                        if (which_index) {
                            if (hash_index.find(TableEntry(table_in)) == hash_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                hash_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                hash_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                        else {
                            if (bst_index.find(TableEntry(table_in)) == bst_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                bst_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                bst_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                    }
                    row.emplace_back(TableEntry(table_in));
                }
                if (columns[j].first == "bool") {
                    string table_in;
                    cin >> table_in;
                    bool real_table_in;
                    if (table_in == "true") {
                        real_table_in = true;
                    }
                    else {
                        real_table_in = false;
                    }
                    if (indexed && indexed_col == j) {
                        if (which_index) {
                            if (hash_index.find(TableEntry(real_table_in)) == hash_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                hash_index.insert(make_pair(TableEntry(real_table_in), insert_vec));
                            }
                            else {
                                hash_index.find(TableEntry(real_table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                        else {
                            if (bst_index.find(TableEntry(real_table_in)) == bst_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                bst_index.insert(make_pair(TableEntry(real_table_in), insert_vec));
                            }
                            else {
                                bst_index.find(TableEntry(real_table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                    }
                    row.emplace_back(TableEntry(real_table_in));
                }
                if (columns[j].first == "string") {
                    string table_in;
                    cin >> table_in;
                    if (indexed && indexed_col == j) {
                        if (which_index) {
                            if (hash_index.find(TableEntry(table_in)) == hash_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                hash_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                hash_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                        else {
                            if (bst_index.find(TableEntry(table_in)) == bst_index.end()) {
                                vector<uint> insert_vec;
                                insert_vec.push_back(static_cast<uint>(data.size()));
                                bst_index.insert(make_pair(TableEntry(table_in), insert_vec));
                            }
                            else {
                                bst_index.find(TableEntry(table_in))->second.push_back(static_cast<uint>(data.size()));
                            }
                        }
                    }
                    row.emplace_back(TableEntry(table_in));
                }
                
            }
            data.push_back(row);
        }
        cout << "Added " << num_rows << " rows to " << name << " from position " << static_cast<uint>(data.size()) - num_rows <<
        " to " << static_cast<uint>(data.size()) - 1 << "\n";
    }
    
    
};

int main(int argc, char *argv[]) {
    xcode_redirect(argc, argv);
    ios_base::sync_with_stdio(false);
    bool quiet = false;
    if ((argc == 2 && (argv[1][1] == 'q' || argv[1][2] == 'q')) ||
        (argc >= 3 && (argv[2][1] == 'q' || argv[2][2] == 'q'))) {
        quiet = true;
    }
    std::unordered_map<std::string, sillytable> tables;
    std::string cmd;
    std::string trash;
    do {
        if (cin.fail()) {
            cerr << "Error: Reading from cin has failed" << endl;
            exit(1);
        }
        cout << "% ";
        cin >> cmd;
        
        switch (cmd[0]) {
            case '#' : {
                getline(cin, trash);
                break;
            }
            case 'C': {
                std::string name;
                cin >> name;
                uint cols;
                cin >> cols;
                if (tables.find(name) != tables.end()) {
                    string trash;
                    getline(cin, trash);
                    std::cout << "Error during CREATE: Cannot create already existing table " << name << "\n";
                    break;
                }
                else {
                    tables.insert(make_pair(name, sillytable(name, cols, quiet)));
                    break;
                }
            }
            case 'R': {
                string remove_name;
                cin >> remove_name;
                if (tables.find(remove_name) != tables.end()) {
                    std::cout << "Table " << remove_name << " deleted\n";
                    tables.erase(remove_name);
                    break;
                }
                else {
                    string trash;
                    getline(cin, trash);
                    std::cout << "Error during REMOVE: " << remove_name << " does not name a table in the database\n";
                    break;
                }
            }
            case 'I': {
                string trash, name;
                cin >> trash >> name;
                if (tables.find(name) == tables.end()) {
                    std::cout << "Error during INSERT: " << name << " does not name a table in the database\n";
                    getline(cin, trash);
                    break;
                }
                else {
                    uint rows_in;
                    cin >> rows_in;
                    cin >> trash;
                    tables.find(name)->second.insert_rows(rows_in);
                    break;
                }
            }
            case 'P': {
                string trash, name;
                cin >> trash >> name;
                if (tables.find(name) == tables.end()) {
                    getline(cin, trash);
                    std::cout << "Error during PRINT: " << name << " does not name a table in the database\n";
                    break;
                }
                else {
                    int cols_in;
                    cin >> cols_in;
                    tables.find(name)->second.print_rows(cols_in);
                    break;
                }
            }
            case 'D': {
                string trash, name;
                cin >> trash >> name;
                if (tables.find(name) == tables.end()) {
                    getline(cin, trash);
                    std::cout << "Error during DELETE: " << name << " does not name a table in the database\n";
                    break;
                }
                else {
                    tables.find(name)->second.delete_rows();
                    break;
                }
            }
            case 'J': {
                string trash, name, other_name;
                cin >> name >> trash >> other_name >> trash;
                if (tables.find(name) == tables.end()) {
                    getline(cin, trash);
                    std::cout << "Error during JOIN: " << name << " does not name a table in the database\n";
                    break;
                }
                if (tables.find(other_name) == tables.end()) {
                    getline(cin, trash);
                    std::cout << "Error during JOIN: " << other_name << " does not name a table in the database\n";
                    break;
                }
                tables.find(name)->second.join_tables(tables.find(other_name)->second);
                break;
            }
            case 'G': {
                string trash;
                string name;
                string index_type;
                cin >> trash >> name >> index_type;
                if (tables.find(name) == tables.end()) {
                    getline(cin, trash);
                    std::cout << "Error during GENERATE: " << name << " does not name a table in the database\n";
                    break;
                }
                tables.find(name)->second.generate_index(index_type);
                break;
            }
            default: {
                if (cmd != "QUIT") {
                    std::cout << "Error: unrecognized command\n";
                    string trash;
                    getline(cin, trash);
                    break;
                }
            }
                
        }
        
    } while (cmd != "QUIT");
    
    std::cout << "Thanks for being silly!\n";
    return 0;
        
}

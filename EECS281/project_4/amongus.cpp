//  Project Identifier: 9B734EC0C043C5A836EA0EBE4BEFEA164490B2C7
//  among_us.cpp
//  project_4
//
//  Created by Ellen Chlachidze on 11/24/22.
//

#include <stdio.h>
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
#include <limits>
#include <iomanip>
#include <numeric>
using namespace std;

class amongusmap {
private:
    struct vertex {
        int x;
        int y;
        int type; // one of 0, 1, or 2; corresponding to lab, outer, or decontamination respectively
    };
    uint num;
    double c_best_dist;
    double c_curlen;
    vector<vertex> vertices;
    vector<uint> c_best_tour;
    vector<uint> c_path;
    vector<vector<double>> c_distances;
public:
    amongusmap() {
        std::cin >> num;
        vertices.reserve(num+1);
        c_best_dist = 0;
        c_curlen = 0;
        read_in();
    }
    double a_distance(vertex a, vertex b) { //RETURNS SQUARE OF DISTANCE!!!
        if ((a.type == 0 && b.type == 1) || (b.type == 0 && a.type == 1)) {
            return numeric_limits<double>::max();
        }
        else {
            double inter_x = static_cast<double>(a.x-b.x);
            double inter_y = static_cast<double>(a.y-b.y);
            return inter_x*inter_x + inter_y*inter_y;
        }
    }
    
    double not_a_distance(vertex a, vertex b) {
        double inter_x = static_cast<double>(a.x-b.x);
        double inter_y = static_cast<double>(a.y-b.y);
        return inter_x*inter_x + inter_y*inter_y;
    }
    void read_in() {
        for (uint i = 0; i < num; i++) {
            int x_in, y_in;
            cin >> x_in >> y_in;
            if ((x_in == 0 && y_in <= 0) || (y_in == 0 && x_in <= 0)) {
                vertices.push_back({x_in, y_in, 2});
            }
            else if (x_in < 0 && y_in < 0) {
                vertices.push_back({x_in, y_in, 0});
            }
            else {
                vertices.push_back({x_in, y_in, 1});
            }
        }
    }
    
    double c_path_distance(size_t permLength) {
        double sum = 0;
        for (uint i = 0; i < permLength - 1; i++) {
            sum += c_distances[c_path[i]][c_path[i+1]];
        }
        sum += c_distances[c_path[permLength - 1]][c_path[0]];
        return sum;
    }
    
    double c_tree_distance(size_t permLength) { //FINISHED?
        vector<vector<uint>> mst;
        mst.resize(2);
        mst[0].resize(num - permLength);
        mst[1].resize(num - permLength);
        do_A_for_C(permLength, mst);
        double sumdist = 0;
        double best_start_dist = numeric_limits<double>::max();
        uint best_start_ind = 0;
        
        for (uint i = 0; i < mst[0].size(); i++) {
            double tempdist = c_distances[c_path[0]][mst[0][i]];
            if (tempdist < best_start_dist) {
                best_start_dist = tempdist;
                best_start_ind = i;
            }
        }
        sumdist += c_distances[c_path[0]][mst[0][best_start_ind]];
        if (permLength > 1) {
            double best_end_dist = numeric_limits<double>::max();
            uint best_end_ind = 0;
            for (uint i = 0; i < mst[0].size(); i++) {
                double tempdist = c_distances[c_path[permLength-1]][mst[0][i]];
                if (tempdist < best_end_dist) {
                    best_end_dist = tempdist;
                    best_end_ind = i;
                }
            }
            sumdist += c_distances[c_path[permLength-1]][mst[0][best_end_ind]];
        }
        for (uint i = 1; i < mst[0].size(); i++) {
            sumdist += c_distances[mst[0][i]][mst[0][mst[1][i]]];
        }
        sumdist += c_curlen;
        return sumdist;
    }
    
    double insertion_cost(vector<uint> &tour_in, uint ind, uint v_in) {
        if (ind != tour_in.size() - 1) {
            return sqrt(not_a_distance(vertices[tour_in[ind]], vertices[v_in])) + sqrt(not_a_distance(vertices[v_in], vertices[tour_in[ind + 1]])) - sqrt(not_a_distance(vertices[tour_in[ind]], vertices[tour_in[ind+1]]));
        }
        else {
            return sqrt(not_a_distance(vertices[tour_in[ind]], vertices[v_in])) + sqrt(not_a_distance(vertices[v_in], vertices[tour_in[0]])) - sqrt(not_a_distance(vertices[tour_in[ind]], vertices[tour_in[0]]));
        }
    }

    
    bool promising(size_t permLength) {
        double tempdist = c_tree_distance(permLength);
        if (tempdist < c_best_dist) {
            return true;
        }
        return false;
    }
    
    void genPerms(size_t permLength) {
      if (permLength == c_path.size()) {
          c_curlen += c_distances[c_path[0]][c_path[permLength - 1]];
          if (c_curlen < c_best_dist ) {
              c_best_dist = c_curlen;
              c_best_tour.clear();
              c_best_tour.resize(c_path.size());
              c_best_tour = c_path;
          }
        c_curlen -= c_distances[c_path[0]][c_path[permLength - 1]];
        return;
      }
      if (!promising(permLength)) {
        return;
      }
      for (size_t i = permLength; i < c_path.size(); ++i) {
        swap(c_path[permLength], c_path[i]);
        c_curlen += c_distances[c_path[permLength]][c_path[permLength - 1]];
        genPerms(permLength + 1);
        c_curlen -= c_distances[c_path[permLength]][c_path[permLength - 1]];
        swap(c_path[permLength], c_path[i]);
      }
    }
    
    void do_A() {
        double sum_weight = 0;
        vector<bool> k;
        vector<double> d;
        vector<int> p;
        k.resize(num);
        d.resize(num);
        p.resize(num);
        k[0] = true;
        d[0] = numeric_limits<double>::max();
        p[0] = -1;
        for (uint j = 1; j < num; j++) {
            d[j] = a_distance(vertices[0], vertices[j]);
            if (d[j] < numeric_limits<double>::max()) {
                p[j] = 0;
            }
            else {
                p[j] = -1;
            }
        }
        for (uint i = 1; i < num; i++) {
            auto vmin = min_element(d.begin(), d.end());
            uint index = static_cast<uint>(vmin - d.begin());
            if (d[index] == numeric_limits<double>::max()) {
                std::cerr << "Cannot construct MST\n";
                exit(1);
            }
            k[index] = true;
            sum_weight += sqrt(d[index]);
            d[index] = numeric_limits<double>::max();
            for (uint l = 0; l < num; l++) {
                if (k[l] == false) {
                    double newdist = a_distance(vertices[l], vertices[index]);
                    if (newdist < d[l]) {
                        d[l] = newdist;
                        p[l] = static_cast<int>(index);
                    }
                }
            }
        }
        
        std::cout << sum_weight << "\n";
        
        for (uint j = 1; j < num; j++) {
            const int v = static_cast<int>(j);
            const int w = static_cast<int>(p[j]);
            std::cout << std::min(v,w) << " " << std::max(v,w) << "\n";
        }
        return;
    }
    
    void do_A_for_C(size_t permLength, vector<vector<uint>> &mst) {
        vector<uint> b;
        b.reserve(num - permLength);
        vector<uint> temp_b;
        temp_b.reserve(permLength);
        for (uint i = 0; i < permLength; i++) {
            temp_b.push_back(c_path[i]);
        }
        for (uint i = 0; i < num; i++) {
            if (find(temp_b.begin(), temp_b.end(), i) == temp_b.end()) {
                b.push_back(i);
            }
        }
        vector<bool> k;
        vector<double> d;
        vector<uint> p;
        k.resize(num - permLength);
        d.resize(num - permLength);
        p.resize(num - permLength);
        
        k[0] = true;
        d[0] = numeric_limits<double>::max();
        p[0] = 0;
        
        for (uint j = 0; j < b.size(); j++) {
            d[j] = c_distances[b[0]][b[j]];
            if (d[j] == 0) {
                d[j] = numeric_limits<double>::max();
            }
            p[j] = 0;
        }
        
        for (uint i = 1; i < b.size(); i++) {
            auto vmin = min_element(d.begin(), d.end());
            uint index = static_cast<uint>(vmin - d.begin());
            k[index] = true;
            d[index] = numeric_limits<double>::max();
            for (uint l = 0; l < b.size(); l++) {
                if (!k[l]) {
                    double newdist = c_distances[b[l]][b[index]];
                    if (newdist < d[l]) {
                        d[l] = newdist;
                        p[l] = static_cast<uint>(index);
                    }
                }
            }
        }
        for (uint i = 0; i < b.size(); i++) {
            mst[0][i] = b[i];
        }
        for (uint i = 0; i < p.size(); i++) {
            mst[1][i] = p[i];
        }
    }
    
    void do_B() {
        vector<uint> tour;
        tour.reserve(num);
        std::fill(tour.begin(), tour.end(), 0);
        tour.push_back(0);
        double firstdist = numeric_limits<double>::max();
        uint firstind = 0;
        for (uint k = 1; k < num; k++) {
            double tempfirst = not_a_distance(vertices[0], vertices[k]);
            if (tempfirst < firstdist) {
                firstdist = tempfirst;
                firstind = k;
            }
        }
        tour.push_back(firstind);
        for (uint i = 1; i < num; i++) {
            if (i != firstind) {
                double mincost = numeric_limits<double>::max();
                uint minind = 0;
                for (uint k = 0; k < tour.size(); k++) {
                    double tempcost = insertion_cost(tour, k, i);
                    if (tempcost < mincost) {
                        mincost = tempcost;
                        minind = k;
                    }
                }
                tour.insert(tour.begin() + minind + 1, i);
            }
        }
        double sumdist = 0;
        for (uint j = 0; j < num-1; j++) {
            sumdist += sqrt(not_a_distance(vertices[tour[j]], vertices[tour[j+1]]));
        }
        sumdist += sqrt(not_a_distance(vertices[tour[0]], vertices[tour[num-1]]));
        std::cout << sumdist << "\n";
        for (uint j = 0; j < num; j++) {
            std::cout << tour[j] << " ";
        }
        std::cout << "\n";
        return;
    }
    
    void do_C() {
        c_best_tour.reserve(num+1);
        std::fill(c_best_tour.begin(), c_best_tour.end(), 0);
        c_path.reserve(num+1);
        c_distances.resize(num);
        vector<double> distancefill;
        distancefill.resize(num);
        std::fill(distancefill.begin(), distancefill.end(), numeric_limits<double>::max());
        std::fill(c_distances.begin(), c_distances.end(), distancefill);
        
        for (uint i = 0; i < num; i++) {
            for (uint j = 0; j < num; j++) {
                if (i == j) {
                    c_distances[i][j] = 0;
                }
                else {
                    c_distances[i][j] = sqrt(not_a_distance(vertices[i], vertices[j]));
                }
            }
        }

        //run B
        c_best_tour.push_back(0);
        double firstdist = numeric_limits<double>::max();
        uint firstind = 0;
        for (uint k = 1; k < num; k++) {
            double tempfirst = c_distances[0][k];
            if (tempfirst < firstdist) {
                firstdist = tempfirst;
                firstind = k;
            }
        }
        c_best_tour.push_back(firstind);
        for (uint i = 1; i < num; i++) {
            if (i != firstind) {
                double mincost = numeric_limits<double>::max();
                uint minind = 0;
                for (uint k = 0; k < c_best_tour.size(); k++) {
                    double tempcost = insertion_cost(c_best_tour, k, i);
                    if (tempcost < mincost) {
                        mincost = tempcost;
                        minind = k;
                    }
                }
                c_best_tour.insert(c_best_tour.begin() + minind + 1, i);
            }
        }
        for (uint j = 0; j < num-1; j++) {
            c_best_dist += c_distances[c_best_tour[j]][c_best_tour[j+1]];
        }
        c_best_dist += c_distances[c_best_tour[0]][c_best_tour[num - 1]];
        // part B
        c_path.resize(num);
        std::iota(c_path.begin(), c_path.end(), 0);
        genPerms(1);

        double sumdist = 0;
        for (uint i = 0; i < c_best_tour.size() - 1; i++) {
            sumdist += c_distances[c_best_tour[i]][c_best_tour[i+1]];
        }
        sumdist += c_distances[c_best_tour[0]][c_best_tour[c_best_tour.size()-1]];
        std::cout << sumdist << "\n";
        for (uint i = 0; i < c_best_tour.size(); i++) {
            std::cout << c_best_tour[i] << " ";
        }
    }

};

int main(int argc, char *argv[]) {
    xcode_redirect(argc, argv);
    ios_base::sync_with_stdio(false);
    cout << std::setprecision(2);
    cout << std::fixed;
    if (argv[1][1] == 'h' || (argv[1][1] == '-' && argv[1][2] == 'h')) {
        std::cout << "helpful error message here u go\n";
        exit(1);
    }
    else {
        amongusmap among_us;
        switch (argv[2][0]) {
        case 'O' : {
            among_us.do_C();
            break;
        }
        case 'M' : {
            among_us.do_A();
            break;
        }
        case 'F' : {
            among_us.do_B();
            break;
        }
        }
    }
    
}

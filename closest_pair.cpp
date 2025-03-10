#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cmath>
#include <chrono>
using namespace std;

struct pairr { //
    int x, y;
    pairr() {}
    pairr(int x, int y) : x(x), y(y) {}
};

vector<pairr> pairs;
#define MAX 1000000000

int distance(pairr p1, pairr p2) { // 求两个点之间的距离
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

void random(int n) { // 随机生成n个点
    for(int i = 0; i < n; i++) {
        pairs.push_back(pairr(rand(), rand())); 
    }
}

// 归并排序部分
bool compareByY(const pairr &a, const pairr &b) {
    return a.y < b.y;
}

void merge(pairr* pairs, int left, int mid, int right) { // 合并两个有序数组
    pairr *A = pairs + left;
    int lb = mid - left, lc = right - mid;
    pairr *B = new pairr[lb]; // 前半部分
    for (int i = 0; i < lb; ++i) B[i] = A[i];
    pairr *C = pairs + mid;  // 后半部分
    for (int i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { // B和C中元素中最小元素放入A
        if ((j < lb) && (!(k < lc) || (B[j].y <= C[k].y))) A[i++] = B[j++];
        if ((k < lc) && (!(j < lb) || (C[k].y < B[j].y))) A[i++] = C[k++];
    }
    delete [] B;
}
   
void merge_sort_by_y(pairr* pairs, int left, int right) {
    if (right - left < 2) return; // 单元素区间自然有序
    int mid = (right + left) / 2;
    merge_sort_by_y(pairs, left, mid);
    merge_sort_by_y(pairs, mid, right);
    merge(pairs, left, mid, right);
}


int find_closest_pair1(vector<pairr> pairs) { //求最近的两个点 暴力算法
    int min = INT_MAX;
    pairr p1, p2;
    for(int i = 0; i < pairs.size(); i++) {
        for(int j = i + 1; j < pairs.size(); j++) {
            int distance = (pairs[i].x - pairs[j].x) * (pairs[i].x - pairs[j].x) + (pairs[i].y - pairs[j].y) * (pairs[i].y - pairs[j].y);
            if(distance < min) {
                min = distance;
                p1 = pairs[i];
                p2 = pairs[j];
            }
        }
    }
    return min;

}

int find_closest_pair2(vector<pairr> pairs) { // 优化后的算法
    if (pairs.size() == 2) {
        return distance(pairs[0], pairs[1]); // 两个点直接求距离
    } else if (pairs.size() == 1) {
        return INT_MAX; // 一个点无法求最近点对距离，设为最大值
    }
    int mid = pairs.size() / 2;
    vector<pairr> left_pairs, right_pairs;
    for(int i = 0; i < mid; i++) {
        left_pairs.push_back(pairs[i]);
    }
    for(int i = mid; i < pairs.size(); i++) {
        right_pairs.push_back(pairs[i]);
    }
    int d1 = find_closest_pair2(left_pairs);
    int d2 = find_closest_pair2(right_pairs); // 分别递归求左右两边的最近点对
    // 合并两侧求最近点对
    int d = min(d1, d2);
    int minn = d;
    for(int i = 0; i < pairs.size(); i++) {
        for (int j = 1; j < 8; j++) {
            if (i + j < pairs.size()) {
                if ((pairs[i].x - pairs[i + j].x) * (pairs[i].x - pairs[i + j].x) < d) {
                    int dis = distance(pairs[i], pairs[i + j]);
                    if (dis < minn) {
                        minn = dis;
                    }
                }
            }
        }
    }
    return minn;
}

int main() {
    cout << "Please select input method: Read from file or generate randomly?\n";
    cout << "1. Read from file\n";
    cout << "2. Generate randomly\n";
    int choice, number;
    cin >> choice;
    if(choice == 1) {
        cout << "Please enter the file name: ";
        string filename;
        cin >> filename;
        ifstream file(filename);
        file >> number;
        for(int i = 0; i < number; i++) {
            int x, y;
            file >> x >> y;
            pairs.push_back(pairr(x, y));
        }
    } else if (choice == 2) {
        cout << "Please enter the number of pairs: ";
        cin >> number;
        random(number);
    } else {
        cout << "illegal input\n";
        return 0;
    }

    // for(int i = 0; i < pairs.size(); i++) {
    //     cout << pairs[i].x << " " << pairs[i].y << endl;
    // }

    merge_sort_by_y(&pairs[0], 0, pairs.size());

    cout << "choose the type of algorithm you want to use: \n";
    cout << "1. Brute force algorithm\n";
    cout << "2. Optimized algorithm\n";
    int type;
    cin >> type;
    double res;

    if(type == 1) {
        auto start = std::chrono::high_resolution_clock::now();
        int final_dis = find_closest_pair1(pairs);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        res = sqrt(final_dis);
        cout << "The distance of the closest pair is :" << res << endl;
        cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
    } else if(type == 2) {
        auto start = std::chrono::high_resolution_clock::now();
        int final_dis = find_closest_pair2(pairs);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        res = sqrt(final_dis);
        cout << "The distance of the closest pair is :"<< res << endl;
        cout << "Time taken: " << duration.count() << " nanoseconds" << endl;
    } else {
        cout << "illegal input\n";
        return 0;
    }
    return 0;
}
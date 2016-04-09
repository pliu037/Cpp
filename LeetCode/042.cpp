// https://leetcode.com/problems/trapping-rain-water/

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

class Solution {
public:
    int trap(std::vector<int>& height) {
        // Finds the peaks in <height>, and for each peak, inserts a pair, (peak's height, peak's index) into <peaks>
        std::vector<std::pair<int, int>> peaks;
    	int sum = 0;
    	int last_max_index = -1;
    	bool increasing = true;
        for (int i = 0; i < height.size(); i ++) {
        	int next_value = i + 1 == height.size() ? 0 : height[i + 1];
        	if (increasing && next_value < height[i]) {
        		peaks.push_back(std::pair<int, int>(height[i], i));
        		increasing = false;
        	}
        	if (!increasing && next_value > height[i]) {
        		increasing = true;
        	}
        }

        /*
        If there are 2 or more peaks, max-heapifies <peaks> and pulls out the 2 highest peaks as the initial left and
        right boundaries
        A max-heap is used to process the highest peaks first since lower peaks between higher peaks will be submerged
        Additionally, since peaks are processed in descending order of height, the latest peak to be processed will
        always have the min height of the 2 boundaries (relevant for the functionality of get_capacity)
        If there are fewer than 2 peaks, then the topology can't hold any water since there are no valleys
        */ 
        if (peaks.size() >= 2) {
            std::make_heap(peaks.begin(), peaks.end());
            std::pair<int, int> check = pop_heap(peaks);
            int left = check.second, right;
            check = pop_heap(peaks);
            if (check.second < left) {
                right = left;
                left = check.second;
            }
            else {
                right = check.second;
            }
            sum += get_capacity(height, left, right, check.first);

            /*
            Continue processing peaks in descending order of height for the same reasons as above. If the peak is
            located to left or the right of the processed area, add the capacity between the previous boundary and the
            new peak and extend the processed area to the new peak.
            */
            while (!peaks.empty()) {
                check = pop_heap(peaks);
                if (check.second > right) {
                    sum += get_capacity(height, right, check.second, check.first);
                    right = check.second;
                }
                else if (check.second < left) {
                    sum += get_capacity(height, check.second, left, check.first);
                    left = check.second;
                }
            }
        }
        return sum;
    }

    // Pops and returns the head of the heap, <vec>
    template <typename T>
    T pop_heap(std::vector<T>& vec) {
        T ret = vec.front();
        std::pop_heap(vec.begin(), vec.end());
        vec.pop_back();
        return ret;
    }

    /*
    Given the vector of heights, the left boundary index, the right boundary index, and the barrier height (the minimum
    height of either the left or right boundary) returns the amount of water that can be contained between the
    boundaries
    The amount of water that can be contained is calculated by summing the difference between the height at every index
    between left and right boundaries and the barrier height, with negative differences 
    */
    int get_capacity(std::vector<int>& height, int left, int right, int barrier) {
        int sum = 0;
        for (int i = left + 1; i < right; i ++) {
            sum += barrier - height[i] <= 0 ? 0 : barrier - height[i];
        }
        return sum;
    }
};

int main() {
	std::vector<int> height;
    height.push_back(5);
    height.push_back(4);
    height.push_back(1);
    height.push_back(2);
	Solution sol;
	std::cout << sol.trap(height) << std::endl;
}
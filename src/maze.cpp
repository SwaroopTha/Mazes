/* Your code here! */
#include "maze.h"
#include <algorithm>

SquareMaze::SquareMaze() {}

void SquareMaze::makeMaze(int width, int height) {

    // initialize the maze
    width_ = width;
    height_ = height;
    size_ = width * height;
    dsets_.addelements(size_);

    // storing downs, rights
    for (int i = 0; i < size_; i++) {
        down_.push_back(true);
        right_.push_back(true);
    }

    // randomization
    int idx = 0;
    while (idx + 1 < size_) {
        int x = rand() % width_;
        int y = rand() % height_;
        bool rand_wall = rand() % 2; // 0 = down, 1 = right
        if (rand_wall) { // right
            //bounds
            if (x >= 0 && x < width_ - 1) {
                if (dsets_.find(width_ * y + x) != dsets_.find(width_ * y  + x + 1)) {
                    setWall(x, y, 0, false);
                    int val = width * y + x;
                    dsets_.setunion(val, val + 1);
                    idx++;
                } else {
                    continue;
                }
            }
        } else { // downs
            if (y >= 0 && y < height_ - 1) {
                if (dsets_.find(width_ * y + x) != dsets_.find(width * y + x + width)) {
                    setWall(x, y, 1, false);
                    int val = width * y + x;
                    dsets_.setunion(val, val + width_);
                    idx++;
                }
            } else {
                continue;
            }
        }
    }
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (dir == 0) {
        right_[y * width_ + x] = exists;
    } else {
        down_[y * width_ + x] = exists;
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if (dir == 0) {
        if ((x + 1 < width_) && !right_[y * width_ + x]) {
            return true;
        }
    } else if (dir == 1) {
        if ((y + 1 < height_) && !down_[y * width_ + x]) {
            return true;
        }
    } else if (dir == 2) {
        if (x >= 1 && !right_[y * width_ + x - 1]) {
            return true;
        }
    } else if (dir == 3) {
        if (y >= 1 && !down_[width_ * y + x - width_]) {
            return true;
        }
    }
    return false;
}


cs225::PNG* SquareMaze::drawMaze() const {
    cs225::PNG* drawing = new cs225::PNG(width_ * 10 + 1, height_ * 10 + 1);
    // blacken top
    for (int j = 0; j <= height_ * 10; j++) {
        cs225::HSLAPixel &pixel = drawing->getPixel(0, j);
        pixel.h = 0;
        pixel.s = 0;
        pixel.l = 0;
    }
    //blacken left 
    for (int i = 10; i <= width_ * 10; i++) {
        cs225::HSLAPixel &pixel = drawing->getPixel(i, 0);
        pixel.h = 0;
        pixel.s = 0;
        pixel.l = 0;
    }

    for (int i = 0; i < width_; i++) {
        for (int j = 0; j < height_; j++) {
            for (int k = 0; k < 11; k++) {
                if (right_[width_ * j + i]) {
                    cs225::HSLAPixel &pixel = drawing->getPixel((i + 1) * 10, j * 10 + k);
                    pixel.h = 0;
                    pixel.s = 0;
                    pixel.l = 0;
                }
                if (down_[width_ * j + i]) {
                    cs225::HSLAPixel &pixel = drawing->getPixel(i * 10 + k, (j + 1) * 10);
                    pixel.h = 0;
                    pixel.s = 0;
                    pixel.l = 0;
                }
            }
        }
    }
    return drawing;
}



vector<int> SquareMaze::solveMaze() {
    vector<bool> vists(size_, false);
    vector<pair<int, int>> solved; // initializer
    size_ = width_ * height_;
    for (int i = 0; i < size_; i++) {
        solved.push_back(make_pair(0,0));
    }

    // bfs algorithm
    vector<int> result;
    vists[0] = true;
    queue<int> qu;

    qu.push(0);
    while (!qu.empty()) {
        int x = qu.front() % width_;
        int y = qu.front() / width_;
        int frnt = qu.front();
        qu.pop();
        int val = solved[frnt].second + 2;

        // direction
        helper(frnt, val, x, y, vists, solved, qu);
    } 
    int len = size_ - height_;
    for (int k = 0; k < width_; k++) {
        if (solved[size_ - height_ + k].second > solved[len].second) {
            len = size_ - height_ + k;
        }
    }

    while (len != 0) {
        resultAppender(len, result, solved);
    }
    std::reverse(result.begin(), result.end());
    return result;

}




void SquareMaze::resultAppender(int &len, vector<int> &result, vector<pair<int, int>> &solved) {
    if (solved[len].first == len - 1) {
        result.push_back(0);
    }
    if (solved[len].first == len - width_) {
        result.push_back(1);
    }
    if (solved[len].first == len + 1) {
        result.push_back(2);
    }
    if (solved[len].first == len + width_) {
        result.push_back(3);
    }
    len = solved[len].first;
}

void SquareMaze::helper(int &front, int &val, int &x, int &y, vector<bool> &visits, vector<pair<int, int>> &solved, queue<int> &qu) {
    
    if (canTravel(x, y, 0)) {
        if (visits[front + 1] == false) {
            visits[front + 1] = true;
            solved[front + 1].first = front;
            solved[front + 1].second = val;
            qu.push(front + 1);
        }
    }
    if (canTravel(x, y, 1)) {
        if (visits[front + width_] == false) {
            visits[front + width_] = true;
            solved[front + width_].first = front;
            solved[front + width_].second = val;
            qu.push(front + width_);
        }
    }
    if (canTravel(x, y, 2)) {
        if (visits[front - 1] == false) {
            visits[front - 1] = true;
            solved[front - 1].first = front;
            solved[front - 1].second = val;
            qu.push(front - 1);
        }
    }
    if (canTravel(x, y, 3)) {
        if (visits[front - width_] == false) {
            visits[front - width_] = true;
            solved[front - width_].first = front;
            solved[front - width_].second = val;
            qu.push(front - width_);
        }
    }
}


cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* soln = drawMaze();
    vector<int> solves = solveMaze();
    // path finder
    int x = 5;
    int d_x = 0;
    int y = 5;
    int d_y = 0;
    // red path
    bool pos = true;
    for (unsigned i = 0; i < solves.size(); i++) {
        pathFinder(soln, solves, x, y, i);
    }
    // end
    cs225::HSLAPixel &pix = soln->getPixel(x, y);
    pix.h = 0;
    pix.s = 1;
    pix.l = 0.5;
    x -= 4;
    y += 5;
    for (int i = 0; i <= 8; i++) {
        cs225::HSLAPixel &pix = soln->getPixel(x, y);
        pix.l = 1;
        x++;
    }
    return soln;

}

void SquareMaze::pathFinder(cs225::PNG*& soln, vector<int> &solves, int &x, int &y, unsigned &i) {
    // red shade logic
    if (solves[i] == 0) {
        for (int j = 0; j < 10; j++) {
            cs225::HSLAPixel &pix = soln->getPixel(x, y);
            pix.h = 0;
            pix.s = 1;
            pix.l = 0.5;
            x += 1;
        }
    } else if (solves[i] == 1) {
        for (int j = 0; j < 10; j++) {
            cs225::HSLAPixel &pix = soln->getPixel(x, y);
            pix.h = 0;
            pix.s = 1;
            pix.l = 0.5;
            y += 1;
        }
    } else if (solves[i] == 2) {
        for (int j = 0; j < 10; j++) {
            cs225::HSLAPixel &pix = soln->getPixel(x, y);
            pix.h = 0;
            pix.s = 1;
            pix.l = 0.5;
            x -= 1;
        }
    } else if (solves[i] == 3) {
        for (int j = 0; j < 10; j++) {
            cs225::HSLAPixel &pix = soln->getPixel(x, y);
            pix.h = 0;
            pix.s = 1;
            pix.l = 0.5;
            y -= 1;
        }
    } 
}

